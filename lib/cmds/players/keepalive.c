#include <lib.h>

inherit LIB_DAEMON;
string err =  "This game appears not to have this feature enabled "+
"in the driver. Consider asking your mud administrator "+
"to recompile with PACKAGE_DSLIB defined.";


mixed cmd(string args) {
    mixed keepalive = this_player()->GetProperty("keepalive");
    if(!intp(keepalive)) keepalive = 0;

#ifndef __DSLIB__
    write(err);
    return 1;
#endif

    if( !args || args == "" ) {
        if(!keepalive){
            write("You have keepalives disabled.");
            return 1;
        }
        else {
            write("Keepalive set to once every "+cardinal(keepalive)+
                    " heartbeat"+(keepalive > 1 ? "s." : "."));
            return 1;
        }
    }
    if(!intp(keepalive = to_int(args)) || !keepalive) {
        this_player()->SetProperty("keepalive",0);
        write("Keepalive disabled.");
        return 1;
    }
    else {
        this_player()->SetProperty("keepalive",keepalive);
        write("Keepalive set to once every "+cardinal(keepalive)+
                " heartbeat"+(keepalive > 1 ? "s." : "."));
        return 1;
    }
}

string GetHelp(){
    return ("Syntax: keepalive [NUMBER]\n\n"
            "Without an argument, keepalive will display your current "
            "keepalive interval. When provided an integer argument, this "
            "command sets how often an invisible 'keepalive' signal is "
            "sent to your client. This is useful for people with an "
            "ISP or modem that kills connections that have been idle "
            "for a while."
#ifndef __DSLIB__
            "\n---NOTE---\n"+err+"\n---\n"
#endif
            "\nSee also: env");
}
