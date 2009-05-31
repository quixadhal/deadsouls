#include <lib.h>

inherit LIB_VEHICLE;

string ReadInstructions(){
    string ret = "To travel, use the \"drive\" command, for example: drive east\n"+ 
        "To have the mech do something with its robotic arms, use the following syntax:\n"+ 
        "direct mech to get boulder\n"+ 
        "direct mech to kill orc\n\n";
    if(environment(this_player()) == this_object()){
        return ret;
    }
    else return "You can read no such thing from here.";
}

static void create() {
    vehicle::create();
    SetKeyName("strider");
    SetId(({"landstrider","walker","scout","columbu","mech"}));
    SetAdjectives(({"landstrider","walker","scout","columbu","strider","class"}));
    SetShort("a landstrider mech");
    SetLong("This is a Columbu class scout vehicle consisting of "
            "a human-sized transparent ball turret mounted on two large birdlike legs "
            "with two robotic arms on each side for manipulation and combat. "
            "Though technically a \"scout\" class mech, it is two meters tall "
            "and ballistic plated, and not to be trifled "
            "with by meat-based creatures.");
    SetVehicleInterior("This is the interior of a highly advanced military mechanized "
            "reconaissance vehicle. There are instructions here you can read.");
    AddItem("instructions","Some documentation on the operation of this mech.");
    AddItem(({"landstrider","walker","scout","columbu","mech","here"}),
            "This is the interior of a highly advanced military mechanized "
            "reconaissance vehicle. There are instructions here you can read.");
    SetRead("instructions", (: ReadInstructions :));
    SetRace("strider");
    SetClass("fighter");
    SetLevel(10);
    SetMelee(1);
    SetMount(1);
    SetMountStyle("driven");
    SetVisibleRiders(0);
    SetOpacity(0);
    SetNoClean(1);
    SetPacifist(0);
    SetNoCondition(1);
}

void init(){
    ::init();
    add_action("DirectMech","direct");
}

mixed DirectMech(string str){
    string what, cmd;
    int i = sscanf(str,"%s to %s",what, cmd);
    if(i != 2) i = sscanf(str,"%s %s",what, cmd);
    if(i != 2 || !answers_to(what,this_object())) return 0;
    if(environment(this_player()) == this_object()){
        write("You enter the commands into mech.");
        call_out("eventForce", 2, cmd);
        return 1;
    }
}
