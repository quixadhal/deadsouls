#include <lib.h>
#include <daemons.h>
#include <talk_type.h>
#include <damage_types.h>
#include <vendor_types.h>
#include <armor_types.h>

inherit LIB_ARMOR;
inherit LIB_SHOOT;
inherit LIB_ACTIVATE;
int tracking, debugging, active, cache, maxcache = 100;
mapping Prey = ([]);
object myroom;

varargs mixed DoWear(object who, mixed where);

string LongDesc(){
    string ret = "A highly advanced weapon of Poleepkwa design, "+
        "meant to be worn.";
    if(!active) return ret;
    if(cache < percent_of(10, maxcache)) ret += " A %^RED%^red%^RESET%^ light is illuminated on it.";
    else if(cache < percent_of(50, maxcache)) ret += " A %^YELLOW%^yellow%^RESET%^ light is illuminated on it.";
    else if(cache < percent_of(80, maxcache)) ret += " A %^GREEN%^green%^RESET%^ light is illuminated on it.";
    else ret += " A %^BLUE%^blue%^RESET%^ light is illuminated on it.";
    return ret;
}

static void create() {
    ::create();
    SetKeyName("plasma cannon");
    SetId(({"cannon"}));
    AddSave(({"cache","Prey"}));
    SetAdjectives(({"shoulder","poleepkwa","plasma"}));
    SetShort("a plasma shoulder cannon");
    SetLong((:LongDesc:));
    SetMass(400);
    SetDollarCost(15000);
    SetVendorType(VT_WEAPON|VT_ARMOR);
    SetArmorType(A_CUSTOM);
    SetRestrictLimbs(({"torso"}));
    SetWear( (: DoWear :) );
    AddItem( ({"light","status light"}), "A status light.");
    set_heart_beat(2);
    Prey = ([]);
}

varargs int OperateThing(object who, mixed what){
    object env;
    string this;
    if(!who) who = this_player();
    env = environment(this_player());
    this = remove_article(GetShort());
    tell_player(who, "You operate your "+this+".");
    tell_object(env, who->GetName() + " operates " + possessive(who) +
            " " + this + ".", ({who}));
    if(what && stringp(what) && !active){
        tell_player(who, "The "+this+" is not activated.");
        return 0;
    }
    return 1;
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
        cache = maxcache;
        fuel = 800;
        who->AddMagicPoints(800);
    }

    dam = cache + random(maxcache);

    if(!active){
        write("The cannon clicks.");
        say(name+"'s cannon emits a click.");
        return 1;
    }

    if(cache < 5){
        cache = 0;
        write("The cannon clicks.");
        say(name+"'s cannon emits a click.");
        if(room) room->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
                "Power cache too low.", "poleepkwa");
        return 1;
    }

    if(fuel < maxcache){
        cache = 0;
        write("The cannon emits a harsh buzzing noise.");
        say(name+"'s cannon emits a harsh buzzing noise.");
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
        write("You fire your plasma cannon "+dir+"!");
        tell_room(env, name+" fires "+possessive(killer)+
                " plasma cannon "+dir+"!", ({killer}));

        if(debugging) bolt->SetDebugging(1);
        bolt->SetOwner(who);
        bolt->SetOwnerOb(this_object());
        bolt->SetDamage(dam);
        bolt->eventMove(env);
        bolt->SetArmed(1);
        bolt->eventDeploy(dir);
        killer->AddMagicPoints(-maxcache);
        cache = 0;
        return 1;
    }

    write("You blast "+patsy+" with your plasma cannon!");
    tell_room(env, name+" blasts "+patsy+" with "+possessive(killer)+
            " plasma cannon!", ({killer, target}));
    target->eventPrint(name+" blasts you with "+possessive(killer)+
            " plasma cannon!");

    dam = cache + random(maxcache);
    cache = 0;

    target->eventReceiveDamage(killer, HEAT, dam);
    killer->AddMagicPoints(-maxcache);
    target->AddEnemy(killer);

    if(tracking){
        if(room && (!Prey["ob"] || Prey["ob"] != target)){
            room->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
                    "New prey being tracked.", "poleepkwa");
        }
        Prey = ([ "ob" : target, "name" : patsy, "room" : environment(target),
                "player" : userp(target) ]);
    }

    return 1;
}

