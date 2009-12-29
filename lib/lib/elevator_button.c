#include <lib.h>

inherit LIB_DUMMY; // These do not show up in desc, but you can look at them
inherit LIB_PRESS; // Makes the item pressable

object car, shaft;
string button_id;

int openDoor(object who) {
    if(!car) return 0;
    if(!who) who = this_player();
    send_messages("press", "$agent_name $agent_verb the button.",
            who, 0, environment(who));
    car->SetDoor(1);
    car->CallMe(1);

}

static void create() {
    dummy::create();
    SetKeyName("button");
    SetAdjectives("elevator","button");
    SetShort("a button");
    SetLong("It is a button that you could probably press."); 
    SetPress((: openDoor :));
}

string SetButtonId(string str){
    button_id = str;
    return button_id;
}

string GetButtonId(){
    return button_id;
}

object SetElevator(object ob){
    car = ob;
    return car;
}

object GetElevator(){
    return car;
}
