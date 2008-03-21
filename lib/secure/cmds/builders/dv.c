#include <lib.h>
#include <objects.h>
#include <daemons.h>

inherit LIB_DAEMON;
string *features = ({ "GetLong", "GetShort", "GetAmbientLight",
    "GetDayLight", "GetNightLight", "GetDayLong", "GetNightLong",
    "GetDayShort", "GetNightShort", "GetExitMap", "GetFlyRoom",
    "GetSinkRoom", "GetClimate", "GetTerrainType", "GetItemsMap",
    "GetItemsMap", "GetProperties", "GetSmellMap", "GetListenMap",
    "GetGravity", "GetMedium", "GetPlayerKill", "GetTown",
     });


mixed cmd(string str){
mapping RoomElements = ([]);
object here = environment(this_player());
if(!here) return 0;

    if(file_exists(base_name(here)+".c")){
        write("This environment is already not virtual.");
        return 1;
    }

    foreach(string feature in features){
        RoomElements[feature] = call_other(here, feature);
    }

    foreach(mixed key, mixed val in RoomElements){
        if(val && sizeof(val)){
            write(key+": "+identify(val));
        }
    }
return 1;
}

void help() {
    message("help", "Syntax: \n\n"
      "\n\n"
      "See also: ", this_player());
}
