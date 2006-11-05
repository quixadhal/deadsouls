#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("shoot");
    SetRules("OBJ at OBJ", "OBJ at STR","OBJ with OBJ");
    SetSynonyms("fire", "gat", "gank");
    SetErrorMessage("Shoot what?");
    SetHelp("Syntax: <shoot OBJECT>\n\n"
      "Shoots a target.\n\n");
}


mixed can_shoot_obj_with_obj(mixed target,mixed shooter) {
    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}

mixed can_shoot_obj_at_obj(mixed shooter,mixed target) {
    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}

mixed can_shoot_obj_at_str(object shooter, string target) { 
    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}

varargs mixed do_shoot_obj_at_obj(mixed shooter,mixed target) {
    return (mixed)shooter->CanShoot(shooter,target);
}

varargs mixed do_shoot_obj_at_str(object shooter, string target) {
    return (mixed)shooter->CanShoot(shooter,target);
    return 1;
}

varargs mixed do_shoot_obj_with_obj(mixed target,mixed shooter) {
    return (mixed)shooter->CanShoot(shooter,target);
}
