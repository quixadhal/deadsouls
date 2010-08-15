/*    /lib/obj/post.c  
 *    from Foundation II  
 *    the user front end to the IIPS version 3.1  
 *    created by Descartes of Borg 940509  
 */  

#include <lib.h>    
#include <daemons.h>    
#include <iips.h>
#include "include/post.h"

inherit LIB_ITEM;    

static private int __Screen, __Lines, __NumLetters;   
static private int __Begin,__Current,__CommandLine,__FromMenu,__IncomingFlag;
static private string __Folder, __Owner, __CurrentMenu;    
static private mapping __Options, __TmpPost, __ChangedOptions;    
static private int *__Delete; 
static private string *__FwdRply;    
static private mapping *__BoxInfo, *__PostalOptions;    

void create() {    
    item::create();    
    SetKeyName("post box");    
    SetId( ({ "post box", "box", POSTAL_ID }) );    
    SetAdjectives( ({ "post", "a", "imaginary" }) );    
    SetShort("a post box");    
    SetLong("A post box.");    
    SetProperty("no steal", 1);    
    SetInvis(1);    
    SetMass(0);    
    SetValue(0);    
    SetPreventGet( (: Destruct :) );    
    SetPreventDrop( (: Destruct :) );    
    SetPreventPut( (: Destruct :) );    
    __PostalOptions = ({     
            ([ "key":"askcc", "value":({"N","Y"}), "desc":"Prompt for cc when "    
             "sending mail:" ]),     
            (["key":"quit", "value":({"N","Y"}), "desc":"Confirm quit from mail:"]),
            (["key":"metoo", "value":({"N","Y"}),"desc":"Send mail to yourself "
             "with you in alias:"]),
            (["key":"delete", "value":({"N","Y"}), "desc":"Confirm deletion of "    
             "letters:" ]),    
            (["key":"notify", "value":({"N","Y"}), "desc":"Notify me when new mail "    
             "is received:" ]),    
            ([ "key":"message", "value":"New mail has arrived from $N\n"    
             "Subject: $S", "desc":"Mail message when notify occurs:" ]),    
            ([ "key": "content", "value":({"message only", "header and message"}),    
             "desc":"Content of letters when reading mail:" ]),    
            ([ "key":"forward", "value":"none", "desc":"Address to forward "    
             "incoming mail to:" ]),    
            ([ "key":"sig file", "value":"none", "desc":"Signature file:" ]),    
            ([ "key":"commands", "value":({"N","Y"}), "desc":"Exclude command "    
             "listing from menus:" ]),    
            ([ "key":"read", "value":({"N","Y"}), "desc":"Read letter when you make "    
             "it current:" ])    
    });    
}    

string tmpmail(){
    mixed tmpmaildir = homedir(this_player());
    if(!tmpmaildir || !directory_exists(tmpmaildir)) return "/tmp/"+this_player()->GetKeyName();
    else return tmpmaildir+"/tmp/mail.tmp";
}

void init() {    
    item::init();    
    if(this_player() != environment(this_object())) {    
        this_object()->eventDestruct();    
        return;    
    }    
    __Options = OPTIONS_D->query_options(__Owner =
            this_player()->GetKeyName());
    if(!(__Lines = to_int(this_player()->GetEnvVar("LINES"))))   
        __Lines = 24;   
    if(!(__Screen = to_int(this_player()->GetEnvVar("SCREEN"))))   
        __Screen = 80;    
    if((__NumLetters = __Lines - 10) < 0) __NumLetters = 5;   
}    

static private void restore_box(string folder) {    
    int x;    

    if(!pointerp(__BoxInfo=FOLDERS_D->query_box_info(__Owner, folder)))    
        __BoxInfo = ({});    
    __Folder = folder;    
    __TmpPost = ([]);    
    __Delete = allocate(x = sizeof(__BoxInfo));    
    if(!x) __Current = -1;    
    else __Current = 0;    
    __Begin = 0;    
}    

static private void save_box() {    
    FOLDERS_D->delete_posts(__Owner, __Folder, __Delete);    
}    

static private void destruct_box(string str) {
    message("mail", str, this_player());
    this_object()->eventDestruct();
}

static private int valid_folder(string str) {    
    int i;

    if(str == "" || !str) return 0;    
    if(str[0] != '=' && str != "new") return 0;    
    i = strlen(str = lower_case(str));    
    while(i-- > 1) if(str[i] < 'a' || str[i] > 'z') return 0;    
    return 1;    
}    

static private int set_current(mixed val) {
    int i;

    if(intp(val)) __Current = val;
    else {
        i = sizeof(__BoxInfo);
        while(i--) {
            if(__BoxInfo[i]["id"] == val) {
                __Current = i;
                break;
            }
        }
    }
    if(__Current < __Begin) __Begin = __Current;
    else if(__Current >= __Begin+10) __Begin = __Current-9;
    return __Current;
}

void start_post(string str) {    
    string *args;
    int i, maxi;;

    if(str && str != "") {    
        maxi = sizeof(args = explode(str, ","));    
        if(args[0][0] == '-' && strlen(args[0]) == 2) {    
            switch(args[0][1]) {    
                case 'r':    
                    restore_box("new");    
                    index_cmd("");    
                    return;    
                case 'f':    
                    if(maxi < 2) {    
                        message("mail", "No folder named.", this_player());    
                        this_object()->eventDestruct();    
                        return;    
                    }    
                    else if(maxi>2 || (!valid_folder(args[1]) &&     
                                args[1] != "new")) {    
                        message("mail", "Illegal folder name.", this_player());    
                        this_object()->eventDestruct();    
                        return;    
                    }    
                    else {    
                        restore_box(args[1]);    
                        indices(0);    
                        return;    
                    }    
                case 'a':    
                    __CommandLine = 1;    
                    if(maxi < 2) index_cmd("a");    
                    else index_cmd(implode(args[1..maxi-1], " "));    
                    return;    
            }    
        }    
        __CommandLine = 1; 
        __TmpPost = ([]); 
        __FwdRply = 0;    
        send_letter(args);    
        return;    
    }    
    restore_box("new");
    indices(0);    
}    

static private void primary_prompt() {     
    message("prompt", "\nCommand: \n", this_player());    
}    

static private void secondary_prompt() {    
    message("prompt", sprintf("\nCommand (%s for %s menu): \n",   
                __CurrentMenu[0..0],  __CurrentMenu),    
            this_player());    
}    

varargs static private void postal_error(string str, string ind, mixed args) {
    message("mail", sprintf("\n%%^RED%%^%s\n", str), this_player());   
    primary_prompt();  
    if(args) input_to(sprintf("%s_cmd", (ind ? ind : __CurrentMenu)), args);  
    else input_to(sprintf("%s_cmd", (ind ? ind : __CurrentMenu)));  
}  

