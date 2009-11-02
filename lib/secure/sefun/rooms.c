#include <daemons.h>
#include <lib.h>
string namen, ret;

int reload_room(object ob){
    string name;
    object *stuff;
    name=base_name(ob);
    stuff = deep_inventory(ob);
    foreach(object item in stuff){
        object *int_inv = ({});
        int_inv = filter(containers(item), (: interactive($1) :));
        if(!sizeof(int_inv) && !interactive(item)) item->eventDestruct();
    }
    ob->eventDestruct();
    load_object(name);
    return 1;
}

int query_night_light(object ob){
    string file;
    string filename = base_name(ob)+".c";
    namen = filename;
    if(file_size(filename) < 1) return -1;
    file = unguarded( (: read_file(namen) :) );
    if(strsrch(file,"SetNightLight") != -1) return 1;
    else return 0;
}

int query_day_light(object ob){
    string filename = base_name(ob)+".c";
    if(file_size(filename) < 1) return -1;
    if(strsrch(read_file(filename),"SetDayLight") != -1) return 1;
    else return 0;
}

int query_ambient_light(object ob){
    string filename = base_name(ob)+".c";
    if(file_size(filename) < 1) return -1;
    if(strsrch(read_file(filename),"SetAmbientLight") != -1) return 1;
    else return 0;
}

varargs string opposite_dir(string str, int article){
    int classic;
    switch(str){
        case "north" : ret = "south";classic = 1;break;
        case "south" : ret = "north";classic = 1;break;
        case "east" : ret = "west";classic = 1;break;
        case "west" : ret = "east";classic = 1;break;
        case "northeast" : ret = "southwest";classic = 1;break;
        case "northwest" : ret = "southeast";classic = 1;break;
        case "southeast" : ret = "northwest";classic = 1;break;
        case "southwest" : ret = "northeast";classic = 1;break;
        case "up" : ret = (article ? "below" : "down");break;
        case "down" : ret = (article ? "above" : "up");break;

        case "fore" : ret = "aft";break;
        case "aft" : ret = "fore";break;
        case "port" : ret = "starboard";break;
        case "starboard" : ret = "port";break;

        case "+y" : ret = "-y";break;
        case "-y" : ret = "+y";break;
        case "+x" : ret = "-x";break;
        case "-x" : ret = "+x";break;
        case "+z" : ret = "-z";break;
        case "-z" : ret = "+z";break;

        case "+x+y" : ret = "-x-y";break;
        case "-x-y" : ret = "+x+y";break;
        case "+x-y" : ret = "-x+y";break;
        case "-x+y" : ret = "+x-y";break;

        case "+x+z" : ret = "-x-z";break;
        case "-x-z" : ret = "+x+z";break;
        case "+x-z" : ret = "-x+z";break;
        case "-x+z" : ret = "+x-z";break;

        case "+y+z" : ret = "-y-z";break;
        case "-y-z" : ret = "+y+z";break;
        case "+y-z" : ret = "-y+z";break;
        case "-y+z" : ret = "+y-z";break;


        case "+x+y-z" : ret = "-x-y+z";break;
        case "+x+y+z" : ret = "-x-y-z";break;
        case "-x-y-z" : ret = "+x+y+z";break;
        case "-x-y+z" : ret = "+x+y-z";break;
        case "+x-y-z" : ret = "-x+y+z";break;
        case "+x-y+z" : ret = "-x+y-z";break;
        case "-x+y-z" : ret = "+x-y+z";break;
        case "-x+y+z" : ret = "+x-y-z";break;

        default : ret = "";break;
    }
    if(article){
        if(classic) return "the "+ret;
    }
    return ret;
}

varargs mixed coordinates(object ob, string type){
    mixed ret;
    object env;
    if(!ob) return;
    if(!(ret = ROOMS_D->GetCoordinates(ob))){
        if(env = room_environment(ob)){
            ret = ROOMS_D->GetCoordinates(env);
        }
    }
    return ret;
}

mixed calculate_coordinates(string str, int x, int y, int z, string type){
    mixed ret, str_ret = ({});
    switch(str){
        case "north" : ret = ({ x, y+1, z });break;
        case "south" : ret = ({ x, y-1, z });break;
        case "east" : ret = ({ x+1, y, z });break;
        case "west" : ret = ({ x-1, y, z });break;
        case "northeast" : ret = ({ x+1, y+1, z });break;
        case "northwest" : ret = ({ x-1, y+1, z });break;
        case "southeast" : ret = ({ x+1, y-1, z });break;
        case "southwest" : ret = ({ x-1, y-1, z });break;
        case "up" : ret = ({ x, y, z+1 });break;
        case "down" : ret = ({ x, y, z-1 });break;
        default: ret = ({ x, y, z }); break;
    }
    if(type != "string") return ret;
    foreach(int element in ret){
        str_ret += ({ itoa(element) });
    }
    return implode(str_ret, ",");
}

object room_environment(object ob){
    foreach(object element in containers(ob)){
        if(inherits(LIB_ROOM, element)){
            return element;
        }
    }
    return 0;
}

varargs int bearing(int x1, int y1, int x2, int y2, int reverse){
    float inter = (reverse ? (y1 - y2) : (y2 - y1));
    float mark = (reverse ? (x1 - x2) : (x2 - x1));
    float range = sqrt(pow(mark, 2) + pow(inter, 2));
    int corr, ret;
    if(x1 == x2 && y1 == y2) return -1;
    corr = ((mark >= 0) ? 0 : 180);
    ret = (acos(to_float(corr ? -inter : inter)/range) * (180/3.141592)) + corr;
    return ret;
}

