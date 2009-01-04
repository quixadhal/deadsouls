inherit "/lib/npc";

void create() {
    ::create();
    SetKeyName("darkone's ghost");
    SetId( ({ "ghost","darkone","Darkone","Darkone's Ghost" }) );
    SetShort( "Darkone, the ghost with a bent neck");
    SetAggressive( 0);
    SetLevel(1);
    SetLong( "This is Darkone's evil ghost.  He is smiling so "
            "evilly that huge black horns have sprouted on his head. "
            "He is deeply grieved he can no longer torture wizards "
            "by nit-picking in their castles.");
    SetMorality(-600);
    SetRace( "wraith");
    SetGender("neuter");
    SetMaxHealthPoints(25);
    SetCurrency("gold",25);
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
        tell_room(environment(this_object()), "Darkone's ghost "
                "groans loudly: Aooooooooooooo...*gerk*  (The howl "
            "cuts off abruptly.)", ({this_object()}));
}
