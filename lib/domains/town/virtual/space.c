#include <lib.h>
#include <dirs.h>
#include <terrain_types.h>
#include <medium.h>
#include <virtual.h>

inherit LIB_VIRT_SKY;

static private int XPosition, YPosition, ZPosition;
int max_north = 2100000000;
int max_south = -2100000000;
int max_east = 2100000000;
int max_west = -2100000000;
int max_up = 2100000000;
int max_down = -2100000000;

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
    string neu, nwu, seu, swu;
    string ned, nwd, sed, swd;
    string nu, nd, su, sd;
    string eu, ed, wu, wd;

    SetNoReplace(1);
    virt_sky::create();
    XPosition = x;
    YPosition = y;
    ZPosition = z;
    SetLongAndItems(x, y, z);
    SetShort("Outer Space");
    if( x >= max_east ) e = "space/" + (max_west) + "," + y + "," + z;
    else e = "space/" + (x+1) + "," + y + "," + z;
    if( x <= max_west ) w = "space/" + (max_east) + "," + y + "," + z;
    else w = "space/" + (x-1) + "," + y + "," + z;
    if( y >= max_north ) n = "space/" + x + "," + (max_south) + "," + z;
    else n = "space/" + x + "," + (y+1) + "," + z;
    if( y <= max_south ) s = "space/" + x+ "," + (max_north) + "," + z;
    else s = "space/" + x + "," + (y-1) + "," + z;
    if( z == max_up ) u = "space/" + x + "," + y + "," + (max_down);
    else u = "space/" + x + "," + y + "," + (z+1);
    if( z == max_down ) d = "space/" + x+ "," + (y) + "," + (max_up);
    else d = "space/" + x + "," + y + "," + (z-1);

    nw = "space/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y+1, max_north)+ "," + z;
    ne = "space/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y+1, max_north)+ "," + z;
    sw = "space/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y-1, max_north, 1, max_south)+ "," + z;
    se = "space/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y-1, max_north, 1, max_south)+ "," + z;

    nu = "space/" + x + "," + LimitTravel(y+1, max_north)+ "," + LimitTravel(z+1, max_up);
    nd = "space/" + x + "," + LimitTravel(y+1, max_north)+ "," + LimitTravel(z-1, max_up, 1, max_down);
    su = "space/" + x + "," + LimitTravel(y-1, max_north, 1, max_south)+ "," + LimitTravel(z+1, max_up);
    sd = "space/" + x + "," + LimitTravel(y-1, max_north, 1, max_south)+ "," + LimitTravel(z-1, max_up, 1, max_down);


    eu = "space/" + LimitTravel(x+1, max_north)+ ","+y+ "," + LimitTravel(z+1, max_up);
    ed = "space/" + LimitTravel(x+1, max_north)+ ","+y+ "," + LimitTravel(z-1, max_up, 1, max_down);
    wu = "space/" + LimitTravel(x-1, max_north, 1, max_south)+ ","+y+ "," + LimitTravel(z+1, max_up);
    wd = "space/" + LimitTravel(x-1, max_north, 1, max_south)+ ","+y+ "," + LimitTravel(z-1, max_up, 1, max_down);

    nwu = "space/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y+1, max_north)+ "," + LimitTravel(z + 1, max_up);
    neu = "space/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y+1, max_north)+ "," + LimitTravel(z + 1, max_up);
    swu = "space/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y-1, max_north, 1, max_south)+ "," + LimitTravel(z + 1, max_up);
    seu = "space/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y-1, max_north, 1, max_south)+ "," + LimitTravel(z + 1, max_up);

    nwd = "space/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y+1, max_north)+ "," + LimitTravel(z -1, max_up, 1, max_down);
    ned = "space/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y+1, max_north)+ "," + LimitTravel(z -1, max_up, 1, max_down);
    swd = "space/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y-1, max_north, 1, max_south)+ "," + LimitTravel(z -1, max_up, 1, max_down);
    sed = "space/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y-1, max_north, 1, max_south)+ "," + LimitTravel(z -1, max_up, 1, max_down);


    SetGoMessage("You can't travel in that direction.");

    if( n ) AddExit("+x", __DIR__ + n);
    if( s ) AddExit("-x", __DIR__ + s);
    if( e ) AddExit("+y", __DIR__ + e);
    if( w ) AddExit("-y", __DIR__ + w);
    if( u ) AddExit("+z", __DIR__ + u);
    if( d ) AddExit("-z", __DIR__ + d);
    if( ne ) AddExit("+x+y", __DIR__ + ne);
    if( nw ) AddExit("+x-y", __DIR__ + nw);
    if( se ) AddExit("-x+y", __DIR__ + se);
    if( sw ) AddExit("-x-y", __DIR__ + sw);

    if( nu ) AddExit("+x+z", __DIR__ + nu);
    if( nd ) AddExit("+x-z", __DIR__ + nd);
    if( su ) AddExit("-x+z", __DIR__ + su);
    if( sd ) AddExit("-x-z", __DIR__ + sd);
    if( eu ) AddExit("+y+z", __DIR__ + eu);
    if( ed ) AddExit("+y-z", __DIR__ + ed);
    if( wu ) AddExit("-y+z", __DIR__ + wu);
    if( wd ) AddExit("-y-z", __DIR__ + wd);

    if( neu ) AddExit("+x+y+z", __DIR__ + neu);
    if( nwu ) AddExit("+x-y+z", __DIR__ + nwu);
    if( seu ) AddExit("-x+y+z", __DIR__ + seu);
    if( swu ) AddExit("-x-y+z", __DIR__ + swu);
    if( ned ) AddExit("+x+y-z", __DIR__ + ned);
    if( nwd ) AddExit("+x-y-z", __DIR__ + nwd);
    if( sed ) AddExit("-x+y-z", __DIR__ + sed);
    if( swd ) AddExit("-x-y-z", __DIR__ + swd);
}

varargs void SetLongAndItems(int x, int y, int z) {
    mapping inv, items;
    string str;
    ::SetLongAndItems(x, y, z);

    inv = ([]);
    str = "Outer space. A big, place.";

    SetItems( ([ ({ "star","stars" })  : "Oddly steady in their shine, they stand silent witness.",
                ]) );

    SetLong(str);
    SetDayLight(30);
    SetNightLight(30);
    SetTerrainType(T_SPACE);
    SetMedium(MEDIUM_SPACE);
}
