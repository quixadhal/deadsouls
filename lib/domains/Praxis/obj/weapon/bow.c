//  /domains/Praxis/obj/weapon/bow.c
//  Short bow used in executions.
//  For the Nightmare mortal law system.
//  Created by Manny@Nightmare 940906

#include <lib.h>

inherit LIB_ITEM;

create() {
    ::create();
    SetKeyName("bow");
    SetId( ({ "bow", "short bow" }) );
    SetShort("short bow");
    SetLong("A small bow.");
    SetMass(10);
    SetClass(2);
    SetValue(10);
}

void init() {
    ::init();
    if(environment(this_object())->query_name() != "guard") {
        write("The short bow crumbles in your hands.");
        destruct();
    }
}

