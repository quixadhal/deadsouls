//  /domains/Praxis/obj/mon/spider.c
//  Semi-Random spiders for the Pit of Spiders
//  Part of the Mortal Law system at Nightmare
//  Created by Manny@Nightmare 940831

#include <lib.h>

inherit LIB_NPC;

create() {
    ::create();
    SetKeyName("spider");
    switch(random(5)) {
        case 0:
            SetId( ({ "spider", "leaper", "black leaper" }) );
            SetShort("Black Leaper");
            SetLong("There are few spiders larger and more greusome than "
                    "the infamous Black Leaper..");
            SetLevel(20);
            break;
        case 1:
        case 2:
            SetId( ({ "spider", "timber spider", "giant timber spider" }) );
            SetShort("Giant Timber Spider");
            SetLong("A very large and ugly grey spider.  It looks quite "
                    "hungry.");
            SetLevel(random(3)+12);
            break;
        default:
            SetId( ({ "spider", "cave spider" }) );
            SetShort("Praxis cave spider");
            SetLong("A medium-sized black spider with a cresent-shaped "
                    "moon on its tail end.");
            SetLevel(random(3)+8);
            break;
    }
    SetRace("insect");
    SetGender("male");
    SetAggressive(999999);
    SetDie( (:"death_func":) );
}

int death_func() {
    message("say", "%^RED%^"+this_object()->query_name()+" squeals out "
            "a loud shreak, then falls dead.", environment(this_object()));
    if(!present("spider", environment(this_object()))) {
        int x;

        message("say", "%^RED%^You notice additional spiders "
                "creeping in from cracks at the base of the pit!", 
                environment(this_object()));
        for(x=0; x<10; ++x) {
            new("/"+__DIR__+"obj/mon/spider")->move(environment(this_object()));
        }
    }
    return 1;
}
