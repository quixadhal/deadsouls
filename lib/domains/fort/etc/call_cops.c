#include <lib.h>
#include <rooms.h>
#include "../customdefs.h"

int calling;

int GetHelp(object sergeant, object *defendants){
    string omsg, imsg;
    omsg = "The Sergeant of the Guard rushes out to help someone in need!";
    imsg = "The Sergeant of the guard rushes in!";
    sergeant->eventMoveLiving(environment(this_object()), omsg, imsg);
    sergeant->eventForce("say What's all this, then?");
    foreach(object offender in defendants){
	sergeant->AddEnemy(offender);
    }
    sergeant->SetHealthPoints(sergeant->GetMaxHealthPoints());
    calling = 0;
    return 1;
}


int eventExtraAction(){
    object sergeant;
    object *defendants;
    object *guards;
    string sarge;

    guards = ({});

    if(this_object()->GetInCombat()){
	defendants = this_object()->GetEnemies();
	if(!sizeof(defendants)) {
	    return 4;
	}

	foreach(object witness in get_livings(environment(this_object()))){
	    if(member_array("guard",witness->GetId()) != -1){
		guards += ({ witness });
	    }
	}

	foreach(object samaritan in guards){
	    foreach(object punk in defendants){
		samaritan->AddEnemy(punk);
	    }
	}


	if(!present("sergeant of the guard",environment(this_object()))){
	    if(calling) {
		return 5;
	    }

	    sergeant = to_object("sergeant of the guard");
	    if(sergeant) sarge = file_name(sergeant);
	    if(!sergeant || strsrch(sarge,"#") == -1 ) sergeant = new(MY_NPC + "/sergeant");
	    if(!sergeant) {
		this_object()->eventForce("say no joy");
		return 2;
	    }
	    if(!present("sergeant of the guard",load_object(ROOM_VOID))){
		sergeant->eventMoveLiving(ROOM_VOID);
		calling = 1;
		if(this_object()->GetRace() == "human") this_object()->eventForce("shout SERGEANT OF THE GUARD! HELP!");
		call_out((: GetHelp, sergeant,  defendants :), 5);
	    }
	}
    }
    return 1;
}

