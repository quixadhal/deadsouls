/*    /cmds/player/rwho.c
 *    from the Dead Souls LPC Library
 *    Sends an Intermud 3 who request
 *    created by Descartes of Borg 950623
 *
 *    IMC2 support added by Shadyman 2006-Sep-12
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int CheckMud(string name){
    if(!(name = INTERMUD_D->GetMudName(name)) ) return 0;
    if(!INTERMUD_D->GetMudList()[name][0]) return 0;
    return 1;
}

mixed cmd(string str) {
    string tmp, s1, s2;
    string network = "i3";

    if( !str ) return "Get a remote who from where?";

    if(grepp(str,"@")){
        if(sscanf(str,"%s@%s", s1, s2) < 2){
            write("Malformed name. Trying anyway...");
        }
        else str = s1;

        if(s2 && lower_case(s2) == "imc2"){
            network = "imc2";
            str = imc2_mud_name(s1);
        }
    }

    if ( network == "imc2" && find_object(IMC2_D) && (tmp = IMC2_D->find_mud(str)) ) {
        IMC2_D->who_out(capitalize(this_player()->GetKeyName()),tmp);
        message("system", "Remote who query sent to " + tmp + " on the IMC2 network.", this_player());
        return 1;
    } else if( CheckMud(str) ) {
        tmp = INTERMUD_D->GetMudName(str);
        SERVICES_D->eventSendWhoRequest(tmp);
        message("system", "Remote who query sent to " + tmp + " on the I3 network.", this_player());
        return 1;
    } else {
        return mud_name() + " is not aware of such a place, or it is not currently available.";
        //Maybe do more processing here for bad spelling, etc?
    }
    return "Oops, rwho checks fell through, and broke.";
}

string GetHelp(){
    return ("Syntax: rwho <mud>\n\n"
            "Gives you a who list in abbreviated form from a mud on the\n"
            "global network following the CD protocols for intermud communication.\n"
            "The information given by the who varies from mud to mud.\n"
            "By default, this command consults the i3 network. You can specify IMC2\n"
            "with the following syntax: rwho mudname@imc2");
}
