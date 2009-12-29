#include <lib.h>

inherit LIB_DAEMON;
string *foenames = ({});

mixed cmd(string str) {
    object *foes = ({});
    object ob;
    string ret;
    foenames = ({});
    if(sizeof(str) && !creatorp(this_player())){
        if(find_player(lower_case(str)) != this_player())
            return "You can only know about your own enemies.";
    }
    if(!str) str = this_player()->GetKeyName();
    ob = find_player(lower_case(str));
    if(!ob) return "No such player found.";
    foes = ob->GetEnemies();
    if(!sizeof(foes)){
        ret = "No creatures bear ill will toward "+capitalize(str)+".";
    }
    else {
        ret = "The following creatures bear ill will toward "+ 
            capitalize(str)+": ";
        filter(foes, (: foenames += ({$1->GetShort()}) :) );
        ret += implode(foenames, ", ")+".";
    }
    return ret;
}

string GetHelp() {
    return ("Syntax: enemies\n\n"
            "This command reports with whom you're not getting along.");
}

