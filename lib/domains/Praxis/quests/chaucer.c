inherit "/std/quest_ob";

void create() {
    ::create();
    set_name("chaucer");
    set_id( ({"chaucer", "quest_object"}) );
    set_short( "%^YELLOW%^The Canterbury Tales%^RESET%^");
    set_long( "Something seems to be amiss inside Chaucer's "
            "most famous work. Aid the distraught pilgrims on their way to "
            "Canterbury.\nRating: Easy");
    set_quest_points(15);
}
