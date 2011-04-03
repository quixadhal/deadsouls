/*    /daemon/race.c
 *    from the Dead Souls  Object Library
 *    handles race configuration and administration
 *    created by Descartes of Borg 960108
 *    Version: @(#) races.c 1.4@(#)
 *    Fixed by Ashon @ Stargate Atlantis 16 March 2006
 */

#include <lib.h>
#include <cfg.h>
#include <save.h>
#include <daemons.h>
#include <privs.h>
#include <armor_types.h>
#include <mouth_types.h>
#include <size_types.h>

inherit LIB_DAEMON;

private mapping Races = ([]);
static private mapping Resistances = ([]);
static private mapping Armors = ([]);
static private mapping Sizes = ([]);
static private mapping Btypes = ([]);
string array FlyingRaces = ({});
string array LimblessCombatRaces = ({});
string array LimblessRaces = ({});
string array NonBitingRaces = ({});
string array SwimmingRaces = ({});
string array NonMeatRaces = ({});
static string SaveFile;

static void ReloadRaces(){
    string *races = get_dir(CFG_RACES+"/");
    Races = ([]);
    foreach(string race in races){
        string str = CFG_RACES+"/"+race;
        if(file_exists(str)) {
            catch( this_object()->AddRace(str) );
        }
    }
}

static void create() {
    daemon::create();
    SaveFile = save_file(SAVE_RACES);
    if(file_exists(SaveFile)){
        RestoreObject(SaveFile);
    }
    if( !sizeof(Races) ) ReloadRaces();
    if(!FlyingRaces) FlyingRaces = ({});
    if(!LimblessCombatRaces) LimblessCombatRaces = ({});
    if(!LimblessRaces) LimblessRaces = ({});
    if(!NonBitingRaces) NonBitingRaces = ({});
    if(!NonMeatRaces) NonMeatRaces = ({});
    SaveObject(SaveFile);
}

    static private void validate() {
        if( !(master()->valid_apply(({ PRIV_ASSIST }))) )
            error("Illegal attempt to modify race data");
    }

int CanFly(string str){
    if( !Races[str] ) return 0;
    if(member_array(str, FlyingRaces) != -1) return 1;
    else return 0;
}

int CanSwim(string str){
    if( !Races[str] ) return 0;
    if(member_array(str, SwimmingRaces) != -1) return 1;
    else return 0;
}

int GetNonMeatRace(string str){
    if(member_array(str,NonMeatRaces) == -1) return 0;
    else return 1;
}

int GetLimblessCombatRace(string str){
    if(member_array(str,LimblessCombatRaces) == -1) return 0;
    else return 1;
}

int GetLimblessRace(string str){
    if(member_array(str,LimblessRaces) == -1) return 0;
    else return 1;
}

int SetNonMeatRace(string str){
    if(member_array(str,NonMeatRaces) != -1) return 0;
    NonMeatRaces += ({ lower_case(str) });
    return 1;
}

int SetLimblessCombatRace(string str){
    if(member_array(str,LimblessCombatRaces) != -1) return 0;
    LimblessCombatRaces += ({ lower_case(str) });
    return 1;
}

int SetLimblessRace(string str){
    if(member_array(str,LimblessRaces) != -1) return 0;
    LimblessRaces += ({ lower_case(str) });
    return 1;
}

int SetFlyingRace(string str){
    FlyingRaces += ({ str });
    return 1;
}

int SetNonBitingRace(string str){
    NonBitingRaces += ({ str });
    return 1;
}

int SetSwimmingRace(string str){
    SwimmingRaces += ({ str });
    return 1;
}

int GetSwimmingRace(string str){
    if(member_array(str,SwimmingRaces) != -1) return 1;
    else return 0;
}

string *GetNonMeatRaces(){
    return copy(NonMeatRaces);
}

string *GetSwimmingRaces(){
    return copy(SwimmingRaces);
}

string *GetLimblessCombatRaces(){
    return copy(LimblessCombatRaces);
}

string *GetLimblessRaces(){
    return copy(LimblessRaces);
}

string *GetFlyingRaces(){
    return copy(FlyingRaces);
}

int GetBitingRace(string str){
    if(member_array(str,NonBitingRaces) == -1) return 1;
    else return 0;
}

int RemoveRaceVars(string str){
    if(previous_object() != this_object()) return 0;
    FlyingRaces  -= ({ str });
    LimblessCombatRaces -= ({ str });
    LimblessRaces -= ({ str });
    NonBitingRaces -= ({ str }); 
    NonBitingRaces -= ({ str });
    SwimmingRaces -= ({ str });
    return 1;
}

int GetRaceMass(string str){
    int Mass = Races[str]["Mass"];
    if(Mass) return Mass;
    else return 0;
}

int GetRaceSize(string str){
    int Size;
    Size = Races[str]["Size"];
    if(Size) return Size;
    else return 0;
}

