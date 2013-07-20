/*    /daemon/create.c
 *    From Dead Souls LPMud
 *    Mediates requests to create and modify objects
 *    You are not expected to understand this.
 */

#include <lib.h>
#include <dirs.h>
#include <daemons.h>
#include <modules.h>
#include <commands.h>
#include ROOMS_H


int eventDelete(object ob, string value);
string global1, global2, globaltmp, globalvalue;

string *base_arr = ({"set_heart_beat", "SetUnique", "SetNoClean","SetNoModify","SetProperties","SetLong","SetShort","SetItems","SetListen","SetSmell","SetInvis"});
string *item_arr = base_arr + ({"SetLanguage","SetRead","SetDefaultRead","SetDisableChance", "SetDamagePoints", "SetVendorType","SetNoCondition","SetMoney","SetKeyName", "SetId", "SetMass","SetCost","SetValue","SetAdjectives","SetDamagePoints","SetBaseCost", "SetPreventGet", "SetPreventPut", "SetDestructOnDrop", "SetPreventDrop" });
string *meal_arr = item_arr + ({ "SetMealType", "SetStrength"}) -({"SetDamagePoints"});
string *storage_arr = item_arr + ({"SetOpacity", "SetMaxCarry","SetInventory", "SetCanClose", "SetCanLock","SetMaxRecurse","SetLocked","SetClosed","SetKey"});
string *room_arr = base_arr - ({"SetUnique"}) + ({"SetFlowLimit","SetFlyRoom", "SetSinkRoom", "SetTerrainType","AddTerrainType","SetLanguage", "SetRead", "SetDefaultRead", "SetNoObviousExits","SetDefaultExits","SetTown","SetNightLong","SetDayLong","SetClimate","SetAmbientLight","SetNightLight","SetDayLight","SetObviousExits", "SetInventory", "SetEnters","SetSkyDomain"});
string *npc_arr = base_arr - ({"SetItems"}) + ({"SetMass","SetBodyType","SetSize","SetRespiration","SetMount","SetCanBefriend","SetDefaultLanguage","SetNativeLanguage","SetCustomXP", "SetSpellBook", "SetCanBite", "SetWimpy","SetWimpyCommand","SetPacifist", "SetBodyComposition", "SetSleeping","SetPermitLoad", "SetAutoStand","SetCurrency","SetSkills","SetStats","SetKeyName", "SetId", "SetLevel", "SetRace", "SetClass","SetGender", "SetInventory", "SetHealthPoints","SetMaxHealthPoints", "SetAdjectives", "SetMelee", "SetPosition", "SetWanderSpeed", "SetEncounter", "SetMorality", "SetHeartBeat", "SetNoCondition", "SetAttackable", "SetPolyglot", "SetPreventGet", "SetMount", "CanBefriend"});
string *barkeep_arr = npc_arr + ({"SetLocalCurrency","SetMenuItems"});
string *trainer_arr = npc_arr + ({"SetNoSpells", "AddTrainingSkills"});
string *vendor_arr = npc_arr + ({"SetLocalCurrency","SetStorageRoom","SetMaxItems","SetVendorType"});
string *armor_arr = item_arr +({"SetRestrictLimbs","SetProtection","SetArmorType"});
string *weapon_arr = item_arr + ({"SetClass","SetWeaponType","SetDamageType","SetHands"});
string *chair_arr = item_arr + ({"SetMaxSitters","SetMaxCarry","SetInventory"});
string *bed_arr = chair_arr + ({"SetMaxLiers"});
string *table_arr = storage_arr + bed_arr;
string *door_arr = ({"SetHiddenDoor", "SetLong","SetShort","SetLocked","SetClosed","SetCanLock","SetKey","SetId"});
string *book_arr = item_arr + ({"SetTitle","SetSource"});
string *worn_storage_arr = armor_arr + storage_arr;
string *vehicle_arr =  npc_arr + ({"SetVehicleInterior"});

string *all_arr = storage_arr + door_arr + room_arr + barkeep_arr + armor_arr + weapon_arr + bed_arr +meal_arr + vendor_arr +trainer_arr;


string GetSettings(string str){
    string ret;
    string *name = ({});
    ret = "";
    switch(str){
        case "room" : name = room_arr; break;
        case "npc" : name = npc_arr; break;
        case "mob" : name = npc_arr; break;
        case "barkeep" : name = barkeep_arr; break;
        case "vendor" : name = vendor_arr; break;
        case "trainer" : name = trainer_arr; break;
        case "armor" : name = armor_arr; break;
        case "armour" : name = armor_arr; break;
        case "weapon" : name = weapon_arr; break;
        case "item" : name = item_arr; break;
        case "door" : name = door_arr; break;
        case "meal" : name = meal_arr; break;
        case "storage" : name = storage_arr; break;
        case "table" : name = table_arr; break;
        case "bed" : name = bed_arr; break;
        case "chair" : name = chair_arr; break;
        case "book" : name = book_arr; break;
        case "worn storage" : name = worn_storage_arr; break;
        case "wornstorage" : name = worn_storage_arr; break;
        case "worn_storage" : name = worn_storage_arr; break;
        case "vehicle" : name = vehicle_arr; break;
        default : name = room_arr;
    }
    foreach(string thing in name){
        ret += thing+", ";
    }
    ret = truncate(ret,2)+".";
    return ret;
}

