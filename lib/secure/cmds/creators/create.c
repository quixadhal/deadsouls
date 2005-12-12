#include <lib.h>
#include <modules.h>
#include <rooms.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string arg;

    if(!str || str == "") {
	write("You'll need to be more specific. Try 'help create'");
	return 1;
    }

    if(sscanf(str,"room%s",arg) == 1 
      && base_name(environment(this_player())) == ROOM_START )
	return "The start room should be edited by hand. Change cancelled.";

    if(sscanf(str,"room%s",arg) == 1) MODULES_ROOM->make(arg);
    else MODULES_GENERIC->make(str);
    //else if(sscanf(str,"item%s",arg) == 1) MOD_ITEM->make(arg);
    //else if(sscanf(str,"armor%s",arg) == 1) MOD_ARMOR->make(arg);
    //else if(sscanf(str,"weapon%s",arg) == 1) MOD_WEAPON->make(arg);
    //else if(sscanf(str,"npc%s",arg) == 1) MOD_NPC->make(arg);
    //else return "That's not a valid object type.";

    return 1;
}

int help() {
    message("system", "Syntax: create room DIRECTION FILE\n"
      "        create npc FILE\n"
      "        create armor FILE\n"
      "        create weapon FILE\n"
      "        create item FILE\n"
      "        create bed FILE\n"
      "        create chair FILE\n"
      "        create table FILE\n\n"
      "This command makes a generic copy of the type of thing "
      "you specify. In the case of a room, the room you are in is "
      "copied into the direction you specify. In the case of other "
      "objects, a generic object appears in the room you are in. "
      "After that object materializes, you can make changes to it "
      "with the \"modify\" command. These changes are saved to file "
      "automatically.\n\n"
      "See also: modify, copy, delete\n",
      this_player());
}
