inherit "/std/quest_ob";

void create() {
    ::create();
    set_name("evil");
    set_id( ({ "evil", "quest_object" }) );
    set_short("The Evil Quest");
    set_long(
            "Find the evil one and plunge the silver artifact into him.\n"+
            "Quest rating: very experienced\n");
    set_quest_points(20);
}
