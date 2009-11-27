/*    /cmds/player/mudlist.c
 *    from the Dead Souls LPC Library
 *    gives information about muds
 *    created by Descartes of Borg 950623
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    mixed *info;
    string *list;
    mapping borg;
    string mud, tempy;
    int all = 0;

    if(sizeof(str) && sizeof(tempy = INTERMUD_D->GetMudName(str) )) str = tempy;

    //imc2 mudlist
    if(sizeof(str) && str[0..3] == "imc2") {
        IMC2_D->mudlist(this_player());
        return 1;
    }

    if( str && str != "" ) {
        mapping tmp;
        string opt, tmpstr;

        tmp = INTERMUD_D->GetMudList();
        if( str[0] != '-' ) str = "-n " + str;
        opt = str[1..1];
        str = str[3..];
        borg = ([]);
        foreach(mud, info in tmp) {
            int x, y, z;

            switch(opt) {
                case "a":
                    all = 1;
                break;
                case "m":
                    x = 5;
                break;
                case "d":
                    x = 7;
                break;
                case "n":
                    x = 0;
                break;
            }
            tmpstr = (x ? info[x] : mud);
            z = strlen(str);
            y = strlen(tmpstr);


            if( lower_case(str) == lower_case(tmpstr) ) {
                borg = ([ mud : info ]);
                break;
            }

            else if( y > z &&
                    lower_case(tmpstr[0..z-1]) == lower_case(str) && info[0] == -1 )
                borg[mud] = info;
        }
    }
    else {
        borg = ([ ]);
        foreach( mud, info in INTERMUD_D->GetMudList() )
            if( all == 1 || info[0] == -1 ) borg[mud] = info;
    }
    if( !sizeof(borg) ) {
        if(this_player()) message("system", "No MUDs match your query.", this_player());
        return 1;
    }
    else if( sizeof(borg) == 1 ) {
        string msg, svc;
        int val, comma = 0;

        mud = keys(borg)[0];
        msg = "\nDetailed information on %^GREEN%^" + mud + "%^RESET%^:\n";
        msg += sprintf("MUD Type: %:-6s Server: %:-20s Library: %s\n",
                borg[mud][8], borg[mud][7], borg[mud][5]);
        msg += "Status: " + borg[mud][9] + "\nAdmin email: " +
            borg[mud][10] + "\n";
        msg += "Services: ";
        foreach(svc, val in borg[mud][11]) {
            if( val == 1 ) {
                if( comma ) msg += ", " + svc;
                else {
                    msg += svc;
                    comma = 1;
                }
            }
        }
        msg += "\nHost: " + borg[mud][1] + "\n";
        msg += "Telnet port: " + borg[mud][2] + "\n";
        if( borg[mud][11]["amcp"] )
            msg += "AMCP version: " + borg[mud][11]["amcp"] + "\n";
        if( borg[mud][11]["http"] )
            msg += "HTTP port (World Wide Web): " + borg[mud][11]["http"]+"\n";
        if( borg[mud][11]["ftp"] )
            msg += "FTP port (File Transfer): " + borg[mud][11]["ftp"] + "\n";
        if( borg[mud][11]["rcp"] )
            msg += "RCP port (Remote Creator): " + borg[mud][11]["rcp"] + "\n";
        if(this_player()) message("info", msg, this_player());
        return 1;
    }
    list = ({});
    foreach(mud, info in borg)
        list += ({ sprintf("%:-15s %:-6s %:-15s %:-18s %s %d",
                    replace_string(mud,"%^","%%^^"), info[8], info[7], info[5], info[1], info[2]) });
    list = sort_array(list, 1);
    list = ({ replace_string(mud_name(),"%^","%%^^") + " recognizes " + consolidate(sizeof(borg), "a mud")+
            " matching your query: ", "" }) + list;
    if(this_player()) this_player()->eventPage(list);
    if(this_player()) return 1;
    else return implode(list,"\n");
}

int alphabet(string a, string b) {
    if((a = lower_case(a)) == (b = lower_case(b))) return 0;
    else if(a > b) return 1;
    else return -1;
}

string GetHelp() {
    return ("Syntax: mudlist [-d] [-m] [-n] [arg]\n"
            "        <mudlist imc2>\n\n"
            "Without any arguments, it gives a full listing of all muds "
            "with which this mud is capable of communication through "
            "tell, mail, finger, rwho, and other intermud services.  "
            "With arguments, <mudlist> requires one and only one option "
            "which must be one of the following:\n"
            "\t-d [driver]: List only muds using the named driver\n"
            "\t-m [mudlib]: List only muds using the named mudlib\n"
            "\t-n [mudname]: List only the muds with the name given\n\n"
            "Note that the argument need not be complete, for example:\n"
            "\t mudlist -n idea\n"
            "will list IdeaExchange as well as any other mud whose name "
            "begins with the string \"idea\".\n"
            "See also: finger, mail, rwho, tell");
}
