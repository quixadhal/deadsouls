inherit "/lib/npc";

void create() {
    ::create();
    SetKeyName("test ghost");
    SetId( ({ "test ghost" }) );
    SetLevel(1);
    SetShort( "test ghost");
    SetLong( "Yup, it's a ghost.");
    SetRace( "wraith");
    SetGender("neuter");
    SetAggressive( 0);
    SetMorality(200);
    SetMaxHealthPoints(25);
    SetHealthPoints(25);
    SetRace("human");
}

void catch_tell(string str) {
    object ob;
    string a, b, c;

    if(sscanf(str, "%s gives %s to you", a, b) == 2) {
        ob = present( lower_case(a), environment(this_object()));
        if(ob) {
            tell_object(ob, "The ghost thanks you for your generosity.");
            tell_room(environment(this_object()), "The ghost thanks "+a+" for "+ob->query_possessive()+" generosity.", ({this_object(), ob}));
        }
    }
    if(sscanf(str, "%ssmiles%s", a, b) == 2) tell_room(environment(this_object()), "The little ghost asks plaintively: Why don't gods like me?", ({this_object()}));
}
