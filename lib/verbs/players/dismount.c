#include <lib.h>
#include ROOMS_H
#include <daemons.h>
#include <function.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("dismount");
    SetRules("","LIV", "from LIV");
    SetErrorMessage("Dismount something?");
    SetHelp("Syntax: dismount <CREATURE>\n\n"
            "This command allows you to get off a  "
            "creature you are riding.\n"
            "See also: mount, befriend, abandon");
}

mixed can_dismount_liv() {
    if(this_player()->CanManipulate()) 
        return bool_reverse(this_player()->GetParalyzed());
}

mixed can_dismount_from_liv() {
    return can_dismount_liv();
}

mixed can_dismount() {
    return can_dismount_liv();
}

mixed do_dismount_liv(object ob) {
    return ob->eventDismount(this_player());
}

mixed do_dismount_from_liv(object ob) {
    return do_dismount_liv(ob);
}

mixed do_dismount() {
    object mount = this_player()->GetProperty("mount");
    if(mount) return do_dismount_liv(mount);
    if(base_name(environment(this_player())) == LIB_CORPSE){
        object new_env = environment(environment(this_player()));
        if(!new_env) new_env = find_object(ROOM_START);
        else {
            write("You get off your dead mount.");
            say(this_player()->GetName()+" gets off "+possessive(this_player())+
                    " dead mount.");
        }
        this_player()->eventMoveLiving(new_env);
    }
    else return write("You don't seem to have a mount.");

}
