/* /wizards/melvaig/imp, Nightmare, November 1992 */
inherit "/std/quest_ob";

void create() {
    ::create();
    set_name("imp_crown");
    set_id( ({ "imp_crown" , "quest_object" }) );
    set_short("The quest for the fabled Imp crown");
    set_long(
            "Destroy the Imperial crown of Simplicity.  Monarchy has run rampant\n"+
            "and is more corrupt than days of old.\n"+
            "This is a quest for Newbies (levels 1-6).\n"+
            "Quest rating: beginner\n"
            );
    set_quest_points(15);
}
