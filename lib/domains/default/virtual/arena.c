#include <lib.h>
#include <dirs.h>
#include <virtual.h>
#include <position.h>

inherit LIB_VIRT_LAND;

static private int XPosition, YPosition;

int max_north = 4999;
int max_south = 4989;
int max_east = 5015;
int max_west = 5005;

varargs void SetLongAndItems(int x, int y, int z);

varargs int LimitTravel(int requested, int maximum, int lessthan){
    if(lessthan && requested < maximum) return maximum;
    else if(lessthan && requested > maximum) return requested;
    else if(requested > maximum) return maximum;
    else return requested;
}

#if 0
int PreExit(mixed arg1, mixed arg2){
    object ob = this_player();
    if(!ob) return 0;
    if(ob->GetPosition() != POSITION_FLYING){
        write("You are not flying!");
        return 0;
    }
    return 1;
}
#endif

varargs static void create(int x, int y) {
    string n, s, e, w;
    string ne, nw, se, sw;
    string fly;

    SetNoReplace(1);
    virt_land::create();
    XPosition = x;
    YPosition = y;
    SetClimate("temperate");
    SetAmbientLight(30);
    SetLongAndItems(x, y);
    SetShort("a flat plain");
    if( x == max_east ) e = "arena/" + (x) + "," + y;
    else e = "arena/" + (x+1) + "," + y;
    if( x == max_west ) w = "arena/" + (x) + "," + y;
    else w = "arena/" + (x-1) + "," + y;
    if( y == max_north ) n = "arena/" + x + "," + (y);
    else n = "arena/" + x + "," + (y+1);
    if( y == max_south ) s = "arena/" + x+ "," + (y);
    else s = "arena/" + x + "," + (y-1);
    //fly = "sky/" + x + "," + y + "," + 1;

    nw = "arena/" + LimitTravel(x - 1, max_west, 1) + "," + LimitTravel(y+1, max_north);
    ne = "arena/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y+1, max_north);
    sw = "arena/" + LimitTravel(x - 1, max_west, 1) + "," + LimitTravel(y-1, max_south, 1);
    se = "arena/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y-1, max_south, 1);

    //SetFlyRoom(__DIR__+fly);
    SetSkyDomain("town");

    SetGoMessage("You can't travel in that direction.");
    if( n ) AddExit("north", __DIR__ + n);
    if( s ) AddExit("south", __DIR__ + s);
    if( e ) AddExit("east", __DIR__ + e);
    if( w ) AddExit("west", __DIR__ + w);
    if( ne ) AddExit("northeast", __DIR__ + ne);
    if( nw ) AddExit("northwest", __DIR__ + nw);
    if( se ) AddExit("southeast", __DIR__ + se);
    if( sw ) AddExit("southwest", __DIR__ + sw);

    //AddExit("up", __DIR__ + "sky/"+x+","+y+",1", (: PreExit :) );
    //AddExit("up", __DIR__ + "sky/"+x+","+y+",1", (: PreExit :) );

    if(y == max_south){
        RemoveExit("south");
        RemoveExit("southeast");
        RemoveExit("southwest");
    }
    if(y == max_north){
        RemoveExit("north");
        RemoveExit("northeast");
        RemoveExit("northwest");
    }
    if(x == max_west){
        RemoveExit("west");
        RemoveExit("northwest");
        RemoveExit("southwest");
    }
    if(x == max_east){
        RemoveExit("east");
        RemoveExit("northeast");
        RemoveExit("southeast");
    }
    if(x == 5005 && y == 4999){
        RemoveExit("west");
        AddExit("west","/domains/default/room/wiz_corr_east3");
        AddItem("sign" , "This is a sign planted on the ground.");
        SetRead( ({"sign"}) , "Wiz labs west of here.");
    }
}

varargs void SetLongAndItems(int x, int y, int z) {
    mapping inv, items;
    string str;
    ::SetLongAndItems(x, y, z);

    inv = ([]);
    str = "You are on a large flat plain, bordered on all sides "
        "by stone walls, forming a large arena for heavy weapons and "
        "mounted combat.";
    if(query_night()) str += " The stars of the night sky glitter overhead.";
    if(x == max_west && y != max_north) str += " A stone wall prevents further travel west.";
    if(x == max_east) str += " A stone wall prevents further travel east.";
    if(y == max_south ) str += " A stone wall prevents further travel south.";
    if(y == max_north) str += " A stone wall prevents further travel north.";
    if(x == max_west && y == max_north) str += "\n%^GREEN%^There is a sign here you can read.%^RESET%^";

    SetItems( ([ "arena" : "A place of violent death and great destruction.",
                ]) );

    if(y == max_north) {
        AddItem( ({ "rock wall","wall","stone wall"}), 
                "This vast stone wall prevents further travel north." );
    }
    else if(y == max_south) {
        AddItem( ({ "rock wall","wall","stone wall"}),
                "This vast stone wall prevents further travel south." );
    }

    if(x == max_east) {
        AddItem( ({ "rock wall","wall","stone wall"}),
                "This vast stone wall prevents further travel east." );
    }
    if(x == max_west) {
        AddItem( ({ "rock wall","wall","stone wall"}),
                "This vast stone wall prevents further travel west." );
    }
    AddItem( ({ "walls","rock walls","stone walls" }),
            "Large walls form the bounds of this killing field." );
    SetLong(str);
    SetDayLight(30);
    SetNightLight(30);
}