varargs static private void postal_success(string str,string ind,mixed args) {
    message("mail", sprintf("\n%%^GREEN%%^%s\n", str), this_player());  
    secondary_prompt();  
    if(args) input_to(sprintf("%s_cmd", (ind ? ind : __CurrentMenu)), args);  
    else input_to(sprintf("%s_cmd", (ind ? ind : __CurrentMenu)));  
}  

varargs static void indices(int x, string str) {    
    int i, maxi;    

    __CurrentMenu = "index";  
    if(str && str != "") {    
        index_cmd(str);    
        return;    
    }    
    __Begin = x;    
    __FromMenu = 1;    
    message("mail", "\n%^INITTERM%^Imaginary Intermud Postal Service "    
            "(IIPS) 3.1     Descartes of Borg 1993, 1994", this_player());    
    message("mail", sprintf("\n%%^CYAN%%^%s", center(sprintf("Folder is %s "
                        "with %s.\n",__Folder,consolidate(maxi=sizeof(__BoxInfo),"one letter\n")),
                    __Screen)), this_player());
    if(!maxi) __Current = -1;    
    else for(i=x; i<maxi && i < x+__NumLetters; i++)    
        message("mail", sprintf("%s%s) %s %s %s %s %s\n",    
                (__Current == i ? "%^YELLOW%^BOLD%^*" : " "),    
                ((i+1 > 99) ? (""+(i+1)) : ((i+1 > 9) ? (" "+(i+1)) : ("  "+(i+1)))),
                (__BoxInfo[i]["read"] ? " " : "N"),    
                (__Delete[i] ? "D" : " "),    
                arrange_string(capitalize(__BoxInfo[i]["from"]), 30),    
                arrange_string(postal_time(__BoxInfo[i]["date"]), 7),    
                arrange_string(__BoxInfo[i]["subject"], __Screen-40)), this_player());    
    index_menu();    
}    

varargs static void aliases(string str) {  
    __CurrentMenu = "alias";    
    if(str && str != "") {    
        alias_cmd(str);    
        return;    
    }    
    message("mail", "\n%^INITTERM%^Imaginary Intermud Postal Service "    
            "(IIPS) 3.1     Descartes of Borg 1993, 1994", this_player());    
    message("mail", sprintf("\n%%^CYAN%%^%s", center(sprintf("%s and "
                        "Personal Alias Menu", mud_name()))), this_player());
    message("mail", sprintf("\n%s\n",    
                format_page(keys(OPTIONS_D->query_groups(__Owner) +    
                        LOCALPOST_D->query_mud_groups()),__Screen/20)),this_player());
    alias_menu();    
}    

varargs static void options(string str) {    
    string tmp;    
    int i, maxi;    

    __CurrentMenu = "option";  
    if(str && str != "") {    
        option_cmd(str);    
        return;    
    }    
    message("mail", "\n%^INITTERM%^Imaginary Intermud Postal Service "    
            "(IIPS) 3.1     Descartes of Borg 1993, 1994", this_player());    
    message("mail", sprintf("\n%%^CYAN%%^%s", center("IIPS 3.1 Options Menu\n",   
                    __Screen)), this_player());    
    maxi = sizeof(__PostalOptions);    
    for(i=0; i<maxi; i++) {    
        if(pointerp(__PostalOptions[i]["value"]))    
            tmp = __PostalOptions[i]["value"][__Options[__PostalOptions[i]["key"]]];    
        else if(!(tmp = __Options[__PostalOptions[i]["key"]]))    
            tmp = __PostalOptions[i]["value"];    
        message("mail", sprintf("%s) %-45s %s\n", (i+1>9) ? (i+1)+"" :" "+(i+1),
                __PostalOptions[i]["desc"], tmp),    
            this_player());    
    }    
    option_menu();    
}    

static private void help(string arg, string ind) {    
    if(!ind) ind = __CurrentMenu;    
    if(arg && arg != "") {    
        help_cmd(arg, ind);    
        return;    
    }    
    message("mail", "\n%^INITTERM%^Imaginary Intermud Postal Service "    
            "(IIPS) 3.1     Descartes of Borg 1993, 1994", this_player());    
    message("mail",sprintf("\n%%^CYAN%%^%s\n",center("IIPS 3.1 Help Menu",   
                    __Screen)), this_player());    
    help_menu(ind);    
}    

static private void index_menu() {    
    if(__Options["commands"]) {
        primary_prompt();
        input_to("index_cmd");
        return;
    }
    message("mail", "\n"+center("a)lias menu, c)hange folder, d)elete, "    
        "f)orward, h)elp, m)ail,", __Screen), this_player());    
    message("mail", center("n)ext letter, o)ptions menu, p)revious "    
        "letter, q)uit, Q)uit without saving,", __Screen), this_player());    
    message("mail", center("s)ave to folder, S)ave to file, "    
        "u)ndelete", __Screen), this_player());    
    message("mail", center("<return> to read selected letter\n", __Screen), 
            this_player());
    primary_prompt();
    input_to("index_cmd");    
}    

static private void alias_menu() {    
    if(__Options["commands"]) {
        primary_prompt();
        input_to("alias_cmd");
        return;
    }
    message("mail", center("d)elete from an alias, e)nter into an alias, "    
        "h)elp, i)ndex menu, ", __Screen), this_player());    
    message("mail", center("l)ist an alias, m)ake an alias, o)ptions menu, "
        "q)uit, Q)uit without saving, ", __Screen), this_player());    
    message("mail", center("r)emove an alias\n", __Screen),this_player());    
    primary_prompt();    
    input_to("alias_cmd");    
}    

static private void option_menu() {    
    if(__Options["commands"]) {
        primary_prompt();
        input_to("option_cmd");
        return;
    }
    message("mail", "\n"+center("Enter the number of an option to "    
                "change it.", __Screen), this_player());    
    message("mail", center("a)lias menu, h)elp, i)ndex menu, "    
        "q)uit, Q)uit without saving,", __Screen), this_player());    
    message("mail", center("s)ave option changes\n", __Screen), this_player()); 
    primary_prompt();    
    input_to("option_cmd");    
}    

