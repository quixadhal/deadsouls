#include <lib.h>

inherit LIB_SENTIENT;

int give_it(string str);
static void create() {
    sentient::create();
    SetKeyName("Dr. Seth Brundle");
    SetId(({"seth","brundle","Dr. Brundle","Seth","Brundle","seth brundle", "Seth Brundle"}));
    SetShort("Dr. Seth Brundle");
    SetLong("Dr. Brundle is a tall, lanky scientist with an olive complexion "+
            "and dark, piercing eyes. He seems extremely upset, and hard at work. He looks "+
            "rumpled, wired, and really nervous.");
    SetInventory(([
                "/domains/campus/obj/omni" : 1,
                "/domains/campus/obj/labkey" : 1,
                ]));
    SetLevel(5);
    SetUnique(1);
    SetRace("human");
    SetGender("male");
    AddCommandResponse("give", (: give_it :));
    AddCommandResponse("omni", (: give_it :) );
    SetAction(25, ({
                "Seth Brundle runs a nervous hand through his hair.",
                "Dr. Brundle twitches momentarily.", "Dr. Seth Brundle ponders.",
                "Seth thinks.", "Dr. Seth Brundle thinks carefully.",
                "Dr. Seth Brundle says, \"You won't be able to get back without an omni.\"",
                "Dr. Seth Brundle peers nervously at the portal."}));
}
void reset(){
    if(!present("omni",this_object())){
        new("/domains/campus/obj/omni")->eventMove(this_object());
        return;
    }
}
int give_it(string str){
    if(!present("omni",this_object())){
        this_object()->eventForce("say I'm afraid I don't have anything to give");
        this_object()->eventForce("ponder");
        return 1;
    }
    if(!present("omni",this_player())){
        write("Dr. Brundle looks at you skeptically for a moment.\n");
        say("Dr. Brundle looks at "+this_player()->GetName()+" skeptically for a moment.\n");
        this_object()->eventForce("give first omni to "+lower_case(this_player()->GetName()) );
        this_object()->eventForce("say Just try not to get killed, "+this_player()->GetName());
        this_object()->eventForce("sigh");
        return 1;
    }
    this_object()->eventForce("snort");
    this_object()->eventForce("say You've already got one, "+this_player()->GetName());
    return 1;
}
void init(){
    ::init();
}
