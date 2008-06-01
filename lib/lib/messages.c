/*    /lib/messages.c
 *    from the Dead Souls Object Library
 *    handles special messages such as leaving, coming, saying, etc.
 *    created by Descartes of Borg 950428
 */


#include "include/messages.h"

private mapping Messages;

static void create(){
    Messages = ([ "come" : "$N enters.", "leave" : "$N leaves $D.",
      "telin" : "$N teleports in.", "telout" : "$N teleports away.",
      "home" : "$N goes home.", "vis" : "$N appears.",
      "invis": "$N disappears.", "clone" : "$N clones $O.",
      "dest" : "$N dests $O." ]);
}

string SetMessage(string msg, string str){
    if(!stringp(msg) || !stringp(str)) error("Bad argument to SetMessage().");
    switch(msg){
    case "come": case "leave": case "telin": case "telout": case "home": 
    case "say": case "ask": case "exclaim": case "login": case "logout":
    case "dest": case "clone": case "vis" : case "invis" :
        return (Messages[msg] = str);
    default: return 0;
    }
}

varargs string GetMessage(string msg, mixed arg){
    string tmp;

    if( !stringp(msg) ) return 0;
    if( !(tmp = Messages[msg]) ) return 0;
    switch(msg){
    case "dest": case "clone":
        tmp = replace_string(tmp, "$O", (string)arg->GetShort());
        break;
    case "leave":
        //tc("tmp: "+tmp);
        //tc("msg: "+msg);
        //tc("arg: "+arg);
        if(stringp(arg) && strsrch(arg, "$N") == -1) 
            tmp = replace_string(tmp, "$D", arg);
        //if(stringp(arg)){ tmp = arg;}
        break;
        //The following case is a bugfix courtesy of Memrosh @ Ascension
    case "come":
        //tc("tmp: "+tmp);
        //tc("msg: "+msg);
        //tc("arg: "+arg);
        if(stringp(arg)){ tmp = arg;}
        break; 
    case "say": case "ask": case "exclaim": return tmp;
    case "login": case "logout":
        tmp = replace_string(tmp, "$M", mud_name());
        break;
    }
    if(strsrch(tmp, "$N") == -1) tmp = "$N "+tmp;
    return capitalize(replace_string(tmp, "$N", this_object()->GetName()));
}

string GetName(){ return 0; }

mapping GetMessages(){ return copy(Messages); }
