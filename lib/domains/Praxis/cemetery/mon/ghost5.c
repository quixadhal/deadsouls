inherit "/lib/npc";

void create() {
    ::create();
    SetKeyName("forlock's ghost");
    SetId( ({ "ghost","forlock","Forlock","Forlock's Ghost" }) );
    SetShort( "Forlock's ghost");
    SetAggressive( 0);
    SetLevel(1);
    SetLong( "This is Forlock's sad ghost, lost, and bereft "
            "of mortal companions.");
    SetMorality(200);
    SetRace( "wraith");
    SetGender("male");
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
    if(sscanf(str, "%ssmiles%s", a, b) == 2) tell_room(environment(this_object()), "Forlock's ghost grimaces, and asks you if you've seen his mud.", ({this_object()}));
}
