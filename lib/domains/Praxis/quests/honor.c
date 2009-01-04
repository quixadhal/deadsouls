inherit "/std/quest_ob";

void create() {
    ::create();
    set_name("honor");
    set_id( ({ "honor", "quest_object" }) );
    set_short("The quest for honor");
    set_long(
            "An aging knight made a mistake in her youth, and now she needs\n"+
            "her honor back.  Do what it takes to return her honor to her.\n"+
            "Quest rating: newbie\n"
            );
    set_quest_points(8);
}
