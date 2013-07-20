/*    /lib/obj/room.c
 *    From the Dead Souls LPC Library
 *    The room object used to represent any room in the game
 *    Created by Descartes of Borg 940711
 *    Version: @(#) room.c 1.20@(#)
 *    Last Modified: 050912
 */

#ifndef NM_STYLE_EXITS
#define NM_STYLE_EXITS 1
#endif

#include <lib.h>
#include ROOMS_H
#include <daemons.h>
#include <function.h>
#include <medium.h>
#include <message_class.h>
#include <talk_type.h>
#include <terrain_types.h>
#include <respiration_types.h>
#include <privs.h>

inherit LIB_SHADOW_HOOK;
inherit LIB_CLEAN;
inherit LIB_CONTAINER;
inherit LIB_EXITS;
inherit LIB_DESCRIPTION;
inherit LIB_INVENTORY;
inherit LIB_LOOK;
inherit LIB_PROPERTIES;
inherit LIB_AMBIANCE;
inherit LIB_READ;
inherit LIB_MONEY;

private function        Bury          = 0;
private string          Climate       = "temperate";
private int             DayLight      = -1970;
private int             counter       = 0;
private static string   DayLong       = 0;
private object array    DummyItems    = ({});
private static int      GasCheck      = time();
private float           Gravity       = 1.0;
private static mixed    Listen        = 0;
private int             NightLight    = -1970;
private static string   NightLong     = 0;
private static int      NoReplace     = 0;
private static int      PlayerKill    = 0;
private static int      PoisonGas     = 0;
private static int      ResetNumber   = 0;
private static mixed    Search        = 0;
private static int      Shade         = 0;
private static mixed    Smell         = 0;
private static mixed    Touch         = 0;
private string          Town          = "wilderness";
private int		DefaultExits  = 1;
private int		Flying        = 1;
private int		Sitting       = 0;
private int		Kneeling      = 0;
private int		Standing      = 0;
private int		Swimming      = 0;
private int		ObviousVisible       = 1;
private int		ActionChance  = 10;
mapping			ItemsMap      = ([]);
private static mixed    global_item;
private static mixed	Action;
private int		tick_resolution	= 5;
private int		TerrainType	= T_OUTDOORS;
private int		RespirationType;
private int		Medium  	= MEDIUM_LAND;
private mapping         ActionsMap      = ([]);
private string          SinkRoom        = "";
private string          FlyRoom         = "";
private string          SkyDomain       = "";
private string          Elevator        = "";
private int             FlowLimit       = 0;
private int array       Coords          = ({});
private string array    Neighbors       = ({});
private mixed array     NeighborCoords  = ({});

string GetClimate();
int GetNightLight();
int GetDayLight();
int GetShade();
varargs mixed DestructEmptyVirtual(object ob);

mixed direct_delete_exit_str(){
    return 1;
}

mixed indirect_delete_exit_str(){
    return 1;
}

varargs int eventPrint(string msg, mixed arg2, mixed arg3);

/***********      /lib/room.c data manipulation functions      **********/

void CheckActions(){
    if(sizeof(ActionsMap)){
        foreach(mixed key, mixed val in ActionsMap){
            if( val > random(100) ){
                if(functionp(key)) evaluate(key);
                else eventPrint(key);
            }
        }
    }

    if( ActionChance > random(100) ){
        int x;

        if( functionp(Action) ) evaluate(Action);
        else if( pointerp(Action) && (x = sizeof(Action)) ){
            mixed act;

            act = Action[random(x)];
            if(functionp(act)){
                evaluate(act);
                return;
            }
            else eventPrint(act);
        }
    }
}

void heart_beat(){
    counter++;
    DestructEmptyVirtual();
    inventory::heart_beat();
    if(counter > 9999) counter = 0;
    CheckActions();
}

void SetAction(int chance, mixed val){
    ActionChance = chance;
    if( stringp(val) ) val = ({ val });
    else if( !functionp(val) && !pointerp(val) )
        error("Bad argument 2 to SetAction()\n");
    Action = val;
}

mixed GetAction(){ return Action; }

mapping SetActionsMap(mapping ActMap){
    if(ActMap && sizeof(ActMap)) ActionsMap = ActMap;
    return copy(ActionsMap);
}

mapping GetActionsMap(){
    return copy(ActionsMap);
}

int SetFrequency(int tick){
    if(tick) tick_resolution = tick;
    else tick_resolution = 5;
    set_heart_beat(0);
    return tick_resolution;
}

int GetFrequency(){
    return tick_resolution;
}

