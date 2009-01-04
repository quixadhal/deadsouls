inherit "/std/quest_ob";

void create() {
    ::create();
    set_name("dragon");
    set_id( ({ "dragon", "quest_object" }) );
    set_short("The quest for the dragon");
    set_long(
            "An evil dragon has been attacking the floating wizard fortress.\n"+
            "Find the fortress and and help the wizards by destroying the dragon.\n"+
            "Quest rating: explorer\n");
    set_quest_points(25);
}
