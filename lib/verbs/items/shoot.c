#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("shoot");
    SetRules("OBJ at STR","WRD with WRD");
    SetSynonyms("fire", "gat", "gank");
    SetErrorMessage("Shoot what?");
    SetHelp("Syntax: <shoot OBJECT>\n\n"
      "Shoots a target.\n\n");
}


mixed can_shoot_obj_at_str(object shooter, string target) { 
    return 1;
} 

mixed can_shoot_wrd_with_wrd(mixed target,mixed shooter) {
    return 1;
}


varargs mixed do_shoot_obj_at_str(object shooter, string target) {
    string what;
    return (mixed)shooter->CanShoot(shooter,target);
    return 1;
}

varargs mixed do_shoot_wrd_with_wrd(mixed target,mixed shooter) {
    object rod;
    if(!rod = present(shooter,this_player())) {
	rod = present(shooter,environment(this_player()));
    }
    if(rod) return (mixed)rod->CanShoot(rod,target);
    else return "Reality has been distorted here. Report this event.";
}



