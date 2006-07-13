/*    /daemon/race.c
 *    from the Dead Souls  Object Library
 *    handles race configuration and administration
 *    created by Descartes of Borg 960108
 *    Version: @(#) races.c 1.4@(#)
 *    Fixed by Ashon @ Stargate Atlantis 16 March 2006
 */

#include <lib.h>
#include <save.h>
#include <privs.h>
#include <armor_types.h>
#include "include/races.h"

inherit LIB_DAEMON;

private mapping Races = ([]);
static private mapping Resistances = ([]);
static private mapping Armors = ([]);
string array FlyingRaces = ({"wtf"});
string array LimblessCombatRaces = ({});
string array LimblessRaces = ({});
string array NonBitingRaces = ({});


string wtf = "/tmp/wtf.txt";


static void create() {
    string array lines;

    daemon::create();

    if( unguarded((: file_size(SAVE_RACES __SAVE_EXTENSION__) :)) > 0 )
	unguarded((: restore_object(SAVE_RACES) :));

    if( !Races ) Races = ([]);
    if(!FlyingRaces) FlyingRaces = ({});
    if(!LimblessCombatRaces) LimblessCombatRaces = ({});
    if(!LimblessRaces) LimblessRaces = ({});
    if(!NonBitingRaces) NonBitingRaces = ({});

    // Hocus pocus to load armor and resistance info
    lines = explode(read_file("/include/armor_types.h"), "\n");
    foreach(string line in lines) {
	string type;

	if( sscanf(line, "#define %s %*s", type) == 2 ) {
	    string file = DIR_DAEMONS "/tmp/" + type + ".c";

	    if( type == "A_MAX_ARMOR_BIT" ) {
		continue;
	    }

	    if( !file_exists(file) ) {
		unguarded((: write_file($(file), "#include <armor_types.h>\n"
		      "int armor() { return " +
		      $(type) + "; }\n") :));
	    }

	    Armors[type] = call_other(file, "armor");
	}
    }

    lines = explode(read_file("/include/damage_types.h"), "\n");
    foreach(string line in lines) {
	string type;

	if( sscanf(line, "#define %s %*s", type) == 2 ) {
	    string file = DIR_DAEMONS "/tmp/" + type + ".c";
	    if( type == "MAX_DAMAGE_BIT" ) {
		continue;
	    }

	    if( !file_exists(file) ) {
		unguarded((: write_file($(file), "#include <damage_types.h>\n"
		      "int damage() { return " +
		      $(type) + "; }\n") :));
	    }

	    Resistances[type] = call_other(file, "damage");
	}
    }
}

static private void validate() {
    if( !((int)master()->valid_apply(({ PRIV_ASSIST }))) )
	error("Illegal attempt to modify race data");
}

int CanFly(string str){
    if( !Races[str] ) return 0;
    if(member_array(str, FlyingRaces) != -1) return 1;
    else return 0;
}

int GetLimblessCombatRace(string str){
    if(member_array(str,LimblessCombatRaces) == -1) return 0;
    else return 1;
}

int GetLimblessRace(string str){
    if(member_array(str,LimblessRaces) == -1) return 0;
    else return 1;
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
    //if(member_array(str,FlyingRaces) != -1) return 0;
    FlyingRaces += ({ str });
    return 1;
}

int SetNonBitingRace(string str){
    //if(member_array(str,FlyingRaces) != -1) return 0;
    NonBitingRaces += ({ str });
    return 1;
}

string *GetLimblessCombatRaces(){
    return LimblessCombatRaces;
}

string *GetLimblessRaces(){
    return LimblessRaces;
}

string *GetFlyingRaces(){
    return FlyingRaces;
}

int GetBitingRace(string str){
    if(member_array(str,NonBitingRaces) == -1) return 1;
    else return 0;
}

