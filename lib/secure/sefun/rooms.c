string namen, ret;

int reload_room(object ob) {
    string name;
    object *stuff;
    name=base_name(ob);
    stuff = deep_inventory(ob);
    foreach(object item in stuff){
        if(!interactive(item) &&
          !interactive(environment(item)) &&
          !interactive(environment(environment(item))) )  
            item->eventDestruct();
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

string opposite_dir(string str){
    switch(str){
    case "north" : ret = "south";break;
    case "south" : ret = "north";break;
    case "east" : ret = "west";break;
    case "west" : ret = "east";break;
    case "northeast" : ret = "southwest";break;
    case "northwest" : ret = "southeast";break;
    case "southeast" : ret = "northwest";break;
    case "southwest" : ret = "northeast";break;
    case "up" : ret = "down";break;
    case "down" : ret = "up";break;
    default : ret = "";break;
    }
    return ret;
}
