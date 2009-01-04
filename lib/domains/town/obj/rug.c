#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_PRESS;
inherit "/lib/events/manipulate";

int MoveRug();
int PushRug();
void create(){
    ::create();
    SetKeyName("persian rug");
    SetId( ({"rug"}) );
    SetAdjectives( ({"persian","ornamental"}) );
    SetShort("a Persian rug");
    SetLong("This is a beautiful, intricately patterned Persian "
            "rug, covering the center of the floor. ");
    SetMass(100);
    SetBaseCost("silver",500);
    SetManipulate( ([ "default" : (: MoveRug :) ]) );
    SetPress( ([ "default" : (: PushRug :) ]) );
    SetVendorType(VT_TREASURE);
}
mixed CanGet(object ob) { return "The rug appears attached to the floor.";}
int MoveRug(){
    send_messages("move", "$agent_name $agent_verb the Persian rug.",
            this_player(), 0, environment(this_player()));
    environment(this_object())->RevealDoor();
    return 1;
}
int PushRug(){
    send_messages("move", "$agent_name $agent_verb the Persian rug.",
            this_player(), 0, environment(this_player()));
    environment(this_object())->RevealDoor();
    return 1;
}
void init(){
    ::init();
}
