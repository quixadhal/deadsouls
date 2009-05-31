// /domains/Praxis/obj/misc/gallows.c
//  Gallows for the Nightmare Mortal Law system
//  Written by Manny@Nightmare  940823

#include <lib.h>

inherit LIB_ITEM;
object hangman;
void set_hangman(object who);


create() {
    ::create();
    SetKeyName("gallows");
    SetId( ({ "gallows", "noose", "platform" }) );
    SetShort("the gallows");
    SetLong( (: "long_func" :) );
    SetPreventGet("You cannot get that!");
}

string long_func() {
    string str, name;

    str = "A large wooden platform with two upright timbers and "
        "a crossbeam with a rope and noose.";
    if(hangman) {
        if(name = capitalize(present("the corpse of "+hangman->query_name(),
                        environment(this_object()))->GetKeyName()))
            str += "\n"+name+" is hanging from the noose.";
        else str += "\n"+hangman->query_cap_name()+" is standing "
            "upon the platform, noose around "+hangman->query_possessive()+" neck.";
    }
    return str;
}

void set_hangman(object who) { hangman = who; }
