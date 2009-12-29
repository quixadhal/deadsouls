#include <lib.h>
#include ROOMS_H
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object array obs, items, users;
    object ob;

    if( !args || args == "" ) {
        ob = environment(this_player());
    }
    else {
        ob = get_object(args);
    }
    if( !ob ) {
        return "No " + args + " found.";
    }
    this_player()->eventPrint(sprintf("Cleaning %O", ob), MSG_SYSTEM);
    say(this_player()->GetName()+" purges "+ob->GetShort()+".");
    obs = filter(all_inventory(ob), (: !userp($1) && 
                !inherits(LIB_DUMMY, $1) && base_name($1) != LIB_DUMMY :) );
    users = filter(deep_inventory(ob), (: userp($1) :) );
    foreach(object user in users){
        if(environment(user) != ob){
            user->eventMove(environment(this_player()));
        }
    }
    foreach(object item in obs) {
        if( item ) {
            if(!(item->eventMove(ROOM_FURNACE))){
                object *srs = deep_inventory(item) + ({ item });
                foreach(object bsns in srs){
                    bsns->eventDestruct();
                    if(bsns) destruct(bsns);
                }
            }
        }
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: purge [OBJECT]\n\n"
            "This command destroys non-dummy, non-user objects in the inventory "
            "of the specified object.");
}