int GetRaceBodyType(string str){
    int Btype = Races[str]["Btype"];
    if(Btype) return Btype;
    else return 0;
}

int GetRaceRespirationType(string str){
    int Rtype = Races[str]["Rtype"];
    if(Rtype) return Rtype;
    else return 0;
}
int GetRaceMouthType(string str){
    int Mtype = Races[str]["Mouthtype"];
    if(Mtype) return Mtype;
    else return 0;
}

mapping GetRace(string str){
    mapping ret = Races[str] + ([]);
    return ret;
}

void AddRace(string file, int player) {
    mapping res;
    string array tmp, parts;
    string race, test_string;
    int x;
    mixed array limb = allocate(4);
    mixed array tmp_limb = allocate(4);
    mapping s; 

    res = ([]);

    res["Resistance"] = ([]);
    res["Skills"] = ([]);
    res["Stats"] = ([]);
    res["Limbs"] = ({});
    res["Mass"] = 0;
    res["Size"] = 0;
    res["Btype"] = 0;
    res["Rtype"] = 0;
    res["Mouthtype"] = 0;

    validate();

    if( !file_exists(file) ) error("No such file: " + file);
    race = last_string_element(file,"/");

    res["Fingers"] = ([]);


    foreach(string line in explode(read_file(file),"\n")){
        mixed *tmp_vision;
        test_string = first_string_element(line," ");
        if(!test_string || !sizeof(test_string)) test_string = line;

        switch(test_string){
            string type = "";

            case "FLYING_RACE":
                line = trim(replace_string(line, "FLYING_RACE", ""));
            if(sizeof(line) && atoi(line) < 1) break;
            else SetFlyingRace(race);
            break;

            case "NOT_MEAT":
                line = trim(replace_string(line, "NOT_MEAT", ""));
            if(sizeof(line) && atoi(line) < 1) break;
            else SetNonMeatRace(race);
            break;

            case "LIMBLESS_RACE":
                line = trim(replace_string(line, "LIMBLESS_RACE", ""));
            if(sizeof(line) && atoi(line) < 1) break;
            else SetLimblessRace(race);
            break;

            case "LIMBLESS_COMBAT_RACE":
                line = trim(replace_string(line, "LIMBLESS_COMBAT_RACE", ""));
            if(sizeof(line) && atoi(line) < 1) break;
            else SetLimblessCombatRace(race);
            break;

            case "NONBITING_RACE":
                line = trim(replace_string(line, "NONBITING_RACE", ""));
            if(sizeof(line) && atoi(line) < 1) break;
            else SetNonBitingRace(race);
            break;

            case "SWIMMING_RACE":
                line = trim(replace_string(line, "SWIMMING_RACE", ""));
            if(sizeof(line) && atoi(line) < 1) break;
            else SetSwimmingRace(race);
            break;

            case "RACE":
                race = replace_string(line, "RACE ", "");
            if( Races[race] ) error(race+": Race already exists");
            break;

            case "SENSITIVITY":
                line = replace_string(line, "SENSITIVITY ", "");
            tmp_vision = map(explode(line, ":"), (: to_int :));
            res["Sensitivity"] = ({ tmp_vision[0], tmp_vision[1] * 10 });
            break;

            case "PLAYER_RACE":
                line = replace_string(line, "PLAYER_RACE ", "");
            if(player || atoi(line) > 0) player = 1;
            else player = 0;
            break;

            case "LANGUAGE":
                //TODO: This should be a Language array to handle multiple 
                //languages but further research is required first.
                res["Language"] = replace_string(line, "LANGUAGE ", "");
            break;

            case "RESISTANCE":			  
                tmp = explode(replace_string(line, "RESISTANCE ", ""), ":");
            x = to_int(tmp[0]);
            if( x == 0 && tmp[0] != "0" ) x = this_object()->GetResistance(tmp[0]);
            res["Resistance"][x] = tmp[1];
            break;

            case "SKILL":      
                tmp = explode(replace_string(line, "SKILL ", ""), ":");
            res["Skills"][tmp[0]] = ({ tmp[1], tmp[2], tmp[3], tmp[4] });
            SKILLS_D->SetSkill(tmp[0], race, tmp[2], 1);
            break;

            case "MASS":
                x = 0;
            sscanf(line, "MASS %d",x);
            if(x) res["Mass"] = x;
            break;

            case "SIZE":
                type = "";
            x = 0;
            if(sscanf(line, "SIZE %s",type)) res["Size"] = this_object()->GetSize(type);
            else res["Size"] = x;
            break;

            case "BODY_TYPE":
                type = "";
            x = 0;
            if(sscanf(line, "BODY_TYPE %s",type)) res["Btype"] = this_object()->GetBodyType(type);
            else res["Btype"] = x;
            break;

            case "RESPIRATION_TYPE":
                type = "";
            x = 0;
            if(sscanf(line, "RESPIRATION_TYPE %s",type)) res["Rtype"] = this_object()->GetRespirationType(type);
            else res["Rtype"] = x;
            break;

            case "STATS":
                tmp = ({});
            s = ([]);
            tmp = explode(replace_string(line, "STATS ",""), ":");
            s["Average"] = copy(to_int(tmp[1]));
            s["Class"] = copy(to_int(tmp[2]));
            res["Stats"][tmp[0]] = s;
            STATS_D->SetStat(tmp[0], race, s["Class"]);
            break;

            case "MOUTH":
                type = "";
            x = 0;
            if(sscanf(line, "MOUTH %s",type)){
                res["Mouthtype"] = this_object()->GetMouthType(type);
            }
            break;

            case "LIMB":
                limb = ({ ({}), ({}), ({}), ({}) });
            tmp_limb = explode(replace_string(line, "LIMB ",""), ":");
            limb[0] = tmp_limb[0];
            limb[1] = (tmp_limb[1] == "0" ? 0 : tmp_limb[1]);
            limb[2] = to_int(tmp_limb[2]);
            limb[3] = map(explode(tmp_limb[3], ","), function(string str) {
                    int x = to_int(str);
                    if( x == 0 && str != "0" ) { return this_object()->GetArmor(str); }
                    return x;
                    });

            res["Limbs"] = ({ res["Limbs"]..., limb });
            res["Limbs"] += ({limb});
            break;

            case "HAND":
                parts = explode(replace_string(line, "HAND ",""), ":");
            res["Fingers"][parts[0]] = to_int(parts[1]);
            break;

            default:
            break;
        } 
    }  

    res["Complete"] = 1;

    if( player ) {
        res["PlayerFlag"] = 1;
    }

    else {
        res["PlayerFlag"] = 0;
    }

    Races[race] = res;
    SaveObject(SaveFile);
} 

