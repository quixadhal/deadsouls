#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <localtime.h>
inherit LIB_ARMOR;

string l_time,tzone;
static void create(){
    armor::create();
    SetKeyName("pocket watch");
    SetId(({"watch","pocketwatch","pocket watch","timepiece"}));
    SetShort("a pocket watch");
    SetLong("This is a beautifully decorated and intricately carved "+
            "pocket watch. It glows with a bright, polished bronze "+
            "luster. It is quietly ticking the seconds, and it looks like you can get the "+
            "accurate time by checking the watch.");
    SetMass(10);
    SetBaseCost("silver", 40);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_AMULET);
}
void init(){
    ::init();
    add_action("check_time","check");
    add_action("pre_check","time");
}
int pre_check(string str){
    if(!str){
        this_object()->check_time("time");
        return 1;
    }
}
int check_time(string str){
    if(str=="time" || str=="watch"){
        if(present("pocket watch",this_player())){
            tzone="/daemon/timezone"->GetTZ();
            l_time="/daemon/current_time"->GetCurrentTime(tzone);
            write("Checking the pocket watch, you see it is "+l_time+".\n");
            say(this_player()->GetName()+" checks "+possessive(this_player())+
                    " pocket watch.\n");
            return 1;
        }
        write("You cannot read the watch face from here.\n");
        return 1;
    }
}
