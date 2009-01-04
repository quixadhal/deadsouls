inherit "/std/quest_ob";

void create() {
    ::create();
    set_name("zemoch");
    set_id( ({ "zemoch", "quest_object", "zemoch quest" }));
    set_short("The Zemoch Quest");
    set_long(
            "You must stop the evil Zemochs from sacrificing any more innocents.\n"
            "Helping others will help yourself in destroying the evil god of the\n"
            "Zemochs, Azash.\n"
            "Quest rating: High Mortal, extremely difficult\n" );
    set_quest_points(45);
}
