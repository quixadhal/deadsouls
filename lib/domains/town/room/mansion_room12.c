#include <lib.h>
inherit LIB_ROOM;

int CheckQuest(object ob);
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Secret Chamber");
    SetLong("You are in a secret underground chamber beneath the "
            "mansion.");
    SetExits( ([
                "up" : "/domains/town/room/mansion_room9"
                ]) );
    SetObviousExits("up");
    SetInventory( ([
                "/domains/town/obj/chest2" : 1,
                ]) );
    SetDoor("up", "/domains/town/doors/trapdoor");
}
void init(){
    ::init();
    CheckQuest(this_player());
}
int CheckQuest(object ob){
    string *quests;
    quests = ob->GetQuests();
    if(!ob->GetQuest("Newbie Mansion Chamber Quest")){
        ob->AddQuest("the Finder of Hidden Chambers","Newbie Mansion Chamber Quest");
        write("%^BOLD%^%^RED%^\n\nCONGRATULATIONS!\n%^RESET%^");
        write("%^BOLD%^%^RED%^You have solved the secret quest of the "+
                "Hidden Mansion Chamber. You have "+
                "earned 2 quest points, and 1500 experience "+
                "points. Nice job!\n\n%^RESET%^");
        say(this_player()->GetName()+" solves newbie quest 1.");
        ob->AddQuestPoints(2);
        ob->AddExperiencePoints(1500);
        return 1;
    }
    return 1;
}
