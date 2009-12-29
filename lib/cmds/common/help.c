/*    /cmds/common/help.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 951021
 *    Version: @(#) help.c 1.3@(#)
 *    Last Modified: 96/12/13
 */

#include <lib.h>
#include <daemons.h>
#include <message_class.h>

inherit LIB_DAEMON;

varargs void HelpMenu(string index);

mixed cmd(string arg) {
    object who = previous_object();
    int array screen = (who->GetScreen() || ({ 80, 24 }));
    string help = "";
    string tmp = "";

    if( arg == "help") {
        help = HELP_D->GetHelp("help");
        write(help);
        return 1;
    }
    if( !arg || arg == "") {
        if(creatorp(this_player())) help = read_file("/doc/help/creators/creator_general");
        else help = read_file("/doc/help/players/player_general");
        if(!sizeof(help)) help = HELP_D->GetHelp("help");
        write(help);
        return 1;
    }
    if( arg == "index" || HELP_D->GetTopics(arg) ) {
        if( arg == "index" ) {
            HelpMenu();
            return 1;
        }
        HelpMenu(arg);
        return 1;
    }
    if(tmp = VERBS_D->GetVerb(arg)){
        arg = last_string_element(tmp, "/")[0..<3];
    }
    if( !(tmp = HELP_D->GetHelp(arg)) ) {
        tmp = this_player()->GetAlias(arg);
        if(tmp) arg = explode(tmp, " ")[0];
        if(!(tmp = HELP_D->GetHelp(arg))){
            return HELP_D->GetLastError();
        }
    }
    help = center(mud_name()+" System Help", screen[0]) + tmp;
    if(sizeof(help) < 2000)    help = wrap(help, screen[0]);
    who->eventPage(explode(help, "\n"), MSG_HELP);
    return 1;
}

static int CanAccess(object who, string index) {
    return HELP_D->CanAccess(who, index);
}

varargs void HelpMenu(string index) {
    function f;
    string tmp;
    string array indices;
    int array scr;
    int y = 0;

    scr = this_player()->GetScreen() || ({ 80, 25 });
    tmp = center(mud_name()+" System Help", scr[0]);
    if( !index ) {
        tmp += "Index: %^GREEN%^main index%^RESET%^\n\n";
        indices = filter(HELP_D->GetIndices(),
                (: CanAccess(this_player(), $1) :));
        foreach(string yuck in indices) {
            int z = strlen(yuck) + 6;

            if( z > y ) {
                y = z;
            }
        }
        tmp += format_page(map(indices, function(string str, string array ind) {
                    int num = member_array(str, ind) + 1;
                    return ("[%^CYAN%^"+(num)+"%^RESET%^] " + str);
                    }, indices), scr[0]/(y+2), 4);
    }
    else if( !HELP_D->CanAccess(this_player(), index) ) {
        message("help", "Invalid index choice.", this_player());
        message("prompt", "Hit <return>: ", this_player());
        input_to(function(string str) { HelpMenu(0); });
        return;
    }
    else {
        string array topics = HELP_D->GetTopics(index);
        string array bing = allocate(sizeof(topics));
        int i = 0;

        tmp += "Index: %^GREEN%^" + index + "%^RESET%^\n\n";
        foreach(string topic in topics) {
            int z = strlen(topic) + 6;

            if( z > y ) {
                y = z;
            }
            bing[i++] = "[%^CYAN%^" + (i+1) + "%^RESET%^] " + topic;
        }
        tmp += format_page(bing, 3);
    }
    f = function(string ind) {
        if( !ind )
            message("prompt", "\n\nEnter a index or 'q' to quit help: ",
                    this_player());
        else
            message("prompt", "\n\nEnter a topic, 'q' to quit help, or "
                    "<return> for main menu: ", this_player());
        input_to(function(string str, string ind) {
                string ret;
                int ind_num;
                int *scr;

                if( str == "q" ) {
                message("system", "Exiting help.", this_player());
                return;
                }
                if( !str || str == "" ) {
                HelpMenu(0);
                return;
                }
                scr = (int *)this_player()->GetScreen() || ({ 80 });
                if( ind_num = to_int(str) ) {
                string array tmp2;
                if( !ind ) tmp2 = filter(HELP_D->GetIndices(),
                    (: CanAccess(this_player(), $1) :));
                else tmp2 = HELP_D->GetTopics(ind);    
                if( ind_num < 1 || ind_num > sizeof(tmp2) ) {
                str = 0;
                HELP_D->SetError("Index number out of range.");
                }
                else str = tmp2[ind_num - 1];
                }
                if( !ind && !HELP_D->GetTopics(str) ) {
                    message("help", "Invalid index choice.", this_player());
                    message("prompt", "Hit <return>: ", this_player());
                    input_to(function(string str) { HelpMenu(); });
                    return;
                }
                else if( !ind ) {
                    HelpMenu(str);
                    return;
                }
                if( !(ret = HELP_D->GetHelpByIndex(ind, str)) ) {
                    message("help", HELP_D->GetLastError(), this_player());
                    message("prompt", "\nHit <return>: ", this_player());
                    input_to(function(string str) { HelpMenu(); });
                    return;
                }
                ret = center(mud_name()+" System Help", scr[0])+wrap(ret, scr[0]);
                this_player()->eventPage(explode(ret, "\n"), "help",
                        function(string ind) {
                        message("prompt", "\n\nHit <return>: ",
                            this_player());
                        input_to(function(string str, string ind) {
                            HelpMenu(ind); }, ind);
                        return;
                        }, ind);
        }, ind);
    };
    this_player()->eventPage(explode(tmp, "\n"), "help", f, index);
}
