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
    string n, s, e, w, u;
    string ne, nw, se, sw;

    SetNoReplace(1);
    virt_land::create();
    XPosition = x;
    YPosition = y;
    SetAmbientLight(0);
    SetLongAndItems(x, y);
    SetShort("The sea floor");
    if( x >= max_east ) e = "bottom/" + (max_west) + "," + y;
    else e = "bottom/" + (x+1) + "," + y;
    if( x <= max_west ) w = "bottom/" + (max_east) + "," + y;
    else w = "bottom/" + (x-1) + "," + y;
    if( y >= max_north ) n = "bottom/" + x + "," + (max_south);
    else n = "bottom/" + x + "," + (y+1);
    if( y <= max_south ) s = "bottom/" + x+ "," + (max_north);
    else s = "bottom/" + x + "," + (y-1);
    u = "sub/" + x + "," + y + ",10";

    nw = "bottom/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y+1, max_north);
    ne = "bottom/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y+1, max_north);
    sw = "bottom/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y-1, max_north, 1, max_south);
    se = "bottom/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y-1, max_north, 1, max_south);

    SetGoMessage("You can't travel in that direction.");
    if( n ) AddExit("north", __DIR__ + n);
    if( s ) AddExit("south", __DIR__ + s);
    if( e ) AddExit("east", __DIR__ + e);
    if( w ) AddExit("west", __DIR__ + w);
    if( ne ) AddExit("northeast", __DIR__ + ne);
    if( nw ) AddExit("northwest", __DIR__ + nw);
    if( se ) AddExit("southeast", __DIR__ + se);
    if( sw ) AddExit("southwest", __DIR__ + sw);
    if( u ) AddExit("up", __DIR__ + u);
}

varargs void SetLongAndItems(int x, int y, int z) {
    mapping inv, items;
    string str;
    ::SetLongAndItems(x, y);

    inv = ([]);
    str = "This is the bottom of a vast sea.";

    SetItems( ([ ({"sea","ocean"}) : "It is vast.",
                ]) );
    SetLong(str);
    SetInventory(inv);
    SetTerrainType(T_SEAFLOOR);
    SetMedium(MEDIUM_WATER);
    SetInventory( ([
                "/domains/town/obj/seawater" : 1,
                ]) );
}
