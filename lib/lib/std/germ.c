/*    /lib/germ.c
 *    From the Dead Souls Mud Library
 *    Germs which cause disease
 *    created by Descartes of Borg 940905
 *    Version: @(#) germ.c 1.12@(#)
 *    Last modified: 97/01/01
 */

#include <lib.h>
#include ROOMS_H
#include <daemons.h>

inherit LIB_ITEM;

private int             Communicable  = 1;
mixed    Cure          = 0;
private static function Infect        = 0;
private static int      LastHeartBeat = time();
private mixed           LifeSpan      = 60;
private int             CannotInfect  = 0;
private int             debug         = 0;
private string          Type          = "cold";
private string          GermName      = "generic germ";
private string array    ImmuneRaces   = ({ "android", "tree", 
        "plant", "elemental", "fish", "gargoyle", "god", "golem", "insect", 
        "slug", "snake", "wraith", "zombie", "bot", "strider", "vehicle",
        "mech" }); 

mixed eventMultiply();

/* ***************  /lib/germ.c data functions  *************** */
int isGerm(){
    return 1;
}

int GetCommunicable(){
    return Communicable;
}

int GetCannotInfect(){
    return CannotInfect;
}

int SetCannotInfect(int i){
    if(!i) i = 0;
    CannotInfect = i;
    return CannotInfect;
}

int SetGermName(string str){
    if(str) GermName = str;
    return 1;
}

string GetGermName(){
    return GermName;
}

/*
 * int SetCommunicable(int x);
 * int x - how infectious the germ is
 *
 * description:
 * Makes the germ multiply and infect on its own.  The more
 * communicable, the easier it infects hosts and the easier
 * it multiplies.
 * Non-communicable germs must be forced to infect hosts
 *
 * returns the degree to which it is communicable
 */
int SetCommunicable(int x){
    if( x > 100 ) x = 100;
    return (Communicable = x);
}

mixed GetCure(){
    return Cure;
}

/* mixed SetCure(mixed val) 
 * mixed val - function to test whether germ is cured 
 *             or int indicating how hard it is to cure
 *             -1 means incurable
 *
 * description
 * If a function, then that function will be used to 
 * determine if a given cure attempt is successful.
 * That function should return 1 for success, 0 for failure
 * If an int and -1, the germ is incurable
 * If an int > 0, then that much of a cure of the right
 * type is needed to cure the disease
 *
 * returns the successfully set value
 */
mixed SetCure(mixed val){
    if(intp(val)){
        Cure=val;
        return;
    }
    return (Cure = val);
}

function GetInfect(){
    return Infect;
}

/* function SetInfect(function f)
 * function f - Function to be called whenever this germ
 *              infects a host
 *
 * Sets a function to be called whenever a host is infected
 *
 * returns the function to be called
 */
function SetInfect(function f){
    Infect = f;
    return f;
}

int GetLifeSpan(){
    return LifeSpan;
}

/* int SetLifeSpan(int x)
 * int x - the length of time the germ can live outside
 *         a host
 * 
 * description
 * Sets the amount of time that can pass for the germ
 * to exist without a host
 *
 * returns the new life span
 */
int SetLifeSpan(mixed x){
    if(functionp(x)){
        x = evaluate(x);
    }
    if(!intp(x)) x = 60;
    //if( x > 600 ) x = 600;
    return (LifeSpan = x);
}

string GetType(){
    return Type;
}

/* string SetType(string type)
 * string type - the nature of the infection
 *
 * description
 * determines what sort of cure is needed
 * valid values are:
 *     viral
 *     bacterial
 *     parasite
 * returns the new germ type
 */
string SetType(string type){
    Type=type;
    if(!Type || Type == "") Type = "foo";
    return Type;
}

/* ***************  /lib/germ.c events  *************** */
/**
 * This event is triggered from within the player object by some
 * attempt by a cleric or other some such thing to cure the player.
 */
mixed eventCure(object who, int x, string type){
    if( type != GetType() ){ // Can't cure this
        return 0;
    }
    if( Cure == -1 ){ // This is incureable
        return 0;
    }
    if( functionp(Cure) ){
        mixed tmp = evaluate(Cure, who, x, type);

        if( tmp != 1 ){
            return tmp;
        }
        Cure = 0;
    }
    else if( intp(Cure) ){
        Cure -= x;
    }
    else {
        error("Bad argument 2 to eventCure().");
    }
    if( Cure > 0 ){
        return 0;
    }
    set_heart_beat(0);
    if(this_object()) this_object()->eventMove(ROOM_FURNACE);
    eventDestruct();
    return 1;
}

