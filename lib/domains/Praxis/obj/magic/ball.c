#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void init() {
    ::init();
    add_action("scry", "scry");
}

void create() {
    ::create();
    SetKeyName("ball");
    SetId( ({ "ball", "base", "crystal ball", "honor_quest_ob" }) );
    SetShort( "%^CYAN%^a crystal ball%^RESET%^");
    SetLong("It has some writing on its base.");
    SetRead("From your grandfather with love.");

    SetMass(190);
    SetValue(50);
    SetVendorType(VT_MAGIC);
    SetProperty("magic item", ({"scry"}) );
    true();
}

int scry(string str) {
    object ob, env;

    if(!str) return notify_fail("Scry what?\n");
    write("You gaze into the crystal ball.");
    if(this_player()->query_mp() < 5) {
        write("Your magic powers are just too weak to help you.");
        return 1;
    }
    if(!(ob = find_living(str)) || creatorp(ob)) {
        write("No such person in our reality.");
        return 1;
    }
    if(!environment(ob)) return notify_fail("Gurble gurble gurble.\n");
    if(environment(ob)->GetProperty("no scry"))
    {
        write("Something is blocking your concentration.");
        return 1;
    }
    this_player()->add_mp(-5);
    if(!(env = environment(ob))) {
        write(ob->query_cap_name()+" is lost.");
        return 1;
    }
    write("You find "+ob->query_cap_name()+" in the crystal ball at:\n");
    write(env->GetShort()+"\n");
    return 1;
}
