/*    /lib/virtual/virt_land.c
 *    From Dead Souls Object Library
 *    A virtual object for rooms
 *    Created by Dvarsk 980310
 *    Version: @(#) virt_land.c 1.4@(#)
 *    Last modified: 98/09/12
 */

#include <lib.h>
#include <medium.h>

inherit LIB_ROOM;

private int CoordX = 0;
private int CoordY = 0;
private int CoordZ = 0;
private object Map_Master;
private int Medium;
private string Virt_Room = "";
private string AreaType = "";
private string Reset_Message;
private mapping Surrounding_Areas=([]);

void SetVirtualExits(int x, int y, int z);

/******************* virt_land.c required overrides ***********/

void SetLongAndItems(int x, int y, int z) {
    SetVirtualExits(x, y, z);
} 

/******************* virt_land.c required settings ************/

int SetMasterMap(mixed map_name){
    if(!stringp(map_name) && !objectp(map_name)){
        error("Bad argument 1 to SetMasterMap()\n\t"
            "Expected: string or object, Got: " + typeof(map_name)
            + ".");
        return 0;
    }
    if(stringp(map_name)){
        if( !(Map_Master = find_object(map_name)) ) {
            string str;

            if( str = catch(call_other(map_name, "???")) ) {
                if( creatorp() ) message("error", str,
                    previous_object());
                return 0;
            }
            Map_Master = find_object(map_name);
        }
    }

    else Map_Master=map_name;
    return 1;

}

int SetVirtFile( string file ){
    string str;
    if( str = catch(call_other(file, "???")) ) {
        if( creatorp() ) message("error", str, previous_object());
                return 0;
    }
    Virt_Room=file;
    SetMasterMap( file );
    return 1;
}

/******************* virt_land.c optional settings ************/

string SetResetMessage(mixed str){ Reset_Message = str;}

/******************* virt_land.c attributes *******************/

void SetCoordinates(int x, int y, int z) {
    CoordX = x;
    CoordY = y;
    CoordZ = z;
}

int SetMedium(int medium){ return Medium = medium;}

mapping GetDirectionMap(){ 
    return (["north":({-1,0,0}),"south":({1,0,0}),
             "east":({0,1,0}),"west":({0,-1,0}),
             "northeast":({-1,1,0}),"northwest":({-1,-1,0}),
             "southeast":({1,1,0}),"southwest":({1,-1,0}),
             "up":({0,0,-1}),"down":({0,0,1}) ]);
}

string GetReverseDirection(string dir){ 
    return (["north":"south","south":"north",
             "east":"west","west":"east",
             "northeast":"southwest","northwest":"southeast",
             "southeast":"northwest","southwest":"northeast",
             "up":"down","down":"up" ])[dir];
}

varargs mixed GetMapType(int x, int y, int z){
    if(!objectp(Map_Master) || nullp(x) ||nullp(y) || nullp(z) ) 
        return 0;
    return Map_Master->GetAreaMap(x,y,z);
}

mapping SetSurroundAreas(int x, int y, int z){
    mapping surrounds = ([]);
    foreach(string dir, int * offset in GetDirectionMap())
        surrounds[dir] = GetMapType((x + offset[1]),
            (y + offset[0]),(z + offset[2]));
    return Surrounding_Areas=surrounds;
}

void ResetSurroundExits(){
    foreach(string dir, string atype in Surrounding_Areas){
            if(atype != "0" && atype != "@"){
                (Virt_Room + "/" 
                    + ( CoordX + GetDirectionMap()[dir][1]) + ","
                    + ( CoordY + GetDirectionMap()[dir][0]) + ","    
                    + ( CoordZ + GetDirectionMap()[dir][2]))->ResetLocation();
            }
    }
}

    
void SetVirtualExits(int x, int y, int z){
    if(sizeof(Virt_Room)){
        string * obexits=({});

        SetSurroundAreas(x,y,z);

        foreach(string dir, string atype in Surrounding_Areas){
            if(atype != "0" && atype != "@"){
                AddExit(dir,Virt_Room+"/"
                    + ( x + GetDirectionMap()[dir][1])+","
                    + ( y + GetDirectionMap()[dir][0])+ ","    
                    + ( z + GetDirectionMap()[dir][2]));
                obexits += ({dir[0..0]+dir[5..5]});
            }
            else if( atype == "@"){
                mixed exit = 
                  Map_Master->GetExternalRoom(
                    sprintf("%d,%d,%d",
                        ( x + GetDirectionMap()[dir][1]),
                        ( y + GetDirectionMap()[dir][0]),    
                        ( z + GetDirectionMap()[dir][2])),
                    sprintf("%d,%d,%d",x,y,z)
                  );
                if(exit){
                    AddExit(dir, exit);
                    obexits += ({dir[0..0]+dir[5..5]});
                }
            }
        }

        SetObviousExits(implode(obexits,", "));
        AreaType = GetMapType(x,y,z);
    }
}

string GetAreaType(){ return AreaType;}

varargs mixed GetCoordinates(int level) {
    if(nullp(level)) return ({ CoordX, CoordY, CoordZ });
    else switch(level){
        case 0: return CoordX;
        case 1: return CoordY;
        case 2: return CoordZ;
    }
    return 0;
}

object GetMasterMap(){ return Map_Master;}

int GetMedium() {
    return nullp(Medium)?MEDIUM_LAND:Medium;
}

varargs string array GetNonExits(string array extra){
    if(nullp(extra)) extra=({});
    return (keys(GetDirectionMap())-GetExits())- extra;
}

mapping GetSurroundAreas(){ return Surrounding_Areas; }

void PreAddEnter(mixed dir, string file){
    if((!stringp(dir)&&!arrayp(dir)) ||  !stringp(file)) return;
    AddEnter(dir,file);
}

/******************* virt_land.c events ***********************/

void ResetLocation(){
    object *objects = all_inventory(this_object());

    reset(query_reset_number());
    SetVirtualExits(CoordX,CoordY,CoordZ);
    SetLongAndItems(CoordX,CoordY,CoordZ);
    if(Reset_Message){
        string ret;
        eventPrint(Reset_Message);
        if(sizeof(objects=filter(objects,(:playerp:))))
            objects->eventDescribeEnvironment(0);
    }
}

/******************* virt_land.c driver applies ***************/

varargs static void create(int x, int y,int z) {
    SetNoReplace(1);
    room::create();
    SetCoordinates(x, y, z);
}
