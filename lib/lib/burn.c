/*    /lib/burn.c
 *    from the Dead Souls V Object Library
 *    inheritable for things that burn
 *    created by Descartes of Borg 960512
 *    Version: @(#) burn.c 1.3@(#)
 *    Last modified: 96/11/03
 */

#include <lib.h>

inherit LIB_FUEL;
inherit LIB_LIGHT;

private int FuelRequired = 1;
private int Heat = 0;
private int MinHeat = 5;
private int BurnRate = 10;
private int BurntValue = 0;

// abstract methods
int SetValue(int x);
// end abstract methods

int GetBurning();

static void create() {
    fuel::create();
    light::create();
    this_object()->AddSave(({ "Heat" }));
    call_out(function() {
	         if( GetBurning() ) set_heart_beat(BurnRate);
             }, 0);
}

int GetBurning() {
    return (Heat >= MinHeat);
}

int GetBurnRate() {
    return BurnRate;
}

static int SetBurnRate(int x) {
    return (BurnRate = x);
}

int GetBurntValue() {
    return BurntValue;
}

static int SetBurntValue(int x) {
    return (BurntValue = x);
}

int GetFuelRequired() {
    return FuelRequired;
}

static int SetFuelRequired(int x) {
    return (FuelRequired = x);
}

int GetHeat() {
    return Heat;
}

static int SetHeat(int x) {
    return (Heat = x);
}

int GetMinHeat() {
    return MinHeat;
}

static int SetMinHeat(int x) {
    return (MinHeat = x);
}

mixed CanBurn(object who) {
    if( environment() != this_player() &&
	environment() != environment(this_player()) ) {
	return "#That is not within your reach!";
    }
    if( FuelRequired && !GetFuelAmount() )
	return capitalize(GetShort()) + " is out of fuel.";
    if( GetBurning() ) return "It is already burning!";
    return light::CanLight(who);
}

mixed direct_burn_obj_with_obj() {
    return CanBurn(this_player());
}

mixed indirect_burn_obj_with_obj(object target, object source) {
    if( !target ) return (source == this_object());
    if( environment() != this_player() )
	return "#You must possess the source to use it.";
    if( !GetBurning() ) return capitalize(GetShort()) + " is not burning!";
    return 1;
}

mixed indirect_burn_obs_with_obj(object *targets, object source) {
    if( !targets ) return (source == this_object());
    if( environment() != this_player() )
	return "#You must possess the source to use it.";
    if( !GetBurning() ) return capitalize(GetShort()) + " is not burning!";
    return 1;
}

mixed direct_light_obj() {
    mixed tmp = CanBurn(this_player());

    if( tmp == 1 ) {
	return "Light it with what?";
    }
}

mixed direct_light_obj_with_obj() {
    return direct_burn_obj_with_obj();
}

mixed indirect_light_obj_with_obj(object target, object source) {
    return indirect_burn_obj_with_obj(target, source);
}

mixed indirect_light_obs_with_obj(object *targets, object source) {
    return indirect_burn_obs_with_obj(targets, source);
}

mixed eventBurnOut() {
    if( FuelRequired ) {
	SetFuelAmount(0);
    }
    SetValue(BurntValue);
    eventDarken();
    set_heart_beat(Heat = 0);
    return 1;
}

varargs mixed eventBurn(object who, object what) {
    int y;

    if( !what ) {
	if( Heat ) return 0;
	else y = MinHeat;
    }
    else y = (Heat + (int)what->GetHeat())/2;
    if( y < Heat ) y = Heat;
    if( !GetBurning() && y >= MinHeat ) {
	if( FuelRequired ) set_heart_beat(BurnRate);
    }
    else if( GetBurning() && y < MinHeat ) {
	if( FuelRequired ) set_heart_beat(0);
    }
    Heat = y;
    if( Heat < 0 ) Heat = 0;
    SetLit(1);
    return 1;
}

mixed eventLight(object who, object what) {
    return eventBurn(who, what);
}

static void heart_beat() {
    if( FuelRequired ) {
	eventDecreaseFuel(1);
	if( !GetFuelAmount() ) eventBurnOut();
    }
}