mapping QueryMap(string str,object ob){
    switch(str){
        case "SetItems" : return ob->GetItemsMap();break;
        case "SetSmell" : return ob->GetSmellMap();break;
        case "SetListen" : return ob->GetListenMap();break;
        case "SetInventory" : return ob->GetInventory();break;
        case "SetEnters" : return ob->GetEnterMap();break;
        case "SetMenuItems" : return ob->GetMenuItems();break;
        case "SetSpellBook" : return ob->GetSpellBook();break;
        case "SetRead" : return ob->GetReadsMap();break;
        default : return ([]);
    }

}

mixed eventModify(object ob, string str){
    string inheritance, out, tmpfile, filename, mode, metamode;
    mixed value, mixed_tmp;
    int invalid, setmap;
    mapping temp_map;
    string *p_array, *array_props, *special_map_array;;

    invalid = 1;

    inheritance = "";

    if(ob->GetDoor() && sizeof(ob->GetDoor())) {
        inheritance = "door";
        ob = load_object(ob->GetDoor());
    }

    filename = base_name(ob)+".c";
    tmpfile = generate_tmp(ob);
    special_map_array = ({ "SetProperties", "SetStats", "SetSkills"});

    if(!check_privs(this_player(),filename)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }

    if(!check_privs(this_player(),tmpfile)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }

    this_object()->eventGeneralStuff(filename);
    global1 = filename;
    global2 = tmpfile;
    unguarded( (: cp(global1,global2) :) );

    if(inherits(LIB_ROOM,ob)) inheritance += " room";
    if(inherits(LIB_NPC,ob)) inheritance += " npc";
    if(inherits(LIB_SENTIENT,ob)) inheritance += " npc";
    if(inherits(LIB_BARKEEP,ob)) inheritance += " barkeep";
    if(inherits(LIB_VENDOR,ob)) inheritance += " vendor";
    if(inherits(LIB_TRAINER,ob)) inheritance += " trainer";
    if(inherits(LIB_WEAPON,ob)) inheritance += " weapon";
    if(inherits(LIB_ARMOR,ob)) inheritance += " armor";
    if(inherits(LIB_ITEM,ob)) inheritance += " item";
    if(inherits(LIB_STORAGE,ob)) inheritance += " storage";
    if(inherits(LIB_CHAIR,ob)) inheritance += " chair";
    if(inherits(LIB_BED,ob)) inheritance += " bed";
    if(inherits(LIB_TABLE,ob)) inheritance += " table";
    if(inherits(LIB_MEAL,ob)) inheritance += " meal";
    if(inherits(LIB_BOOK,ob)) inheritance += " book";
    if(inherits(LIB_DOOR,ob)) inheritance += " door";
    if(inherits(LIB_WORN_STORAGE,ob)) inheritance += " worn_storage";
    if(inherits(LIB_VEHICLE,ob)) inheritance += " vehicle";

    if(!inheritance || inheritance == ""){
        write("The object you want to modify lacks an init() function.");
        write("Please correct this by issuing the initfix command, then try again.");
        write("\nIf you are certain the object has a working init(), then");
        write("This error indicates that its library object type is not");
        write("currently supported by the Quick Creation System.");
        return 1;
    }

    if(sscanf(str,"%s %s %d",mode, metamode, value) != 3) sscanf(str,"%s %s %s",mode, metamode, value);
    if(member_array(mode,special_map_array) == -1){
        if(sscanf(str,"%s %d",mode, value) != 2) sscanf(str,"%s %s",mode, value); 
    }
    if(!value) {
        value = 0;
        if(!mode) mode = str;
    }

    if(mode == "delete") {
        if(value) mode = value;
        value = "delete";
    }

    setmap = 0;
    switch(lower_case(mode)){
        case "short" : out = "SetShort";break;
        case "setshort" : out = "SetShort";break;
        case "long" : out = "SetLong";break;
        case "setlong" : out = "SetLong";break;
        case "setnightlong" : out = "SetNightLong";break;
        case "nightlong" : out = "SetNightLong";break;
        case "daylong" : out = "SetDayLong";break;
        case "setdaylong" : out = "SetDayLong";break;
        case "climate" : out = "SetClimate";break;
        case "setclimate" : out = "SetClimate";break;
        case "light" : out = "SetAmbientLight";break;
        case "ambientlight" : out = "SetAmbientLight";break;
        case "setlight" : out = "SetAmbientLight";break;
        case "nightlight" : out = "SetNightLight";break;
        case "setnightlight" : out = "SetNightLight";break;
        case "daylight" : out = "SetDayLight";break;
        case "setdaylight" : out = "SetDayLight";break;
        case "obvious" : out = "SetObviousExits";break;
        case "setobvious" : out = "SetObviousExits";break;
        case "obviousexits" : out = "SetObviousExits";break;
        case "setobviousexits" : out = "SetObviousExits";break;
        case "town" : out = "SetTown";break;
        case "settown" : out = "SetTown";break;
        case "keyname" : out = "SetKeyName";break;
        case "setkeyname" : out = "SetKeyName";break;
        case "name" : out = "SetKeyName";break;
        case "setname" : out = "SetKeyName";break;
        case "id" : out = "SetId";break;
        case "setid" : out = "SetId";break;
        case "class" : out = "SetClass";break;
        case "setclass" : out = "SetClass";break;
        case "maxhealth" : out = "SetMaxHealthPoints";break;
        case "maxhealthpoints" : out = "SetMaxHealthPoints";break;
        case "setmaxhealth" : out = "SetMaxHealthPoints";break;
        case "setmaxhealthpoints" : out = "SetMaxHealthPoints";break;
        case "level" : out = "SetLevel";break;
        case "setlevel" : out = "SetLevel";break;
        case "race" : out = "SetRace";break;
        case "setrace" : out = "SetRace";break;
        case "health" : out = "SetHealthPoints";break;
        case "healthpoints" : out = "SetHealthPoints";break;
        case "sethealth" : out = "SetHealthPoints";break;
        case "sethealthpoints" : out = "SetHealthPoints";break;
        case "gender" : out = "SetGender";break;
        case "setgender" : out = "SetGender";break;
        case "mass" : out = "SetMass";break;
        case "setmass" : out = "SetMass";break;
        case "weight" : out = "SetMass";break;
        case "setweight" : out = "SetMass";break;
        case "cost" : out = "SetCost";break;
        case "setcost" : out = "SetCost";break;
        case "value" : out = "SetValue";break;
        case "setvalue" : out = "SetValue";break;
        case "dollarcost" : out = "SetDollarCost";break;
        case "setdollarcost" : out = "SetDollarCost";break;
        case "adjectives" : out = "SetAdjectives";break;
        case "setadjectives" : out = "SetAdjectives";break;
        case "adj" : out = "SetAdjectives";break;
        case "setadj" : out = "SetAdjectives";break;
        case "adjs" : out = "SetAdjectives";break;
        case "setadjs" : out = "SetAdjectives";break;
        case "damage" : out = "SetDamagePoints";break;
        case "setdamage" : out = "SetDamagePoints";break;
        case "damagepoints" : out = "SetDamagePoints";break;
        case "setdamagepoints" : out = "SetDamagePoints";break;
        case "restrictlimbs" : out = "SetRestrictLimbs";break;
        case "setrestrictlimbs" : out = "SetRestrictLimbs";break;
        case "protection" : out = "SetProtection";break;
        case "setprotection" : out = "SetProtection";break;
        case "setprotections" : out = "SetProtection";break;
        case "protections" : out = "SetProtection";break;
        case "armortype" : out = "SetArmorType";break;
        case "setarmortype" : out = "SetArmorType";break;
        case "weapontype" : out = "SetWeaponType";break;
        case "setweapontype" : out = "SetWeaponType";break;
        case "damagetype" : out = "SetDamageType";break;
        case "setdamagetype" : out = "SetDamageType";break;
        case "hands" : out = "SetHands";break;
        case "sethands" : out = "SetHands";break;
        case "carry" : out = "SetMaxCarry";break;
        case "maxcarry" : out = "SetMaxCarry";break;
        case "setmaxcarry" : out = "SetMaxCarry";break;
        case "closed" : out = "SetClosed";break;
        case "setclosed" : out = "SetClosed";break;
        case "locked" : out = "SetLocked";break;
        case "setlocked" : out = "SetLocked";break;
        case "canclose" : out = "SetCanClose";break;
        case "setcanclose" : out = "SetCanClose";break;
        case "canlock" : out = "SetCanLock";break;
        case "setcanlock" : out = "SetCanLock";break;
        case "key" : out = "SetKey";break;
        case "setkey" : out = "SetKey";break;
        case "recurse" : out = "SetMaxRecurse";break;
        case "maxrecurse" : out = "SetMaxRecurse";break;
        case "setmaxrecurse" : out = "SetMaxRecurse";break;
        case "sitters" : out = "SetMaxSitters";break;
        case "maxsitters" : out = "SetMaxSitters";break;
        case "setmaxsitters" : out = "SetMaxSitters";break;
        case "liers" : out = "SetMaxLiers";break;
        case "maxliers" : out = "SetMaxLiers";break;
        case "setmaxliers" : out = "SetMaxLiers";break;
        case "item" : out = "SetItems";setmap = 1;break;
        case "setitem" : out = "SetItems";setmap = 1;break;
        case "items" : out = "SetItems";setmap = 1;break;
        case "setitems" : out = "SetItems";setmap = 1;break;
        case "inventory" : out = "SetInventory";setmap = 1;break;
        case "inv" : out = "SetInventory";setmap = 1;break;
        case "setinv" : out = "SetInventory";setmap = 1;break;
        case "setinventory" : out = "SetInventory";setmap = 1;break;
        case "smell" : out = "SetSmell";setmap = 1;break;
        case "setsmell" : out = "SetSmell";setmap = 1;break;
        case "listen" : out = "SetListen";setmap = 1;break;
        case "setlisten" : out = "SetListen";setmap = 1;break;
        case "enter" : out = "SetEnters";setmap = 1;break;
        case "enters" : out = "SetEnters";setmap = 1;break;
        case "setenters" : out = "SetEnters";setmap = 1;break;
        case "setenter" : out = "SetEnters";setmap = 1;break;
        case "exit" : out = "SetExits";setmap = 1;break;
        case "exits" : out = "SetExits";setmap = 1;break;
        case "setexits" : out = "SetExits";setmap = 1;break;
        case "setexit" : out = "SetExits";setmap = 1;break;
        case "setproperty" : out = "SetProperties";break;
        case "property" : out = "SetProperties";break;
        case "setproperties" : out = "SetProperties";break;
        case "properties" : out = "SetProperties";break;
        case "props" : out = "SetProperties";break;
        case "setprops" : out = "SetProperties";break;
        case "setskill" : out = "SetSkills";break;
        case "skill" : out = "SetSkills";break;
        case "skills" : out = "SetSkills";break;
        case "setskills" : out = "SetSkills";break;
        case "setstats" : out = "SetStats";break;
        case "stats" : out = "SetStats";break;
        case "stat" : out = "SetStats";break;
        case "setstat" : out = "SetStats";break;
        case "value" : out = "SetBaseCost";break;
        case "setvalue" : out = "SetBaseCost";break;
        case "val" : out = "SetBaseCost";break;
        case "setval" : out = "SetBaseCost";break;
        case "cost" : out = "SetBaseCost";break;
        case "setcost" : out = "SetBaseCost";break;
        case "basecost" : out = "SetBaseCost";break;
        case "setbasecost" : out = "SetBaseCost";break;
        case "setcurrency" : out = "SetCurrency";break;
        case "currency" : out = "SetCurrency";break;
        case "setcurr" : out = "SetCurrency";break;
        case "curr" : out = "SetCurrency";break;
        case "setmoney" : out = "SetMoney";break;
        case "money" : out = "SetMoney";break;
        case "setnocondition" : out = "SetNoCondition";break;
        case "nocondition" : out = "SetNoCondition";break;
        case "mealtype" : out = "SetMealType";break;
        case "setmealtype" : out = "SetMealType";break;
        case "strength" : out = "SetStrength";break;
        case "setstrength" : out = "SetStrength";break;
        case "setmealstrength" : out = "SetStrength";break;
        case "mealstrength" : out = "SetStrength";break;
        case "vendor" : out = "SetVendorType";break;
        case "setvendor" : out = "SetVendorType";break;
        case "setvendortype" : out = "SetVendorType";break;
        case "vendortype" : out = "SetVendorType";break;
        case "settitle" : out = "SetTitle";break;
        case "title" : out = "SetTitle";break;
        case "source" : out = "SetSource";break;
        case "setsource" : out = "SetSource";break;
        case "melee" : out = "SetMelee";break;
        case "setmelee" : out = "SetMelee";break;
    }

    if(!out){
        switch(mode){
            case "position" : out = "SetPosition";break;
            case "setposition" : out = "SetPosition";break;
            case "autostand" : out = "SetAutoStand";break;
            case "setautostand" : out = "SetAutoStand";break;
            case "damagepoints" : out = "SetDamagePoints";break;
            case "setdamagepoints" : out = "SetDamagePoints";break;
            case "wanderspeed" : out = "SetWanderSpeed";break;
            case "setwanderspeed" : out = "SetWanderSpeed";break;
            case "encounter" : out = "SetEncounter";break;
            case "setencounter" : out = "SetEncounter";break;
            case "sethostile" : out = "SetEncounter";break;
            case "hostile" : out = "SetEncounter";break;
            case "aggressive" : out = "SetEncounter";break;
            case "setaggressive" : out = "SetEncounter";break;
            case "morality" : out = "SetMorality";break;
            case "setmorality" : out = "SetMorality";break;
            case "setheartbeat" : out = "SetHeartBeat";break;
            case "heartbeat" : out = "SetHeartBeat";break;
            case "include" : out = "include";break;
            case "inherit" : out = "inherit";break;
            case "load" : out = "SetPermitLoad";break;
            case "permitload" : out = "SetPermitLoad";break;
            case "setpermitload" : out = "SetPermitLoad";break;
            case "modify" : if(value && intp(value)) value = bool_reverse(value);out = "SetNoModify";break;
            case "setmodify" : if(value && intp(value)) value = bool_reverse(value); out = "SetNoModify";break;
            case "setnomodify" : out = "SetNoModify";break;
            case "nomodify" : out = "SetNoModify";break;
            case "open" : if(value && intp(value)) value = bool_reverse(value);out = "SetClosed";break;
            case "setopen" : if(value && intp(value)) value = bool_reverse(value);out = "SetClosed";break;
            case "sleep" : out = "SetSleeping";break;
            case "setsleep" : out = "SetSleeping";break;
            case "sleeping" : out = "SetSleeping";break;
            case "setsleeping" : out = "SetSleeping";break;
            case "disable" : out = "SetDisableChance";break;
            case "disablechance" : out = "SetDisableChance";break;
            case "setdisable" : out = "SetDisableChance";break;
            case "setdisablechance" : out = "SetDisableChance";break;
            case "noclean" : out = "SetNoClean";break;
            case "setnoclean" : out = "SetNoClean";break;
            case "bodycomp" : out = "SetBodyComposition";break;
            case "bodycomposition" : out = "SetBodyComposition";break;
            case "setbodycomposition" : out = "SetBodyComposition";break;
            case "setpacifist" : out = "SetPacifist";break;
            case "pacifist" : out = "SetPacifist";break;
            case "unique" : out = "SetUnique";break;
            case "setunique" : out = "SetUnique";break;
            case "setwimpy" : out = "SetWimpy";break;
            case "wimpy" : out = "SetWimpy";break;
            case "wimpycommand" : out = "SetWimpyCommand";break;
            case "setwimpycommand" : out = "SetWimpyCommand";break;
            case "setcanbite" : out = "SetCanBite";break;
            case "canbite" : out = "SetCanBite";break;
            case "localcurrency" : out = "SetLocalCurrency";break;
            case "setlocalcurrency" : out = "SetLocalCurrency";break;
            case "menuitems" : out = "SetMenuItems";setmap = 1;break;
            case "setmenuitems" : out = "SetMenuItems";setmap = 1;break;
            case "menu" : out = "SetMenuItems";setmap = 1;break;
            case "maxitems" : out = "SetMaxItems";break;
            case "setmaxitems" : out = "SetMaxItems";break;
            case "setstorageroom" : out = "SetStorageRoom";break;
            case "storageroom" : out = "SetStorageRoom";break;
            case "defaultread" : out = "SetDefaultRead";break;
            case "setdefaultread" : out = "SetDefaultRead";break;
            case "setread" : out = "SetRead";setmap = 1;break;
            case "read" : out = "SetRead";setmap = 1;break;
            case "language" : out = "SetLanguage";break;
            case "lang" : out = "SetLanguage";break;
            case "setlang" : out = "SetLanguage";break;
            case "setlanguage" : out = "SetLanguage";break;
            case "setspellbook" : out = "SetSpellBook";break;
            case "spellbook" : out = "SetSpellBook";break;
            case "spells" : out = "SetSpellBook";break;
            case "addtrainingskills" : out = "AddTrainingSkills";break;
            case "settrainingskills" : out = "AddTrainingSkills";break;
            case "trainingskills" : out = "AddTrainingSkills";break;
            case "training" : out = "AddTrainingSkills";break;
            case "nospells" : out = "SetNoSpells";break;
            case "setnospells" : out = "SetNoSpells";break;
            case "defaultexits" : out = "SetDefaultExits";break;
            case "setdefaultexits" : out = "SetDefaultExits";break;
            case "setnoobviousexits" : out = "SetNoObviousExits";break;
            case "noobviousexits" : out = "SetNoObviousExits";break;
            case "xp" : out = "SetCustomXP";break;
            case "setxp" : out = "SetCustomXP";break;
            case "setcustomxp" : out = "SetCustomXP";break;
            case "customxp" : out = "SetCustomXP";break;
            case "nativelanguage" : out = "SetNativeLanguage";break;
            case "setnativelanguage" : out = "SetNativeLanguage";break;
            case "heartbeat" : out = "set_heart_beat";break;
            case "setheartbeat" : out = "set_heart_beat";break;
            case "set_heart_beat" : out = "set_heart_beat";break;
            case "heart_beat" : out = "set_heart_beat";break;
            case "hiddendoor" : out = "SetHiddenDoor";break;
            case "sethiddendoor" : out = "SetHiddenDoor";break;
            case "opacity" : out = "SetOpacity";break;
            case "setopacity" : out = "SetOpacity";break;
            case "setterraintype" : out = "SetTerrainType";break;
            case "terraintype" : out = "AddTerrainType";break;
            case "addterraintype" : out = "AddTerrainType";break;
            case "terrain" : out = "AddTerrainType";break;
            case "defaultlanguage" : out = "SetDefaultLanguage";break;
            case "defaultlang" : out = "SetDefaultLanguage";break;
            case "setdefaultlanguage" : out = "SetDefaultLanguage";break;
            case "bodytype" : out = "SetBodyType";break;
            case "setbodytype" : out = "SetBodyType";break;
            case "setsize" : out = "SetSize";break;
            case "size" : out = "SetSize";break;
            case "respiration" : out = "SetRespiration";break;
            case "setrespiration" : out = "SetRespiration";break;
            case "setinvis" : out = "SetInvis";break;
            case "invis" : out = "SetInvis";break;
            case "attackable" : out = "SetAttackable";break;
            case "setattackable" : out = "SetAttackable";break;
            case "flowlimit" : out = "SetFlowLimit";break;
            case "setflowlimit" : out = "SetFlowLimit";break;
            case "flow" : out = "SetFlowLimit";break;
        }
    }

    if(!out){
        switch(mode){
            case "polyglot" : out = "SetPolyglot";break;
            case "setpolyglot" : out = "SetPolyglot";break;
            case "setvehicleinterior" : out = "SetVehicleInterior";break;
            case "vehicleinterior" : out = "SetVehicleInterior";break;
            case "interior" : out = "SetVehicleInterior";break;
            case "preventget" : out = "SetPreventGet";break;
            case "setpreventget" : out = "SetPreventGet";break;
            case "setpreventdrop" : out = "SetPreventDrop";break;
            case "preventdrop" : out = "SetPreventDrop";break;
            case "setdestructondrop" : out = "SetDestructOnDrop";break;
            case "destructondrop" : out = "SetDestructOnDrop";break;
            case "flyroom" : out = "SetFlyRoom";break;
            case "setflyroom" : out = "SetFlyRoom";break;
            case "setsinkroom" : out = "SetSinkRoom";break;
            case "sinkroom" : out = "SetSinkRoom";break;
            case "skydomain" : out = "SetSkyDomain";break;
            case "setskydomain" : out = "SetSkyDomain";break;
            case "befriend" : out = "CanBefriend";break;
            case "setbefriend" : out = "CanBefriend";break;
            case "setcanbefriend" : out = "CanBefriend";break;
            case "canbefriend" : out = "CanBefriend";break;
            case "mount" : out = "SetMount";break;
            case "setmount" : out = "SetMount";break;
            default : out = mode;
        }
    }

    if(!value) value = 0;

    if(value == "delete") {
        eventDelete(ob, out);
        unguarded( (: rm(global2) :) );
        return 1;
    }

    if(out == "SetExits") {
        write("SetExits is a special setting, which isn't modified like others.");
        write("To make an exit to a room, or to create a new room, type:\n");
        write("create room DIRECTION FILE");
        write("For example: create room east test_room1");
        write("To get rid of an exit, it's: delete exit DIRECTION");
        write("For example: delete exit east");
        return 1;
    }

    if(out == "SetEnters") {
        write("SetEnters is a special setting, which isn't modified like others.");
        write("To make an Enter, first identify an item that already exists ");
        write("in SetItems. For example, if SetItems contains a pub:\n");
        write("create enter pub test_pub1\n");
        write("To get rid of that enter: delete enter pub");
        write("Please note that if the \"thing to be entered\" isn't already ");
        write("in SetItems, things won't work right.");
        return 1;
    }

    if(out == "SetInventory"){
        write("SetInventory is a special setting, which isn't modified like others.");
        write("To add something to something else's inventory, the \"thing ");
        write("to be added\" has to be in your environment, or carried by you.");
        write("So if you want to add a chair to your sample room:\n");
        write("home");
        write("go east");
        write("cd /domains/town/obj");
        write("clone chair");
        write("add chair to room");
        write("1\n");
        write("If you want to add a sword to your fighter: \n");
        write("clone sword");
        write("add sword to fighter");
        write("wield sword\n");
        write("To remove items from a thing's permanent inventory:");
        write("delete chair");
        write("delete sword from fighter");
        return 1;
    }

    if(out == "include" || out == "inherit"){
        this_object()->eventModHeader(ob, out,value);
        return 1;
    }

    if(grepp(inheritance,"room") && member_array(out,room_arr) != -1) invalid = 0;
    if(grepp(inheritance,"barkeep") && member_array(out,barkeep_arr) != -1) invalid = 0;
    if(grepp(inheritance,"vendor") && member_array(out,vendor_arr) != -1) invalid = 0;
    if(grepp(inheritance,"trainer") && member_array(out,trainer_arr) != -1) invalid = 0;
    if(grepp(inheritance,"npc") && member_array(out,npc_arr) != -1) invalid = 0;
    if(grepp(inheritance,"weapon") && member_array(out,weapon_arr) != -1) invalid = 0;
    if(grepp(inheritance,"armor") && member_array(out,armor_arr) != -1) invalid = 0;
    if(grepp(inheritance,"storage") && member_array(out,storage_arr) != -1) invalid = 0;
    if(grepp(inheritance,"chair") && member_array(out,chair_arr) != -1) invalid = 0;
    if(grepp(inheritance,"bed") && member_array(out,bed_arr) != -1) invalid = 0;
    if(grepp(inheritance,"table") && member_array(out,table_arr) != -1) invalid = 0;
    if(grepp(inheritance,"meal") && member_array(out,meal_arr) != -1) invalid = 0;
    if(grepp(inheritance,"door") && member_array(out,door_arr) != -1) invalid = 0;
    if(grepp(inheritance,"book") && member_array(out,book_arr) != -1) invalid = 0;
    if(grepp(inheritance,"worn_storage") && member_array(out,worn_storage_arr) != -1) invalid = 0;
    if(grepp(inheritance,"vehicle") && member_array(out,vehicle_arr) != -1) invalid = 0;
    else if(grepp(inheritance,"item") && member_array(out,item_arr) != -1) invalid = 0;

    if(invalid) {
        write("Invalid property.");
        return 1;
    }
    if(out == "SetProtection"){
        this_object()->eventStartArmorQuestions(value,ob);
        return 1;
    }

    if(out == "SetBaseCost"){
        this_object()->eventModCost(ob, metamode, value);
        return 1;
    }

    if(member_array(out,special_map_array) != -1) {
        this_object()->eventSpecialMapHandler(ob,out,metamode,value);
        return 1;
    }

    if(out == "SetMoney" || out == "SetCurrency"){
        this_object()->eventModMoney(ob, metamode, value);
        return 1;
    }

    if(setmap == 1) {
        temp_map = QueryMap(out, ob);
        temp_map = this_object()->eventStartMappingQuestions(temp_map, ob, tmpfile, out);
        return 1;
    }

    array_props = ({"AddTrainingSkills", "SetId","SetAdjectives","SetRestrictLimbs"});
    if(member_array(out,array_props) != -1){
        this_object()->eventStartGenericQuestions(ob, tmpfile, ({value}), out);
        return 1;
    }

    if(grepp(inheritance,"door") && out != "SetHiddenDoor" ){
        string cote = this_object()->eventEvaluateDoorSide(ob);
        if(cote) this_object()->eventProcessDoor(ob, out, value, cote);
        else this_object()->eventProcessDoor(ob, out, value);
        return 1;
    }

    else {
        switch(out){
            case "SetLong" : p_array = ({"SetShort","SetAmbientLight","SetDayLight","SetNightLight","create()","create ()","create"}); break;
            case "SetDayLong" : p_array = ({"SetShort","SetAmbientLight","SetDayLight","SetNightLight","SetNightLong","SetLong","create()","create ()","create"}); break;
            case "SetNightLong" : p_array = ({"SetShort","SetAmbientLight","SetDayLight","SetNightLight","SetDayLong","SetLong","create()","create ()","create"}); break;
            case "SetShort" : p_array = ({"SetAmbientLight","SetDayLight","SetNightLight","create()","create ()","create"}); break;
            case "SetHealthPoints" : p_array = ({"SetInventory","SetMaxHealhPoints","SetClass","SetRace","SetLong"});break;
            case "SetMaxHealthPoints" : p_array = ({"SetInventory","SetHealhPoints","SetClass","SetRace","SetLong"});break;
            case "SetNativeLanguage" : p_array = ({"SetRace"});break;
            case "SetSize" : p_array = ({"SetRace"});break;
            case "SetBodyType" : p_array = ({"SetRace"});break;
            case "SetRespiration" : p_array = ({"SetRace"});break;
            case "SetClass" : p_array = ({"SetRace","SetLong"});break;
            case "SetMaxHealthPoints" : p_array = ({"SetKeyName","SetId"});break;
            case "SetStats" : p_array = ({"SetRace"});break;
            case "SetMass" : p_array = ({"SetRace","SetLong"});break;
            case "SetSmell" : p_array = ({"SetItems","SetInventory","SetListen","SetLong","SetDayLong","SetNightLong","SetShort"});break;
            case "SetListen" : p_array = ({"SetItems","SetInventory","SetSmell","SetLong","SetDayLong","SetNightLong","SetShort"});break;
            case "SetItems" : p_array = ({"SetAmbientLight","SetDayLight","SetNightLight","create()","create ()","SetShort","SetLong","SetDayLong","SetNightLong"});break;
            case "SetAmbientLight" : p_array = ({"SetDayLight","SetNightLight","create()","create ()","SetShort","SetLong","SetDayLong","SetNightLong"});break;
            case "SetDayLight" : p_array = ({"SetAmbientLight","SetNightLight","create()","create ()","SetShort","SetLong","SetDayLong","SetNightLong"});break;
            case "SetNightLight" : p_array = ({"SetAmbientLight","SetDayLight","create()","create ()","SetShort","SetLong","SetDayLong","SetNightLong"});break;
            case "SetHiddenDoor" : p_array = ({ "SetClosed", "SetLocked" }); break;
            default : p_array = ({"SetItems","SetLong","SetDayLong","SetNightLong","SetShort"});
        }

        this_object()->eventModString(tmpfile, out, value, p_array);
    }
    mixed_tmp = reload(tmpfile);
    if(!mixed_tmp || !intp(mixed_tmp)) {
        write("This would screw up your file. Aborting modification.");
        return 1;
    }
    this_object()->eventGeneralStuff(tmpfile);
    global1 = tmpfile;
    global2 = filename;
    unguarded( (: cp(global1,global2) :) );
    reload(ob);

    rm(tmpfile);
    return MODULES_CREATE+" Done.";
}

