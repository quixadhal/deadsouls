#include <lib.h>

inherit LIB_STORAGE;

mixed direct_enter_obj(){
    return 1;
} 
mixed direct_enter_into_obj(){
    return direct_enter_obj();
}

int eventEnter(object who){
    int ret;

    if(GetClosed()){
        write("You bump into the wardrobe's closed door.");
        say(this_player()->GetName()+" bumps into the wardrobe.");
        return 0;
    }

    ret = who->eventMoveLiving("/domains/town/room/secret",
            "$N enters the wardrobe.", "$N arrives from the bedroom west.");

    if(!ret){
        write("You fail to enter it.");
        return 0;
    }
    return 1;
}

void create() {
    ::create();
    SetKeyName("wardrobe");
    SetId(({"wardrobe"}));
    SetAdjectives(({"wood","tall"}));
    SetShort("a tall wooden wardrobe");
    SetLong("This piece of furniture is designed to hold "+
            "clothing and related accessories.");
    SetMass(2000);
    SetMaxCarry(2000);
    SetInventory(([
                "/domains/town/armor/long_boot_l" : 1,
                "/domains/town/armor/bearsuit" : 1,
                "/domains/town/armor/scarf" : 1,
                "/domains/town/armor/belt" : 1,
                "/domains/town/armor/goggles" : 1,
                "/domains/town/armor/shirt" : 1,
                "/domains/town/armor/jeans" : 1,
                "/domains/town/armor/hunting_cap" : 1,
                "/domains/town/armor/vest" : 1,
                "/domains/town/armor/long_boot_r" : 1,
                "/domains/town/armor/jacket" : 1,
                "/domains/town/armor/overcoat" : 1,
                ]));
    SetBaseCost("silver",150);
    SetCanClose(1);
    SetClosed(1);
    AddMoney("silver",100);
    SetPreventGet("The wardrobe does not budge.");
}

void init(){
    ::init();
}
