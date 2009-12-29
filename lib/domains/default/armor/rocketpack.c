#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <medium.h>
inherit LIB_ARMOR;
inherit LIB_ACTIVATE;
inherit LIB_READ;

int activated;
int charge = 1000;
int maxcharge = 1000;
object rocketshadow, owner;
string current_direction;
mapping Directions = ([]);

string LongD(){
    string ret = "This is a large device worn on the back and designed to \"boost\" " +
        "the wearer through the air or through space. For continuous operation, "+
        "it should probably be activated.  The fuel gauge "+
        "reads "+to_int(percent(charge,maxcharge))+" percent.";
    return ret;
}

static void create(){
    armor::create();
    if(!Directions) Directions = ([]);
    SetKeyName("rocket pack");
    SetId(({"pack","rocket"}));
    SetAdjectives(({"large","rocket"}));
    SetShort("a small rocket pack");
    SetLong( (: LongD :) );
    SetMass(800);
    SetBaseCost(18000);
    SetArmorType(A_VEST);
    AddSave( ({ "charge", "Directions" }) );
    SetRestrictLimbs( ({ "torso" }) );
    set_heart_beat(1);
}

void init(){
    ::init();
    add_action("boost","boost");
}

int SetCharge(int i){
    charge = i;
}

int SetMaxCharge(int i){
    maxcharge = i;
}

void heart_beat(){
    int medium;
    if(activated) charge--;
    if(charge < 1) charge = 0;
    if(!charge && activated){
        object env = environment();
        activated = 0;
        if(env){
            tell_object(env,"The rocket pack sputters and fails.");
        }
    }

    if(!owner) return;
    medium = environment(owner)->GetMedium();
    if(medium == MEDIUM_SPACE){
        int i;
        object env = environment(owner);
        if(!current_direction){
            current_direction = env->GetExits()[random(sizeof(env->GetExits()))];
        }
        if(activated){
            if(!Directions[current_direction]) Directions[current_direction] = 1;
            else Directions[current_direction]++;
        }
        foreach(mixed key, mixed val in Directions){
            if(val > 20) val = 20;
            for(i=val;i>0;i--){
                this_object()->boost(key,1);
            }
        }
    }
}

int boost(string str, int coasting){
    string dest,imsg,omsg;
    mapping Exit, Doors;
    int ret, medium;
    object env;
    if(!owner) return 0;
    env = environment(owner);
    current_direction = str;
    medium = environment(owner)->GetMedium();
    if(!env) return 0;
    Doors = env->GetDoorsMap();

    if(medium == MEDIUM_SPACE && !coasting){
        if(Directions[opposite_dir(str)]){
            Directions[opposite_dir(str)]--;
        }
        else {
            if(!Directions[str]) Directions[str] = 1;
            else Directions[str]++;
            //call_out( (: boost  :), 1, str, 1);
        }
    }

    if(!coasting && environment(this_object()) != owner){
        write("You don't have the rocket pack.");
        return 1;
    }
    if(!coasting && !GetWorn()){
        write("You're not wearing the rocket pack.");
        return 1;
    }
    if(!coasting && !str || !env){
        write("Boost in which direction?");
        return 1;
    }

    if(!coasting && !charge){
        write("The rocket pack is out of fuel.");
        return 1;
    }
    dest = env->GetExit(str);
    if(!dest){
        if(str == "up" && env->GetFlyRoom()) dest = env->GetFlyRoom();
        else if(str == "down" && env->GetSinkRoom()) dest = env->GetSinkRoom();
        else {
            if(!coasting) write("No such direction");
            return 1;
        }
    }
    Exit = environment(owner)->GetExitData(str);

    if( sizeof(Doors) && Doors[str] && Doors[str]->GetClosed() ){
        message("my_action", "You bump into " +
                Doors[str]->GetShort(str) + ".", owner);
        return 1;
    }

    if( Exit && Exit["pre"] &&
            !(evaluate(Exit["pre"], str)) ){
        return 1;
        //return 1;
    }

    if(!sizeof(dest)){
        write("You can't go that way.");
        return 1;
    }

    omsg = owner->GetName()+" rockets "+str+".";
    imsg = owner->GetName()+" rockets in.";

    if(!coasting) write("You engage your rocket pack's boosters to rocket you "+str+".");
    if(!coasting) charge--;

    ret = owner->eventMoveLiving(dest, omsg, imsg, str);
    if(ret){
        if( Exit && Exit["post"] ) evaluate(Exit["post"], str);
        if(activated){
            if(!coasting) write("The rocket pack's boosters resume normal operation.");
        }
        else {
            if(!coasting) write("The rocket pack's boosters sputter and go silent.");
        }
    }

    return 1;
}

int eventTurnOn(){
    if(!(this_object()->GetWorn()) || !this_player() ||
            environment(this_object()) != this_player()){
        write("You are not wearing the rocket pack.");
        return 1;
    }
    write("You activate the rocket pack with a deep, rumbling roar!");
    say(this_player()->GetName()+" fires up "+possessive(this_player())+" "
            "rocket pack with a deep, rumbling roar!");
    activated = 1;
    return 1;
}

int eventTurnOff(){
    write("You deactivate the rocket pack. The boosters sputter and go silent.");
    say(this_player()->GetName()+" deactivates "+possessive(this_player())+" "
            "rocket pack. The boosters sputter and go silent.");
    activated = 0;
    return 1;
}

int GetRunning(){
    return activated;
}

mixed eventEquip(object who, string array limbs){
    mixed success = armor::eventEquip(who, limbs);
    rocketshadow = new("/shadows/rocketpack");
    owner = who;
    if(success){
        if(rocketshadow) rocketshadow->eventShadow(who);
    }
    else if(rocketshadow) destruct(rocketshadow);
    activated = 0;
    return success;
}

varargs mixed eventUnequip(object who) {
    mixed success;
    if(!who) who = this_player();
    success = armor::eventUnequip(who);
    if(success){
        if(rocketshadow) rocketshadow->eventUnshadow();
    }
    if(rocketshadow) destruct(rocketshadow);
    return success;
}

int eventDecrementCharge(){
    if(!GetWorn()) return 0;
    if(previous_object() != rocketshadow) return 0;
    if(charge < 1) charge = 0;
    else charge--;

    if(charge < 50){
        tell_object(environment(this_object()),"The rocket pack beeps loudly!");
        return charge;
    }

    if(charge < 100){
        tell_object(environment(this_object()),"The rocket pack beeps softly.");
        return charge;
    }

    return charge;
}   

int GetRemainingCharge(){
    return charge;
}