int eventDelete(object ob, string value){
    string tmpfile, filename;
    mixed mixed_tmp;

    filename = base_name(ob)+".c";
    tmpfile = generate_tmp(ob);


    if(!check_privs(this_player(),filename)){

        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }

    if(!check_privs(this_player(),tmpfile)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }

    global1 = filename;
    global2 = tmpfile;
    unguarded( (: cp(global1,global2) :) );
    mixed_tmp = reload(tmpfile);
    if(!mixed_tmp || !intp(mixed_tmp)) {
        write("Target file is screwed up. Aborting delete.");
        return 1;
    }
    globalvalue = value;
    unguarded( (: globaltmp = remove_matching_line(read_file(global2),globalvalue,1) :) );
    unguarded( (: write_file(global2,globaltmp,1) :) );
    mixed_tmp = reload(tmpfile);
    if(!mixed_tmp || !intp(mixed_tmp)) {
        write("This change would screw up your file. Aborting delete.");
        return 1;
    }
    else unguarded( (: cp(global2, global1) :) );
    reload(ob);
    unguarded( (: rm(global2) :) );
    write("Setting deleted.");
    return 1;
}

int eventResumeArrayMod(object target, string tmpfile, string *NewArr, string func){
    string filename, ret, array_string;
    string *p_array;
    mixed mx;
    filename = base_name(target)+".c";

    if(!check_privs(this_player(),filename)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }
    global2 = tmpfile;
    global1 = filename;
    unguarded( (: cp(global1, global2) :) );
    array_string = func + "( ({";
    foreach(string foo in NewArr){
        array_string += "\""+foo+"\", ";
    }
    array_string = truncate(array_string, 2);
    array_string +=  "}) );";
    switch(func){
        case "SetId" : p_array = ({"SetName","SetKeyName","create()","create ()","SetAdjectives"});break;
        case "SetAdjectives" : p_array = ({"SetName","SetKeyName","create()","create ()","SetId"});break;
        case "SetRestrictLimbs" : p_array = ({ "SetArmorType","SetDamagePoints","SetMass"});break;
        default : p_array = ({"SetLong","SetShort","SetDayLong","SetNightLong"});
    }
    ret = remove_matching_line(read_file(tmpfile),func);
    ret = this_object()->eventAppend(ret,p_array,"\n"+array_string+"\n");
    globaltmp = ret;
    unguarded( (: write_file(global2,globaltmp,1) :) );
    this_object()->eventGeneralStuff(tmpfile);
    mx = reload(tmpfile);
    if(!mx || !intp(mx)){
        write("This change would screw up the object. Aborting.");
        return 1;
    }
    global1 = tmpfile;
    global2 = filename;
    unguarded( (: cp(global1, global2) :) );
    reload(target);
    write(func+" modification complete.");
    unguarded( (: rm(global1) :) );
    return 1;
}

