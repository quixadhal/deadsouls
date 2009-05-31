inherit "/lib/npc";

create() {
    ::create();
    SetKeyName("descartes's ghost");
    SetId( ({ "ghost","descartes","Descartes","Descartes's Ghost" }) );
    SetShort("Descartes's ghost");
    SetAggressive( 0);
    /*
       set_speed(9);
       set_moving(1);
     */

    SetLevel(1);
    SetLong("This is an android ghost, rusted immobile by a sudden, unexpected mud-rain.\n");
    SetMorality(400);
    SetRace( "wraith");
    SetGender("male");
    SetMaxHealthPoints(25);
    SetHealthPoints(25);
    AddLimb("head", "FATAL", 25, 0, 4);
    AddLimb("torso", "FATAL", 50, 0, 4);
    AddLimb("right arm", "right hand", 20, 0, 4);
    AddLimb("right hand", "", 15, 0, 4);
    AddLimb("left arm", "left hand", 20, 0, 4);
    AddLimb("left hand", "", 15, 0, 4);
    AddLimb("right leg", "right foot", 25, 0, 4);
    AddLimb("right foot", "", 20, 0, 4);
    AddLimb("left leg", "left foot", 25, 0, 4);
    AddLimb("left foot", "", 20, 0, 4);
}

void catch_tell(string str) {
    object ob;
    string a, b, c;

    if(sscanf(str, "%s gives %s to you", a, b) == 2) {
        ob = present( lower_case(a), environment(this_object()));
        if(ob) {
            tell_object(ob, "The ghost thanks you for your generosity.\n");
            tell_room(environment(this_object()), "The ghost thanks "+a+" for "+ob->query_possessive()+" generosity.\n", ({this_object(), ob}));
        }
    }
    if(sscanf(str, "%ssmiles%s", a, b) == 2) tell_room(environment(this_object()), " Descartes, the immobile ghost, makes muffled meeping sounds.\n", ({this_object()}));
}
