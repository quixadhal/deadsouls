#include <lib.h>
#include ROOMS_H
inherit LIB_ITEM;
inherit LIB_PRESS;

int check_environs();
int vanish_count = 50;
static void create() {
    item::create();
    SetKeyName("omni");
    SetId(({"omni","Omni","device"}));
    SetShort("an omni");
    SetLong("This is a small, round metal device, copper in color, "+
            "and similar in appearance to a pocket watch. There is a blinking red light "+
            "next to the tiny metal button at its top.");
    SetMass(10);
    SetDollarCost(300);
    AddItem("button", "A button on the omni.");
}
void init() {
    ::init();
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
mixed eventPress(object who, string where) {
    object *rooms = objects( (: inherits(LIB_ROOM, $1) :) );
    object target;
    int success = 0;
    rooms = filter(rooms, (: member_array(base_name($1), ({ ROOM_START,
                        ROOM_FURNACE, ROOM_VOID, ROOM_POD, ROOM_DEATH, ROOM_WIZ,
                        ROOM_ARCH, ROOM_NETWORK, ROOM_ROUTER, LIB_FURNACE,  
                        ROOM_FREEZER }) ) == -1  && strsrch(base_name($1),"/realms/") &&
                !inherits(LIB_FURNACE,$1) && 
                !($1->GetVirtual() && !grepp(base_name($1),",")) &&
                last_string_element(base_name($1),"/") != "death" :) );
    say(who->GetName()+" turns "+dbz_colors("multicolored")+" and disappears!");
    write("You feel momentarily disoriented and find yourself elsewhere!");
    while(!success){
        target = rooms[random(sizeof(rooms)-1)];
        success = who->eventMove(target);
    }
    who->eventDescribeEnvironment();
    tell_room(target, "With a multicolored flash, "+who->GetName()+
            " appears!",who);
    return 1;
}

void heart_beat(){
    object env = environment();
    object *holders;
    if(!env) return;
    holders = filter(containers(this_object()),
            (: interactive($1) :) );
    if(interactive(env)) vanish_count--;
    else if(sizeof(holders) && random(100) > 80) vanish_count--;
    if(vanish_count < 0){
        tell_object(env,"The omni glows brightly and disappears!");
        this_object()->eventDestruct();
    }
    if(vanish_count == 5){
        tell_object(env,"The omni begins to glow a dull red.");
        this_object()->SetLong("This is a small, round metal device, copper in color, "+
                "and similar in appearance to a pocket watch. There is a blinking red light "+
                "next to the tiny metal button at its top. It is glowing a dull red.");
        this_object()->SetShort("an omni (glowing)");
    }
}

int check_environs(){
    if( interactive(environment()) ){
        set_heart_beat(10);
        return 1;
    }
    return 1;
}
