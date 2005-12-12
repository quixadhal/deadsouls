#include <lib.h>
#include "include/clip.h"
#include <vendor_types.h>
inherit LIB_STORAGE;
private int caliber, millimeter, MaxAmmo, ammo;
private string pistoltype,ammotype;
void create(){
    ::create();
    SetKeyName("magazine");
    SetId(({"clip","magazine"}));
    SetAdjectives(({"ammunition","pistol"}));
    SetShort("a pistol ammunition clip");
    SetLong("This is a slender, spring-loaded container for semiautomatic "+
      "pistol ammunition.");
    SetMass(10);
    SetValue(1);
    SetVendorType(VT_TREASURE);
}
int CanReceive(object ob){
    object id;
    string *namen;
    namen=ob->GetId();
    if(member_array("bullet",namen) == -1){
	write("Only bullets fit into the magazine.");
	return 0;
    }
    if(ob->GetAmmoType() != this_object()->GetAmmoType() ){
	write("That round is not the correct type for the magazine.");
	return 0;
    }
    if(ob->GetMillimeter() != this_object()->GetMillimeter() ){
	write("That round is not the correct size for the magazine.");
	return 0;
    }
    if(ob->GetCaliber() != this_object()->GetCaliber() ){
	write("That round is not the correct caliber for the magazine.");
	return 0;
    }
    if(ob->GetPistolType() != "auto" && ob->GetRifleType() != "auto"){
	write("That round is not a semiautomatic round.");
	return 0;
    }
    if(ammo == MaxAmmo){
	write("The magazine is filled to capacity.");
	return 0;
    }
    ammo++;
    return 1;
}
int CanRelease(object ob){
    ammo--;
    return 1;
}
int MinusAmmo(int i) { ammo -= i; return 1; }
int PlusAmmo(int i) { ammo += i; return 1; }
int SetMaxAmmo(int i) { MaxAmmo=i; return 1; }
int SetPistolType(string str) {pistoltype=str; return 1; }
int SetAmmoType(string str) {ammotype=str; return 1; }
int SetCaliber(int x) { caliber=x; return 1; }
int SetMillimeter(int x){ millimeter=x; return 1; }
int GetMillimeter() { return millimeter; }
int GetCaliber() { return caliber; }
string GetPistolType() { return pistoltype; }
string GetAmmoType() { return ammotype; }
