#include <lib.h>
#include <damage_types.h>
#include <position.h>

inherit LIB_SENTIENT;
inherit LIB_ACTIVATE;
int active, ammo = 1000;

int eventShootDude(object ob){
    int dam, numlimbs;
    string limbname, str;
    string *limbs;
    if(!environment()) return 0;
    str = ob->GetName();
    if(!ammo){
        tell_room(environment(),"The gun turret clicks.");
        active = 0;
        return 0;
    }
    tell_object(this_object(),"You fire at "+ob->GetName()+"!");
    tell_room(environment(),"The gun turret fires at "+ob->GetName()+"!",
            ({this_object(),ob}));
    tell_object(ob,"The gun turret fires at you!");
    ammo--;
    if(random(100) < 10) return 1;
    limbs=ob->GetLimbs();
    numlimbs=sizeof(limbs);
    limbname = limbs[random(numlimbs-1)]; 
    tell_room(environment(this_object()),
            "The bullet smashes into "+
            capitalize(str)+"'s "+limbname+"!\n",ob);
    tell_object(ob,"The bullet smashes into your "+limbname+"!\n");
    ob->SetAttack(this_agent());
    if(!present("firearms_wound",ob)){
        new(LIB_WOUND)->eventMove(ob);
    }
    ob->AddLead("gunshot_wounds", 1);
    dam = 20;
    dam *= random(10);
    dam -= random(ob->GetStatLevel("coordination"));
    ob->eventReceiveDamage(this_object(),(PIERCE), dam, 0, limbname);
    if(!ob->GetInCombat()){
        ob->eventForce("attack "+this_object()->GetKeyName());
    }
    return 1;
}

int eventHoseTarget(object target){
    int repeat = 10;
    if(!target) return 0;
    //write("Hosing target: "+identify(target));
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
                $1->GetRace() != "bot" :));
    targets = scramble_array(targets);
    targets -= ({ this_object() });
    if(!(targs = sizeof(targets))) return 0;
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

int AddLegs(){
    AddLimb("first leg", "torso", 2);
    AddLimb("second leg", "torso", 2);
    AddLimb("third leg", "torso", 2);
    return 1;
}

int ActivateTurret(){
    if(!ammo){
        write("The turret clicks and goes silent.");
        return 0;
    }
    active = 1;
    eventForce("say TURRET IS NOW FULLY ARMED AND OPERATIONAL.");
    eventTargetScan();
    set_heart_beat(1);
    return 1;
}

static void create() {
    sentient::create();
    SetKeyName("gun turret");
    SetId( ({"bot", "robot","turret"}) );
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a gun turret");
    SetLong("This is a four foot tall metal turret on tripod legs "+
            "designed to automatically acquire and engage any moving "
            "targets with its massive 20 millimeter guns.");
    SetPosition(POSITION_STANDING);
    SetLevel(1);
    SetPacifist(1);
    SetNoClean(0);
    SetRace("bot");
    SetClass("fighter");
    SetGender("neuter");
    SetPolyglot(1);
    call_out("AddLegs",0);
    SetLanguage("common",100);
    SetDefaultLanguage("common");
}

void init(){
    ::init();
}

int eventTurnOn(){
    if(active){
        write("The turret is already active.");
        return 1;
    }
    else {
        call_out("ActivateTurret",7);
        write("You activate the gun turret.");
        eventForce("say TURRET ACTIVE.");
        eventForce("say YOU HAVE 5 SECONDS TO REACH MINIMUM SAFE DISTANCE.");
    }
    return 1;
}

int eventTurnOff(){
    if(!active){
        write("The turret is already inactive.");
        return 1;
    }
    else {
        write("It seems this turret cannot be deactivated.");
        //write("You deactivate the gun turret.");
        //active = 0;
    }
    return 1;
}

void heart_beat(){
    ::heart_beat();
    eventTargetScan();
}

varargs int eventReceiveDamage(mixed agent, int type, int x, int internal,
        mixed limbs){
    ActivateTurret();
    return ::eventReceiveDamage(agent, type, x, internal, limbs);
}
