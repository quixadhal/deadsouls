#include <lib.h>

inherit LIB_ITEM;
int picking_quality = 1;

void create(){
    item::create();
    SetKeyName("lockpick");
    SetAdjectives(({"lockpicking","picking"}));
    SetId(({ "tool","pick" }));
    SetShort("a lockpick");
    SetLong("A tool for picking locks.");
    SetMass(10);
}

void init(){
    ::init();
}

int SetPickingQuality(int i){
    picking_quality = i;
    return picking_quality;
}

int GetPickingQuality(){
    return picking_quality;
}

varargs mixed CanPick(mixed who, mixed what){
    if(environment(this_object()) == this_player()){
    }
    return 1;
}


mixed indirect_pick_str_on_obj_with_obj(string str, object target, object tool,
        string str2, string targ_id){
    if( remove_article(lower_case(str)) != "lock" ){
        return "Pick the what?";
    }
    targ_id = remove_article(lower_case(targ_id));
    return CanPick(this_player(), targ_id);
}


varargs mixed eventPickLock(mixed who, mixed id, mixed tool){
    return picking_quality;
}