void RemoveRace(string race) {
    validate();
    map_delete(Races, race);
    RemoveRaceVars(race);
    if(Races[race]) 
        SaveObject(SaveFile);
}

string ConvertPipe(string str){
    str = replace_string(str," ","");
    str = replace_string(str,"|","+");
    return str;
}

int GetArmor(string foo) {
    string str = ConvertPipe(foo);
    string file = DIR_DAEMONS "/tmp/" + str + ".c";

    if( !unguarded((: file_exists($(file)) :)) ) {
        unguarded((: write_file($(file), "#include <armor_types.h>\n" +
                        "int armor() { return " + $(str) + "; }\n") :));
    }
    return call_other(file, "armor"); 
}

int GetSize(string foo) {
    string str = ConvertPipe(foo);
    string file = DIR_DAEMONS "/tmp/" + str + ".c";

    if( !unguarded((: file_exists($(file)) :)) ) {
        unguarded((: write_file($(file), "#include <size_types.h>\n" +
                        "int size() { return " + $(str) + "; }\n") :));
    }
    return call_other(file, "size"); 
}

int GetBodyType(string foo) {
    string str = ConvertPipe(foo);
    string file = DIR_DAEMONS "/tmp/" + str + ".c";

    if( !unguarded((: file_exists($(file)) :)) ) {
        unguarded((: write_file($(file), "#include <body_types.h>\n" +
                        "int btype() { return " + $(str) + "; }\n") :));
    }
    return call_other(file, "btype"); 
}

int GetRespirationType(string foo) {
    string str = ConvertPipe(foo);
    string file = DIR_DAEMONS "/tmp/" + str + ".c";

    if( !unguarded((: file_exists($(file)) :)) ) {
        unguarded((: write_file($(file), "#include <respiration_types.h>\n" +
                        "int rtype() { return " + $(str) + "; }\n") :));
    }
    return call_other(file, "rtype"); 
}

int GetMouthType(string foo) {
    string str = ConvertPipe(foo);
    string file = DIR_DAEMONS "/tmp/" + str + ".c";

    if( !unguarded((: file_exists($(file)) :)) ) {
        unguarded((: write_file($(file), "#include <mouth_types.h>\n" +
                        "int mtype() { return " + $(str) + "; }\n") :));
    }
    return call_other(file, "mtype"); 
}

int GetResistance(string str) {
    string file = DIR_DAEMONS "/tmp/" + str + ".c";

    if( !unguarded((: file_exists($(file)) :)) ) {
        unguarded((: write_file($(file), "#include <damage_types.h>\n" +
                        "int damage() { return " + $(str) + "; }\n") :));
    }
    return call_other(file, "damage"); 
}

