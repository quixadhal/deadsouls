#include <lib.h>
#include "../customdefs.h"

inherit LIB_NPC;
string privs;

int AllowPass(object who, object what){
    if(who->GetKeyName() == privs) return 1;
    return ::AllowPass(who, what);
}

int CheckVisitor(object who){
    eventForce("say I'm sorry, "+remove_article(who->GetShort())+
            ". Only "+capitalize(privs)+" is allowed there.");
    return 1;
}

static void create(){
    npc::create();
    privs = query_privs();
    SetKeyName("fighter");
    SetId( ({"human","npc","warrior","","fighter"}) );
    SetShort("a fighter");
    SetLong("This is a sample warrior NPC.");
    SetLevel(10);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetInventory(([
                MY_ARMOR "/chainmail.c" : "wear chainmail",
                MY_ARMOR "/leather_boot_r.c" : "wear boot",
                MY_ARMOR "/leather_boot_l.c" : "wear boot",
                MY_WEAP "/sword.c" : "wield sword",
                ]));
    SetStat("strength",40);
    SetGuard("west", (: CheckVisitor :));
    SetGuard("east", "The fighter blocks your travel in that direction.");
}
void init(){
    ::init();
}
