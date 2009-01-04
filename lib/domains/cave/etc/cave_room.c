#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

static void create(){
    ::create();
}

int CanReceive(object ob){
    int ret, level;
    level = ob->GetLevel();
    if((creatorp(ob) && !archp(ob)) || (userp(ob) && level > 20)){
        //tc("denying: "+identify(ob));
        write("Sorry. This area is for normal players.");
        return 0;
    }
    ret = room::CanReceive(ob);
    //if(this_player() && this_player() != ob) ret = 0;
    if(ret){
        object *inv = deep_inventory(ob);
        foreach(object element in inv){
            string name = base_name(element);
            level = element->GetLevel();
            if(creatorp(element) && !archp(element)
                    || (userp(element) && level > 20)){
                write("Sorry. This is a player-only area.");
                return 0;
            }
            if(!strsrch(name,"/realms/") || !strsrch(name,"/open/")){
                element->eventMove(ROOM_FURNACE);
            }
        }
        //ob->ChangeClass(ob->GetClass());
    }
    return ret;    
}

void init(){
    ::init();
}