static private void help_menu(string ind) {  
    int i, maxi;

    message("help", "\n\n\tEnter one of the following to visit "  
            "another menu:", this_player());  
    message("help", center("a)lias menu, i)ndex menu, o)ptions menu",  
        __Screen), this_player());  
    message("help", "\n\tOr enter one of the following for detailed help:",  
            this_player());    
    switch(ind) {  
        case "index":  
            message("help", center("c)hange folder, d)elete, f)orward, "  
            "m)ail letter, n)ext letter,", __Screen), this_player());  
        message("help", center("p)revious letter, q)uit, "  
            "Q)uit without saving, r)eply,", __Screen), this_player());  
        message("help", center("s)ave to folder, S)ave to file,",  
            __Screen), this_player());  
        message("help", center("x) help from another menu, y) detailed "  
            "IIPS user manual\n", __Screen), this_player());  
        break;  
        case "alias":  
            message("help", center("d)elete from an alias, e)nter into "  
            "an alias, m)ake an alias, q)uit,", __Screen), this_player());  
        message("help", center("Q)uit without saving, r)emove "  
            "an alias,", __Screen), this_player());  
        message("help", center("x) help from another menu, y) detailed "  
            "IIPS user manual\n", __Screen), this_player());  
        break;  
        case "option":  
            for(i=0, maxi = sizeof(__PostalOptions); i<maxi;i++)  
                message("help", center(sprintf("%d) %s", (i+1),   
                            __PostalOptions[i]["desc"]), __Screen), this_player());   
        message("help", sprintf("\n%s",center("Also: q)uit, Q)uit without " 
                "saving, s)ave option changes,", __Screen)), this_player());  
        message("help", center("x) help from another menu, y) detailed "  
            "IIPS user manual\n", __Screen), this_player());  
        break;  
        default:   
        postal_error("Invalid postal menu.");  
        return;  
    }  
    primary_prompt();    
    input_to("help_cmd", ind);    
}  

static void index_cmd(string str) {    
    string cmd, args;    
    string *tmp;    
    int x;    

    __CurrentMenu = "index";    
    if(__IncomingFlag) {    
        __IncomingFlag = 0;  
        postal_error("New mail has arrived!\nCommand Ignored.");  
        return;    
    }    
    if(str == "" || !str) {    
        if(__Current < 0 || __Current >= sizeof(__BoxInfo)) {    
            postal_error("No current letter set.");  
            return;    
        }    
        if(__FromMenu) read_letter(__Current);    
        else if(set_current(__Current+1) < sizeof(__BoxInfo))
            read_letter(__Current);
        else read_letter(set_current(__Current - 1));
        return;    
    }    
    if(sscanf(str, "%d", x) && x) {
        if(x>0 && x <= sizeof(__BoxInfo)) {    
            if(__Options["read"]) read_letter(set_current(x-1));    
            else {    
                set_current(x-1);
                indices(__Begin);
            }    
        }    
        else postal_error("Invalid letter number.");  
        return;    
    }    
    if(!sizeof(tmp = explode(str, " "))) cmd = "";
    else cmd = tmp[0][0..0];
    if(sizeof(tmp) > 1) args = implode(tmp = tmp[1..sizeof(tmp)-1], " ");    
    else {
        args = "";
        tmp = ({});
    }
    switch(cmd) {    
        case "a": aliases(args); return;    
        case "c": change_folder(args); return;    
        case "d": case "u": delete_letter(cmd, args); return;    
        case "f": forward_letter(args, 0); return;    
        case "h": help(args, "index"); return;    
        case "i":     
            indices(__Current < __Begin+__NumLetters ? __Begin :    
                    __Current-(__NumLetters-1), args);    
        return;    
        case "m": 
            __FwdRply = 0; 
        __TmpPost = ([]);  
        send_letter(tmp);  
        return;    
        case "n": index_cmd(""+(__Current+2)); return;    
        case "o": options(args); return;    
        case "p": index_cmd(""+(__Current)); return;    
        case "q": case "Q": quit_box(cmd); return;    
        case "r": reply(args); return;    
        case "s": case "S": save_letter(cmd, args); return;    
        default:  
        postal_error("Invalid postal command.");  
        return;    
    }    
}    

static void alias_cmd(string str) {    
    string cmd, args;    
    string *tmp;    

    __CurrentMenu = "alias";    
    if(str == "" || !str) {
        if(__CommandLine) destruct_box("Invalid alias command.");
        else postal_error("Invalid alias command.");  
        return;    
    }    
    cmd = (tmp = explode(str, " "))[0][0..0];    
    if(sizeof(tmp) > 1) args = implode(tmp[1..sizeof(tmp)-1], " ");    
    else args = "";    
    switch(cmd) {    
        case "a": aliases(args); return;
        case "d": case "e": alias_members(cmd, args); return;    
        case "m": case "r": alias_creation(cmd, args); return;    
        case "h": help(args, "alias"); return;    
        case "i":     
            indices(__Current < __Begin+__NumLetters ? __Begin :    
                    __Current-(__NumLetters-1), args);    
        return;    
        case "l": list_alias(args); return;    
        case "o": options(args); return;    
        case "q": case "Q": quit_box(cmd); return;    
        case "s": aliases(args); return;    
        default: alias_cmd(0); return;    
    }    
}    

static void option_cmd(string str) {    
    string cmd, args;    
    string *tmp;    
    int x;    

    __CurrentMenu = "option";    
    if(str == "" || !str) {    
        postal_error("Invalid option command.");  
        return;    
    }    
    if(sscanf(str, "%d", x) && x) {
        change_option(x-1);    
        return;    
    }    
    cmd = (tmp = explode(str, " "))[0][0..0];    
    if(sizeof(tmp) > 1) args = implode(tmp[1..sizeof(tmp)-1], " ");    
    else args = "";    
    switch(cmd) {    
        case "a": aliases(args); return;    
        case "h": help(args, "option"); return;    
        case "i":     
            indices(__Current < __Begin+__NumLetters ? __Begin :    
                    __Current-(__NumLetters-1), args);    
        return;    
        case "o": options(args); return;    
        case "q": case "Q": quit_box(cmd); return;    
        case "s": save_options(); return;    
        default: option_cmd(0); return;    
    }    
}    

static void help_cmd(string str, string ind) {    
    string tmp, file;      if(str == "" || !str) {    
        postal_error("Invalid help command.", "help", ind);  
        return;    
    }    
    switch(str = str[0..0]) {  
        case "a": aliases(""); return;
        case "i": indices(__Begin); return;
        case "o": options(""); return;
        case "q": case "Q": file = "quitting"; break;
    }
    if(!(tmp = read_file(DIR_POSTAL_HELP+"/"+file))) tmp = "Not found.\n";
    this_player()->eventPage(explode(tmp, "\n"), "help", (: end_help :)); 
}

void end_help() {
    secondary_prompt();
    input_to("help_cmd");
}

static private string postal_time(mixed val) {    
    string *parts;    
    string heure;    
    int x;    

    if(stringp(val) && x = to_int(val)) heure = ctime(time());    
    else if(stringp(val)) heure = val;    
    else heure = ctime(val);    
    parts = explode(replace_string(heure, "  ", " "), " ");    
    return sprintf("%s %s", parts[2], parts[1]);    
}    

