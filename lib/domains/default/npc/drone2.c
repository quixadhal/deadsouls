#include <lib.h>
#include <damage_types.h>
#include <position.h>
#include ROOMS_H

inherit LIB_SENTIENT;
inherit LIB_ACTIVATE;
int incept, newenv = 1, active = 0, ammo = 1000;
mixed visited = ({});

int eventShootDude(object ob){
    int pre, post, dam, numlimbs;
    string limbname, str;
    string *limbs;
    if(!environment()) return 0;
    str = ob->GetName();
    if(!ammo){
        tell_room(environment(),"The killbot clicks.");
        active = 0;
        return 0;
    }
    tell_object(this_object(),"You fire at "+ob->GetName()+"!");
    tell_room(environment(),"The killbot fires at "+ob->GetName()+"!",
            ({this_object(),ob}));
    tell_object(ob,"The killbot fires at you!");
    ammo--;
    if(random(100) < 10) return 1;
    limbs=ob->GetLimbs();
    numlimbs=sizeof(limbs);
    limbname = limbs[random(numlimbs-1)]; 
    tell_room(environment(this_object()),
            "The bullet smashes into "+
            capitalize(str)+"'s "+limbname+"!\n",ob);
    tell_object(ob,"The bullet smashes into your "+limbname+"!\n");
    dam = 20;
    dam *= random(10);
    dam -= random(ob->GetStatLevel("coordination"));
    pre = ob->GetHealthPoints();
    ob->eventReceiveDamage(this_object(),(PIERCE), dam, 0, limbname);
    post = ob->GetHealthPoints();
    if(post < pre){
        if(!present("firearms_wound",ob)){
            new(LIB_WOUND)->eventMove(ob);
        }
        ob->AddLead("gunshot_wounds", 1);
        ob->SetAttack(this_object());
        ob->AddEnemy(this_object());
        if(!ob->GetInCombat()){
            ob->eventForce("attack "+this_object()->GetKeyName());
        }
    }
    else {
        AddNonTargets(ob);
    }
    return 1;
}

int eventHoseTarget(object target){
    int repeat = 10;
    if(!target) return 0;
    while(repeat && !(target->GetDying())){
        eventShootDude(target);
        repeat--;
    }
    return 1;
}

int eventTargetScan(){
    object *targets;
    int targs;
    if(!environment() || !active) return 0;
    targets = filter(get_livings(environment()), (: !($1->GetInvis()) &&
                ($1->GetRace() != "god") && member_array($1, GetNonTargets()) == -1 &&
                ($1->GetRace() != "bot") :));
    targets = scramble_array(targets);
    targets -= ({ this_object() });
    if((targs = sizeof(targets)) < 1){
        if(newenv) eventForce("say Environment scan complete.");
        newenv = 0;
        eventQuell();
        return 0;
    }
    if(targs > 10) targs = 10;
    else {
        eventForce("say "+cardinal(targs)+" target"+
                ((targs > 1) ? "s" : "" )+" acquired.");
    }
    targs--;
    targets = targets[0..targs];
    foreach(object target in targets){
        eventHoseTarget(target);
    }
    return 1;
}

int ActivateTurret(){
    if(!ammo){
        write("The killbot clicks and goes silent.");
        return 0;
    }
    active = 1;
    eventForce("say KILLBOT IS NOW FULLY ARMED AND OPERATIONAL.");
    eventTargetScan();
    set_heart_beat(1);
    return 1;
}

static void create() {
    sentient::create();
    visited = ({});
    incept = time();
    SetKeyName("killbot");
    SetId( ({"drone","bot", "robot","turret"}) );
    SetAdjectives(({"kill","non-player", "non player"}));
    SetShort("a killbot");
    SetLong("This is a hovering orb with a machine gun hanging "+
            "underneath it and a single red glowing eye scanning the area.");
    SetPosition(POSITION_FLYING);
    SetLevel(1);
    SetPacifist(1);
    SetNoClean(1);
    SetRace("bot");
    SetClass("fighter");
    SetGender("neuter");
    SetPolyglot(1);
    SetWanderSpeed(1);
    SetPermitLoad(1);
    SetLanguage("common",100);
    SetDefaultLanguage("common");
}

void init(){
    ::init();
}

int eventTurnOn(){
    if(active){
        write("The killbot is already active.");
        return 1;
    }
    else {
        call_out("ActivateTurret",7);
        write("You activate the killbot.");
        eventForce("say KILLBOT ACTIVE.");
        eventForce("say YOU HAVE 5 SECONDS TO REACH MINIMUM SAFE DISTANCE.");
    }
    return 1;
}

int eventTurnOff(){
    if(!active){
        write("The killbot is already inactive.");
        return 1;
    }
    else {
        write("It seems this killbot cannot be deactivated.");
    }
    return 1;
}

void heart_beat(){
    if(!clonep()) return;
    ::heart_beat();
    if(!active){
        ActivateTurret();
    }
    if(time() - incept > 60){
        mixed *rooms = objects( (: inherits(LIB_ROOM, $1) :) );
        int success;
        object target;
        incept = time();
        rooms = filter(rooms, (: member_array(base_name($1), ({ ROOM_START,
                            ROOM_FURNACE, ROOM_VOID, ROOM_POD, ROOM_DEATH, ROOM_WIZ,
                            ROOM_ARCH, ROOM_NETWORK, ROOM_ROUTER, LIB_FURNACE,
                            ROOM_FREEZER }) ) == -1  && strsrch(base_name($1),"/realms/") &&
                    !inherits(LIB_FURNACE,$1) &&
                    !($1->GetVirtual() && !grepp(base_name($1),",")) &&
                    last_string_element(base_name($1),"/") != "death" :) );
        while(!success){
            target = rooms[random(sizeof(rooms)-1)];
            success = this_object()->eventMoveLiving(target);
        }
    }
    eventTargetScan();
}

varargs int eventReceiveDamage(mixed agent, int type, int x, int internal,
        mixed limbs){
    object env = room_environment();
    if(env){
        tell_room(env,"The killbot %^RED%^BEEPS%^RESET%^.");
    }
    ActivateTurret();
    return ::eventReceiveDamage(agent, type, x, internal, limbs);
}

int eventMove(mixed dest){
    int ret;
    object env = room_environment();
    string location;
    if(!clonep()) return 0;
    if(!env) location = ROOM_START;
    else if(clonep(env)) location = file_name(env);
    else location = base_name(env);
    if(location) this_object()->SetProperty("LastLocation", location);
    if(member_array(env, visited) == -1) visited += ({ env });
    ret = ::eventMove(dest);
    AddStaminaPoints(GetMaxStaminaPoints());
    newenv = 1;
    if(ammo < 1000){
        if(!ammo)  SetWimpy(90);
        else SetWimpy(100 - (ammo/10));
        ammo++;
    }
    return ret;
}

varargs int eventDie(mixed arg){
    int ret, brooms;
    object newbot, env = room_environment();
    if(env){
        for(brooms = 2;brooms > 0;brooms--){
            newbot = new(base_name(this_object()));
            ret = newbot->eventMove(env);
            if(ret){
                tell_room(env,"The killbot has teleported in reinforcements.");
            }
        }
    }
    return ::eventDie(arg);
}

int eventDestruct(){
    object gift, env;
    env = room_environment();
    catch( gift = new("/domains/default/weap/grenade") );
    if(gift && env){
        gift->eventMove(env);
        gift->SetArmed(1);
    }
    return ::eventDestruct();
}


