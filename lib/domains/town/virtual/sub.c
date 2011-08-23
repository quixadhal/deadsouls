#include <lib.h>
#include <dirs.h>
#include <terrain_types.h>
#include <medium.h>
#include <virtual.h>

inherit LIB_VIRT_SUBSURFACE;

static private int XPosition, YPosition, ZPosition;
int max_north = 1000000;
int max_south = -1000000;
int max_east = 1000000;
int max_west = -1000000;
int max_up = 1000;
int max_down = 0;

varargs void SetLongAndItems(int x, int y, int z);

varargs int LimitTravel(int requested, int maximum, int lessthan, int minimum){
    if(lessthan && requested < minimum) return maximum;
    else if(lessthan && requested > maximum) return requested;
    else if(requested > maximum) return minimum;
    else return requested;
}

varargs static void create(int x, int y, int z) {
    string n, s, e, w, u, d;
    string ne, nw, se, sw;

    SetNoReplace(1);
    virt_subsurface::create();
    XPosition = x;
    YPosition = y;
    ZPosition = z;
    SetClimate("temperate");
    SetDayLight(( (30 - z < 0) ? 0 : 30 - z));
    SetNightLight(( (10 - z < 0) ? 0 : 10 - z));
    SetLongAndItems(x, y, z);
    SetShort("Underwater");
    if( x >= max_east ) e = "sun/" + (max_west) + "," + y + "," + z;
    else e = "sub/" + (x+1) + "," + y + "," + z;
    if( x <= max_west ) w = "sub/" + (max_east) + "," + y + "," + z;
    else w = "sub/" + (x-1) + "," + y + "," + z;
    if( y >= max_north ) n = "sub/" + x + "," + (max_south) + "," + z;
    else n = "sub/" + x + "," + (y+1) + "," + z;
    if( y <= max_south ) s = "sub/" + x+ "," + (max_north) + "," + z;
    else s = "sub/" + x + "," + (y-1) + "," + z;
    if( z == max_up ) u = "sub/" + x + "," + y + "," + (z);
    else u = "sub/" + x + "," + y + "," + (z-1);
    if( z == max_down ) d = "sub/" + x+ "," + (y) + "," + z;
    else d = "sub/" + x + "," + y + "," + (z+1);

    nw = "sub/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y+1, max_north)+ "," + z;
    ne = "sub/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y+1, max_north)+ "," + z;
    sw = "sub/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y-1, max_north, 1, max_south)+ "," + z;
    se = "sub/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y-1, max_north, 1, max_south)+ "," + z;

    SetGoMessage("You can't travel in that direction.");

    if( n ) AddExit("north", __DIR__ + n);
    if( s ) AddExit("south", __DIR__ + s);
    if( e ) AddExit("east", __DIR__ + e);
    if( w ) AddExit("west", __DIR__ + w);
    if( u ) AddExit("up", __DIR__ + u);
    if( d ) AddExit("down", __DIR__ + d);
    if( ne ) AddExit("northeast", __DIR__ + ne);
    if( nw ) AddExit("northwest", __DIR__ + nw);
    if( se ) AddExit("southeast", __DIR__ + se);
    if( sw ) AddExit("southwest", __DIR__ + sw);

    if(z == 1){
        RemoveExit("up");
        AddExit("up", __DIR__ + "surface/" + x + "," + y);
    }

    if(z == 10){
        RemoveExit("down");
        AddExit("down", __DIR__ + "bottom/" + x + "," + y);
    }
}

varargs void SetLongAndItems(int x, int y, int z) {
    mapping inv, items;
    string str;
    ::SetLongAndItems(x, y, z);

    inv = ([]);
    str = "This is the dark space below the surface of a great sea.";

    SetItems( ([ ({ "sea", "ocean" })  : "A seemingly endless body of water.",
                ]) );

    SetLong(str);
    SetTerrainType(T_UNDERWATER);
    SetMedium(MEDIUM_WATER);
    SetInventory( ([
                "/domains/town/obj/seawater" : 1,
                ]) );
}
