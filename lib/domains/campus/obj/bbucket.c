#include <lib.h>

inherit LIB_STORAGE;

void create() {
    ::create();
    SetKeyName("bin");
    SetId(({"bin","can","dustbin","trash","rubbish"}));
    SetAdjectives(({"metal","small","blue","recycling","trash","garbage","dust"}));
    SetShort("a recycling bin");
    SetLong("This is a blue trash can, marked with "+
            "the letters \"/dev/null\".");
    SetMass(274);
    SetBaseCost("silver",50);
    SetMaxCarry(999999);
}

int tidy_up(){
    object *inv;
    inv = all_inventory(this_object());
    foreach(object thing in inv){
        if(thing) thing->eventMove(load_object("/domains/town/room/furnace"));
    }
    return 1;
}

int eventReceiveObject(object ob){
    write("You make a deposit into the the recycling bin.\n");
    call_out((: tidy_up :),1);
    return 1;
}
mixed CanGet(object ob) { return "The bucket does not budge.";}

void init(){
}
