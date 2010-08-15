#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>
#include <daemons.h>

inherit LIB_SHOOT;
inherit LIB_AIM;
inherit LIB_SURFACE;
inherit LIB_ACTIVATE;

static int active = 0;

static void create() {
    ::create();
    SetKeyName("manipulator");
    SetId(({"gun","zpem"}));
    SetAdjectives(({"zero","point","energy","bulky","boxy","futuristic"}));
    SetShort("a zero-point energy manipulator");
    SetLong("A large, bulky device appearing to be a kind of boxy, "
            "futuristic gun.");
    set_heart_beat(1);
    SetMass(400);
    SetDollarCost(5);
    SetVendorType(VT_WEAPON);
    SetHands(2);
    SetMaxCarry(20000);
    SetDamageType(BLUNT);
    SetWeaponType("blunt");
}

varargs mixed eventShoot(object shooter, mixed target, string direction){
    string name,patsy;
    object killer, env;
    object *inv = all_inventory(this_object());

    //tc("target: "+identify(target));

    if(!active){
        write("You have to activate it first.");
        return 1;
    }

    if(!target || direction){
        if(!sizeof(inv)){
            write("You can't do that.");
            return 1;
        }
    }

    if(sizeof(inv)){
        write("The zero-point energy manipulator is already actively "+
          "manipulating "+inv->GetShort()+".");
        return 1;
    }

    if(living(target)){
        if(!(RACES_D->GetNonMeatRace(target->GetRace()))){
            write("The zero-point energy manipulator does not work "+
              "well on meat-based objects.");
            return 1;
        }
    }

    if(base_name(target) == LIB_CORPSE || base_name(target) == LIB_LIMB){
        write("The zero-point energy manipulator does not work "+
              "well on meat-based objects.");
        return 1;
    }

    if(target->isDummy() || target->GetMass() > 5000 ||
        target->GetPreventGet()){
        write("The zero-point energy manipulator clicks and whines.");
        return 1;
    }

    if(!GetWielded()){
        write("You are not wielding it.");
        return 0;
    }

    killer = this_player();
    name = killer->GetName();
    env = environment(killer);
    patsy = target->GetShort();

    tell_room(env,name+" picks up "+patsy+" with "+possessive(killer)+" "+
            "zero-point energy manipulator.",({killer,target}) );
    tell_object(killer,"You surround "+patsy+" in a zero-point energy field, "+
            " and "+nominative(target)+" is picked up by "+
            "your zero-point energy manipulator.");
    tell_object(target,name+" picks you up with "+possessive(killer)+" "+
            "zero-point energy manipulator. You are helpless in its field.");

    active = 1;
    target->SetZPG(1);
    target->eventMove(this_object());
    return 1;
}

int eventTurnOff(){
    object *inv;
    if(!active){
        write("It's already inactive.");
        return 1;
    }

    inv = all_inventory();
    active = 0;
    write("You deactivate the zero-point energy manipulator.");
    if(sizeof(inv)){
        inv->eventMove(room_environment(this_object()));
        foreach(object ob in inv){
            tell_object(ob, "You are released from "+this_player()->GetName()+
              "'s zero-point energy manipulator.");
            tell_room(environment(this_player()), capitalize(ob->GetShort())+
              " is "+
              "released from "+this_player()->GetName()+"'s zero-point "+
              "energy manipulator.", ({ ob, this_player() }) );
            write("You release "+ob->GetShort()+" from your zero-point "+
              "energy manipulator.");
        }
    }
    return 1;
}

int eventTurnOn(){
    if(active){
        write("It's already active.");
        return 1;
    }
    write("You activate the zero-point energy manipulator.");
    say(this_player()->GetName()+" activates "+possessive(this_player())+
      " zero-point energy manipulator.");
    active = 1;
    return 1;
}


int GetZPEM(){
    return 1;
}

void init(){
    object *inv = all_inventory();
    ::init();
    if(!active){
        if(sizeof(inv) && room_environment(this_object())){
            inv->eventMove(room_environment(this_object()));
            eventDestruct();
        }
    }
}

void heart_beat(){
    object *inv = all_inventory();
    if(!active){
        if(sizeof(inv) && room_environment(this_object())){
            inv->eventMove(room_environment(this_object()));
            eventDestruct();
        }
    }
}

int CanReceive(object ob){
    object *inv = all_inventory(this_object());
    if(!active) return 0;
    if(sizeof(inv)) return 0;
    return ::CanReceive(ob);
}

mixed eventUnequip(object who){
    mixed tmp = surface::eventUnequip(who);
    if(tmp == 1){
        eventTurnOff();
    }
    return tmp;
}
