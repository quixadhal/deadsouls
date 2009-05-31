#include <lib.h>
#include <boobytraps.h>

inherit LIB_BOOBYTRAP_SHADOW;

varargs mixed SpringTrap(mixed arg1, mixed arg2){
    write("You are pricked by a poison needle trap!");
    say(this_player()->GetCapName()+" is pricked by a poison needle trap!");
    this_player()->AddPoison(90);
    return boobytrap_shadow::SpringTrap(arg1, arg2);
}

void create(){
    SetTrapType(BOOBYTRAP_ALL);
}