varargs mapping GetRemoteRaces(string str) {
    mapping mp = ([]);
    mapping foo = ([]);

    if(str && Races[str]) foo[str] = Races[str];
    else foo = copy(Races);

    foreach(string race, mapping res in foo) {
        mapping stats = ([]);

        mp[race] = ([]);
        mp[race]["limbs"] = res["Limbs"];
        mp[race]["resistance"] = res["Resistance"];

        foreach(string stat, mapping st in res["Stats"]) {
            stats[stat] = ([]);
            stats[stat]["class"] = st["Class"];
            stats[stat]["average"] = st["Average"];
        }

        mp[race]["stats"] = stats;
        mp[race]["fingers"] = res["Fingers"];
        mp[race]["sensitivity"] = res["Sensitivity"];
        mp[race]["player"] = res["PlayerFlag"];
        mp[race]["language"] = res["Language"];

    }
    return mp;
}

void SetComplete(string race) {
    mapping res;

    validate();

    if( !Races[race] ) error("No such race");
    else res = Races[race];
    res["Complete"] = 1;
    SaveObject(SaveFile);
}

void SetLightSensitivity(string race, int array sensitivity) {
    mapping res;

    validate();

    if( !Races[race] ) error("No such race");
    else res = Races[race];
    if( sensitivity[0] < 1 ) error("Invalid sensitivity value");
    if( sensitivity[1] > 99 ) error("Invalid sensitivity value");
    if( sensitivity[0] > sensitivity[1] ) error("Invalid sensitivity value");
    res["Sensitivity"] = sensitivity;
    SaveObject(SaveFile);
}

void SetCharacterLimbs(string race, mixed array args) {
    mapping res = Races[race];
    mixed array tmp = ({});

    if( !res || !res["Complete"] || sizeof(args) != 2 ) return;
    args[0] = copy(res["Limbs"]);
    foreach(string finger, int count in res["Fingers"])
        tmp = ({ tmp..., ({ finger, count }) });
    args[1] = tmp;
}

void SetCharacterRace(string race, mixed array args) {
    mapping res = Races[race];
    mixed array tmp;
    mapping StatMap;
    string schluss;

    if( !res || !res["Complete"] || sizeof(args) != 5 ) return;
    tmp = ({});
    foreach(int key, string val in res["Resistance"])
        tmp = ({ tmp..., ({ key, val }) });
    args[0] = tmp;
    tmp = ({});
    StatMap = copy(res["Stats"]);
    schluss = "";
    foreach(schluss in keys(StatMap)){
        tmp = ({ tmp..., ({ schluss, StatMap[schluss]["Average"], StatMap[schluss]["Class"] }) });
    }
    args[1] = tmp;
    args[2] = res["Language"];
    args[3] = res["Sensitivity"];
    args[4] = res["Skills"]; 
}

varargs string array GetRaces(int player_only) {

    return filter(keys(Races), function(string race, int player_only) {
            mapping res = Races[race];

            if( !res["Complete"] ) return 0;
            if( player_only && !res["PlayerFlag"] )
            return 0;
            return 1;
            }, player_only);
}

string GetHelp(string race) {
    mapping res = Races[race];
    string array limbs;
    string help = "Race: " + race + "\n\n";
    string tmp, h_file;
    int x;

    if( !res ) return 0;
    h_file = "/doc/help/races/"+lower_case(race);
    if(file_exists(h_file)) return read_file(h_file); 
    limbs = map(res["Limbs"], (: $1[0] :));
    limbs = distinct_array(limbs);
    help += "Limbs:\n";
    help += capitalize(item_list(map(limbs, (: add_article :)))) + ".\n";
    help += "\nFingered limbs:\n";
    foreach(string finger, int count in res["Fingers"])
        help += "\t" + finger + " (" + count + ")\n";
    limbs = regexp(limbs, ".* wing");
    if( sizeof(limbs) ) {
        help += "\nFlying\n";
    }

    else {
        help += "\nNon-flying\n";
    }

    x = res["Sensitivity"][0];
    if( x < 11 ) tmp = "excellent";
    else if( x < 16 ) tmp = "above average";
    else if( x < 21 ) tmp = "good";
    else if( x < 26 ) tmp = "average";
    else if( x < 31 ) tmp = "below average";
    else if( x < 36 ) tmp = "very poor";    
    else tmp = "extremely poor";
    help += "\nNight vision: " + tmp + "\n";
    x = res["Sensitivity"][1];
    if( x < 61 ) tmp = "extremely poor";
    else if( x < 66 ) tmp = "very poor";
    else if( x < 71 ) tmp = "below average";
    else if( x < 76 ) tmp = "average";
    else if( x < 81 ) tmp = "good";
    else if( x < 86 ) tmp = "above average";
    else tmp = "excellent";
    help += "Day vision: " + tmp + "\n\n";
    return help;
}

public mapping GetResistances() {
    return copy(Resistances);
}

public mapping GetArmors() {
    return copy(Armors);
}
