#include <lib.h>
#include <daemons.h>
#include ROOMS_H

inherit LIB_STORAGE;

string DescribeItems(mixed var);
string DescribeLiving(mixed var);
void RegisterPortal();

object dest;

mixed direct_enter_obj(){
    return 1;
}

mixed direct_enter_into_obj(){
    return direct_enter_obj();
}

void create() {
    object tmp;
    ::create();
    tmp = PORTAL_D->GetOrangePortal();
    if(tmp) dest = tmp;
    else catch( dest = load_object(ROOM_START) );
    SetKeyName("portal");
    SetAdjectives( ({"strangely","glowing","pulsating","blue"}) );
    SetId( ({ "aperture" }) );
    SetShort("a glowing blue portal");
    SetLong("A strangely glowing and pulsating portal.");
    SetMass(0);
    SetBaseCost("silver",1);
    SetMaxCarry(0);
    SetPreventGet("You fail.");
    SetNoClean(1);
    SetNoCondition(1);
    call_out( (: RegisterPortal :), 0);
}

void RegisterPortal(){
    object env = room_environment(this_object());
    eventMove(env);
    dest = PORTAL_D->GetOrangePortal();
    PORTAL_D->RegisterPortal(env);
}

int eventEnter(object who){
    object tmp = PORTAL_D->GetOrangePortal();
    mixed pg;
    if(tmp) dest = tmp;
    if(dest == environment(who)){
        dest = 0;
        pg = filter(deep_inventory(who),
                (: base_name($1) == "/domains/default/obj/generator" :) );
        if(sizeof(pg)) pg = pg[0];
        if(pg && objectp(pg)) pg->eventMove(environment(who));
        while(!dest){
            string room = random(1000)+","+random(1000)+","+random(1000);
            catch(dest = load_object("/domains/town/virtual/space/"+room));
        }
    }
    if(dest){
        who->eventMoveLiving(dest);
    }
    else {
        write("You are unable to enter it.");
    }
    return 1;
}

mixed eventDescribeEndpoint() {
    int i;
    string file, str;
    object env, *livings, *items;
    dest = PORTAL_D->GetOrangePortal();

    str = "in the portal";
    env = dest;
    if(dest) file = base_name(dest)+".c";

    if(!env || env->GetProperty("no peer")){
        return "You can't see anything in there.";
    }
    if( (i = this_player()->GetEffectiveVision(file,1)) > 5 )
        return "It is too bright in there.";
    else if( i < 3 )
        return "It is too dark there.";

    items = filter(all_inventory(env),
            (: !$1->GetInvis(this_player()) :) );
    items = items - (livings = filter(items, (: living :)));
    message("my_action", "%^GREEN%^"
            "Looking "+str+" you see...",
            this_player() );
    message("other_action",
            this_player()->GetCapName()+" looks "+str+".",
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

varargs mixed eventShowInterior(object who, object target){
    eventDescribeEndpoint();
    return 1;
}

mixed eventFall(){
    return 0;
}

mixed eventSink(){
    return 0;
}

void init(){
    ::init();
}
