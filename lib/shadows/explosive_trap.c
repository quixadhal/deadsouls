#include <lib.h>
#include <boobytraps.h>
#include <damage_types.h>

inherit LIB_BOOBYTRAP_SHADOW;

varargs mixed SpringTrap(mixed arg1, mixed arg2){
    object ob = this_player();
    mixed *stuffs = ({});
    if(!ob) ob = this_object()->GetShadowedObject();
    if(!ob || !environment(ob)) return 0;
    tell_room(environment(ob),"\n----\nKABOOOOM!\n----\n");
    write("You trip an exploding boobytrap!");
    say(ob->GetCapName()+" trips an exploding boobytrap!");

    stuffs=ob->GetLimbs();
    foreach(string limb in stuffs){
        if(limb != "head" && limb != "torso" && limb != "neck") ob->RemoveLimb(limb,this_object());
    }

    foreach(object victim in get_livings(environment(ob))){
        victim->eventReceiveDamage("explosive device",BLUNT, random(1000)+300, 1);
    }

    return boobytrap_shadow::SpringTrap(arg1, arg2);
}

void create(){
    SetTrapType(BOOBYTRAP_OPEN | BOOBYTRAP_CLOSE |BOOBYTRAP_PICK);
}
