#include <lib.h>
#include <vendor_types.h>

inherit LIB_BARKEEP;

static void create() {
    barkeep::create();
    SetKeyName("gloria");
    SetId(({"woman","worker","gloria","lady","lunch lady"}));
    SetShort("Gloria, the lunch lady");
    SetLong("Gloria is a short, tired-looking woman getting along in "+
            "her years. She doesn't seem very happy...in fact, she looks downright "+
            "grumpy. Maybe if you ask her, she'll sell you something on the menu. Then "+
            "again, maybe not.");
    SetInventory(([
                "/domains/campus/armor/collar" : "wear collar on neck",
                "/domains/campus/obj/spam" : 1,
                "/domains/campus/armor/foodsmock" : "wear smock",
                ]));
    SetMenuItems(([
                ({ "sandwich", "ham sandwich" }) : "/domains/campus/meals/ham_sand",
                ({ "burger", "hamburger" }) : "/domains/campus/meals/burger",
                ({ "salad", "salad of the day" }) : "/domains/campus/meals/salad",
                ({ "milk", "carton of milk" }) : "/domains/campus/meals/milk",
                ({ "gatorade", "sports drink" }) : "/domains/campus/meals/gator",
                ]));
    SetLevel(1);
    SetRace("human");
    SetSkill("bargaining", 1);
    SetGender("female");
    SetLocalCurrency("dollars");
}
void init(){
    ::init();
}