static private string header(mapping borg) {    
    int x;

    return sprintf("%s from %%^GREEN%%^%s%%^RESET%%^\nTo: %s%s"    
            "Subject: %%^GREEN%%^%s",     
            (stringp(borg["date"]) ? ((x=to_int(borg["date"])) ? ctime(x) :     
                                      borg["date"]) : ctime(borg["date"])), capitalize(borg["from"]),    
            wrap(implode(borg["to"], ",  "), __Screen),    
            (sizeof(borg["cc"]) ? sprintf("Cc: %s",wrap(implode(borg["cc"], ",  "),
                                                        __Screen)) : ""), borg["subject"]);    
}    

static void change_folder(string str) {    
    if(str == "" || !str) {    
        message("prompt", "Change to which folder? \n", this_player());    
        input_to("get_folder");    
        return;    
    }    
    if(!valid_folder(str)) {    
        __FromMenu = 1;  
        postal_error("Invalid folder name.");  
        return;    
    }    
    if(!__Options["delete"] || member_array(1, __Delete) == -1)
        next_folder("", str);
    else {
        message("prompt", "Delete marked letters (default 'y')? \n",
                this_player());
        input_to("next_folder", str);
    }
}

static void next_folder(string str, string folder) {
    if(str == "" || !str) str = "y";
    else str = (lower_case(str)[0..0]);
    if(str == "y") save_box();
    else if(str != "n") {
        message("prompt", "Invalid command.  Enter 'y' or 'n': \n",this_player());
        input_to("next_folder");
    }
    restore_box(folder);    
    indices(__Begin);    
}    

static void get_folder(string str) {    
    if(str == "" || !str) {  
        __FromMenu = 1;  
        postal_error("Invalid folder name.");  
        return;    
    }    
    change_folder(str);    
}     

static private void delete_letter(string cmd, string args) {    
    int x, i, from, to, maxi;    

    if(args == "" || !args) {    
        from = __Current;    
        to = __Current;    
    }    
    else {
        sscanf(args, "%d-%d", from, to);
        if(!to) to = from-1;
        else to--;
        from--;
    }
    if(from > to || from < 0 || to >= sizeof(__BoxInfo)) {
        postal_error("Invalid letter range.");  
        return;    
    }        
    for(i= from; i < to+1; i++) {    
        if(cmd == "u" && !__Delete[i]) {    
            __FromMenu = 1;  
            postal_error(sprintf("Letter %d is not marked for deletion!",i+1));  
        }    
        else if(cmd == "d" && __Delete[i])  
            postal_error(sprintf("Letter %d is already marked for deletion!",i+1));  
        else if(!__BoxInfo[i]["read"] && cmd == "d" &&    
                !__Options["unread delete"]) {    
            message("mail", sprintf("\n%%^RED%%^Letter %d is still unread!",   
                        i+1), this_player());  
            message("prompt","Delete it anyways (default n): \n",this_player());   
            input_to("unread_delete", ({ i+1, to+1 }));    
            return;    
        }    
        else __Delete[i] = (cmd == "u" ? 0 : 1);    
    }    
    if(cmd == "d" && (args == "" || !args)) {    
        for(i=__Current, maxi = sizeof(__BoxInfo); i<maxi; i++) {    
            if(!__Delete[i]) {    
                set_current(i);
                if(!__FromMenu) {    
                    read_letter(__Current);   
                    return;    
                }    
                else {    
                    indices(__Current < __Begin+__NumLetters ? __Begin :    
                            __Current-(__NumLetters-1));    
                    return;    
                }    
            }    
        }    
    }    
    if(__FromMenu) {    
        indices(__Current < __Begin+__NumLetters ? __Begin :    
                __Current-(__NumLetters-1));    
        return;    
    }  
    if(to == from) postal_success(sprintf("Letter %d %s",from+1,  
                (cmd == "d" ? "marked for deletion" : "undeleted")));    
    else postal_error(sprintf("Letters %d through %d %s.",    
                (from+1), (to+1), (cmd == "d" ? "marked for deletion" : "undeleted")));  
}    

static void unread_delete(string str, int *milk) {    
    if(str == "" || !str) str = "y";    
    else str = lower_case(str[0..0]);    
    if(str == "y") __Delete[milk[0]-1] = 1;    
    if(milk[0] < milk[1]) {    
        delete_letter("d", milk[0]+"-"+milk[1]);    
        return;    
    }    
    __FromMenu = 1;    
    secondary_prompt();    
    input_to("index_cmd");    
}    

static private void quit_box(string cmd) {   
    int i;   

    if(__Options["quit"]) {   
        message("prompt", "Do you really wish to quit (default 'n')? \n",    
                this_player());
        input_to("confirm_quit", cmd);   
        return;   
    }   
    really_quit(cmd);   
}   

static private void really_quit(string cmd) {   
    int i;

    if(cmd == "Q") {   
        message("mail", "\nExiting from IIPS without saving deletions.\n",   
                this_player());   
        this_object()->eventDestruct();   
        return;   
    }   
    if(!__Options["delete"]) {   
        save_box();   
        message("mail", "\nExiting from IIPS.\n", this_player());   
        this_object()->eventDestruct();   
        return;   
    }   
    i = sizeof(__Delete);   
    while(i--) {   
        if(__Delete[i]) {   
            message("prompt", "Delete marked letters (default 'y')? \n",   
                    this_player());   
            input_to("confirm_delete");   
            return;   
        }   
    }   
    message("mail", "\nExiting from IIPS.\n",   
            this_player());   
    this_object()->eventDestruct();   
}   

static void confirm_quit(string str, string cmd) {   
    if(str == "" || !str) str = "n";   
    else str = lower_case(str)[0..0];   
    if(str == "y") {
        really_quit(cmd);
        return;
    }
    else if(str == "n") {   
        indices(__Current < __Begin+__NumLetters ? __Begin :   
                __Current - (__Lines-1));   
        return;   
    }   
    message("prompt", "Answer 'y' or 'n': \n", this_player());   
    input_to("confirm_quit", cmd);   
}   

static void confirm_delete(string str) {   
    if(str == "" || !str) str = "y";   
    else str = lower_case(str)[0..0];   
    if(str == "y") {   
        save_box();   
        message("mail", "\nExiting from IIPS.\n", this_player());   
        this_object()->eventDestruct();   
        return;   
    }   
    else if(str == "n") {   
        message("mail", "\nMarked letters will remain undeleted.\n"   
                "Exiting from IIPS.\n", this_player());   
        this_object()->eventDestruct();   
        return;   
    }   
    message("prompt", "Answer 'y' or 'n': \n", this_player());   
    input_to("confirm_delete");   
}   

