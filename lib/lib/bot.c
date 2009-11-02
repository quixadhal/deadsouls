#include <lib.h>
#include ROOMS_H
#include <daemons.h>

inherit LIB_SENTIENT;

static int NoBotCondition = 0;

static void create(){
    sentient::create();
}

void init(){
    sentient::init();
}

varargs int eventDie(mixed agent){
    int x;
    string killer, death_annc;
    object env = room_environment(this_object());
    if(objectp(agent)) killer = agent->GetName();
    if(stringp(agent)) killer = agent;
    if(!agent) killer = "UNKNOWN";
    SetUndead(!(x = GetUndead()));
    if( objectp(agent) ){
        if( x ) agent->eventDestroyEnemy(this_object());
        else agent->eventKillEnemy(this_object());
    }
    if( env ){
        object *obs;
        string *currs;
        object ob;
        string curr;
        int i;
        env = room_environment(this_object());
        ob = new(LIB_BOT_CORPSE);
        ob->SetCorpse(this_object());
        ob->eventMove(environment());
        obs = filter(all_inventory(), (: !($1->GetRetainOnDeath()) :));
        i = sizeof(obs);
        while(i--) obs[i]->eventMove(ob);
        currs = this_object()->GetCurrencies() || ({});
        foreach(curr in currs){
            object pile;
            int amt;

            if( amt = this_object()->GetCurrency(curr) ){
                pile = new(LIB_PILE);
                pile->SetPile(curr, amt);
                pile->eventMove(ob);
                this_object()->AddCurrency(curr, -amt);
            }
        }
    }
    death_annc = killer + " has destroyed "+ this_object()->GetName()+".";
    if(env) tell_room(env,this_object()->GetName()+" breaks down!",
            ({ this_object() }) ); 
    CHAT_D->eventSendChannel("SYSTEM","death",death_annc,0);

    this_object()->eventMove(ROOM_FURNACE);
    return 1;
}

string GetHealthShort(){
    string cl, sh;
    float h;

    if( !(sh = this_object()->GetShort()) ) return 0;
    h = percent(GetHealthPoints(), GetMaxHealthPoints());
    if( this_object()->GetNoBotCondition() || h > 90.0 ) cl = "%^BOLD%^GREEN%^";
    else if( h > 75.0 ) cl = "%^GREEN%^";
    else if( h > 50.0 ) cl = "%^BOLD%^BLUE%^";
    else if( h > 35.0 ) cl = "%^BLUE%^";
    else if( h > 20.0 ) cl = "%^BOLD%^RED%^";
    else cl = "%^RED%^";
    return cl + capitalize(sh);
}

string GetLong(string nom){
    string *limbs;
    string str;
    float h;

    if(!nom) nom = this_object()->GetKeyName();
    str = ::GetLong();
    if(!(this_object()->GetNoBotCondition())){
        str += capitalize(nom);
        h = percent(GetHealthPoints(), GetMaxHealthPoints());
        if( h < 10.0 ) str += " is barely functional.\n";
        else if( h < 20.0 ) str += " is functioning poorly.\n";
        else if( h < 30.0 ) str += " is severely damaged.\n";
        else if( h < 40.0 ) str += " is badly damaged.\n";
        else if( h < 50.0 ) str += " is damaged.\n";
        else if( h < 60.0 ) str += " has sustained minor damage.\n";
        else if( h < 70.0 ) str += " looks a bit battered.\n";
        else if( h < 80.0 ) str += " is in decent shape.\n";
        else if( h < 90.0 ) str += " is quite keen.\n";
        else str += " is operating at optimal levels.\n";
    }
    limbs = GetMissingLimbs();
    if( sizeof(limbs) ){
        int i, maxi;

        str += nom + " is missing " + add_article(limbs[0]); 
        for(i=1, maxi = sizeof(limbs); i<maxi; i++){
            if( i < maxi-1 ) str += ", " + add_article(limbs[i]);
            else {
                if( maxi > 2 ) str += ",";
                str += " and " + add_article(limbs[i]);
            }
        }
        str += ".\n";
    }
    return str;
}

int SetNoBotCondition(int foo){
    NoBotCondition = foo;
    return NoBotCondition;
}

int GetNoBotCondition(){
    return NoBotCondition;
}
