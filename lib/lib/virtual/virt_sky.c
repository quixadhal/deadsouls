/*    /lib/virtual/virt_sky.c
 *    From DeadSouls Object Library
 *    A virtual object for air rooms
 *    Created by Dvarsk 980310
 *    Version: @(#) virt_sky.c 1.1@(#)
 *    Last modified: 98/08/25
 */

#include <lib.h>
#include <medium.h>
#include <position.h>

inherit LIB_VIRT_LAND;

private mixed PreventLand;

varargs static void create(int x, int y,int z) {
    virt_land::create( x,  y, z);
    SetMedium(MEDIUM_AIR);
}

varargs static void Setup(int x, int y,int z) {
    virt_land::create( x,  y, z);
    SetMedium(MEDIUM_AIR);
}


string GetGround() {
    return GetExit("down");
}

string SetGround(string str) {
    AddExit("down", str);
    return str;
}

mixed GetPreventLand() {
    return PreventLand;
}

mixed SetPreventLand(mixed val) {
    return (PreventLand = val);
}

mixed CanFly(object who, string dir) {
    if( !dir || dir == "" ) {
	return "Fly where?";
    }
    if( dir == "up" ) {
	return "You cannot get any higher than you already are!";
    }
    else if( dir == "down" ) {
	if( stringp(PreventLand) ) {
	    return PreventLand;
	}
	else if( functionp(PreventLand) ) {
	    mixed tmp = evaluate(PreventLand, who);

	    if( tmp != 1 ) {
		return tmp;
	    }
	}
	if( !GetGround() ) {
	    return "There is nothing below you but open sea.";
	}
    }
    if( who->GetPosition() != POSITION_FLYING ) {
	return "You are not flying!";
    }
    return virt_land::CanGo(who, dir);
}

mixed CanGo(object who, string dir) {
    return CanFly(who, dir);
}

mixed eventFly(object who, string dir) {
    mapping exit = GetExitData(dir);
    
    if( GetDoor(dir) && GetDoor(dir)->GetClosed() ) {
	who->eventPrint("You fly into " + GetDoor(dir)->GetShort(dir) + ".");
	eventPrint(who->GetName() + " flies into " +
		   GetDoor(dir)->GetShort(dir) + ".", who);
	return 1;
    }
    if( exit["pre"] && !evaluate(exit["pre"], dir) ) {
	return 1;
    }
    who->eventMoveLiving(exit["room"], "$N flies " + dir + ".", "$N flies in.");
    if( dir =="down" ) {
	who->eventLand();
    }
    if( exit["post"] ) {
	evaluate(exit["post"], dir);
    }
    return 1;
}

mixed eventGo(object who, string dir) {
    if(GetMedium()==MEDIUM_AIR) return eventFly(who, dir);
    return ::eventGo(who, dir);
}

mixed eventReceiveObject(object ob) {
    int x = ob->GetLift();
    mixed rtn;
    
    rtn = virt_land::eventReceiveObject(ob);
    if(GetMedium() != MEDIUM_AIR) return rtn;
    if( !ob->CanFly() ) { // Things that cannot fly fall down
	if( living(ob) ) {
	    ob->eventFall();
	}
	else {
	    if( !GetGround() ) {   // Over uncharted areas, die!
		ob->eventDestruct();
	    }
	    else {
		string short;
		
		if( !ob->GetInvis() && (short = ob->GetShort()) ) {
		    GetGround()->eventPrint(capitalize(short) + " comes raining "
					    "down from the sky.");
		}
		ob->eventMove(GetGround());
	    }
	}
    }
    return rtn;
}
