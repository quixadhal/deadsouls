#include <lib.h>
#include <virtual.h>
#include <medium.h>
#include <message_class.h>
#include <position.h>

inherit LIB_VIRT_LAND;

varargs static void create(int x, int y,int z){
    virt_land::create( x,  y, z);
    SetMedium(MEDIUM_WATER);
}

varargs static void Setup(int x, int y,int z){
    virt_land::create( x,  y, z);
    SetMedium(MEDIUM_WATER);
}

string GetGround(){
    return GetExit("down");
}

string SetGround(string str){
    AddExit("down", str);
    return str;
}

mixed eventReceiveObject(object ob){

    return virt_land::eventReceiveObject(ob);
}