int GetTerrainType(){
    return TerrainType;
}

int SetTerrainType(int i){
    if(i) TerrainType = i;
    else return TerrainType;
}

int AddTerrainType(int i){
    if(!bitshiftedp(i)) return 0;
    else TerrainType = TerrainType | i;
    return TerrainType;
}

int RemoveTerrainType(int i){
    if(!bitshiftedp(i)) return 0;
    else TerrainType = TerrainType ^ i;
    return TerrainType;
}

int GetAmbientLight(){
    int a, dayset, nightset;

    dayset = this_object()->GetDayLight();
    nightset = this_object()->GetNightLight();

    if(dayset == -1970 && nightset == -1970 ){
        a = ambiance::GetAmbientLight();
    }

    else if( query_night() && nightset != -1970 ){
        a = nightset;
    }
    else if(!query_night() && dayset != -1970){
        a = dayset;
    }
    else {
        a = ambiance::GetAmbientLight();
    }

    foreach(object ob in all_inventory()){
        a += ob->GetRadiantLight();
    }

    return a;
}

function GetBury(){
    return Bury;
}

function SetBury(function what){
    Bury = what;
}

static string GetExtraLong(){
    int i;
    string *l = ({}),*tmp;
    string ret;
    object array stuff;
    ret = "  ";
    tmp = ({});
    stuff=all_inventory(this_object()); 
    for(i=0; i<sizeof(stuff);i++){
        if(sizeof(tmp = ({ stuff[i]->GetRoomAffectLong() })) ) l += tmp;
    }
    if( !sizeof(l) ) return 0;
    ret += implode(l, "	 ");
    return ret;
}

string GetInternalDesc(){
    string ret, tmp;

    if( DayLong && !query_night() ){
        ret = DayLong;
    }
    else if( NightLong && query_night() ){
        ret = NightLong;
    }
    else {
        ret = container::GetInternalDesc();
    }
    if( !ret ){
        ret = "";
    }
    if( tmp = GetExtraLong() ){
        ret += GetExtraLong();
    }
    return ret;
}

int GetResetNumber(){
    return ResetNumber;
}

string array GetId(){ return ({}); }

string SetDayLong(string str){ return (DayLong = str); }

string GetDayLong(){ return DayLong; }

string SetNightLong(string str){ return (NightLong = str); }

string GetNightLong(){ return NightLong; }

string SetClimate(string str){ 
    if(str == "indoors" && TerrainType == T_OUTDOORS) TerrainType = T_INDOORS;
    return (Climate = str); 
}

string GetClimate(){ return Climate; }

float SetGravity(float h){ return (Gravity = h); }

float GetGravity(){ return Gravity; }

int GetDayLight(){
    return DayLight;
}

static int SetDayLight(int x){
    return (DayLight = x);
}

object array GetDummyItems(){
    DummyItems = ({});
    foreach(object item in all_inventory(this_object())){
        if(base_name(item) == LIB_DUMMY){
            DummyItems += ({ item });
        }
    }
    return DummyItems;
}

varargs void AddItem(mixed item, mixed val, mixed adjectives){
    object ob, same_dummy;
    object *dummies = filter(all_inventory(this_object()), (: base_name(LIB_DUMMY) :) );
    global_item = item;

    if( objectp(item) ){
        same_dummy = filter(all_inventory(),(: base_name($1) == base_name(global_item) :));
        if(sizeof(same_dummy) && base_name(item) != LIB_ELEVATOR_BUTTON){
            return;
        }
        ob = item;
    }
    else {
        if( stringp(item) ){
            item = ({ item });
        }
        if( stringp(adjectives) ){
            adjectives = ({ adjectives });
        }
        same_dummy = filter(dummies,(: member_array(global_item[0],$1->GetId()) != -1 :));
        if(sizeof(same_dummy)) return;
        ob = new(LIB_DUMMY, item, val, adjectives);
    }
    ob->eventMove(this_object());
    DummyItems = ({ DummyItems..., ob });
}

mapping RemoveItem(mixed item){
    if( objectp(item) ){
        DummyItems -= ({ item });
        item->eventDestruct();
        return copy(Items);
    }
    else if( !arrayp(item) ){
        item = ({ item });
    }
    foreach(object ob in GetDummyItems()){
        if( sizeof(ob->GetId() & item) ){
            ob->eventDestruct();
            DummyItems -= ({ ob });
            return copy(Items);
        }
    }
}

