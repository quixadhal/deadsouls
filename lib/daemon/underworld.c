/*    /daemon/underworld.c
 *    From Dead Souls LPMud
 *    This object belongs to Dead Souls LPMud and is NOT part of
 *    the Dead Souls LPC Library distribution
 *    Any use of this object or the code contained within
 *    outside of Dead Souls LPMud without the express written
 *    permission of the Dead Souls LPMud administration is prohibted
 *    copyright (c) 1995 Dead Souls LPMud
 *    created by Descartes of Borg 950402
 *    Version: @(#) underworld.c 1.5@(#)
 *    Last modified: 96/10/27
 */

#ifdef Dead SoulsLPMud

#include <lib.h>
#include <dirs.h>
#include <rooms.h>

inherit LIB_DAEMON;

string array UndeadTypes = ({ "ghost", "phantom", "vampire", "shade",
			      "wraith", "zombie", "mummy", "skeleton",
			      "ghoul", "werewolf", "liche" });

static void create() {
    daemon::create();
    SetNoClean(1);
}

int CanFly(string type) {
    switch(type) {
    case "ghost": case "phantom": case "vampire": case "shade":
    case "wraith":
	return 2;

    default:
	return 0;
    }
}

int GetMaterial(string type) {
    switch(type) {
    case "ghost": case "phantom": case "shade": case "wraith":
	return 0;

    case "vampire": case "zombie": case "mummy": case "skeleton":
    case "liche": case "ghoul":
	return 1;

    default:
	return 1;
    }
}

int isUndeadType(string type) {
    return (member_array(type, UndeadTypes) != -1);
}

string GetRandomUndead(object who) {
    int x;
    
    if( who->ClassMember("mage") ) {
	x = 10;
    }
    else {
	x = 9;
    }
    switch(random(x)) {
    case 0:
	return "ghost";

    case 1:
	return "zombie";

    case 2:
	return "vampire";

    case 3:
	return "ghoul";

    case 4:
	return "skeleton";

    case 5:
	return "mummy";

    case 6:
	return "phantom";

    case 7:
	return "shade";

    case 8:
	return "wraith";

    case 9:
	return "liche";
    }
}

string GetStandardRoom() {
    return ROOM_START;
}

string GetUnderworldRoom() {
    return ROOM_START;
    return DIR_UNDERWORLD_DOMAIN "/start";
}

#endif // Dead SoulsLPMud
