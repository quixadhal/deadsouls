inherit "/std/quest_ob";

void create() {
    ::create();
    set_name("pyramid");
    set_id( ({ "pyramid", "quest_object" }));
    set_short("%^BLUE%^%^BOLD%^The Amonia Ra Gypt Quest%^RESET%^");
    set_long(
            "Evil forces have captured the arch priestess of the Egytian Sun God, "
            "Ammonia Ra Gypt. Try to find the kidnappers and free the innocent before "
            "she gets scacrificed to Osiris.\n"
            "Quest rating: High mortal.\n" );
    set_quest_points(35);
}
