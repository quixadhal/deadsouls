#include <lib.h>
#include <dirs.h>
#include <terrain_types.h>
#include <medium.h>
#include <virtual.h>

inherit LIB_VIRT_LAND;

static private int XPosition, YPosition;
int max_north = 1000000;
int max_south = -1000000;
int max_east = 1000000;
int max_west = -1000000;
int max_up = 1000;
int max_down = 0;

varargs void SetLongAndItems(int x, int y, int z);

varargs int LimitTravel(int requested, int maximum, int lessthan){
    if(lessthan && requested < maximum) return maximum;
    else if(lessthan && requested > maximum) return requested;
    else if(requested > maximum) return maximum;
    else return requested;
}

varargs static void create(int x, int y) {
    string n, s, e, w;
    string ne, nw, se, sw;
    string sink, fly;

    SetNoReplace(1);
    virt_land::create();
    XPosition = x;
    YPosition = y;
    SetClimate("temperate");
    SetLongAndItems(x, y);
    SetShort("a great sea");
    if( x >= max_east ) e = "surface/" + (max_west) + "," + y;
    else e = "surface/" + (x+1) + "," + y;
    if( x <= max_west ) w = "surface/" + (max_east) + "," + y;
    else w = "surface/" + (x-1) + "," + y;
    if( y >= max_north ) n = "surface/" + x + "," + (max_south);
    else n = "surface/" + x + "," + (y+1);
    if( y <= max_south ) s = "surface/" + x+ "," + (max_north);
    else s = "surface/" + x + "," + (y-1);
    sink = "sub/" + x + "," + y + "," + 1;
    fly = "sky/" + x + "," + y + "," + 1;

    nw = "surface/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y+1, max_north);
    ne = "surface/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y+1, max_north);
    sw = "surface/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y-1, max_north, 1, max_south);
    se = "surface/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y-1, max_north, 1, max_south);

    SetSinkRoom(__DIR__+sink);
    SetFlyRoom(__DIR__+fly);

    SetGoMessage("You can't travel in that direction.");
    if( n ) AddExit("north", __DIR__ + n);
    if( s ) AddExit("south", __DIR__ + s);
    if( e ) AddExit("east", __DIR__ + e);
    if( w ) AddExit("west", __DIR__ + w);
    if( ne ) AddExit("northeast", __DIR__ + ne);
    if( nw ) AddExit("northwest", __DIR__ + nw);
    if( se ) AddExit("southeast", __DIR__ + se);
    if( sw ) AddExit("southwest", __DIR__ + sw);

    if(x == 5 && (y < 50 && y > -50)){
        RemoveExit("west");
        RemoveExit("northwest");
        RemoveExit("southwest");
        AddExit("west", "/domains/town/room/shore");
        AddExit("northwest", "/domains/town/room/shore");
        AddExit("southwest", "/domains/town/room/shore");
    }
}

varargs void SetLongAndItems(int x, int y, int z) {
    mapping inv, items;
    string str;
    ::SetLongAndItems(x, y);

    inv = ([]);
    str = "This is the surface of a vast sea.";
    if(query_night()) str += " The stars of the night sky glitter overhead.";
    else str+= " The sun seems oddly dim in the sky.";

    SetItems( ([ ({"sea","ocean"}) : "It is vast.",
                ]) );
    SetLong(str);
    SetDayLight(30);
    SetNightLight(30);
    SetInventory(inv);
    SetTerrainType(T_SURFACE);
    SetMedium(MEDIUM_SURFACE);
    SetInventory( ([
                "/domains/town/obj/seawater" : 1,
                ]) );
}
