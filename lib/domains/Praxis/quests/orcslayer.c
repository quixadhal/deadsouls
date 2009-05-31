inherit "/std/quest_ob";

void create() {
    ::create();
    set_name("orcslayer");
    set_id( ({ "orcslayer", "quest_object" }) );
    set_short("The quest of the immortals");
    set_long(
            "Unity is the grantor of immortality.  He has a quest which he\n"+
            "requires of all mortals before they ascend to the status of\n"+
            "high mortal.  Go to him and ask of him the task.\n"+
            "Quest rating: easy\n"
            );
    set_quest_points(20);
}
