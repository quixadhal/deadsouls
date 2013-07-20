#include <lib.h>
#include <daemons.h>
#include <dirs.h>
#include <terrain_types.h>
#include <medium.h>
#include <virtual.h>

inherit LIB_VIRT_SKY;

static private int XPosition, YPosition, ZPosition;
int overland;
string dexit;
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
        mapping gloc = ROOMS_D->GetGrid(x+","+y+",0");

        if(sizeof(gloc)){
            dexit = gloc["room"];
            SetSinkRoom(gloc["room"]);
        }

        if(dexit){
            RemoveExit("down");
            AddExit("down", dexit);
        }

        else {
            if( (x < 10 && x > -26) && (y < 26 && y > -10) ){
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

//int eventReceiveObject(object ob){
//    if(this_object() && ob && (living(ob) || ob->GetMapper())){
//       tc("1");
//        if(MASTER_D->GetPerfOK()){
//            int array Coords = ROOMS_D->SetRoom(this_object(), ob);
//            tc("2: "+identify(Coords));
//            CompileNeighbors(Coords);
//        }
//    }
//    return ::eventReceiveObject(ob);
//}

void init(){
    ::init();
}