mapping SetItems(mixed items){
    if(sizeof(DummyItems)) DummyItems->eventDestruct();
    DummyItems = ({});
    if( arrayp(items) ){
        items->eventMove(this_object());
        DummyItems = items;
    }
    else if( mapp(items) ){
        ItemsMap = items;
        foreach(mixed key, mixed val in items){
            string array adjs = ({});
            object ob;

            if( objectp(key) ){
                object *same_dummy = ({});
                global_item = key;
                same_dummy = filter(all_inventory() ,(: base_name($1) == base_name(global_item) :));
                if(sizeof(same_dummy)) continue;
                ob = key;
            }

            if( stringp(key) ){
                key = ({ key });
            }
            else {
                if( sizeof(key) == 2 && arrayp(key[0]) ){
                    adjs = key[1];
                    key = key[0];
                }
            }
            ob = new(LIB_DUMMY, key, val, adjs);
            if(ob){
                ob->eventMove(this_object());
                DummyItems = ({ DummyItems..., ob });
            }
        }
    }
    else {
        error("Bad argument 1 to SetItems(), expected object array or "
                "mapping.\n");
    }
    return copy(ItemsMap);
}

mapping GetItemsMap(){
    return copy(ItemsMap);
}

mapping GetSmellMap(){
    mapping Smells = ([]);
    foreach(object ob in GetDummyItems()){
        if( ob->GetSmell() ){
            Smells[ob->GetId()] = ob->GetSmell();
        }
    }
    if(this_object()->GetSmell()) Smells["default"] = this_object()->GetSmell();
    return copy(Smells);

}

mapping GetListenMap(){
    mapping Listens = ([]);
    foreach(object ob in GetDummyItems()){
        if( ob->GetListen() ){
            Listens[ob->GetId()] = ob->GetListen();
        }
    }
    if(this_object()->GetListen()) Listens["default"] = this_object()->GetListen();
    return copy(Listens);
}

mapping QueryMap(string str){
    switch(str){
        case "SetItems" : return GetItemsMap();break;
        case "SetSmell" : return GetSmellMap();break;
        case "SetListen" : return GetListenMap();break;
        case "SetInventory" : return this_object()->GetInventory();break;
        default : return ([]);
    }

}

varargs void AddListen(mixed item, mixed val){
    if( !val ){
        Listen = item;
        return;
    }
    if( !item || item == "default" ){
        Listen = val;
        return;
    }
    if( stringp(item) ){
        item = ({ item });
    }
    foreach(string tmp in item){
        foreach(object ob in GetDummyItems()){
            if( ob->id(tmp) ){
                ob->SetListen(val);
                break;
            }
        }
    }
}

mixed GetListen(){
    return Listen;
}

varargs void RemoveListen(mixed item){
    if( !item || item == "default" ){
        Listen = 0;
        return;
    }
    foreach(object ob in GetDummyItems()){
        if( stringp(item) ){
            if( ob->id(item) ){
                ob->SetListen(0);
            }
        }
        else if( arrayp(item) ){
            if( sizeof(ob->GetId() & item) ){
                ob->SetListen(0);
            }
        }
    }
}

varargs void SetListen(mixed items, mixed arg){
    if( !mapp(items) ){
        if( !arg ){
            AddListen("default", items);
        }
        else {
            AddListen(items, arg);
        }
        return;
    }
    foreach(mixed key, mixed val in items){
        AddListen(key, val);
    }
}

int SetMedium(int medium){
    Medium = medium;
    return Medium;
}

int GetMedium(){
    return Medium;;
}

int GetNightLight(){
    return NightLight;
}

int GetClimateExposed(){
    string *nonexposed_media = ({ MEDIUM_SPACE, MEDIUM_WATER });
    int nonexposed_terrain = (T_SEAFLOOR|T_INDOORS|T_UNDERWATER|T_UNDERGROUND|T_SPACE|T_PLANAR|T_BIOLOGICAL);
    if(GetClimate()=="indoors") return 0;
    if(GetTerrainType() & (nonexposed_terrain)) return 0;
    if(member_array(GetMedium(), nonexposed_media) != -1) return 0;
    return 1;
}

static int SetNightLight(int x){
    return (NightLight = x);
}

int SetNoReplace(int x){ return (NoReplace = x); }

int GetNoReplace(){ return NoReplace; }

int GetPlayerKill(){
    return PlayerKill;
}

int SetPlayerKill(int x){
    return (PlayerKill = x);
}

int AddPoisonGas(int x){
    PoisonGas += x;
    return PoisonGas;
}