mixed eventEncounter(object who){
    if( !living(who) ){
        return 1;
    }
    if( !query_heart_beat() ){
        set_heart_beat(5);
    }
    if(this_object() && environment(this_object())) eventMultiply();
    return 1;
}

void init(){
    object env = environment();
    object *livs;
    if(!env) return;
    ::init();
    livs = (get_livings(env) || ({}));
    if(living(env)) env = environment(env);
    livs += get_livings(env);
    set_heart_beat(1);
    if(sizeof(livs)){
        foreach(object liv in livs){
            eventEncounter(liv);
        }
    }
}

mixed eventInfect(object ob){
    mixed tmp;
    object *presbane;
    string race;
    string *bane = ({});
    if(!ob){
        return 0;
    }

    if(RELOAD_D->GetWarmBootInProgress()){
        return 0;
    }
    if(environment() && base_name(environment()) == ROOM_VOID){
        return 0;
    }
    if(!this_object()){
        return 0;
    }
    if(!ob){
        return 0;
    }
    if(!ob->CanReceive(this_object())){
        return 0;
    }
    race = ob->GetRace();

    //presbane = present("bane",ob);
    presbane = filter(all_inventory(ob), (: inherits(LIB_BANE,$1) :) );
    if(sizeof(presbane)){
        foreach(object foo in presbane){
            bane += foo->QueryBane();
        }
    }

    if(sizeof(bane)){
        if(member_array(GetKeyName(),bane) != -1) return 0;
        if(member_array("all",bane) != -1) return 0;
        foreach(string foo in GetId()){
            if(member_array(foo,bane) != -1) return 0;
        }
    }

    if(present(this_object()->GetKeyName(),ob) ) return 0;
    if(race && member_array(race, ImmuneRaces) != -1) return 0; 
    if(ob->GetUndead() == 1) return 0;
    if(ob->GetAquatic() == 1) return 0;
    if(ob->GetNonCarbonBased() == 1) return 0;

    if( functionp(Infect) ){
        if(!this_object()){
            return 0;
        }
        tmp = evaluate(Infect, ob);
        if( tmp == 1 ){
            eventMove(ob);
            if(this_object() && environment(this_object()) != ob){
                eventMove(ROOM_FURNACE);
                set_heart_beat(0);
            }
            else set_heart_beat(5);
            return tmp;
        }
    }
    if(!eventMove(ob)) eventMove(ROOM_FURNACE);
    set_heart_beat(5);
    return 1;
}


mixed eventMultiply(){
    object ob, germ, winner;
    object *targs;

    if( Communicable > random(1000) && !CannotInfect){

        if( (ob = environment()) && living(ob) ){
            if(environment(environment())) ob = environment(environment());
        }

        if(!ob) return 0;

        //move to the host's environment
        if(!present(this_object()->GetName(),ob) ){
            germ = new(base_name(this_object()));
            if( germ && living(ob)  ) germ->eventInfect(ob);
            else if(!germ->eventMove(ob)) germ->eventMove(ROOM_FURNACE);
        }

        //find someone to infect
        if(!sizeof(get_livings(ob))) return 0;
        targs = filter(get_livings(ob), (: !query_carrying($1,base_name(this_object())) :) );
        if(sizeof(targs)) winner = targs[random(sizeof(targs))];
        if(this_object() && winner) new(base_name(this_object()))->eventInfect(winner);
    }
    return 1; 
}

void eventSuffer(object ob){
}

/* ***************  /lib/germ.c driver applies  *************** */

static void create(){
    item::create();
    AddSave(({ "Communicable", "LifeSpan", "Type" }));
    SetInvis(1);
    SetMass(0);
    SetValue(0);
    SetPreventDrop("");
    SetPreventGet("");
    SetPreventPut("");
}

static void heart_beat(){
    object env;
    int interval;

    if(!env = environment()) eventMove(ROOM_FURNACE);

    interval = time() - LastHeartBeat;
    LastHeartBeat = time();

    if( Communicable ){
        eventMultiply();
    }

    if( env && living(env) && environment(env)  && member_array(env->GetRace(),ImmuneRaces) == -1){
        eventSuffer(env);
    }

    if(LifeSpan == -1) return;
    LifeSpan -= interval;
    if( LifeSpan < 5 ){
        if(this_object()) this_object()->eventMove(ROOM_FURNACE);
        eventDestruct();
        return;
    }
}
