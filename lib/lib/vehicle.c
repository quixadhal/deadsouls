#include <lib.h>
#include <daemons.h>
#include <medium.h>
#include ROOMS_H
#include <position.h>

#define SOBER_COMBAT

inherit LIB_BOT;
inherit LIB_ENTER;
inherit LIB_EXITS;
inherit LIB_READ;

private object array    DummyItems    = ({});
mapping			ItemsMap      = ([]);
private static mixed    global_item;
string VehicleInterior = "";

mixed GetVehicleInterior();

void create(){
    bot::create();
    enter::create();
    SetPacifist(1);
    SetNoCondition(0);
    SetEnterMessage("");
}

void init(){
    string rvoid = ROOMS_D->GetVoid(this_object());
    string exterior = rvoid;
    bot::init();
    if(environment()){
        exterior = file_name(environment());
    }
    SetEnter( file_name(this_object()));
    SetExits( (["out": exterior ]) );
}

int inventory_accessible(){
    if(environment(this_player()) == this_object()){
        return 1;
    }
    return 0;
}

mixed eventMount(object who){
    int rider_weight;
    string weight = "weight";
    if(!who) return 0;
    if(environment() && environment()->GetMedium() == MEDIUM_SPACE){
        weight = "mass";
    }
    rider_weight = who->GetMass();
    if(!environment(this_object())) return 0;
    if(environment(who) && environment(who) == this_object()){
        return write("You are already mounted.");
    }
    if(rider_weight + this_object()->GetCarriedMass() > this_object()->GetMaxCarry()){
        return write("This vehicle cannot handle that much "+weight+".");
    }
    else {
        string int_desc = GetVehicleInterior();
        this_object()->SetNoClean(1);
        write("You enter "+this_object()->GetShort()+".");
        if(int_desc) write(int_desc);
        say(who->GetName()+" enters "+this_object()->GetShort()+".");
        who->SetProperty("mount", this_object());
        if(who->eventMove(this_object())) return AddRider(who);
        else return 0;
    }
}

varargs mixed eventEnter(object who, string what, string verb){
    if(query_verb() == "fly"){
        return this_object()->eventDrive(what);
    }
    return eventMount(who);
}

mixed CanGo(object who, string str){
    if( who->GetParalyzed() ) return "You are unable to move.";
    else return 1;
}

mixed eventGo(object who, string str){
    if(query_verb() == "fly"){
        return this_object()->eventDrive(str);
    }
    if(str == "out"){
        return eventDismount(who);
    }
    else return exits::eventGo(who, str);
}

mixed direct_drive(){
    return this_object()->GetMount();
}

mixed direct_drive_word_str(){
    return this_object()->GetMount();
}

mixed direct_fly(){
    return this_object()->GetMount();
}

mixed direct_fly_word_str(){
    return this_object()->GetMount();
}

int eventDrive(string direction){
    string travel_cmd, s1, s2;
    object *guys = get_livings(this_object());
    if(interactive(this_object())) return 0;
    if(sscanf(direction,"%s %s",s1,s2) == 2){
        if(s1 == "enter" || s1 == "into") travel_cmd = "enter";
    }

    if(!s1 || s1 == "") switch(this_object()->GetPosition()){
        case POSITION_STANDING : travel_cmd = "go";break;
        case POSITION_SITTING : travel_cmd = "crawl";break;
        case POSITION_LYING : travel_cmd = "crawl";break;
        case POSITION_FLYING : travel_cmd = "fly";break;
        default : travel_cmd = "go";
    }
    else direction = s2;
    this_object()->eventForce(travel_cmd+" "+direction);
    return 1;
}

varargs string GetInternalDesc(){
    object array items = all_inventory();
    string desc;
    int surfacep;

    surfacep = inherits(LIB_SURFACE , this_object());

    desc = "";
    if(this_object()->CanClose() && GetOpacity() > 33){
        if(this_object()->GetClosed()) desc += " It is closed. ";
        else desc += " It is open. ";
    }
    if(!surfacep) desc = desc + capitalize(add_article(GetShort(), 1));
    if(surfacep) desc = "On "+add_article(GetShort(), 1);
    items = filter(items, (: !($1->isDummy()) && !($1->GetInvis()) :));
    if( sizeof(items) ){
        if(surfacep){
            desc = desc+" you see " + item_list(items) + ".";
        }
        else desc = desc + " contains " + item_list(items) + ".";
    }
    else {
        if(!surfacep) desc = desc + " is completely empty.";
        else desc = desc + " you see nothing.";
    }
    return desc;
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
        same_dummy = filter(dummies,(: ($1->GetId())[0] == (global_item->GetId())[0] :));
        if(sizeof(same_dummy)) return;
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
            ob->eventMove(this_object());
            DummyItems = ({ DummyItems..., ob });
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

void AddRead(mixed item, mixed val){
    if( stringp(item) ){
        item = ({ item });
    }
    foreach(string tmp in item){
        foreach(object ob in GetDummyItems()){
            if( ob->id(tmp) ){
                ob->SetRead(val);
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
    foreach(mixed key, mixed val in items){
        AddRead(key, val);
    }
}

varargs int eventDie(mixed agent){
    object *riders=get_livings(this_object());
    if(riders && sizeof(riders)){
        foreach(object rider in riders) eventBuck(rider);
    }
    return bot::eventDie(agent);
}

int SetVehicleInterior(mixed arg){
    VehicleInterior = arg;
    return 1;
}

mixed GetVehicleInterior(){
    mixed ret;
    if(stringp(VehicleInterior)) return VehicleInterior;
    else if(functionp(VehicleInterior)){
        ret = evaluate(VehicleInterior);
        if(!stringp(ret)) return 0;
        return ret;
    }
    return 0;
}

varargs int CanFly(mixed who, mixed where){
    if(where && !(environment(this_object())->GetExit(where)) &&
            !(environment(this_object())->GetEnter(where))) return 0;

    if(this_object()->GetPosition() == POSITION_FLYING) return 1;
    return 0;
}

int eventMove(mixed dest){
    int ret;
    object env = environment();
    string location;

    if(!env) location = ROOM_START;
    else if(clonep(env)) location = file_name(env);
    else location = base_name(env);

    if(location) this_object()->SetProperty("LastLocation", location);
    ret = ::eventMove(dest);
    AddStaminaPoints(GetMaxStaminaPoints());
    return ret;
}
