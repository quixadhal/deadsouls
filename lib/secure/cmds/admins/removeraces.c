#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    foreach(string str in RACES_D->GetRaces()){
        RACES_D->RemoveRace(str);
    }
    write("Done.");
    return 1;
}

string GetHelp(){
    return ("Syntax: removeraces\n\n"
            "Gets rid of ALL races known by the race daemon. "
            "Unless you want to be very sad later, you should "
            "add your races with the addraces command immediately. ");
}
