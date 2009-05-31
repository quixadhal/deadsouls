inherit "/std/quest_ob";

void create() {
    ::create();
    set_name("hellfire");
    set_id( ({ "hellfire", "quest_object" }));
    set_short("The Hellfire Quest");
    set_long(
            "Locate the raging hellfire that is burning out of control\n"
            "and put the fire out.\n"
            "Quest rating: easy explorer.\n" );
    set_quest_points(20);
}
