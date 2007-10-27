#include <lib.h>
#include <rooms.h>

inherit LIB_SENTIENT;

static void create() {
    ::create();
}

varargs int eventDie(object agent) {
    int x;

    SetUndead(!(x = GetUndead()));
    if( agent ) {
        if( x ) agent->eventDestroyEnemy(this_object());
        else agent->eventKillEnemy(this_object());
    }
    if( environment() ) {
        object *obs;
        string *currs;
        object ob;
        string curr;
        int i;

        ob = new(LIB_BOT_CORPSE);
        ob->SetCorpse(this_object());
        ob->eventMove(environment());
        obs = filter(all_inventory(), (: !((int)$1->GetRetainOnDeath()) :));
        i = sizeof(obs);
        while(i--) obs[i]->eventMove(ob);
        currs = (string *)this_object()->GetCurrencies() || ({});
        foreach(curr in currs) {
            object pile;
            int amt;

            if( amt = (int)this_object()->GetCurrency(curr) ) {
                pile = new(LIB_PILE);
                pile->SetPile(curr, amt);
                pile->eventMove(ob);
                this_object()->AddCurrency(curr, -amt);
            }
        }
    }
    this_object()->eventMove(ROOM_FURNACE);
    return 1;
}

string GetLong(string nom) {
    string *limbs;
    string str;
    float h;

    if(!nom) nom = this_object()->GetKeyName();
    str = ::GetLong();
    if(!(this_object()->GetNoCondition())){
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
    if( sizeof(limbs) ) {
        int i, maxi;

        str += nom + " is missing " + add_article(limbs[0]); 
        for(i=1, maxi = sizeof(limbs); i<maxi; i++) {
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
