/*    /lib/lamp.c
 *    from the Dead Souls V Object Library
 *    a lamp-like inheritable that burns and such
 *    created by Descartes of Borg 960513
 */

#include <lib.h>
#include "include/lamp.h"

inherit LIB_ITEM;
inherit LIB_LIGHT;
inherit LIB_FUEL;

private int BurnRate = 5;

static void create() {
    item::create();
    fuel::create();
    light::create();
}

static void heart_beat() {
    eventDecreaseFuel(1);
    if( GetFuelAmount() < 1) {
	eventDarken();
    }
}

mixed direct_light_obj() {
    if( GetFuelAmount() < 1 )
	return "There is no " + GetFuelType() + " in " + GetShort() + ".";
    return 1;
}

mixed eventDarken() {
    light::eventDarken();
    set_heart_beat(0);
    return 1;
}

varargs mixed eventLight(object who, object tool) {
    if( GetLit() ) return light::eventLight(who, tool);
    eventLight(who, tool);
    set_heart_beat(BurnRate);
    return 1;
}

int GetBurnRate() {
    return BurnRate;
}

static int SetBurnRate(int x) {
    return (BurnRate = x);
}

int GetRadiantLight(int ambient) {
    if( !GetLit() ) return 0;
    else return item::GetRadiantLight(ambient);
}

string GetShort() {
    return item::GetShort() + light::GetShort();
}

varargs string GetLong(string unused) {
    string tmp;

    tmp = fuel::GetLong(unused);
    if( tmp != "" ) return item::GetLong(unused) + "\n" + tmp;
    else return item::GetLong(unused);
}
