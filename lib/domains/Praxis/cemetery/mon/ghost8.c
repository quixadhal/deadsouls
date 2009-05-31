inherit "/lib/npc";

void create() {
    ::create();
    SetKeyName("shadowwolf's ghost");
    SetId( ({ "ghost","shadowwolf","Shadowwolf","Shadowwolf's Ghost" }) );
    SetShort( "Shadowwolf's ghost");
    SetAggressive( 0);
    SetLevel(1);
    SetLong( "The ghost is a pale wraithe, holding a NI clutched "
            "tightly to his chest.");
    SetMorality(100);
    SetRace( "wraith");
    SetGender("male");
    SetMaxHealthPoints(25);
    SetHealthPoints(5);
    SetRace("human");
    RemoveLimb("left foot",this_object());
    RemoveLimb("right hand",this_object());
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
    if(sscanf(str, "%stickle%s", a, b) == 2) tell_room(environment(this_object()), "The ghost says: It's only a flesh wound!!", ({this_object()}));
}
