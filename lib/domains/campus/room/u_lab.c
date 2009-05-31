#include <lib.h>
inherit LIB_ROOM;

string panel_look();
int dial;
string tele_where;
static void create() {
    room::create();
    dial=0;
    tele_where="a dark, metal corridor";
    SetClimate("indoors");
    SetAmbientLight(40);
    SetShort("Underground Lab");
    SetLong("You are in a messy, wildly disorganized underground "+
            "laboratory. There are notes strewn everywhere, and a large chalkboard "+
            "is covered with bizarre schematics and formulae. On the east wall is "+
            "some mysterious, glowing portal, about the size of a door. Next to the "+
            "portal is a control panel, and a crudely drawn sign. There is a door on "+
            "the west wall.");
    SetItems(([
                ({"lab","room","laboratory"}) : "This appears to be some sort of "+
                "secret physics lab.",
                "mess" : "It's definitely messy in here.",
                ({"note","notes"}) : "These are notes covered in highly technical "+
                "equations and drawings.",
                ({"board","chalkboard"}) : "It is filled with lots of jargon and symbols you "+
                "don't recognize.",
                ({"schematics","formulae"}) : "Complicated stuff, it looks like.",
                ({"schematic","formula"}) : "Complicated stuff, it looks like.",
                "east wall" : "Other than the mysterious glowing portal and the "+
                "control panel, the east wall seems pretty ordinary.",
                "west wall" : "The west wall has a door leading west.",
                "wall" : "It's a wall.",
                ({"portal","glowing portal","mysterious glowing portal"}) : "This is just "+
                "what you'd expect a dimensional portal to look like, if you knew what "+
                "it was. It pulsates with some weird energy, and you feel mildly "+
                "nauseous looking into it. You feel strangely compelled to enter it.",
                ({"panel","control panel"}) : (: panel_look :),
                "sign" : "A crude, hand-drawn sign in red ink next to the portal." ]));
    SetInventory(([
                ]));
    SetRead("sign","The sign reads: Enter at your own risk! This portal leads to some "+
            "places of questionable safety and unstable reality...you are warned!");

    AddRead( ({"schematics","formulae"}), ({"You don't understand them." }) );
    AddRead( ({"notes"}) , ({"They are incomprehensible to you."}) );
    AddRead( ({"board","chalkboard"}) , ({"It is full of cryptic equations. Buried in the middle of one "+
                "is 'Step 3: a miracle happens'."}) );
    SetExits((["west":"/domains/campus/room/tunnel3"]));
    SetEnters((["portal":"/domains/campus/room/access1"]));
    SetDoor("west","/domains/campus/doors/u_door");
    SetObviousExits("w");
    new("/domains/campus/npc/seth")->eventMove(this_object());
}
void init(){
    ::init();
    add_action("set_dial","turn");
    add_action("set_dial","set");
}
string panel_look(){
    return "The control panel contains a single dial and a tiny screen. "+
        "The dial is set to: "+dial+" and the screen reads: "+tele_where+".";
}
int set_dial(string str){
    int i;
    object ob;
    ob=present("seth",this_object());
    if(str=="dial"){
        write("You spin the dial.\n");
        say(this_player()->GetName()+" fiddles with the control panel.\n");
        if(ob){
            ob->eventForce("look at "+lower_case(this_player()->GetName()));
        }
        return 1;
    }
    if(sscanf(str,"dial to %d",i) > 0){
        if(i > 10) i=0;
        write("You set the dial to "+i+".\n");
        say(this_player()->GetName()+" turns the control panel dial.\n");
        if(ob){
            ob->eventForce("say I hope you know what you're doing");
        }
        dial=i;
        this_object()->set_portal(dial);
        return 1;
    }
    else return 0;
}
int set_portal(int i){
    if(i==0) { SetEnters((["portal":"/domains/campus/room/access1"]));
        tele_where="a dark, metal corridor";
        return 1;
    }
    if(i==1) { SetEnters((["portal":"/realms/boy/area/room3"]));
        tele_where="village gates";
        return 1;
    }
    if(i==2) { SetEnters((["portal":"/realms/dana/room/sc/marsh"]));
        tele_where="a marsh";
        return 1;
    }
    if(i==3) { SetEnters((["portal":"/realms/ember/swamp/swamp1"]));
        tele_where="a swamp";
        return 1;
    }
    if(i==4) { SetEnters((["portal":"/realms/haderach/land/cities/hartland/rooms/townsq"]));
        tele_where="a town square";
        return 1;
    }
    if(i==5) { SetEnters((["portal":"/realms/kyranna/law/maindesk"]));
        tele_where="a police department";
        return 1;
    }
    if(i==6) { SetEnters((["portal":"/realms/temujin/village/room/green"]));
        tele_where="a ruined village";
        return 1;
    }
    if(i==7) { SetEnters((["portal":"/realms/war/arena/3b1"]));
        tele_where="an arena";
        return 1;
    }
    if(i==8) { SetEnters((["portal":"/realms/zackron/metro/train/platfmn"]));
        tele_where="a train station";
        return 1;
    }
    if(i==9) { SetEnters((["portal":"/realms/hiccups/rooms/fen1"]));
        tele_where="entrance gates";
        return 1;
    }
    if(i==10) { SetEnters((["portal":"/domains/Midian/room/kaliid3"]));
        tele_where="road by a church";
        return 1;
    }
}
