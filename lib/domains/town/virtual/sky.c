#include <lib.h>
#include <dirs.h>
#include <terrain_types.h>
#include <medium.h>
#include <virtual.h>
#define __DIR__ "/domains/town/virtual/"

inherit LIB_VIRT_SKY;

static private int XPosition, YPosition, ZPosition;
int overland;
string dexit;

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

    int max_north = 100000;
    int max_south = 1;
    int max_east = 100000;
    int max_west = 1;
    int max_up = 1000;
    int max_down = 0;

    SetNoReplace(1);
    virt_sky::create();
    XPosition = x;
    YPosition = y;
    ZPosition = z;
    SetClimate("temperate");
    SetLongAndItems(x, y, z);
    SetShort("The sky");
    if( x >= max_east ) e = "sky/" + (max_west) + "," + y + "," + z;
    else e = "sky/" + (x+1) + "," + y + "," + z;
    if( x <= max_west ) w = "sky/" + (max_east) + "," + y + "," + z;
    else w = "sky/" + (x-1) + "," + y + "," + z;
    if( y >= max_north ) n = "sky/" + x + "," + (max_south) + "," + z;
    else n = "sky/" + x + "," + (y+1) + "," + z;
    if( y <= max_south ) s = "sky/" + x+ "," + (max_north) + "," + z;
    else s = "sky/" + x + "," + (y-1) + "," + z;
    if( z == max_up ) u = "sky/" + x + "," + y + "," + (z);
    else u = "sky/" + x + "," + y + "," + (z+1);
    if( z == max_down ) d = "sky/" + x+ "," + (y) + "," + z;
    else d = "sky/" + x + "," + y + "," + (z-1);

    nw = "sky/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y+1, max_north)+ "," + z;
    ne = "sky/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y+1, max_north)+ "," + z;
    sw = "sky/" + LimitTravel(x - 1, max_east, 1, max_west) + "," + LimitTravel(y-1, max_north, 1, max_south)+ "," + z;
    se = "sky/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y-1, max_north, 1, max_south)+ "," + z;

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

    if( (x < 26 && x > 0) && (y < 26 && y > 0) ){
        overland = 1;
    }
    if(z == 1000){
        RemoveExit("up");
        AddExit("up","/domains/town/virtual/space/1,1,1");
        return;
    }
    if(z == 1){
        string wut;

        if(y==2 && x==28) dexit = "/domains/town/room/mountain_road";
        else if(y==1 && x==28) dexit = "/domains/town/room/road";
        else if(y==100000 && x==22) dexit = "/domains/town/room/valley";
        else if(y==100000 && x==23) dexit = "/domains/town/room/clearing";
        else if(y==100000 && x==24) dexit = "/domains/town/room/forest_path1";
        else if(y==100000 && x==25) dexit = "/domains/town/room/bridge";
        else if(y==100000 && x==26) dexit = "/domains/town/room/road2";
        else if(y==100000 && x==27) dexit = "/domains/town/room/road1";
        else if(y==100000 && x==28) dexit = "/domains/town/room/vill_road1";
        else if(y==100000 && x==29) dexit = "/domains/town/room/vill_road2";
        else if(y==100000 && x==30) dexit = "/domains/town/room/vill_road3";
        else if(y==100000 && x==31) dexit = "/domains/town/room/vill_road4";
        else if(y==100000 && x==32) dexit = "/domains/town/room/shore";
        else if(y==99999 && x==28) dexit = "/domains/town/room/south_road1";
        else if(y==99998 && x==28) dexit = "/domains/town/room/south_road2";
        else if(y==99997 && x==28) dexit = "/domains/campus/room/npath2";
        else if(y==99996 && x==28) dexit = "/domains/campus/room/npath";
        else if(y==99995 && x==28) dexit = "/domains/campus/room/usquare";
        else if(y==99999 && x==26) dexit = "/domains/town/room/gate";
        else if(y==99998 && x==26) dexit = "/domains/town/room/mansion_ext";
        else if(y==99998 && x==25) dexit = "/domains/town/room/garden";

        if(dexit){
            RemoveExit("down");
            AddExit("down", dexit);
        }

        else {
            if( (x < 26 && x > 0) && (y < 26 && y > 0) ){
                wut = "forest";
            }
            else { 
                wut = "surface";
            }
            RemoveExit("down");
            AddExit("down", __DIR__ + wut+"/" + x + "," + y);
        }
    }
}

varargs void SetLongAndItems(int x, int y, int z) {
    mapping inv, items;
    string str;
    ::SetLongAndItems(x, y, z);

    inv = ([]);
    if(overland || dexit) str = "You are in the air above a small island in a vast sea that stretches from horizon to horizon.";
    else str = "You are in the air above a vast sea that stretches from horizon to horizon.";
    if(query_night()) str += " The stars of the night sky glitter overhead.";

    SetItems( ([ ({ "sea", "ocean" })  : "A seemingly endless body of water.",
                ]) );

    SetLong(str);
    SetDayLight(30);
    SetNightLight(30);
    SetTerrainType(T_MIDAIR);
    SetMedium(MEDIUM_AIR);
}
