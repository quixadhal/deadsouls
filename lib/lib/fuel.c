/*    /lib/fuel.c
 *    from the Dead Souls V object Library
 *    fuel for things which need fuel
 *    this could be battery charge, gas, whatever
 *    created by Descartes of Borg 960512
 */

#include "include/fuel.h"

private int FuelAmount = 100;
private int MaxFuel = 100;
private int Refuelable = 1;
private string FuelType = "oil";

static void create() {
    this_object()->AddSave(({ "FuelAmount" }));
}

mixed eventDecreaseFuel(int x) {
    if( x < 1 ) return 0;
    FuelAmount -= x;
    if( FuelAmount < 1 ) FuelAmount = 0;
    return 1;
}

mixed eventRefuel(int x) {
    if( x < 1 ) return 0;
    FuelAmount += x;
    if( FuelAmount > MaxFuel ) FuelAmount = MaxFuel;
    return 1;
}

int GetFuelAmount() {
    return FuelAmount;
}

static int SetFuelAmount(int x) {
    return (FuelAmount = x);
}

string GetFuelType() {
    return FuelType;
}

static string SetFuelType(string str) {
    return (FuelType = str);
}


varargs string GetLong(string val) {
    /*    string str;

    if(!(str = item::GetLong(val)) || str == "") return str;
    else return sprintf("%s%s", str, (GetFuel() ? "" :
				      sprintf("\nThe %s is out of fuel.", GetKeyName())));
				      */
    if( !GetFuelAmount() )
	return capitalize(GetShort()) + " is out of fuel.";
    else return "";
}

int GetMaxFuel() {
    return MaxFuel;
}

static int SetMaxFuel(int x) {
    return (MaxFuel = x);
}

int GetRefuelable() {
    return Refuelable;
}

static int SetRefuelable(int x) {
    return (Refuelable = x);
}

