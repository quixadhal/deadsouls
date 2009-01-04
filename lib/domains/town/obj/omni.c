#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_PRESS;
int check_environs();
int vanish_count;
static void create() {
    item::create();
    SetKeyName("omni");
    SetId(({"omni","Omni","device"}));
    SetShort("an omni");
    SetLong("This is a small, round metal device, copper in color, "+
            "and similar in appearance to a pocket watch. There is a blinking red light "+
            "next to the tiny metal button at its top.");
    SetMass(10);
    SetBaseCost("silver",300);
    AddItem("button", "A button on the omni.");
    SetVendorType(VT_MAGIC);
}

void init() {
    vanish_count=7200;
    check_environs();
}
mixed CanPress(object who, string target) {
    if(!present(this_object()->GetKeyName(),who ) && target == "button"){
        return "You do not have the Omni!";
    }
    if(this_object()->GetId() == target){
        return "You cannot push that.";
    }
    return 1;
}
mixed eventPress(object who, string target) {
    say(this_player()->GetName()+" turns multicolored and disappears.\n");
    write("You feel momentarily disoriented and find yourself back in the tavern.\n");
    this_player()->eventMoveLiving("/domains/town/room/tavern");
    return 1;
}
void heart_beat(){
    vanish_count--;
    if(vanish_count < 0){
        tell_object(environment(),"The omni glows brightly and disappears!");
        this_object()->eventDestruct();
    }
    if(vanish_count == 50){
        tell_object(environment(),"The omni begins to glow a dull red.");
        this_object()->SetLong("This is a small, round metal device, copper in color, "+
                "and similar in appearance to a pocket watch. There is a blinking red light "+
                "next to the tiny metal button at its top. It is glowing a dull red.");
        this_object()->SetShort("an omni (glowing)");
    }
}
int check_environs(){
    if( interactive(environment()) ){
        set_heart_beat(1);
        return 1;
    }
    return 1;
}
