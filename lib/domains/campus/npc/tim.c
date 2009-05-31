#include <lib.h>

inherit LIB_SENTIENT;

int DoSomething(){
    int rand;
    string thing1, thing2, thing3, thing4, thing5;
    string thing6, thing7, thing8, thing9, thing10;
    if(!this_object() || !clonep() || !environment()) return 0;
    rand = random(10);

    thing1 = "I think the campus is still under serious construction.";
    thing2 = "I wonder when the classroom building will open.";
    thing3 = "There's a village up north that's still being built, I heard.";
    thing4 = "Any travel north of here may be unsafe, you know.";
    thing5 = "Oh man, I need to get that stuff I left in the building basement.";
    thing6 = "Why won't Kim ever buy or sell weapons?";
    thing7 = "How am I supposed to leave campus if Kim won't sell me a weapon?";
    thing8 = "Maybe there's a shop in the village up north where you can buy whatever you need.";
    thing9 = "I heard you can get in trouble with the dean if you climb the tree.";
    thing10 = "Some dude told me \bree-yark\" is \"we surrender\" in \"Kobold\". Does that even make any sense?";

    switch(rand){
        case 1 : eventForce("say "+thing1);break;
        case 2 : eventForce("say "+thing2);break;
        case 3 : eventForce("say "+thing3);break;
        case 4 : eventForce("say "+thing4);break;
        case 5 : eventForce("say "+thing5);break;
        case 6 : eventForce("say "+thing6);break;
        case 7 : eventForce("say "+thing7);break;
        case 8 : eventForce("say "+thing8);break;
        case 9 : eventForce("say "+thing9);break;
        case 10 : eventForce("say "+thing10);break;
        default : eventForce("smile");
    }
}

void create() {
    sentient::create();
    SetKeyName("Tim");
    SetId(({"tim"}));
    SetShort("Tim");
    SetLong("Tim is a student in the Virtual Campus.");
    SetLevel(5);
    SetRace("human");
    SetGender("male");
    SetAction(25, (: DoSomething :));
}

void init(){
    ::init();
}
