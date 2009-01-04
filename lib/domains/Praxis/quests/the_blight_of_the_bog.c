inherit "/std/quest_ob";

void create() {
    ::create();
    set_name("the_blight_of_the_bog");
    set_id( ({"blight", "quest_object", "the blight of the bog"}) );
    set_short( "%^GREEN%^%^BOLD%^The Blight of the Bog%^RESET%^");
    set_long( "Rumours have been floating about concerning a strange "
            "disease that is ravaging sections of the North Forest. "
            "You must find the source of this sickness and figure out how "
            "to cure the horrible blight before it devastates the entire "
            "forest surrounding the bog.\nRating: High Mortal");
    set_quest_points(40);
}