int GetPoisonGas(){
    int x;

    if( PoisonGas < 1 ){
        return 0;
    }
    x = time() - GasCheck;
    GasCheck = time();
    if( x > 0 && x < 4 ){
        x = 1;
    }
    else {
        x = x/4;
    }
    PoisonGas -= x;
    if( PoisonGas < 0 ){
        PoisonGas = 0;
    }
    return PoisonGas;
}

int SetPoisonGas(int x){
    return (PoisonGas = x);
}

varargs void AddRead(mixed item, mixed val, string lang){
    if( stringp(item) ){
        item = ({ item });
    }
    foreach(string tmp in item){
        foreach(object ob in GetDummyItems()){
            if( ob->id(tmp) ){
                ob->SetRead(val);
                if(lang) ob->SetLanguage(lang);
                break;
            }
        }
    }
}

void RemoveRead(mixed item){
    foreach(object ob in GetDummyItems()){
        if( stringp(item) ){
            if( ob->id(item) ){
                ob->SetRead(0);
            }
        }
        else if( arrayp(item) ){
            if( sizeof(ob->GetId() & item) ){
                ob->SetRead(0);
            }
        }
    }
}

varargs void SetRead(mixed items, mixed arg){
    if( !mapp(items) ){
        AddRead(items, arg);
        return;
    }
    else foreach(mixed key, mixed val in items){
        AddRead(key, val, ((arg && stringp(arg)) ? arg : 0));
    }
}

int GetShade(){
    return Shade;
}

static int SetShade(int x){
    return (Shade = x);
}

varargs void AddSearch(mixed item, mixed val){
    if( !val ){
        Search = item;
        return;
    }
    if( !item || item == "default" ){
        Search = val;
        return;
    }
    if( stringp(item) ){
        item = ({ item });
    }
    foreach(string tmp in item){
        foreach(object ob in GetDummyItems()){
            if( ob->id(tmp) ){
                ob->SetSearch(val);
                break;
            }
        }
    }
}

varargs mixed GetSearch(){
    return Search;
}

varargs void RemoveSearch(mixed item){
    if( !item || item == "default" ){
        Search = 0;
        return;
    }
    foreach(object ob in GetDummyItems()){
        if( stringp(item) ){
            if( ob->id(item) ){
                ob->SetSearch(0);
            }
        }
        else if( arrayp(item) ){
            if( sizeof(ob->GetId() & item) ){
                ob->SetSearch(0);
            }
        }
    }
}

varargs void SetSearch(mixed items, mixed arg){
    if( !mapp(items) ){
        if( !arg ){
            AddSearch("default", items);
        }
        else {
            AddSearch(items, arg);
        }
        return;
    }
    foreach(mixed key, mixed val in items){
        AddSearch(key, val);
    }
}

varargs void AddSmell(mixed item, mixed val){
    if( !val ){
        Smell = item;
        return;
    }
    if( !item || item == "default" ){
        Smell = val;
        return;
    }
    if( stringp(item) ){
        item = ({ item });
    }
    foreach(string tmp in item){
        foreach(object ob in GetDummyItems()){
            if( ob->id(tmp) ){
                ob->SetSmell(val);
                break;
            }
        }
    }
}

varargs mixed GetSmell(){
    return Smell;
}

varargs void RemoveSmell(mixed item){
    if( !item || item == "default" ){
        Smell = 0;
        return;
    }
    foreach(object ob in GetDummyItems()){
        if( stringp(item) ){
            if( ob->id(item) ){
                ob->SetSmell(0);
            }
        }
        else if( arrayp(item) ){
            if( sizeof(ob->GetId() & item) ){
                ob->SetSmell(0);
            }
        }
    }
}

varargs void SetSmell(mixed items, mixed arg){
    if( !mapp(items) ){
        if( !arg ){
            AddSmell("default", items);
        }
        else {
            AddSmell(items, arg);
        }
        return;
    }
    foreach(mixed key, mixed val in items){
        AddSmell(key, val);
    }
}

varargs void AddTouch(mixed item, mixed val){
    if( !val ){
        Touch = item;
        return;
    }
    if( !item || item == "default" ){
        Touch = val;
        return;
    }
    if( stringp(item) ){
        item = ({ item });
    }
    foreach(string tmp in item){
        foreach(object ob in GetDummyItems()){
            if( ob->id(tmp) ){
                ob->SetTouch(val);
                break;
            }
        }
    }
}

