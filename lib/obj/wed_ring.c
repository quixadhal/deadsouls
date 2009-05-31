/*    /obj/wed_ring.c
 *    from Dead Souls LPMud
 *    created by Descartes of Borg 951211
 */

#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ARMOR;

private string Spouse;

mixed eventMove(mixed dest);
string SetSpouse(string str);
string GetSpouse();
string RealLong();

static void create() {
    armor::create();
    AddSave( ({ "Spouse" }) );
    SetKeyName("wedding ring");
    SetId( ({ "ring", "official wedding ring" }) );
    SetAdjectives( ({ "golden", "gold" }) );
    SetShort("a wedding ring of gold");
    SetLong( (:RealLong:) );
    SetVendorType(VT_ARMOR | VT_MAGIC);
    SetMass(10);
    SetValue(0);
    SetPreventDrop("You may not drop your wedding ring!");
    SetRetainOnDeath(1);
    SetDamagePoints(1000000);
    SetProtection(BLUNT, 3);
    SetProtection(BLADE, 3);
    SetProtection(KNIFE, 3);
    SetArmorType(A_RING);
    SetRestrictLimbs( ({"left hand"}) );
}

mixed eventMove(mixed dest) {
    mixed tmp;

    if(archp(this_player())) return armor::eventMove(dest);
    if( !environment() ) {
        tmp = armor::eventMove(dest);
        //if( tmp == 1 ) environment()->eventForce("wear wedding ring");
        return tmp;
    }
    else return 0;
}

string SetSpouse(string str) {
    if( Spouse ) return Spouse;
    else return Spouse = str;
}

string GetSpouse() { return Spouse; }

string RealLong() {
    return ("This beautiful golden band was given to you by " +
            GetSpouse() + " to capture your eternal love.");
}

