#include <lib.h>
#include <vendor_types.h>

inherit LIB_ITEM;
inherit LIB_TURN;

static int turned = 0;
object Turner;

void create(){
    ::create();
    SetKeyName("waterwheel");
    SetId( ({"wheel"}) );
    SetAdjectives( ({"water", "large","metal","iron"}) );
    SetShort("a large iron wheel embedded in the bedrock");
    SetLong("This is an iron wheel embedded in the bedrock.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetInvis(0);
    SetPreventGet("It's planted firmly in the bedrock.");
    SetNoCondition(1);
    set_heart_beat(1);
    SetUnique(1);
}

void init(){
    ::init();
}

varargs mixed CanTurn(object who, object what){
    if(turned){
        return "#The wheel turns no further.";
    }
    else return 1;
}

int eventTurn(object who){
    object pipe, env = environment();
    if(env) pipe = present("water pipe", env);
    if(!who) who = this_player();
    if(!turned){
        if(pipe){
            write("You turn the wheel.");
            say(who->GetName()+" turns the wheel.");
            if(!pipe->GetFlowing()){
                tell_room(env,"Water starts pouring from the pipe.");
            }
            pipe->openpipe(1);
            turned = 1;
            Turner = who;
        }
        else {
            write("The wheel turns loosely to no apparent effect.");
            say(who->GetName()+" turns the wheel to no apparent effect.");
        }
    }
    else {
        write("Nothing happens.");
        say(who->GetName()+" tries to turn the wheel. Nothing happens.");
    }
    return 1;
}

object GetTurner(){
    return Turner;
}