varargs void RemoveTouch(mixed item){
    if( !item || item == "default" ){
        Touch = 0;
        return;
    }
    foreach(object ob in GetDummyItems()){
        if( stringp(item) ){
            if( ob->id(item) ){
                ob->SetTouch(0);
            }
        }
        else if( arrayp(item) ){
            if( sizeof(ob->GetId() & item) ){
                ob->SetTouch(0);
            }
        }
    }
}

varargs void SetTouch(mixed items, mixed arg){
    if( !mapp(items) ){
        if( !arg ){
            AddTouch("default", items);
        }
        else {
            AddTouch(items, arg);
        }
        return;
    }
    foreach(mixed key, mixed val in items){
        AddTouch(key, val);
    }
}

string GetTown(){ return Town; }

string SetTown(string town){ return (Town = town); }

/** this stuff is for backwards compat **/

mixed SetProperty(string prop, mixed val){
    if( prop == "light" ){
        if( !val ) return val;
        if( val < 0 ) return val;
        else return ambiance::SetAmbientLight(val*25);
    }
    else if( prop == "night light" ){
        if( !val ) return val;
        if( val < 0 ) return val;
        else return SetNightLight(15*val);
    }

    else return properties::SetProperty(prop, val);
}

mixed SetProperties(mapping mp){
    if( mp["light"] ){
        SetProperty("light", mp["light"]);
    }
    else if( mp["night light"] ){
        SetProperty("night light", mp["night light"]);
    }
    return properties::SetProperties(mp);
}


string GetLong(){
    return GetInternalDesc();
}

string SetLong(string str){
    return SetInternalDesc(str);
}

int CanAttack( object attacker, object who ){
    if( PlayerKill ){
        return 1;
    }
    attacker->RemoveHostile( who );
    return 0; 
}

varargs int eventShow(object who, string args){
    string str;

    if( !(str = SEASONS_D->GetLong(args)) ){
        who->eventPrint("You do not see that there.");
        return 1;
    } 
    who->eventPrint(str);
    eventPrint(who->GetName() + " looks at the " + args + ".", who);
}

/***********          /lib/room.c events          ***********/
mixed eventBuryItem(object who, object tool, object what){
    if( !functionp(Bury) ){
        return "You cannot bury things here!";
    }
    if( functionp(Bury) & FP_OWNER_DESTED ){
        return "You cannot bury things here.";
    }
    return evaluate(Bury, who, tool, what);
}

varargs mixed eventHearTalk(object who, object target, int cls, string verb,
        string msg, string lang){
    object *obs;
    string exit, door;

    switch(cls){
        case TALK_PRIVATE:
            return 1;

        case TALK_SEMI_PRIVATE:
            target->eventHearTalk(who, target, cls, verb, msg, lang);
            eventPrint("%^BOLD%^CYAN%^" + who->GetName() +
                    " whispers something to " + target->GetName() + ".",
                    MSG_CONV, ({ who, target }));
            return 1;

        case TALK_LOCAL:
            obs = get_livings(this_object(),1);
            if(sizeof(obs)) obs -= ({ who });
            if(sizeof(obs))
                obs->eventHearTalk(who, target, cls, verb, msg, lang);
            obs = get_livings(this_object(),2);
            if(sizeof(obs)) obs -= ({ who });
            if(sizeof(obs))
                obs->eventHearTalk(who, target, cls, verb, msg, lang);
            return 1;

        case TALK_AREA:
            foreach(exit in GetExits()){
                string tmp;

                tmp = GetExit(exit);
                if( !find_object(tmp) ) continue;
                tmp->eventHearTalk(who, target, TALK_LOCAL, verb, msg, lang);
            }
            foreach(exit in GetEnters(1)){
                string tmp;

                tmp = GetEnter(exit);
                if( !find_object(tmp) ) continue;
                if( (door = GetDoor(exit)) && door->GetClosed() ) continue;
                tmp->eventHearTalk(who, target, TALK_LOCAL, verb, msg, lang);
            }
            obs = filter(all_inventory(),
                    (: $1->is_living() && $1 != $(who) :));
            obs->eventHearTalk(who, target, cls, verb, msg, lang);
            return 1;

    }
}

int eventMove(){ return 0; }