static private void save_letter(string cmd, string args) {   
    string *tmp;   
    string folder;   
    int letter, i, maxi;   

    if(args && args != "") {   
        if(sizeof(tmp = explode(args, " ")) == 2) {   
            if(!sscanf(tmp[0], "%d", letter) || !letter) {
                sscanf(tmp[1], "%d", letter);
                folder = tmp[0];   
            }   
            else folder = tmp[1];   
        }   
        else if(!sscanf(args, "%d", letter) || !letter) folder = args;
        else folder = args;   
    }   
    else {   
        folder = 0;   
        letter = 0;   
    }   
    if(!letter) letter = __Current;   
    else letter--;   
    if(!folder) {   
        if(cmd == "S") message("prompt","Save to which file? \n",this_player());   
        else message("prompt", sprintf("Save to which folder (default %s)? \n",   
                    sprintf("=%s", convert_name(__BoxInfo[letter]["from"]))),   
                this_player());   
        input_to("get_save_location", ({ letter, cmd }));   
        return;   
    }   
    if(cmd == "s") {   
        if(!valid_folder(folder)) {   
            postal_error("Invalid folder name.");  
            return;   
        }   
        FOLDERS_D->add_post(__Owner, folder, __BoxInfo[letter]);   
        message("mail", "\nLetter saved.\n", this_player());   
        set_current(letter);
        __Delete[__Current] = 1;   
    }   
    else if(cmd == "S") {   
        folder = absolute_path(this_player()->get_path(), folder);   
        if(!creatorp(this_player()) ||  
                !(master()->valid_write(folder, this_player()))) {   
            postal_error("Access denied.");  
            return;   
        }   
        write_file(folder,   
                LETTERS_D->query_letter(__BoxInfo[letter]["id"]));   
        message("mail", sprintf("Letter saved to %s.\n", folder),    
                this_player());   
        set_current(letter);
        __Delete[__Current] = 1;   
    }   
    if(!__FromMenu)    
        for(i=__Current, maxi = sizeof(__BoxInfo); i<maxi; i++) {   
            if(!__Delete[i]) {   
                read_letter(__Current);   
                return;   
            }   
        }   
    secondary_prompt();
    input_to("index_cmd");
}   

static void get_save_location(string str, mixed *vals) {   
    if(str == "" || !str) {   
        if(vals[1] == "s")   
            str = sprintf("=%s", convert_name(__BoxInfo[vals[0]]["from"]));   
        else {  
            postal_error("Invalid file name.");  
            return;   
        }   
    }   
    save_letter(vals[1], sprintf("%d %s", vals[0]+1, str));
}   

static private void read_letter(int x) {   
    string tmp;   

    if(__Options["content"])
        message("Nmail", sprintf("\n%%^INITTERM%%^Letter %d%s\n", (x+1),   
                    (__Delete[x] ? " [DELETED]:" : ":")), this_player());   
    else message("Nmail", "\n%^INITTERM%^\n", this_player());
    if(__Options["content"]) tmp = header(__BoxInfo[x])+"\n";
    else tmp = "";
    tmp+=sprintf("\n%s\n",LETTERS_D->query_letter(__BoxInfo[x]["id"]));   
    if(!__BoxInfo[x]["read"]) FOLDERS_D->mark_read(__Owner, __Folder, x);
    __BoxInfo[x]["read"] = 1;   
    this_player()->eventPage(explode(tmp, "\n"), "mail", (: end_read :));   
}   

void end_read() {   
    __FromMenu = 0;   
    secondary_prompt();   
    input_to("index_cmd");   
}      

static private void alias_members(string cmd, string args) {   
    string *members, *old_members;   
    string grp;

    if(args == "" || !args) {   
        message("prompt", sprintf("%s which alias? \n",    
                    (cmd == "e" ? "Enter into" : "Delete from")), this_player());   
        input_to("get_alias", cmd);   
        return;   
    }   
    if(sizeof(members = explode(args, " ")) == 1) {   
        message("prompt", sprintf("%s which members? \n",    
                    (cmd == "e" ? "Enter" : "Delete")), this_player());   
        input_to("get_members", ({ cmd, args }));   
        return;   
    }   
    if(!sizeof(members -= ({ grp =  members[0] }))) { 
        if(__CommandLine) destruct_box("Invalid alias member.");
        else postal_error("Invalid alias member.");  
        return;   
    }   
    if(user_exists(grp = lower_case(grp)) ||    
            LOCALPOST_D->query_mud_group(grp)) {   
        if(__CommandLine) destruct_box("Invalid alias.");
        else postal_error("Invalid alias.");  
        return;   
    }   
    if(!(old_members = OPTIONS_D->query_group(__Owner, grp))) {   
        if(__CommandLine) destruct_box("No such alias.");
        else postal_error(sprintf("No such alias %s.", grp));  
        return;   
    }   
    if(cmd == "e") members = distinct_array(members + old_members);   
    else if(!sizeof(members = old_members - members)) members = 0;   
    OPTIONS_D->set_group(__Owner, grp, members);   
    if(cmd == "d") {   
        if(!members)    
            message("mail", "\n%^GREEN%^All members deleted.\n", this_player());   
        else message("mail", "\n%^GREEN%^Members deleted from alias.\n",   
                this_player());   
    }   
    else message("mail", "\n%^GREEN%^Members entered into alias.\n",
            this_player());   
    if(__CommandLine) this_object()->eventDestruct();
    else {
        secondary_prompt();   
        input_to("alias_cmd");
    }   
}   

static void get_alias(string str, string cmd) {   
    if(str == "" || !str) { 
        if(__CommandLine) destruct_box("Invalid alias name.");
        else postal_error("Invalid alias name.");  
        return;   
    }   
    alias_members(cmd, str);   
}   

static void get_members(string str, string *args) {   
    if(str == "" || !str) { 
        if(__CommandLine) destruct_box("Invalid alias members.");
        else postal_error("Invalid alias members.");  
        return;   
    }   
    alias_members(args[0], sprintf("%s %s", args[1], str));   
}   

