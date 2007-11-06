#include <lib.h>
#include <boobytraps.h>

inherit LIB_BOOBYTRAP_SHADOW;

varargs mixed SpringTrap(mixed arg1, mixed arg2){
    write("You are pricked by a poison needle trap!");
    say(this_player()->GetCapName()+" is pricked by a poison needle trap!");
    this_player()->AddPoison(10);
    call_out( (: eventUnshadow :), 1);
}

void create(){
    SetTrapType(BOOBYTRAP_OPEN | BOOBYTRAP_CLOSE);
}