void AddRace(string file, int player) {
    class Race res;
    string array lines, tmp, parts;
    string race, test_string;
    int i, x;
    mixed array limb = allocate(4);
    mixed array tmp_limb = allocate(4);
    class Stat s; 

    res = new(class Race);

    res->Resistance = ([]);
    res->Skills = ([]);
    res->Stats = ([]);
    res->Limbs = ({});

    validate();

    if( !file_exists(file) ) error("No such file: " + file);
    race = last_string_element(file,"/");

    lines = explode(read_file(file), "\n");

    lines = filter(lines, function(string str) {
	  if( strlen(str) == 0 ) {
	      return 0;
	  }
	  if( str[0] == '#' ) {
	      return 0;
	  }
	  if( str[0] == ' ' || str[0] == '\t' ) {
	      return 0;
	  }
	  return 1;
	});

      res->Fingers = ([]);


      foreach(string line in explode(read_file(file),"\n")){
	  test_string = first_string_element(line," ");
	  if(!test_string || !sizeof(test_string)) test_string = line;

	  switch(test_string){

	  case "FLYINGRACE":
	      SetFlyingRace(race);
	      break;

	  case "LIMBLESSRACE":
	      SetLimblessRace(race);
	      break;

	  case "LIMBLESSCOMBATRACE":
	      SetLimblessCombatRace(race);
	      break;

	  case "NONBITINGRACE":
	      SetNonBitingRace(race);
	      break;

	  case "RACE":
	      race = replace_string(line, "RACE ", "");
	      if( Races[race] ) error(race+": Race already exists");
	      break;

	  case "SENSITIVITY":
	      line = replace_string(line, "SENSITIVITY ", "");
	      res->Sensitivity = map(explode(line, ":"), (: to_int :));
	      break;

	  case "PLAYER_RACE":
	      line = replace_string(line, "PLAYER_RACE ", "");
	      if(!player && to_int(line) > 0) player = 1;
	      break;

	  case "LANGUAGE":
	      //TODO: This should be a Language array to handle multiple 
	      //languages but further research is required first.
	      res->Language = replace_string(line, "LANGUAGE ", "");
	      break;

	  case "RESISTANCE":			  
	      tmp = explode(replace_string(line, "RESISTANCE ", ""), ":");
	      x = to_int(tmp[0]);
	      if( x == 0 && tmp[0] != "0" ) x = GetResistance(tmp[0]);
	      res->Resistance[x] = tmp[1];
	      break;

	  case "SKILL":      
	      tmp = explode(replace_string(line, "SKILL ", ""), ":");
	      res->Skills[tmp[0]] = ({ tmp[1], tmp[2], tmp[3], tmp[4] });
	      break;

	  case "STATS":
	      tmp = ({});
	      s = new (class Stat);
	      tmp = explode(replace_string(line, "STATS ",""), ":");
	      s->Average = copy(to_int(tmp[1]));
	      s->Class = copy(to_int(tmp[2]));
	      res->Stats[tmp[0]] = s;
	      break;

	  case "LIMB":
	      limb = ({ ({}), ({}), ({}), ({}) });
	      tmp_limb = explode(replace_string(line, "LIMB ",""), ":");
	      limb[0] = tmp_limb[0];
	      limb[1] = (tmp_limb[1] == "0" ? 0 : tmp_limb[1]);
	      limb[2] = to_int(tmp_limb[2]);
	      limb[3] = map(explode(tmp_limb[3], ","), function(string str) {
		    int x = to_int(str);
		    if( x == 0 && str != "0" ) { return GetArmor(str); }
		    return x;
		  });

		res->Limbs = ({ res->Limbs..., limb });
		res->Limbs += ({limb});
		break;

		case "HAND":
		parts = explode(replace_string(line, "HAND ",""), ":");
		res->Fingers[parts[0]] = to_int(parts[1]);
		break;

		default:
		break;
	    } 
	  }  

	  res->Complete = 1;

	  if( player ) {
	      res->PlayerFlag = 1;
	  }

	  else {
	      res->PlayerFlag = 0;
	  }

	  Races[race] = res;
	  wtf = save_variable(Races[race]);
	  save_object(SAVE_RACES);
      } 

      void RemoveRace(string race) {
	  validate();
	  wtf = save_variable(Races[race]);
	  map_delete(Races, race);
	  if(Races[race]) 
	      save_object(SAVE_RACES);
      }


      int GetArmor(string str) {
	  string file = DIR_DAEMONS "/tmp/" + str + ".c";

	  if( !unguarded((: file_exists($(file)) :)) ) {
	      unguarded((: write_file($(file), "#include <armor_types.h>\n" +
		    "int armor() { return " + $(str) + "; }\n") :));
	  }
	  return call_other(file, "armor"); 
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

	  foreach(string race, class Race res in foo) {
	      mapping stats = ([]);

	      mp[race] = ([]);
	      mp[race]["limbs"] = res->Limbs;
	      mp[race]["resistance"] = res->Resistance;

	      foreach(string stat, class Stat st in res->Stats) {
		  stats[stat] = ([]);
		  stats[stat]["class"] = st->Class;
		  stats[stat]["average"] = st->Average;
	      }

	      mp[race]["stats"] = stats;
	      mp[race]["fingers"] = res->Fingers;
	      mp[race]["sensitivity"] = res->Sensitivity;
	      mp[race]["player"] = res->PlayerFlag;
	      mp[race]["language"] = res->Language;

	  }
	  return mp;
      }

      void SetComplete(string race) {
	  class Race res;

	  validate();

	  if( !Races[race] ) error("No such race");
	  else res = Races[race];
	  res->Complete = 1;
	  save_object(SAVE_RACES);
      }

      void SetLightSensitivity(string race, int array sensitivity) {
	  class Race res;

	  validate();

	  if( !Races[race] ) error("No such race");
	  else res = Races[race];
	  if( sensitivity[0] < 1 ) error("Invalid sensitivity value");
	  if( sensitivity[1] > 99 ) error("Invalid sensitivity value");
	  if( sensitivity[0] > sensitivity[1] ) error("Invalid sensitivity value");
	  res->Sensitivity = sensitivity;
	  save_object(SAVE_RACES);
      }

      void SetCharacterLimbs(string race, mixed array args) {
	  class Race res = Races[race];
	  mixed array tmp = ({});

	  if( !res || !res->Complete || sizeof(args) != 2 ) return;
	  args[0] = copy(res->Limbs);
	  foreach(string finger, int count in res->Fingers)
	  tmp = ({ tmp..., ({ finger, count }) });
	  args[1] = tmp;
      }

      void SetCharacterRace(string race, mixed array args) {
	  class Race res = Races[race];
	  mixed array tmp;
	  mapping StatMap;
	  string schluss;

	  if( !res || !res->Complete || sizeof(args) != 5 ) return;
	  tmp = ({});
	  foreach(int key, string val in res->Resistance)
	  tmp = ({ tmp..., ({ key, val }) });
	  args[0] = tmp;
	  tmp = ({});
	  StatMap = copy(res->Stats);
	  schluss = "";
	  foreach(schluss in keys(StatMap)){
	      tmp = ({ tmp..., ({ schluss, StatMap[schluss]->Average, StatMap[schluss]->Class }) });
	  }
	  args[1] = tmp;
	  args[2] = res->Language;
	  args[3] = res->Sensitivity;
	  args[4] = res->Skills; 
      }

      varargs string array GetRaces(int player_only) {

	  return filter(keys(Races), function(string race, int player_only) {
		class Race res = Races[race];

		if( !res->Complete ) return 0;
		if( player_only && !res->PlayerFlag )
		    return 0;
		return 1;
	    }, player_only);
      }

      string GetHelp(string race) {
	  class Race res = Races[race];
	  string array limbs;
	  string help = "Race: " + race + "\n\n";
	  string tmp, h_file;
	  int x;

	  if( !res ) return 0;
	  h_file = "/doc/help/races/"+lower_case(race);
	  if(file_exists(h_file)) return read_file(h_file); 
	  limbs = map(res->Limbs, (: $1[0] :));
	  limbs = singular_array(limbs);
	  help += "Limbs:\n";
	  help += capitalize(item_list(map(limbs, (: add_article :)))) + ".\n";
	  help += "\nFingered limbs:\n";
	  foreach(string finger, int count in res->Fingers)
	  help += "\t" + finger + " (" + count + ")\n";
	  limbs = regexp(limbs, ".* wing");
	  if( sizeof(limbs) ) {
	      help += "\nFlying\n";
	  }

	  else {
	      help += "\nNon-flying\n";
	  }

	  x = res->Sensitivity[0];
	  if( x < 11 ) tmp = "excellent";
	  else if( x < 16 ) tmp = "above average";
	  else if( x < 21 ) tmp = "good";
	  else if( x < 26 ) tmp = "average";
	  else if( x < 31 ) tmp = "below average";
	  else if( x < 36 ) tmp = "very poor";    
	  else tmp = "extremely poor";
	  help += "\nNight vision: " + tmp + "\n";
	  x = res->Sensitivity[1];
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