static private void alias_creation(string cmd, string args) {  
    string *members;

    if(args == "" || !args) {  
        message("prompt", sprintf("Name of alias to %s: \n",   
                    (cmd == "m" ? "make" : "remove")), this_player());  
        input_to("get_alias_name", cmd);  
        return;  
    }  
    if(cmd == "m" && sizeof(members=explode(args=lower_case(args)," ")) == 1) {  
        message("prompt", "Enter members for the alias: \n", this_player());  
        input_to("get_new_alias_members", ({ cmd, args }));  
        return;  
    }  
    else if(cmd == "m") members = members - ({ args = members[0] });  
    else members = 0;  
    if(cmd == "m" && (user_exists(args) || LOCALPOST_D->query_mud_group(args))) {  
        if(__CommandLine) destruct_box("Invalid alias name.");
        else postal_error("Invalid alias name.");  
        return;  
    }  
    if(!(OPTIONS_D->query_group(__Owner, args))) {  
        if(cmd == "r") { 
            if(__CommandLine) destruct_box("No such alias to remove.");
            else postal_error("No such alias to remove.");  
            return;  
        }  
    }  
    else if(cmd == "m") {  
        if(__CommandLine) destruct_box("Alias already exists.");
        else postal_error("That alias already exists.");  
        return;  
    }  
    OPTIONS_D->set_group(__Owner, args, members);  
    if(__CommandLine) 
        destruct_box(sprintf("Alias %s.", (cmd == "m" ? "made" : "removed")));
    else 
        postal_success(sprintf("Alias %s.", (cmd == "m" ? "made" : "removed")));
}           

static void get_alias_name(string str, string cmd) {  
    if(str == "" || !str) {
        if(__CommandLine) destruct_box("Invalid alias name.");
        else postal_error("Invalid alias name.");  
        return;  
    }  
    alias_creation(cmd, lower_case(str));  
}  

static void get_new_alias_members(string str, string *args) {  
    if(str == "" || !str) { 
        if(__CommandLine) destruct_box("Invalid alias members.");
        else postal_error("Invalid alias members.");  
        return;  
    }  
    alias_creation(args[0], sprintf("%s %s", args[1], lower_case(str)));  
}  

static private void list_alias(string str) {  
    string *who;  

    if(str == "" || !str) {
        if(__CommandLine) destruct_box("Invalid alias name.");
        else postal_error("Invalid alias name.");  
        return;  
    }  
    if(!(who = LOCALPOST_D->query_mud_group(str=lower_case(str)))) {  
        who = OPTIONS_D->query_group(__Owner, str);  
    }  
    if(!who) {
        if(__CommandLine) destruct_box("No such alias exists.");
        else postal_error("No such alias exists.");  
        return;  
    }  
    message("mail", "\n%^INITTERM%^"+center(sprintf("Alias: %s", str),  
                __Screen), this_player());  
    message("mail", "\n\n"+implode(who, ",   "), this_player());
    if(__CommandLine) this_object()->eventDestruct();
    else {
        secondary_prompt();  
        input_to("alias_cmd");
    }
}  

static private void save_options() {  
    string *cles;
    int i;

    if(!__ChangedOptions) {  
        postal_error("No options have changed.");  
        return;  
    }  
    i = sizeof(cles = keys(__ChangedOptions));
    while(i--) 
        OPTIONS_D->set_option(__Owner, cles[i], __ChangedOptions[cles[i]]);
    __Options = OPTIONS_D->query_options(__Owner);  
    postal_success("New options now saved.");  
}  

static private void change_option(int x) {  
    if(pointerp(__PostalOptions[x]["value"])) {  
        message("mail", sprintf("%s\n\t0) %s\n\t1) %s\n", 
            __PostalOptions[x]["desc"],
            __PostalOptions[x]["value"][0], __PostalOptions[x]["value"][1]),  
            this_player());  
    }
    else message("mail", __PostalOptions[x]["desc"], this_player());  
    message("prompt", "Enter in a proper value: \n", this_player());  
    input_to("really_change_option", 0, x);  
}  

static void really_change_option(string str, int x) {  
    int y;

    if(!__ChangedOptions) __ChangedOptions = ([]);
    if(pointerp(__PostalOptions[x]["value"])) {  
        if((y=to_int(str)) != 0 && y != 1) {
            postal_error("Invalid value.  No option has changed.");  
            return;  
        }  
        __ChangedOptions[__PostalOptions[x]["key"]] = y;  
    }  
    else __ChangedOptions[__PostalOptions[x]["key"]] = (str == "" ? 0 : str);  
    postal_success("You must remember to save for this option to take effect.");  
}  

static private void reply(string str) {
    int x, i;

    if(str == "" || !str) x = __Current;
    else x = to_int(str)-1;
    if(x < 0 || x >= sizeof(__BoxInfo)) {
        postal_error("Invalid letter number for reply.");
        return;
    }
    set_current(x);
    __TmpPost = ([ "date":time(), "from": this_player()->GetKeyName()]);
    if((__TmpPost["subject"] = __BoxInfo[x]["subject"])[0..2] != "Re:") 
        __TmpPost["subject"] = sprintf("Re: %s", __TmpPost["subject"]);
    message("prompt", "Include original text (default 'n'): \n", this_player());
    input_to("get_reply_confirm");
}

static void get_reply_confirm(string str) {
    if(str == "" || !str) str = "n";
    else str = lower_case(str)[0..0];
    if(str == "y") __FwdRply = ({ "r", query_reply_text() });
    else if(str != "n") {
        message("prompt", "Answer 'y' or 'n': \n", this_player());
        input_to("get_reply_confirm");
        return;
    }
    else __FwdRply = 0;
    message("mail", sprintf("\n%s", center("Reply to: a)ll, c)c list and "
            "sender, s)ender only, t)o list and sender",__Screen)), this_player());
    message("prompt", "\nEnter choice (default 's'): \n", this_player());
    input_to("get_reply_list");
}

static void get_reply_list(string str) {
    int i;

    if(str == "" || !str) str = "s";
    else str = lower_case(str)[0..0];
    __TmpPost["to"] = ({ convert_name(__BoxInfo[__Current]["from"]) });
    switch(str) {
        case "a": 
            __TmpPost["to"] += __BoxInfo[__Current]["to"];
        __TmpPost["cc"] = __BoxInfo[__Current]["cc"];
        break;
        case "c": __TmpPost["cc"] = __BoxInfo[i]["cc"]; break;
        case "s": __TmpPost["cc"] = ({}); break;
        case "t":
            __TmpPost["to"] += __BoxInfo[__Current]["to"];
        __TmpPost["cc"] = ({});
        break;
        default:
        message("prompt","Invalid choice.  Choose again: \n", this_player());
        input_to("get_reply_list");
        return;
    }
    if(!__Options["askcc"]) get_cc("");
    else {
        message("prompt", "Copies to: \n", this_player());
        input_to("get_cc");
    }  
}

static private string query_reply_text() {
    string tmp;

    tmp = LETTERS_D->query_letter(__BoxInfo[__Current]["id"]);
    return sprintf("\n>%s\n", replace_string(tmp, "\n", "\n>"));
}

