inherit "/lib/npc";

void create() {
    ::create();
    SetKeyName("Melissa's Ghost");
    SetId( ({ "ghost","melissa","Melissa","Melissa's Ghost" }) );
    SetShort( "Melissa's ghost");
    SetAggressive( 0);
    SetLevel(1);
    SetLong( "A misty figure in a long flowing white gown.  "
            "She is wringing her hands and weeping.");
    SetMorality(200);
    SetRace( "wraith");
    SetGender("female");
    SetCurrency("gold",random(50));
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
    if(sscanf(str, "%ssmiles%s", a, b) == 2) 
        tell_room(environment(this_object()), "The little ghost weeps piteously.", ({this_object()}));
}
