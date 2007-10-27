#include <lib.h>

inherit LIB_NPC;

void check_quest(string *what);
void check_immortal(string a, string b);

void create() {
    ::create();
    SetKeyName("unity");
    SetId(  ({ "unity" }) );
    SetShort( "Unity, the grantor of immortality");
    SetAggressive( 0);
    SetLevel(28);
    SetLong( "Unity grants immortality to those high mortals with mentors to sponsor them.");
    SetMorality(1000);
    SetRace( "human");
    SetMaxHealthPoints(100000);
    SetHealthPoints(100000);
    SetRace("human");
}

void catch_tell(string str) {
    object ob;
    string a, b, c;

    if(this_player() == (ob = this_object())) return;
    if(sscanf(str, "%squest%s", a, b) == 2 || sscanf(str, "%stask%s", a, b) == 2) {
        this_object()->eventForce("speak in farsi You must find for me the Orc Slayer!  The evil orcs have taken it so that it cannot be used against them.  The people of Praxis must have it back.");
        return;
    }
    if(sscanf(str, "%shere%slayer%s", a, b, c) == 3) {
        this_object()->eventForce("speak in farsi The Orc Slayer is in the treasury in the Valley of the Orcs.");
        return;
    }
    if(sscanf(str, "%s gives you %s", a, b) == 2) {
        call_out("check_quest", 2, ({ lower_case(a), lower_case(b) }) );
    }
}

void check_quest( string *what ) {
    object tp, ob;
    string who, that;

    who = what[0];
    that = what[1];
    tp = present(who, environment(this_object()));
    ob = present("sword");
    if(!tp) return;
    if(!ob) {
        tell_object(tp, "Unity says: Nice try.  Now leave or die.");
        return;
    }
    if(!ob->id("the_one_orc_slayer")) {
        tell_object(ob, "Unity says: This is very nice, but I do not need it.");
        eventForce("give "+that+" to "+who);
        return;
    }
    tell_room(environment(this_object()), "Unity smiles happily.", this_object());
    tell_room(environment(this_object()), tp->query_cap_name()+" has completed the quest for the Orc Slayer.", ({ tp, this_object() }));
    tell_object(tp, "Unity says: You have done well.  You shall some day be a high mortal!");
    ob->destruct();
    if(tp->set_quest("orcslayer")) tp->add_exp(1000);
}
