#include <lib.h>
#include <daemons.h>
#include <talk_type.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_RIFLE;
int active, cache;
mapping Prey = ([]);
object myroom;

varargs mixed DoWield(object who, mixed where);

string LongDesc(){
    string ret = "A highly advanced weapon of Poleepkwa design.";
    if(!active) return ret;
    if(cache < 5) ret += " A %^RED%^red%^RESET%^ light is illuminated on it.";
    else if(cache < 25) ret += " A %^YELLOW%^yellow%^RESET%^ light is illuminated on it.";
    else if(cache < 40) ret += " A %^GREEN%^green%^RESET%^ light is illuminated on it.";
    else ret += " A %^BLUE%^blue%^RESET%^ light is illuminated on it.";
    return ret;
}

static void create() {
    ::create();
    SetKeyName("plasma rifle");
    SetId(({"rifle"}));
    AddSave(({"cache","Prey"}));
    SetAdjectives(({"small","poleepkwa","plasma"}));
    SetShort("a small plasma rifle");
    SetLong((:LongDesc:));
    SetHands(2);
    SetMass(100);
    SetDollarCost(5000);
    SetVendorType(VT_WEAPON);
    SetClass(20);
    SetDamageType(BLUNT);
    SetFirearmType("auto");
    SetWeaponType("blunt");
    SetWield( (: DoWield :) );
    AddItem( ({"light","status light"}), "A status light.");
    set_heart_beat(2);
    Prey = ([]);
}

varargs mixed eventShoot(object who, mixed target, string dir, string whom){
    string name, patsy;
    object bolt, killer, env, room = room_environment();
    int fuel, dam;
    killer = (who || this_player());
    fuel = killer->GetMagicPoints();
    name = killer->GetName();
    env = environment(killer);
    if(target) patsy = target->GetName();

    if(creatorp(who)){
        cache = 50;
        fuel = 800;
        who->AddMagicPoints(800);
    }

    dam = cache + random(50);

    if(!active){
        write("The rifle clicks.");
        say(name+"'s rifle emits a click.");
        return 1;
    }

    if(cache < 5){
        cache = 0;
        write("The rifle clicks.");
        say(name+"'s rifle emits a click.");
        if(room) room->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
                "Power cache too low.", "poleepkwa");
        return 1;
    }

    if(fuel < 50){
        cache = 0;
        write("The rifle emits a harsh buzzing noise.");
        say(name+"'s rifle emits a harsh buzzing noise.");
        if(room) room->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
                "Operator essence too low.", "poleepkwa");
        return 1;
    }

    if(dir){
        if(!env){
            write("No environment.");
            return 1;
        }
        if(!env->GetExit(dir)){
            write("You can't shoot in that direction.");
            return 1;
        }
        bolt = new("/domains/default/weap/plasma");
        if(!bolt){
            write("There appears to be some sort of malfunction.");
            return 1;
        }
        write("You fire your plasma rifle "+dir+"!");
        tell_room(env, name+" fires "+possessive(killer)+
                " plasma rifle "+dir+"!", ({killer}));
        bolt->SetOwner(who);
        bolt->SetDamage(dam);
        bolt->eventMove(env);
        bolt->SetArmed(1);
        bolt->eventDeploy(dir);
        killer->AddMagicPoints(-50);
        cache = 0;
        return 1;
    }

    write("You blast "+patsy+" with your plasma rifle!");
    tell_room(env, name+" blasts "+patsy+" with "+possessive(killer)+
            " plasma rifle!", ({killer, target}));
    target->eventPrint(name+" blasts you with "+possessive(killer)+
            " plasma rifle!");

    dam = cache + random(50);
    cache = 0;
    Prey = ([ "ob" : target, "name" : patsy, "player" : (userp(target)) ]);

    target->eventReceiveDamage(killer, HEAT, dam);
    killer->AddMagicPoints(-50);
    target->AddEnemy(killer);

    Prey = ([ "ob" : target, "name" : patsy, "room" : environment(target),
            "player" : userp(target) ]);

    if(room) room->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
            "New prey being tracked.", "poleepkwa");

    return 1;
}

void heart_beat(){
    int notify;
    string str;
    object env = environment();
    object room = room_environment();
    if(active && !GetWorn()){
        if(env) tell_room(env, "The "+remove_article(GetShort())+
                " whines and clicks off.");
        active = 0;
    }
    else if(active){
        int fuel = env->GetMagicPoints();
        if(!fuel){
            active = 0;
            if(environment(env)){
                tell_room(environment(env), env->GetName()+"'s "+
                        "plasma rifle whines and clicks off.", ({env}));
                env->eventPrint("Your plasma rifle whines and clicks off.");
            }
        }
        else if(cache < 50){
            env->AddMagicPoints(-1);
            cache++;
        }
    }
    if(active){
        object tmp;
        object penv, menv;
        if(Prey["name"] && !Prey["ob"]){
            if(Prey["player"] && tmp = find_player(Prey["name"])){
                Prey["ob"] = tmp;
                Prey["room"] = environment(tmp);
                Prey["lost"] = 0;
                if(room){
                    room->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
                            "Contact with prey re-established.", "poleepkwa");
                }
                return;
            }
            else if(!Prey["player"]) Prey = ([]);
            if(room && !Prey["lost"]){
                room->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
                        "Contact with prey lost.", "poleepkwa");
                Prey["lost"] = 1;
            }
            return;
        }
        penv = room_environment(Prey["ob"]);
        menv = room_environment(this_object());
        if(Prey["ob"] && (!Prey["room"] || penv != Prey["room"])){
            if(room) room->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
                    "Prey on the move.", "poleepkwa");
            Prey["room"] = penv;
            notify = 1;
        }
        if(menv != myroom && Prey["ob"]){
            myroom = menv;
            notify = 1;
        }
        if(notify && room){
            string mycords = ROOMS_D->GetCoordinates(menv);
            string theircords = ROOMS_D->GetCoordinates(penv);
            int i, j, x1, y1, x2, y2, bearing;
            i = sscanf(mycords, "%d,%d,%*s", x1, y1);
            j = sscanf(theircords, "%d,%d,%*s", x2, y2);
            if(i < 2 || j < 2){
                return; 
            }
            bearing = bearing(x2, y2, x1, y1, 1);
            if(bearing != -1) str = "Prey's bearing currently: "+bearing;
            else str = "Prey is in the immediate environment.";
            room->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
                    str, "poleepkwa");
        }
    }
}

void init(){
    object env = environment();
    ::init();
    if(active && !GetWorn()){
        if(env) tell_room(env, "The "+remove_article(GetShort())+
                " whines and clicks off.");
        active = 0;
    }
}

varargs mixed DoWield(object who, mixed where){
    string extra;
    object env = environment(who);
    if(creatorp(who) || who->GetRace() == "poleepkwa"){
        active = 1; 
        if(creatorp(who)) cache = 50;
    }
    if(active) extra = " and it beeps and clicks on.";
    else extra = ".";
    who->eventPrint("You wield "+GetShort()+extra);
    if(env) tell_room(env, who->GetName()+" wields "+
            GetShort()+extra, ({who}));
    return 1;
}

mixed eventUnequip(object who){
    mixed ret = ::eventUnequip(who);
    object env = environment(who);
    if(ret && active){
        who->eventPrint("The rifle whines and clicks off.");
        if(env) tell_room(env, who->GetName()+"'s rifle "+
                "whines and clicks off.", ({who}));
        active = 0;
    }
    return ret;
}
