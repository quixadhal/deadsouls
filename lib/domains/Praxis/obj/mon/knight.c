inherit "/lib/npc";

create() {
    ::create();
    SetKeyName("knight");
    SetId( ({ "knight" }) );
    SetShort("A local knight");
    SetLevel(13);
    SetLong( "She serves justice.\n");
    SetGender("female");
    SetRace( "human");
    SetMorality(300);
    SetHealthPoints(450);
    AddLimb("head", "FATAL", 100, 0, 4);
    AddLimb("torso", "FATAL", 201, 0, 4);
    AddLimb("right arm", "right hand", 70, 0, 4);
    AddLimb("right hand", "", 50, 0, 4);
    AddLimb("left arm", "left hand", 70, 0, 4);
    AddLimb("left hand", "", 50, 0, 4);
    AddLimb("right leg", "right foot", 70, 0, 4);
    AddLimb("right foot", "", 50, 0, 4);
    AddLimb("left leg", "left foot", 70, 0, 4);
    AddLimb("left foot", "", 50, 0, 4);
}

void catch_tell(string str) {
    object tp;
    string a, b, c;

    if(sscanf(str, "%suest%s", a, b) == 2) {
        tell_room(environment(this_object()), "The knight says: I ventured into the goblin caves on a quest for glory.\nI ended up barely escaping with my life.\nAlthough I am now quite respected, I still feel honorless over the loss of\nmy great-grandfather's magic crystal ball.\nPlease return it to me!\n", ({ this_object() }));
        return;
    }
    if(sscanf(str, "%s gives you %s", a, b) == 2) {
        a = lower_case(a);
        tp = present(a, environment(this_object()));
        if(!tp) return;
        call_out("check_ball", 2, tp);
        return;
    }
}

void check_ball(object tp) {
    object ob;

    ob = present("honor_quest_ob", this_object());
    if(!ob) {
        tell_object(tp, "This is very nice, but it is not what I seek.\n");
        return;
    }
    ob->destruct();
    if(!tp->set_quest("honor")) return;
    tell_object(tp, "The knight says: You have restored my honor!\nThe knight thanks you very much.\n");
    tell_object(tp, "The knight pays you 10000 copper coins in reward.\n");
    tp->AddCurrency("copper", 10000);
    tp->add_exp(500);
    tp->add_alignment(20);
}
