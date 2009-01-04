inherit "/lib/npc";

void create() {
    ::create();
    SetKeyName("flamme's ghost");
    SetId( ({ "ghost","flamme","Flamme","Flamme's Ghost" }) );
    SetShort( "Flamme's ghost");
    SetAggressive( 0);
    SetLevel(1);
    SetLong( "The ghost still has a few tufts of auburn hair "
            "sticking out in all directions. She is smiling, but "
            "with a rather surprised look on her face.");
    SetMorality(600);
    SetRace( "wraith");
    SetGender("female");
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
    if(sscanf(str, "%ssmiles%s", a, b) == 2) tell_room(environment(this_object()), "The little ghost asks: What happened to me?", ({this_object()}));
}
