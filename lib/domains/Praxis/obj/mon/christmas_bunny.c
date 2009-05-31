inherit "/lib/sentient";

int count_eggs;

void create() {
    ::create();
    SetKeyName("easter bunny");
    SetId(  ({ "bunny", "easter bunny" }) );
    SetShort( "A Confused Easter bunny");
    SetLong( "Hippity Hoppity.\nOh, joy, it's the Easter Bunny.\nWatch him drool.\nHe is hiding Easter eggs all over Nightmare.\n");
    SetAggressive( 0);
    SetWanderSpeed(30);
    SetGender("male");
    SetLevel(30);
    SetMorality(200);
    SetRace("rodent");
    SetMaxHealthPoints(500000);
    SetHealthPoints(500000);
    SetAction(10,
            ({ "The Easter Bunny drools.\n",
             "The Easter Bunny farts at you.\n",
             "The Easter bunny looks around, confused.\n",
             "The Easter Bunny mumbles about rotten eggs.\n" })
            );
    SetSkill("melee", 400);
}

void catch_tell(string str) {
    string who;

    //if(!interact("enters", str)) return;
    sscanf(str, "%s enters%*s", who);
    who = lower_case(who);
    call_out("egghead", 2, who);
}

void egghead(string who) {
    object ob, it;

    ob = present(who, environment(this_object()));
    if(!ob) return;
    if(random(101) < 25) {
        tell_object(ob, "The Easter Bunny throws an egg at you!\n");
        tell_room(environment(this_object()), "The Easter Bunny throws an egg at "+ob->query_cap_name()+".\n", ({ ob }));
        it = new("/domains/Praxis/obj/misc/easter_egg");
        it->move(ob);
    }
}

void heart_beat() {
    object it;
    count_eggs++;
    if(count_eggs > 60) {
        count_eggs = 0;
        it = new("/domains/Praxis/obj/misc/easter_egg");
        it->move(this_object());
        eventForce("hide egg");
    }
    ::heart_beat();
}
