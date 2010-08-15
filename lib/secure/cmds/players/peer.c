/*   /secure/cmds/player/peer.c
 *   Peer into an adjacent room wihout actually entering
 *   Blitz@Dead SoulsIV
 */

#include <lib.h>

inherit LIB_DAEMON;

string DescribeItems(mixed var);
string DescribeLiving(mixed var);

mixed cmd(string str) {
    int i, err;
    string file;
    object env, *livings, *items;

    if( !sizeof(str) ) return "Syntax: peer <direction>";
    switch(str){
        case "n" : str = "north";break;
        case "ne" : str = "northeast";break;
        case "nw" : str = "northwest";break;
        case "s" : str = "south";break;
        case "se" : str = "southeast";break;
        case "sw" : str = "southwest";break;
        case "e" : str = "east";break;
        case "w" : str = "west";break;
        case "u" : str = "up";break;
        case "d" : str = "down";break;
    }
    env = environment(this_player());
    if( !file = env->GetExit(str) ) file = env->GetEnter(str);
    if( !sizeof(file) )
        return "You cannot peer that way.";
    if( (i = this_player()->GetEffectiveVision()) > 5 )
        return "It is too bright to do that.";
    if( env->GetDoor(str) && !((env->GetDoor(str))->CanPeer()) ) {
        message("my_action", sprintf("%s is blocking your view %s.",
                    (capitalize(env->GetDoor(str)->GetShort(str))), str),
                this_player() );
        return 1;
    }
    err = catch(env = load_object(file));
    if(err || !env){
        message("my_action", "It is not safe to peer "+str+"!", this_player() );
        return 1;
    }
    if(env->GetProperty("no peer")){
        return "You can't see in that direction.";
    }
    if(env->GetProperty("nopeer")){
        return "You can't see in that direction.";
    }
    if( (i = this_player()->GetEffectiveVision(file,1)) > 5 )
        return "It is too bright in that direction.";
    else if( i < 3 )
        return "It is too dark there.";

    items = filter(all_inventory(env),
            (: !$1->GetInvis(this_player()) :) );
    items = items - (livings = filter(items, (: living :)));
    message("my_action", "%^GREEN%^"
            "Peering "+str+" you see...",
            this_player() );
    message("other_action",
            this_player()->GetCapName()+" peers "+str+".",
            environment(this_player()), this_player() );
    message("room_description",
            ("\n"+env->GetLong(0)+"\n" || "\nA void.\n"),
            this_player() );
    if( sizeof(items) )
        message("room_inventory",
                "%^MAGENTA%^" + DescribeItems(items) + "%^RESET%^\n",
                this_player() );
    if( sizeof(livings) )
        message("room_inventory",
                "%^BOLD%^%^RED%^" + DescribeLiving(livings) + "%^RESET%^",
                this_player() );
    return 1;
}

string DescribeItems(mixed var) {
    mapping m = ([ ]);
    string *shorts, ret;
    int i, max;

    if( !arrayp(var) ) return "";
    i = sizeof( shorts = map(var, (: $1->GetShort() :)) );
    while(i--) {
        if( !sizeof(shorts[i]) ) continue;
        if( m[ shorts[i] ] ) m[ shorts[i] ]++;
        else m[ shorts[i] ] = 1;
    }
    i = max = sizeof( shorts = keys(m) );
    ret = "";
    for(i=0; i<max; i++) {
        if( m[ shorts[i] ] < 2 ) ret += shorts[i];
        else ret += consolidate(m[shorts[i]], shorts[i]);
        if( i == (max - 1) ) {
            if( max>1 || m[ shorts[i] ] > 1 ) ret += " are here.";
            else ret += " is here.";
        }
        else if( i == (max - 2) ) ret += ", and ";
        else ret += ", ";
    }
    return capitalize(ret);
}

string DescribeLiving(mixed var) {
    mapping m = ([ ]);
    string *shorts, ret;
    int i;
    if( !arrayp(var) ) return "";
    i = sizeof( shorts = map(var, (: $1->GetShort() :)) );
    while(i--) {
        if( !sizeof(shorts[i]) ) continue;
        if( m[ shorts[i] ] ) m[ shorts[i] ]++;
        else m[ shorts[i] ] = 1;
    }
    ret = "";
    i = sizeof( shorts = keys(m) );
    while(i--) if( m[ shorts[i] ] > 1 )
        ret += (consolidate(m[shorts[i]], shorts[i]) + "\n");
    else ret += (shorts[i] + "\n");
    return ret;
}

string GetHelp(){
    return ("Syntax: peer <direction>\n\n"
            "Allows you to look into an adjacent room without actually "
            "entering it.  Note that light and doorways affect what you "
            "see.");
}