varargs int eventPrint(string msg, mixed arg2, mixed arg3){
    object *targs;
    int msg_class;
    targs = filter(all_inventory(), 
            (: $1->is_living() || $1->GeteventPrints() :));

    if( !arg2 && !arg3 ){
        msg_class = MSG_ENV;
    }
    else if( objectp(arg2) || arrayp(arg2) ){
        if( objectp(arg2) ) arg2 = ({ arg2 });
        foreach(mixed mount in arg2){
            object *riders;
            if(!mount) continue;
            riders = mount->GetRiders();
            if(riders) targs += riders;
        }
        targs -=  arg2;
        if( !arg3 ) msg_class = MSG_ENV;
        else if(intp(arg3)) msg_class = arg3;
    }
    else if(!arg3){
        if(intp(arg2)) msg_class = arg2;
        else msg_class = MSG_ENV;
    }
    else if( objectp(arg3) || arrayp(arg3) ){
        if( objectp(arg3) ) arg3 = ({ arg3 });
        foreach(mixed mount in arg3){
            object *riders;
            if(!mount) continue;
            riders = mount->GetRiders();
            if(riders) targs += riders;
        }
        targs -= arg3;
        msg_class = arg2;
    }
    if(sizeof(targs)) targs->eventPrint(msg, msg_class);
    return 1;
}

static void create(){
    exits::create();
    inventory::reset(query_reset_number());
    set_heart_beat(0);
    if( replaceable(this_object()) && !GetNoReplace() ){
        string array tmp= inherit_list(this_object());
        if( sizeof(tmp) == 1 ){
            replace_program(tmp[0]);
        }
    }
}

int eventDestruct(){
    if(GetPersistent()){
        SaveObject();
    }
    return ::eventDestruct();
}

int CanReceive(object ob){
    if(!GetProperty("no teleport") || !living(ob)) return container::CanReceive(ob);
    else {
        string verb = query_verb();
        string *allowed = ({ "go", "climb", "jump", "enter", "fly", "crawl" });
        if(member_array(verb, allowed) == -1 && !archp(this_player())){
            write("Your teleportation is prevented.");
            return 0;
        }
    }
    return container::CanReceive(ob);
}

varargs void reset(int count){
    mixed *inv;

    if(origin() == "driver") return;

    inv = deep_inventory(this_object());
    if(inv && sizeof(inv)){
        if( sizeof(filter(inv, (: interactive($1) || $1->GetNoClean() :))) ){
            return;
        }
    }
    inventory::reset(count);
    all_inventory()->reset(count);
    ResetNumber++;
}

int id(){
    return 0;
}

int inventory_accessible(){
    return 1;
}

int inventory_visible(){
    return 1;
}

int SetNoDefaultExits(int i){
    if(!i) i = 0;
    DefaultExits = bool_reverse(i);
    ObviousVisible = DefaultExits;
    return DefaultExits;
}

int SetDefaultExits(int i){
    if(!i) i = 0;
    DefaultExits = i;
    ObviousVisible = DefaultExits;
    return DefaultExits;
}

int SetCanFly(int i){
    if(i && i > 0) Flying = 1;
    else Flying = 0;
}

mixed CanFly(object who, string dest){
    if(!who) who = this_player();
    if(!dest) dest = "";
    if(MEDIUM_WATER == Medium || MEDIUM_SPACE == Medium) return 0;
    if(sizeof(FlyRoom)) return 1;
    return Flying;
}

int SetCanSwim(int i){
    if(i && i > 0) Swimming = 1;
    else Swimming = 0;
}

mixed CanSwim(object who, string dest){
    if(!who) who = this_player();
    if(!dest) dest = "";
    if((MEDIUM_WATER == Medium) || (MEDIUM_SURFACE == Medium) ) return 1;
    if(GetTerrainType() & (T_ALL_SEA)) return 1;
    return Swimming;
}

int SetCanStand(int i){
    if(i && i > 0) Standing = 1;
    else Standing = 0;
}

mixed CanStand(object who, string dest){
    if(!who) who = this_player();
    if(!dest) dest = "";
    if(GetTerrainType() & (T_SPACE | T_UNDERWATER | T_SURFACE)) return 0;
    if(GetTerrainType() & (T_SEAFLOOR)) return 1;
    if((MEDIUM_AIR == Medium)) return 0;
    if((MEDIUM_LAND == Medium)) return 1;
    return Standing;
}

int SetCanSit(int i){
    if(i && i > 0) Sitting = 1;
    else Sitting = 0;
}

mixed CanSit(object who, string dest){
    if(!who) who = this_player();
    if(!dest) dest = "";
    if(GetTerrainType() & (T_SPACE | T_UNDERWATER | T_SURFACE)) return 0;
    if(GetTerrainType() & (T_SEAFLOOR)) return 1;
    if((MEDIUM_AIR == Medium)) return 0;
    if((MEDIUM_LAND == Medium)) return 1;
    return Sitting;
}

int SetCanKneel(int i){
    if(i && i > 0) Kneeling = 1;
    else Kneeling = 0;
}

