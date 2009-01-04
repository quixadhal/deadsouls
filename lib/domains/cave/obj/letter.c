#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_READ;

string ReadFun(){
    string ret = "Dearest Kasatka,\n";
    ret += "You are right, and I must no longer do you the dishonour "+
        "of lying to you about the eastern frontiers. You deserve to know the "+
        "truth of our most desperate situation. The humanoids are an "+
        "implacable foe. They fight with the fury of demons, and seem "+
        "infinite in number. They are immune to diplomacy, insatiable "+
        "in their lust for blood and treasure, and revel in the horrors of "+
        "war they bring with them.\n\n"+
        "My finest sergeant's little boy was slaughtered in the last "+
        "attack on our outpost. He had feared sending him to the "+
        "western lands because of how many roving bands of human "+
        "adventurers now prey on the weak. Instead, here inside our own "+
        "stronghold, they came, and took his little life. A defenseless "+
        "child, who before he died, saw his mother fall under a "+
        "dwarven hammer.\n\n"+
        "Kasatka, my love for you is deathless. It binds me with mighty "+
        "chains that none can break. Yet duty holds me fast, and I pray "+
        "that what we do here, and what we may yet do if reinforcements "+
        "arrive in time, will stop this horrendous plague that threatens "+
        "our beau-\n\nThey are here! I hear them outside my door! Kasatka, "+
        "if these lines fall under your eye, and I am no more, know th";
    return ret;
}

void create(){
    ::create();
    SetKeyName("letter");
    SetId(({"letter","sheet","paper","sheet of paper"}));
    SetAdjectives(({"small"}));
    SetShort("a letter");
    SetLong("This is a sheet of paper with a message printed on it. ");
    SetMass(1);
    AddItem("message","A message written on paper.");
    SetRead("default", (: ReadFun :) );
    SetRead(({"message"}), (: ReadFun :) );
    SetLanguage("Tangetto");
}

void init(){
    ::init();
}
