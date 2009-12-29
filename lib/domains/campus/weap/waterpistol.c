/*    /domains/Examples/weapon/sword.c
 *    from the Dead Souls LPC Library
 *    a simple sword example, nothing fancy
 *    created by Descartes of Borg 950402
 */

#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_PISTOL;


static void create() {
    ::create();
    SetKeyName("water pistol");
    SetId(({"pistol","gun"}));
    SetAdjectives(({"water","toy","plastic","squirt"}));
    SetShort("a small plastic squirt gun");
    SetLong("A small red see-through plastic squirt gun in the shape "+
            "of a ray gun.");
    SetMass(10);
    SetDollarCost(5);
    SetVendorType(VT_WEAPON);
    SetClass(10);
    SetDamageType(BLUNT);
    SetFirearmType("auto");
    SetWeaponType("blunt");
}

varargs mixed eventShoot(object shooter, mixed target, string direction){
    string name,patsy;
    object killer, env;

    if(!target || direction){
        write("You can't do that.");
        return 1;
    }

    killer = this_player();
    name = killer->GetName();
    env = environment(killer);
    patsy = target->GetName();

    tell_room(env,name+" drenches "+patsy+" with "+possessive(killer)+" "+
            "water gun!",({killer,target}) );
    tell_object(killer,"You drench "+patsy+" with your water gun! Ha ha ha, "+
            nominative(target)+" looks like a dope!");
    tell_object(target,name+" drenches you with "+possessive(killer)+" "+
            "water gun! Ha ha ha! You look like a big wet dope!");
    return 1;
}

void init(){
    ::init();
}