mixed CanKneel(object who, string dest){
    if(!who) who = this_player();
    if(!dest) dest = "";
    if(GetTerrainType() & (T_SPACE | T_UNDERWATER | T_SURFACE)) return 0;
    if(GetTerrainType() & (T_SEAFLOOR)) return 1;
    if((MEDIUM_AIR == Medium)) return 0;
    if((MEDIUM_LAND == Medium)) return 1;
    return Kneeling;
}

int SetNoObviousExits(int i){
    if(!i) i = 0;
    ObviousVisible = bool_reverse(i);
    DefaultExits = ObviousVisible;
    return ObviousVisible;
}

int GetNoObviousExits(){
    return bool_reverse(ObviousVisible);
}

int GenerateObviousExits(){
    string *normals;
    string *exits;
    string dir_string, enters;
    exits = filter(GetExits(), (: !($1 == "up" && !(this_object()->GetVirtualSky()) &&
                    load_object(GetExit($1))->GetVirtualSky()) :) );
    enters = "";
    normals = ({ "north", "south", "east", "west", "up", "down" });
    normals += ({ "northeast", "southeast", "northwest", "southwest" });
    normals += ({ "out" });
    dir_string = "";

    if(sizeof(GetEnters(1)-({0}))){
        foreach(string enter in this_object()->GetEnters(1)){
            enters += "enter "+enter;
            if(member_array(enter,this_object()->GetEnters(1)) != 
                    sizeof(this_object()->GetEnters(1)) -1){
                enters +=", ";
            }
        }
    }

    if(NM_STYLE_EXITS){
        if(member_array("north",exits) != -1) dir_string += "n, ";
        if(member_array("south",exits) != -1) dir_string += "s, ";
        if(member_array("east",exits) != -1) dir_string += "e, ";
        if(member_array("west",exits) != -1) dir_string += "w, ";
        if(member_array("northeast",exits) != -1) dir_string += "ne, ";
        if(member_array("northwest",exits) != -1) dir_string += "nw, ";
        if(member_array("southeast",exits) != -1) dir_string += "se, ";
        if(member_array("southwest",exits) != -1) dir_string += "sw, ";
        if(member_array("up",exits) != -1) dir_string += "u, ";
        if(member_array("down",exits) != -1) dir_string += "d, ";
        if(member_array("out",exits) != -1) dir_string += "out, ";
    }
    else dir_string = implode(exits,", ")+", ";
    if(sizeof(this_object()->GetEnters(1) - ({0}) )){
        if(sizeof(this_object()->GetExits())) dir_string += ", ";
        dir_string += enters;
    }
    if(last(dir_string,2) == ", ") dir_string = truncate(dir_string,2);
    dir_string = replace_string(dir_string,", , ",", ");
    if(ObviousVisible) SetObviousExits(dir_string);
    return 1;
}

int eventReceiveObject(object ob){
    this_object()->SetSky();
    if(this_object() && ob && (living(ob) || ob->GetMapper())){
        //tc("1");
        if(MASTER_D->GetPerfOK()){
            int array Coords = ROOMS_D->SetRoom(this_object(), ob);
            //tc("2: "+identify(Coords));
            this_object()->CompileNeighbors(Coords);
        }
    }
    return container::eventReceiveObject(ob);
}

int eventReleaseObject(object ob){
    return container::eventReleaseObject(ob);
}

string SetFlyRoom(string str){
    FlyRoom = str;
    return FlyRoom;
}

string GetFlyRoom(){
    return FlyRoom;
}

string SetSinkRoom(string str){
    SinkRoom = str;
    return SinkRoom;
}

string GetSinkRoom(){
    return SinkRoom;
}

string SetSkyDomain(string str){
    SkyDomain = str;
    this_object()->SetSky();
    return SkyDomain;
}

string GetSkyDomain(){
    return SkyDomain;
}

int SetFlowLimit(int i){
    FlowLimit = i;
    return FlowLimit;
}

int GetFlowLimit(){
    return FlowLimit;
}

int SetRespirationType(int i){
    RespirationType = i;
    return RespirationType;
}

int GetRespirationType(){
    return RespirationType;
}

varargs mixed DestructEmptyVirtual(object ob){
    mixed *inv;
    if(ob && environment(ob) && environment(ob) == this_object()) return 0;
    if(!this_object()->GetVirtual()){
        return 0;
    }
    if(this_object()->GetNoClean()){
        return 0;
    }
    if(this_object()->GetExemptVirtual()){
        return 0;
    }
    inv  = filter(all_inventory(this_object()),
            (: !inherits(LIB_BASE_DUMMY, $1) :) );
    if(!sizeof(inv)){
        all_inventory(this_object())->eventDestruct();
        return eventDestruct();
    }
}