static private void forward_letter(string str, int flag) {
    string *args;
    int i, x;

    if(str == "" || !str) {
        message("prompt", "To: \n", this_player());
        input_to("get_forward_list");
        return;
    }
    i = sizeof(args = explode(lower_case(str), " "));
    if(!flag) {
        while(i--) if(sscanf(args[i], "%d", x) && x) break;
        if(!x) x = __Current;
        else {
            x--;
            if( !i ) {
                if(sizeof(args) == 1) args = ({});
                else args = args[1..];
            }
            else if( i == sizeof(args)) {
                args = args[0..<2];
            }
            else if(sizeof(args) == 1)
                args = ({});
            else args = args[0..(i-1)] + args[(i+1)..];
        }
        if(x < 0 || x > sizeof(__BoxInfo)) {
            postal_error("Letter number is out of range.");
            return;
        }
        if(!sizeof(args)) {
            forward_letter(0, 1);
            return;
        }
    }
    set_current(x);
    __TmpPost["to"] = args;
    if((__TmpPost["subject"]=__BoxInfo[__Current]["subject"])[0..4] !=
            "(fwd)") __TmpPost["subject"] = sprintf("(fwd) %s",__TmpPost["subject"]);
    __TmpPost["from"] = this_player()->GetKeyName();
    __TmpPost["date"] = time();
    message("prompt", "Comment on original letter (default 'n')? \n", 
            this_player());
    input_to("confirm_comments");
}

static void get_forward_list(string str) {
    if(str == "" || !str) {
        message("prompt","Invalid recipients.  Abort forward (default 'y')? \n",
                this_player());
        input_to("confirm_forward_abort");
        return;
    }
    else forward_letter(str, 1);
}

static void confirm_comments(string str) {
    if(str == "" || !str) str = "n";
    else str = lower_case(str)[0..0];
    if(str == "y") __FwdRply = ({ "f", query_forward_text() });
    else if(str == "n") {
        __FwdRply = 0;
        __TmpPost["message"] = query_forward_text();
    }
    else {
        message("prompt", "Answer 'y' or 'n': \n", this_player());
        input_to("confirm_comments");
        return;
    }
    if(!__Options["askcc"]) get_cc("");
    else {
        message("prompt", "Copies to: \n", this_player());
        input_to("get_cc");
    }
}

static private string query_forward_text() {
    string tmp;

    tmp = LETTERS_D->query_letter(__BoxInfo[__Current]["id"]);
    tmp = ">"+replace_string(tmp, "\n", "\n>");
    return sprintf("Original letter sent by %s %s:\n%s\n%s\n", 
            capitalize(__BoxInfo[__Current]["from"]), 
            postal_time(__BoxInfo[__Current]["date"]),
            center("---  ---    Begin Forwarded Text    ---  ---"), tmp);
}

static void confirm_forward_abort(string str) {
    if(str == "" || !str) str = "y";
    else str = lower_case(str)[0..0];
    if(str == "y") {
        postal_error("Forward aborted.");
        return;
    }
    else if(str != "n") {
        message("prompt", "Answer 'y' or 'n': \n", this_player());
        input_to("confirm_forward_abort");
        return;
    }
    message("prompt", "To: \n", this_player());
    input_to("get_forward_list");
}

static private void send_letter(string *args) { 
    string flag, tmp; 
    int j, i, maxi, x; 

    if(!args || !sizeof(args)) { 
        message("prompt", "To: \n", this_player()); 
        input_to("get_to"); 
        return; 
    } 
    __FwdRply = 0;
    __TmpPost = ([ "from": this_player()->GetKeyName(),"date":time()]);
    for(i=0, maxi = sizeof(args); i<maxi; i++) { 
        if(args[i] == "" || !args[i]) continue; 
        if(!flag && i < maxi-1 && args[i][0] == '-' && strlen(args[i]) > 1) { 
            if(member_array(args[i][1..1], ({"c","s","i"})) == -1)  
                continue; 
            else flag = args[i][1..1]; 
        } 
        else if(!flag && args[i][0] == '\\' && strlen(args[i]) > 1)  
            args[i] = args[i][1..strlen(args[1])-1];  
        else if(flag) { 
            if(args[i][0] != '\'') tmp = args[i]; 
            else if(args[i][x=strlen(args[i])-1] =='\'') tmp =args[i][1..x-1]; 
            else { 
                tmp = args[j=i][1..x]; 
                while(++j < maxi && args[j][x=strlen(args[j])-1] != '\'') 
                    if(x > 0) tmp += " "+args[j][0..x-1]; 
                if(args[j][x] == '\'' && x > 1) tmp += " "+args[j][0..x-1]; 
                i = j; 
            } 
            switch(flag) { 
                case "c":  
                    if(!__TmpPost["cc"]) __TmpPost["cc"] = ({}); 
                __TmpPost["cc"] += ({ convert_name(tmp) });  
                break; 
                case "s": __TmpPost["subject"] = tmp; break; 
                case "i":  
                    tmp=absolute_path(this_player()->get_path(),tmp); 
                if(sscanf(tmp,user_path(this_player())+"%s", flag) != 1 || 
                        !(master()->valid_read(tmp,this_player()))) { 
                    if(__CommandLine) { 
                        this_object()->eventDestruct(); 
                        return; 
                    } 
                    __TmpPost = ([]); 
                    postal_error("Access denied."); 
                    return; 
                } 
                if(!file_exists(tmp)) { 
                    if(__CommandLine) { 
                        this_object()->eventDestruct(); 
                        return; 
                    } 
                    __TmpPost = ([]); 
                    postal_error(sprintf("File %s does not exist.",tmp)); 
                    return; 
                } 
                if(!(__TmpPost["message"] = read_file(tmp))) 
                    __TmpPost["message"] = "EMPTY FILE"; 
                break; 
            } 
            flag = 0; 
        } 
        else {
            if(!grepp(args[i],"@")){
                if(!__TmpPost["to"]) __TmpPost["to"] = ({ trim(convert_name(args[i])) });
                else __TmpPost["to"] += ({ trim(convert_name(args[i])) });
            }
            else {
                if(!__TmpPost["to"]) __TmpPost["to"] = ({ trim(args[i]) });
                else __TmpPost["to"] += ({ trim(args[i]) });
            }
        } 
    }
    if(!__TmpPost["to"]) { 
        message("prompt", "To: \n", this_player()); 
        input_to("get_to"); 
        return; 
    } 
    if(!__TmpPost["subject"]) { 
        message("prompt", "Subject:\n ", this_player()); 
        input_to("get_subject"); 
        return; 
    } 
    else message("mail", sprintf("Subject: %s", __TmpPost["subject"]), 
            this_player()); 
    if(!__TmpPost["cc"] && __Options["askcc"]) { 
        message("prompt", "Copies to: \n", this_player()); 
        input_to("get_cc"); 
        return; 
    } 
    else if(!__TmpPost["cc"]) __TmpPost["cc"] = ({}); 
    if(!__TmpPost["message"]) get_cc("");
    else {
        __TmpPost["message"] = sprintf("%s%s", __TmpPost["message"],
                query_signature());
        confirm_send(); 
    } 
} 

