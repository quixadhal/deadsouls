#include <lib.h>
#include <climb.h>
#include <position.h>

inherit LIB_CHAMBER;
inherit LIB_CLIMB;

varargs mixed ClimbMe(mixed args...){
    return 1;
}

static void create() {
    ::create();
    SetKeyName("tree");
    SetId(({"oak"}));
    SetAdjectives(({"large"}));
    SetShort("a large tree");
    SetLong("This is a very large tree. It has been around since long "+
            "before the Virtual Campus, and looks like it will be around long "+
            "after, as well.");
    SetChamberInterior("You are among the sturdy branches of "+
            "the large tree in University Square.");
    SetRace("tree");
    SetPacifist(1);
    SetPosition(POSITION_NULL);
    SetMelee(0);
    SetNoClean(1);
    SetMount(1);
    SetMountStyle("driven");
    SetVisibleRiders(0);
    SetOpacity(0);
    SetMaxHealthPoints(20);
    SetMaxCarry(30000);
    SetNoCondition(1);
    SetAttackable(0);
    SetClimb( (: ClimbMe :), CLIMB_DOWN );
    SetClimb( (: ClimbMe :), CLIMB_OUT );
    SetClimb( (: ClimbMe :), CLIMB_UP );
    SetClimb( (: ClimbMe :), CLIMB_INTO );
    SetClimb( (: ClimbMe :), CLIMB_THROUGH );
}

void init(){
    ::init();
}

varargs mixed eventClimb(object who, int type, string where){
    mixed ret;
    switch(type){
        case CLIMB_DOWN:
            ret = eventDismount(who);
            break;
        case CLIMB_OUT:
            ret = eventDismount(who);
            break;
        default :
            ret = eventMount(who);
            break;
    }
    return ret;
}