mixed eventPostRelease(object ob){
    mixed *inv;
    mixed ret = ::eventPostRelease(ob);
    DestructEmptyVirtual(ob);
    return ret;
}

void CompileNeighbors(mixed coords){
#if GRID
    int x, x2;
    int y, y2;
    int a, b, c;
    if(!Neighbors) Neighbors = ({});
    if(sizeof(Neighbors) || !(MASTER_D->GetPerfOK())) return;
    if(!sizeof(Coords)){
        mixed crds = ROOMS_D->GetCoordinates(this_object());
        if(sizeof(crds)){
            sscanf(crds,"%d,%d,%d",a,b,c);
            Coords = ({ a, b, c });
        }
        if(!sizeof(Coords)) return;
    }
    x2 = Coords[0]+2;
    y2 = Coords[1]+2;
    NeighborCoords = ({ });
    for(x = Coords[0]-1; x < x2; x++){
        for(y = Coords[1]-1; y < y2; y++){
            NeighborCoords += ({ ({ x, y, 0 }) });
        }
    }
    NeighborCoords -= ({ Coords });
    foreach(mixed foo in NeighborCoords){
        mixed bar = ROOMS_D->GetGrid(foo[0]+","+foo[1]+","+foo[2]);
        if(bar && bar["room"]){
            Neighbors += ({ bar["room"] });
        }
    }
#endif
}

mixed GetNeighbors(){
    return copy(Neighbors);
}

mixed GetNeighborCoords(){
    return copy(NeighborCoords);
}

mixed GetCoords(){
    return copy(Coords);
}

string SetCoordinates(string str){
    string ret;
    int x, y, z, i;
    if(!str) return 0;
    i = sscanf(str, "%d,%d,%d", x, y, z);
    if(i != 3){
        i = sscanf(str, "%d,%d", x, y);
        z = 0;
    }
    if(i < 2) return 0;
    ret = x+","+y+","+z;
    if(sizeof(ROOMS_D->GetGrid(ret))) return 0;
    ROOMS_D->SetRoom(this_object(), this_object(), ret);
    Coords = ({ x, y, z });
    return ret;
}

void SetSky(){
    if(GetMedium() != MEDIUM_LAND && GetMedium() != MEDIUM_SURFACE){
        return;
    }
    if(sizeof(SkyDomain) && !sizeof(FlyRoom)){
        mixed coords = this_object()->GetCoords();
        if(!sizeof(coords)) return;
        SetFlyRoom("/domains/"+SkyDomain+"/virtual/sky/"+
          coords[0]+","+coords[1]+","+(coords[2]+1));
        if(sizeof(FlyRoom)){
            catch(load_object(FlyRoom)->SetSinkRoom(base_name(this_object())));
        }
    }
}

static void init(){
    object prev = previous_object();
    SetSky();
    if(undefinedp(RespirationType)){
        switch(GetMedium()){
            case MEDIUM_WATER : RespirationType = R_WATER; break;
            case MEDIUM_SPACE : RespirationType = R_VACUUM; break;
            case MEDIUM_AIR : RespirationType = R_AIR; break;
            case MEDIUM_LAND : RespirationType = R_AIR; break;
            case MEDIUM_SURFACE : RespirationType = (R_AIR | R_WATER); break;
            case MEDIUM_METHANE : RespirationType = R_METHANE; break;
            default : RespirationType = R_AIR; break;
        }
    }
    if(this_object()->GetProperty("indoors")) SetClimate("indoors");
    if(!sizeof(GetObviousExits()) && DefaultExits > 0 && ObviousVisible){
        GenerateObviousExits();
    }
    if((Action && (sizeof(Action) || functionp(Action)))
            || sizeof(ActionsMap)){
        set_heart_beat(tick_resolution);
    }
#if GRID
    if(this_object() && prev && (living(prev) || prev->GetMapper())){
        if(MASTER_D->GetPerfOK()){
            Coords = ROOMS_D->SetRoom(this_object(), prev);
            CompileNeighbors(Coords);
        }
    }
#endif
    if(this_object()->GetVirtual() && !query_heart_beat()){
        set_heart_beat(1);
    }
}

string GetElevator(){
    return Elevator;
}

string SetElevator(string str){
    if(str) Elevator = str;
    return Elevator;
}