static void get_to(string str) { 
    if(str == "" || !str) { 
        if(__CommandLine) { 
            this_object()->eventDestruct(); 
            return; 
        } 
        postal_error("No recipients given.  Mail aborted."); 
        return; 
    } 
    send_letter(explode(str, ",")); 
} 

static void get_subject(string str) { 
    string tmp; 

    if(str == "" || !str) { 
        message("prompt", "No subject given.  Continue (default 'n')? \n", 
                this_player()); 
        input_to("confirm_subject"); 
        return; 
    } 
    __TmpPost["subject"] = str; 
    if(!__TmpPost["cc"] && __Options["askcc"]) { 
        message("prompt", "Copies to: \n", this_player()); 
        input_to("get_cc"); 
        return; 
    } 
    else if(!__TmpPost["cc"]) __TmpPost["cc"] = ({}); 
    if(!__TmpPost["message"]) get_cc("");
    else {
        __TmpPost["message"] = sprintf("%s%s", __TmpPost["message"],
                query_signature());
        confirm_send(); 
    } 
} 

static void confirm_subject(string str) { 
    if(str == "" || !str) str = "n"; 
    else str = lower_case(str)[0..0]; 
    if(str == "n") { 
        if(__CommandLine) { 
            this_object()->eventDestruct(); 
            return; 
        } 
        __TmpPost = ([]); 
        postal_error("Mail aborted."); 
        return; 
    } 
    else if(str == "y") get_subject("[No Subject]"); 
    else { 
        message("prompt", "Answer 'y' or 'n': \n", this_player()); 
        input_to("confirm_subject"); 
        return; 
    } 
} 

static void get_cc(string str) { 
    string tmp; 

    if(!__TmpPost["cc"]) __TmpPost["cc"] = ({});
    if(str && str != "") __TmpPost["cc"] += explode(str, ",");
    if(__TmpPost["message"]) {
        __TmpPost["message"] = sprintf("%s%s", __TmpPost["message"],
                query_signature()); 
        confirm_send();
        return; 
    }
    if(file_exists(tmp = tmpmail()))
        rm(tmp);
    if(!__FwdRply || __FwdRply[0] == "f") { 
        message("mail",sprintf("\n%%^INITTERM%%^%s", header(__TmpPost)), 
                this_player()); 
        this_player()->eventEdit(tmp, (: complete_send :));
        return; 
    } 
    write_file(tmp, __FwdRply[1]); 
    message("mail", sprintf("\n%%^INITTERM%%^%s", header(__TmpPost)),
            this_player());
    this_player()->eventEdit(tmp, (: complete_send :));
} 

void complete_send() { 
    string str; 

    if( !(str = read_file(tmpmail())))
        str = "No message."; 
    if(__FwdRply && __FwdRply[0] == "f")
        str = sprintf("%s\n%s\n%s", str, __FwdRply[1], query_signature());
    else str = sprintf("%s%s", str, query_signature()); 
    __TmpPost["message"] = str;
    __FwdRply = 0;
    confirm_send(); 
} 

static private string query_signature() { 
    string tmp; 

    if(!__Options["sig file"]) return ""; 
    if(strsrch(__Options["sig file"], " ") != -1) 
        return sprintf("\n%s", __Options["sig file"]);     
    tmp = absolute_path(homedir(this_player()), 
            __Options["sig file"]); 
    if(!(master()->valid_read(tmp, this_player())) || !file_exists(tmp)) {
        return sprintf("\n%s", __Options["sig file"]); 
    } 
    return read_file(tmp); 
} 

static private void confirm_send() {
    message("mail", center("e)dit, f)orget, s)end", __Screen), this_player());
    message("prompt", "\nCommand (default 's'): \n", this_player());
    input_to("handle_send_choice");
}

static void handle_send_choice(string str) {
    string tmp = tmpmail();

    if(str == "" || !str) str = "s";
    else str = lower_case(str)[0..0];
    switch(str) {
        case "s":
            notify_send(LOCALPOST_D->send_post(copy(__TmpPost)));
        break;
        case "f":
            __TmpPost = ([]);
        postal_error("Mail aborted!");
        break;
        case "e":
            if(file_exists(tmp)) rm(tmp); 
        write_file(tmp, __TmpPost["message"]);
        map_delete(__TmpPost, "message");
        this_player()->eventEdit(tmp, (: complete_send :));
        break;
        default:
        message("prompt", "Invalid command.  Command: \n", this_player());
        input_to("handle_send_choice");
        break;
    }
}

static private void notify_send(string *failures) { 
    string *arr; 
    string tmp; 

    if(!sizeof(failures)) 
        message("mail", "\n\t%^GREEN%^Mail successfully sent!", this_player()); 
    else { 
        message("mail", sprintf("\nFailed to send to: %s", 
                    implode(failures, ",  ")), this_player()); 
        if(creatorp(this_player())) { 
            write_file(tmp=homedir()+"/tmp/dead.letter", 
                    __TmpPost["message"]);
        } 
        else write_file(tmp = sprintf("%s/%s.letter", homedir()+"/tmp", 
                    this_player()->GetKeyName()), __TmpPost["message"]); 
        message("mail", sprintf("A copy of the letter was saved to %s", tmp), 
                this_player()); 
    } 
    if(sizeof(arr=distinct_array(__TmpPost["to"]+__TmpPost["cc"])-failures)) 
        message("mail", sprintf("Mail sent to: %s", implode(arr,",  ")), 
                this_player()); 
    if(__CommandLine) { 
        this_object()->eventDestruct(); 
        return; 
    } 
    secondary_prompt(); 
    input_to("index_cmd"); 
} 

void incoming_post() {
    mixed tmp;
    string id;
    int *delete;

    if(__Folder != "new") return;
    if(file_name(previous_object()) != FOLDERS_D) return;
    id = (__Current < 0 ? 0 : __BoxInfo[__Current]["id"]);
    delete =map_array(tmp=FOLDERS_D->query_box_info(__Owner,"new"), 
            "order_box", this_object());
    __BoxInfo = copy(tmp);
    set_current(id);
    __Delete = copy(delete);
    __IncomingFlag = 1;
}

int order_box(mapping borg) {
    int i;

    i = sizeof(__BoxInfo);
    while(i--) if(__BoxInfo[i]["id"] == borg["id"]) return __Delete[i];
    return 0;
}
