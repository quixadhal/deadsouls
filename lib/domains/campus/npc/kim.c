#include <lib.h>
#include <vendor_types.h>

inherit LIB_VENDOR;

static void create() {
    vendor::create();
    SetKeyName("kim");
    SetId(({"vendor","kim","Kim","kimmie","employee","bookstore employee"}));
    SetShort("Kim, the bookstore employee");
    SetLong("Kim is a twenty-something college student trying to "+
            "make enough money to pay for school and feed herself. She seems "+
            "friendly enough, and her bright blue eyes seem to smile at you "+
            "on their own. You could probably buy something from her here, "+
            "since that's her job, or she might even buy something from you, if "+
            "she thinks it's worth something. Don't try to buy anything if you don't "+
            "have enough money, though...she looks like she can mean business when "+
            "she has to.");
    SetInventory(([
                "/domains/campus/armor/collar" : "wear collar on neck",
                "/domains/campus/armor/jeans" : "wear jeans",
                "/domains/campus/armor/shirt" : "wear shirt",
                ]));
    SetLevel(1);
    SetRace("human");
    SetLanguage("common", 100);
    SetSkill("bargaining", 1);
    SetGender("female");
    SetLocalCurrency("dollars");
    SetStorageRoom("/domains/campus/room/bookstore2");
    SetMaxItems(10000);
    SetVendorType(VT_TREASURE | VT_ARMOR);
    SetGuard("/domains/campus/obj/register","Kim prevents your theft.");
}
void init(){
    ::init();
}