int ReportHit(object target){
    object here, room;
    string name;
    int player;
    if(!tracking || !active) return 0;
    //if(!inherits(LIB_MISSILE, previous_object())) return 0;
    here = environment(environment());
    if(Prey["ob"] && target == Prey["ob"]){
        here->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
                "Hit scored on target.", "poleepkwa");
        return 1;
    }
    room = environment(target);
    name = target->GetName();
    player = userp(target);
    Prey = (["ob" : target, "name" : name, "room" : room, "player" : player]); 
    here->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
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
                        "plasma cannon whines and clicks off.", ({env}));
                env->eventPrint("Your plasma cannon whines and clicks off.");
            }
        }
        else if(cache < maxcache){
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
                if(tracking && room){
                    room->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
                            "Contact with prey re-established.", "poleepkwa");
                }
                return;
            }
            else if(!Prey["player"]) Prey = ([]);
            if(room && !Prey["lost"]){
                if(tracking){
                    room->eventHearTalk(this_object(),0,TALK_LOCAL,"say",
                            "Contact with prey lost.", "poleepkwa");
                }
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
        if(tracking && notify && room){
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

varargs mixed DoWear(object who, mixed where){
    string extra;
    object env = environment(who);
    if(creatorp(who) || who->GetRace() == "poleepkwa"){
        active = 1; 
        if(creatorp(who)) cache = maxcache;
    }
    if(active) extra = " and it beeps and clicks on.";
    else extra = ".";
    who->eventPrint("You wear "+GetShort()+extra);
    if(env) tell_room(env, who->GetName()+" wears "+
            GetShort()+extra, ({who}));
    return 1;
}

mixed eventUnequip(object who){
    mixed ret = ::eventUnequip(who);
    object env = environment(who);
    if(ret && active){
        who->eventPrint("The cannon whines and clicks off.");
        if(env) tell_room(env, who->GetName()+"'s cannon "+
                "whines and clicks off.", ({who}));
        active = 0;
    }
    return ret;
}

int eventTurnOn(mixed str){
    if(!OperateThing(this_player(), str)) return 1;
    if(str && stringp(str)){
        if(str == "debugging" && creatorp(this_player())){
            write("Debugging enabled.");
            debugging = 1;
            return 1;
        }
        else if(str == "tracking"){
            if(tracking){
                write("Tracking already enabled.");
                return 1;
            }
            write("Tracking enabled.");
            tracking = 1;
            return 1;
        }
        else {
            write("The "+remove_article(GetShort())+" has "+
                    "no such feature.");
            return 1;
        }
    }
    if(active){
        write("The cannon is already on.");
        return 1;
    }
    if(!(this_object()->GetWorn()) || !this_player() ||
            environment(this_object()) != this_player()){
        write("You are not wearing the "+remove_article(GetShort())+".");
        return 1;
    }
    write("You activate the "+remove_article(GetShort())+".");
    say(this_player()->GetName()+"'s "+remove_article(GetShort())+
            " beeps and clicks on.");
    active = 1;
    return 1;
}

int eventTurnOff(mixed str){
    if(!OperateThing(this_player())) return 1;
    if(str && stringp(str)){
        if(str == "debugging" && creatorp(this_player())){
            write("Debugging disabled.");
            debugging = 0;
            return 1;
        }
        else if(str == "tracking"){
            if(!tracking){
                write("Tracking already disabled.");
                return 1;
            }
            write("Tracking disabled.");
            tracking = 0;
            return 1;
        }
        else {
            write("The "+remove_article(GetShort())+" has "+
                    "no such feature.");
            return 1;
        }
    }
    if(!active){
        write("The cannon is already off.");
        return 1;
    }
    write("You deactivate the "+remove_article(GetShort())+".");
    say(this_player()->GetName()+"'s "+remove_article(GetShort())+
            " whines and clicks off.");
    active = 0;
    return 1;
}
