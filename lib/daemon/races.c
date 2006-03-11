/*    /daemon/race.c
 *    from the Dead Souls  Object Library
 *    handles race configuration and administration
 *    created by Descartes of Borg 960108
 *    Version: @(#) races.c 1.4@(#)
 *    Last modified: 96/11/10
 */

#include <lib.h>
#include <save.h>
#include <privs.h>
#include "include/races.h"

inherit LIB_DAEMON;

private mapping Races = ([]);
static private mapping Resistances = ([]);
static private mapping Armors = ([]);

static private string array FlyingRaces = ({"avidryl","bat","demon","dragon","faerie","gargoyle","griffin","insect","pegasus","bird", "wraith"});
static private string array LimblessCombatRaces = ({"snake","slug","god","android","elemental","fish","tree","plant"});
static private string array LimblessRaces = ({"snake","slug","elemental","fish"});
static private string array NonBitingRaces = ({"golem","android","cow","deer","elemental","god"});

static void create() {
    string array lines;

    daemon::create();
    if( unguarded((: file_size(SAVE_RACES __SAVE_EXTENSION__) :)) > 0 )
	unguarded((: restore_object(SAVE_RACES) :));
    if( !Races ) Races = ([]);
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

string *LimblessCombatRaces(){
    return LimblessCombatRaces;
}

int GetLimblessRace(string str){
    if(member_array(str,LimblessRaces) == -1) return 0;
    else return 1;
}

string *LimblessRaces(){
    return LimblessRaces;
}

int GetBitingRace(string str){
    if(member_array(str,NonBitingRaces) == -1) return 1;
    else return 0;
}

void AddRace(string file, int player) {
    class Race res;
    string array lines, tmp;
    string race;

    validate();
    if( !file_exists(file) ) error("No such file: " + file);
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
      race = lines[0];
      if( Races[race] ) error("Race already exists");
      res = new(class Race);
      res->Sensitivity = map(explode(lines[1], ":"), (: to_int :));
      res->Language = lines[2];
      res->Skills = ([]);
      lines = lines[3..];
      //tc("lines: "+identify(lines),"blue");
      while(sizeof(tmp = explode(lines[0], ":")) == 5) {
	  res->Skills[tmp[0]] = ({ tmp[1], tmp[2], tmp[3], tmp[4] });
	  //tc("skills: "+identify(res->Skills),"red");
	  lines = lines[1..];
	  //tc("lines: "+identify(lines),"yellow");
      }
      res->Resistance = ([]);
      while(sizeof(tmp = explode(lines[0], ":")) == 2) {
	  int x = to_int(tmp[0]);

	  if( x == 0 && tmp[0] != "0" ) x = GetResistance(tmp[0]);
	  res->Resistance[x] = tmp[1];
	  lines = lines[1..];

      }
      //tc("lines: "+identify(lines),"green");
      res->Stats = ([]);
      while(sizeof(tmp = explode(lines[0], ":")) == 3) {
	  class Stat s = new (class Stat);
	  s->Average = to_int(tmp[1]);
	  s->Class = to_int(tmp[2]);
	  res->Stats[tmp[0]] = s;
	  lines = lines[1..];
      }
      //tc("lines: "+identify(lines),"red");
      res->Limbs = ({});
      while(sizeof(tmp = explode(lines[0], ":")) == 4) {
	  mixed array limb = allocate(4);

	  limb[0] = tmp[0];
	  limb[1] = (tmp[1] == "0" ? 0 : tmp[1]);
	  limb[2] = to_int(tmp[2]);
	  limb[3] = map(explode(tmp[3], ","), function(string str) {
		int x = to_int(str);

		if( x == 0 && str != "0" )
		    return GetArmor(str);
		return x;
	      });
	    res->Limbs = ({ res->Limbs..., limb });
	    if( sizeof(lines) > 1 ) lines = lines[1..];
	    else {
		lines = ({});
		break;
	    }
	}
	  res->Fingers = ([]);
	  if( sizeof(lines) ) {
	      foreach(string hand in lines) {
		  string array parts = explode(hand, ":");

		  res->Fingers[parts[0]] = to_int(parts[1]);
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
	  save_object(SAVE_RACES);
      }

      void RemoveRace(string race) {
	  validate();
	  map_delete(Races, race);
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

      mapping GetRemoteRaces() {
	  mapping mp = ([]);

	  foreach(string race, class Race res in Races) {
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
	  // if(!sizeof(args) == 5)
	  //args = ({ ({}), ({}), ({}), ({}), ({}) });
	  //tc("args: "+sizeof(args)+", "+identify(args));

	  if( !res || !res->Complete || sizeof(args) != 5 ) return;
	  tmp = ({});
	  foreach(int key, string val in res->Resistance)
	  tmp = ({ tmp..., ({ key, val }) });
	  args[0] = tmp;
	  tmp = ({});
	  foreach(string key, class Stat stat in res->Stats)
	  tmp = ({ tmp..., ({ key, stat->Average, stat->Class }) });
	  args[1] = tmp;
	  args[2] = res->Language;
	  args[3] = ({ res->Sensitivity[0], res->Sensitivity[1] });
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
	  string tmp;
	  int x;

	  if( !res ) return 0;
	  limbs = map(res->Limbs, (: $1[0] :));
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
