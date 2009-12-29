#include <lib.h>
#include <cfg.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd() {
    string *races = get_dir(CFG_RACES+"/");
    foreach(string race in races){
        string str = CFG_RACES+"/"+race;
        if(file_exists(str)) {
            catch( RACES_D->AddRace(str) );
        }
    }
    write("Done.");
    return 1;
}

string GetHelp(){
    return ("Syntax: addraces\n\n"
            "Imports all the race files in CFG_RACES into  "
            "RACES_D. It may fail if all races have not been "
            "removed prior to its use.");
}