int eventResumeMappingChange(object target, string tmpfile, mapping NewMap, string func){
    string map_string, filename, ret;
    mixed mx;
    string *p_array;
    map_string = this_object()->eventStringifyMap(NewMap);
    map_string = func+"("+map_string+");";
    filename = base_name(target)+".c";

    if(!check_privs(this_player(),filename)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }

    global2 = tmpfile;
    global1 = filename;
    unguarded( (: cp(global1, global2) :) );

    ret = remove_matching_line(read_file(tmpfile),func);
    switch(func){
        case "SetHealthPoints" : p_array = ({"SetInventory","SetMaxHealhPoints","SetClass","SetRace","SetLong"});break;
        case "SetMaxHealthPoints" : p_array = ({"SetClass","SetRace","SetHealthPoints","SetLong"});break;
        case "SetSmell" : p_array = ({"SetItems","SetInventory","SetListen","SetLong","SetDayLong","SetNightLong","SetShort"});break;
        case "SetListen" : p_array = ({"SetItems","SetInventory","SetSmell","SetLong","SetDayLong","SetNightLong","SetShort"});break;
        default : p_array = ({"SetItems","SetLong","SetDayLong","SetNightLong","SetShort"});
    }
    ret = this_object()->eventAppend(ret,p_array,"\n"+map_string+"\n");
    global1 = tmpfile;
    global2 = ret;
    unguarded( (: write_file(global1,global2,1) :) );
    mx = reload(tmpfile);
    if(!mx || !intp(mx)){
        write("This change would screw up the object. Aborting.");
        return 1;
    }
    this_object()->eventGeneralStuff(tmpfile);
    global1 = tmpfile;
    global2 = filename;
    unguarded( (: cp(global1, global2) :) );
    if(target) reload(target);
    write(func+" modification complete.");
    unguarded( (: rm(global1) :) ); 
    return 1;
}

int eventAddSettings(object ob, string tmp, mapping NewMap, string func){
    string filename, new_lines;
    filename = base_name(ob)+".c";

    if(!check_privs(this_player(),filename)){
        write("You do not have sufficient privileges to perform this action.");
        return 1;
    }
    global2 = filename;
    unguarded( (: global1 = read_file(global2) :) );

    global1 = remove_matching_line(global1, func , 1);
    global2 = tmp;
    new_lines = "\n";
    foreach(string key, mixed val in NewMap){
        if(intp(val)) new_lines += func+"(\""+key+"\", "+val+");\n";
        else new_lines += func+"(\""+key+"\", \""+val+"\");\n";
    }
    global1 = this_object()->eventAppend(global1,({func,"SetClass","SetRace","SetLevel","SetItems","SetInventory","SetLong", "SetClosed"}),new_lines);
    unguarded( (: write_file(global2, global1,1) :) );
    this_object()->eventGeneralStuff(global2);
    global1 = filename;
    unguarded( (: cp(global2, global1) :) );
    unguarded( (: rm(global2) :) );
    reload(ob);
    return 1;
}
