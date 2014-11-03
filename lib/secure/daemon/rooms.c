#include <lib.h>
#include <daemons.h>
#include ROOMS_H
#include <save.h>
#ifndef ROOM_ZERO
#define ROOM_ZERO "/domains/town/room/start"
#endif

inherit LIB_DAEMON;

mapping Workrooms = ([]);

mapping WorldMap = ([]);
mapping WorldGrid = ([]);
static mapping DroneCache = ([]);
static string last_exit;
static int global_manual, cache_timer = time();
int debugging;
static string *cards = ({ "north", "south", "east", "west",
        "northeast", "northwest", "southeast", "southwest",
        "up", "down" });
static string void_room, SaveFile;
static string *unsafes = ({ "/realms/", "/open/", "/estates/" });

void create(){
    int err;
    object vvoid;
#ifdef __FLUFFOS__ && GRID
    SaveFile = save_file(SAVE_ROOMS);
    if(!file_exists(SaveFile) && file_exists(old_savename(SaveFile))){
        cp(old_savename(SaveFile), SaveFile);
    }
    if(!WorldMap) WorldMap = ([]);
    if(!WorldGrid) WorldGrid = ([]);
    if(file_exists(SaveFile)){
        RestoreObject(SaveFile, 1);
    }
    set_heart_beat(300);
#endif
#ifdef ROOM_VIRT_VOID
    err = catch(vvoid = load_object(ROOM_VIRT_VOID));
    if(!err && vvoid) void_room = ROOM_VIRT_VOID;
#endif
    if(undefinedp(void_room)) void_room = ROOM_VOID;
}

static void heart_beat(){
    SaveObject(SaveFile, 1);
}

string GetVoid(mixed ob){
    if(!ob) ob = previous_object();
    if(!ob) return ROOM_VOID;
    if(void_room == ROOM_VOID) return ROOM_VOID;
    if(stringp(ob)) ob = find_object(ob);
    if(interactive(ob)){
        return void_room+"/user_"+ob->GetKeyName();
    }
    if(clonep(ob)){
        return void_room+"/object_"+file_name(ob);
    }
    return ROOM_VOID;
}

string GetRoomZero(){
    return ROOM_ZERO;
}

mixed GenerateNames(int x){
#ifdef __FLUFFOS__ && GRID
    int first, second, third;
    third = (x & 32767);
    second = (x & 536870911) - third;;
    first = ((x >> 29) << 29);
    return ({ first, second, third });
#else
    return ({ 0, 0, 0 });
#endif
}

int eventDestruct(){
    SaveObject(SaveFile, 1);
    return daemon::eventDestruct();
}

int SetDebugging(int i){
    if(!this_player() || !adminp(this_player())) return 0;
    if(i) debugging = 1;
    else debugging = 0;
    return debugging;
}

mixed validate_last_room(string room, object player){
#ifdef __FLUFFOS__ && GRID
    string location_str, current_room_name;
    object location_ob, current_room;
    mapping origin_room = ([]);
    mapping origin_room_exits = ([]);
    mixed tmp, ret;
    if(player) location_str=player->GetProperty("LastLocation");
    if(!location_str) return 0;
    if(location_str) location_ob=load_object(location_str);
    if(location_ob) tmp = this_object()->GetCoordinates(location_ob);
    if(tmp) origin_room = this_object()->GetGrid(tmp);
    current_room = room_environment(player);
    current_room_name = base_name(room_environment(player));

    if(room == ROOM_ZERO || (mapp(origin_room) &&
                origin_room["room"] && origin_room["room"] == location_str)){
        mapping exits = location_ob->GetExitMap();
        if(location_ob->GetFlyRoom()){
            exits["up"] = location_ob->GetFlyRoom();
        }
        location_ob->DestructEmptyVirtual();
        foreach(mixed key, mixed val in exits){
            if(val == current_room_name){
                ret = ([ opposite_dir(key) : location_str ]);
                break;
            }
        }
        if(ret){
            return ret;
        }
    }
    return 0;
#else
    return 0;
#endif
}    

void zero(){
    WorldMap = ([]);
    WorldGrid = ([]);
    Workrooms = ([]);
    SaveObject(SaveFile, 1);
    ROOM_ZERO->init();
}

varargs mapping GetGridMap(string str){
#ifdef __FLUFFOS__ && GRID
    mapping ret;
    string prefix, room, coords;
    object ob;
    int err;
    if(!sizeof(str)) return copy(WorldGrid);
    err = catch(ob = load_object(str));
    if(err || !ob){
        this_object()->UnSetRoom(str, 1);
        return 0;
    }
    else {
    }
    prefix = path_prefix(str);
    room = last_string_element(str, "/");
    if(ob){
        coords = this_object()->GetCoordinates(ob);
        ret = this_object()->GetGrid(coords);
    }
    if(!ret) ret = ([]);
    if(WorldMap[prefix] && WorldMap[prefix][room]){
        ret += WorldMap[prefix][room];
    }
    ob->DestructEmptyVirtual();
    return ret;
#else
    return ([]);
#endif
}

varargs mapping GetWorldMap(string str){
    return copy(WorldMap);
}

string StrCoord(mapping TmpMap){
    string ret = TmpMap["x"] + ",";
    ret += TmpMap["y"] + ",";
    ret += TmpMap["z"];
    return ret;
}

varargs mixed SetGrid(string arg_room, string coord, object player, int unset){
#ifdef __FLUFFOS__ && GRID
    string room; 
    mixed a, b, c, d, e, f, g, h, i, j, k, l, m, n;
    int p, q, x, y, z;
    mixed xarr, yarr, zarr;

    if(this_player() && adminp(this_player())){
        room = arg_room;
    }
    else {
        room = base_name(previous_object());
    }

    if(!player) player = previous_object();

    if(!player->GetProperty("LastLocation") && 
            base_name(player) != room) return 0;

    if(inherits(LIB_ROOM, previous_object())) global_manual = 1;

    sscanf(coord,"%d,%d,%d",x,y,z);
    xarr = GenerateNames(x);
    yarr = GenerateNames(y);
    zarr = GenerateNames(z);
    a = xarr[0];
    b = xarr[1];
    c = xarr[2];
    d = yarr[0];
    e = yarr[1];
    f = yarr[2];
    g = zarr[0];
    h = zarr[1];
    i = zarr[2];
    if(!WorldGrid[a]) WorldGrid[a] = ([]);
    if(!WorldGrid[a][b]) WorldGrid[a][b] = ([]);
    if(!WorldGrid[a][b][c]) WorldGrid[a][b][c] = ([]);
    if(!WorldGrid[a][b][c][d]) WorldGrid[a][b][c][d] = ([]);
    if(!WorldGrid[a][b][c][d][e]) WorldGrid[a][b][c][d][e] = ([]);
    if(!WorldGrid[a][b][c][d][e][f]) WorldGrid[a][b][c][d][e][f] = ([]);
    if(!WorldGrid[a][b][c][d][e][f][g]) WorldGrid[a][b][c][d][e][f][g] = ([]);
    if(!WorldGrid[a][b][c][d][e][f][g][h]) WorldGrid[a][b][c][d][e][f][g][h] = ([]);
    if(!WorldGrid[a][b][c][d][e][f][g][h][i]) WorldGrid[a][b][c][d][e][f][g][h][i] = ([]);
    if(unset && sizeof(WorldGrid[a][b][c][d][e][f][g][h][i])){
        WorldGrid[a][b][c][d][e][f][g][h][i] = 0;
        return 1;
    }
    if(sizeof(WorldGrid[a][b][c][d][e][f][g][h][i]) ||
            sizeof(this_object()->GetCoordinates(coord))){
        return WorldGrid[a][b][c][d][e][f][g][h][i];
    }
    else {
        if(global_manual || validate_last_room(room, player)){ 
            MAP_D->RemoveCache(coord);
            global_manual = 0;
            WorldGrid[a][b][c][d][e][f][g][h][i] =
                ([ "room" : room, "coord" : coord ]);
        }
        return WorldGrid[a][b][c][d][e][f][g][h][i];
    }
#else
    return 0;
#endif
}

mixed GetGrid(string str){
#ifdef __FLUFFOS__ && GRID
    mixed room, a, b, c, d, e, f, g, h, i, j, k, l, m, n;
    string coord=str;
    int p, q,x,y,z;
    mixed xarr, yarr, zarr;
    sscanf(coord,"%d,%d,%d",x,y,z);
    xarr = GenerateNames(x);
    yarr = GenerateNames(y);
    zarr = GenerateNames(z);
    a = xarr[0];
    b = xarr[1];
    c = xarr[2];
    d = yarr[0];
    e = yarr[1];
    f = yarr[2];
    g = zarr[0];
    h = zarr[1];
    i = zarr[2];
    if(!WorldGrid[a] ||
            !WorldGrid[a][b] ||
            !WorldGrid[a][b][c] ||
            !WorldGrid[a][b][c][d] ||
            !WorldGrid[a][b][c][d][e] ||
            !WorldGrid[a][b][c][d][e][f] ||
            !WorldGrid[a][b][c][d][e][f][g] ||
            !WorldGrid[a][b][c][d][e][f][g][h] ||
            !WorldGrid[a][b][c][d][e][f][g][h][i]){
        return ([]);
    }
    return copy(WorldGrid[a][b][c][d][e][f][g][h][i]);
#else
    return ([]);
#endif
}

varargs int UnSetRoom(mixed arg_ob, int auto){
#ifdef __FLUFFOS__ && GRID
    string creator, name, prefix, room_name;
    string grid;
    mixed ob;

    if(auto || (this_player() && adminp(this_player()))){
        ob = arg_ob;
    }
    else{
        ob = previous_object();
    }

    if(objectp(ob)){
        name = base_name(ob);
    }
    else {
        name = ob;
    }
    grid = this_object()->GetCoordinates(name);

    if(grid && grid != "0,0,0"){
        MAP_D->RemoveCache(grid);
        SetGrid(name, grid, this_player(), 1);
    }
    prefix = path_prefix(name);
    room_name = last_string_element(name, "/");
    if(WorldMap[prefix] && WorldMap[prefix][room_name]){
        WorldMap[prefix][room_name] = 0;
    }
    if(sscanf(name,"/realms/%s/workroom",creator)){
        Workrooms[creator] = 0;
    }
    return 1;
#else
    return 0;
#endif
}

varargs mixed SetRoom(object arg_ob, object player, string manual){
#ifdef __FLUFFOS__ && GRID
    string last_str, creator, name, prefix, room_name, coord;
    mapping tmpexits, backup_direction, TmpMap = ([]);
    object ob;

    if(this_player() && adminp(this_player())){
        ob = arg_ob;
    }
    else{
        ob = previous_object();
    }
    if(clonep(ob)){
        return 0;
    }
    if(!(last_str = player->GetProperty("LastLocation")) &&
            player != ob){
        return 0;
    }
    if(!(last_str)) last_str = "";
    name = base_name(ob);
    prefix = path_prefix(name);
    if(prefix == "/domains/default/virtual/void") return 0;
    if(manual && (!strsrch(name, "/open/") || !strsrch(name, "/realms/"))){
        if(!this_player() || !archp(this_player())){
            return 0;
        }
    }
    /* Still need to figure out exclusions for
     * an island or continent perimeter :(
     */
    //if(prefix == "/domains/town/virtual/surface" ||
    //        prefix == "/domains/town/virtual/sub"){
    //    return 0;
    //}
    room_name = last_string_element(name, "/");
    //tc("last_str: "+last_str);
    //tc("room_name: "+room_name);
    //tc("name: "+name);
    if(!sizeof(WorldMap)){
        if(name == ROOM_FURNACE){
            return 0;
        }
    }
    if(!manual && WorldMap[prefix] && WorldMap[prefix][room_name]){
        if(StrCoord(WorldMap[prefix][room_name]["coords"]) == "0,0,0" &&
                name != ROOM_ZERO){
            UnSetRoom(ob);
            //tc("alpha");
        }
        else{
            //tc("omega");
            return ({ WorldMap[prefix][room_name]["coords"]["x"],
                    WorldMap[prefix][room_name]["coords"]["y"],
                    WorldMap[prefix][room_name]["coords"]["z"] });
        }
    }
    sscanf(name,"/realms/%s/workroom",creator);
    foreach(string element in unsafes){
        if(!strsrch(last_str, element)){
            if(strsrch(name, element) && !creator)
                return 0;
        }
    }
    if(!WorldMap[prefix]){
        WorldMap[prefix] = ([]);
    }
    if(!WorldMap[prefix][room_name]){
        WorldMap[prefix][room_name] = ([ "date" : time(), 
                "coords" : ([ "x" : 0, "y" : 0, "z" : 0 ]),
                "exits" : ob->GetExitMap(),
                ]);
    }
    WorldMap[prefix][room_name]["exits"]["sink"] = ob->GetSinkRoom();
    WorldMap[prefix][room_name]["exits"]["fly"] = ob->GetFlyRoom();
    if(debugging){
    }
    if(creator){
        int x,y,z,next = sizeof(Workrooms);
        if(Workrooms[creator]){
            //tc("beta");
            return ({ Workrooms[creator]["coords"]["x"],
                    Workrooms[creator]["coords"]["y"],
                    Workrooms[creator]["coords"]["z"] });
        }
        x = 1000000000;
        y = 1000000000;
        z = -1000000000;
        x += ( next * 10000 );
        manual = x+","+y+","+z;
        Workrooms[creator] = ([ "grid" : manual, 
                "coords" : ([ "x" : x, "y" :y, "z" : z ]),
                "time" : time(), "number" : next ]); 
    }
    if(manual){
        int res, x, y, z;
        res = sscanf(manual,"%d,%d,%d",x,y,z);
        if(res == 3){
            WorldMap[prefix][room_name]["coords"] =
                ([ "x" : x, "y" :y, "z" : z ]);
            global_manual = 1;
            SetGrid(name, manual, player);
            //tc("gamma");
            return ({ x, y, z });
        }
    }

    //if(ob->GetVirtual() && sizeof(WorldMap[prefix])){
    //    TmpMap = WorldMap[prefix];
    //}

    tmpexits = WorldMap[prefix][room_name]["exits"];
    if(!sizeof(tmpexits) || member_array(player->GetProperty("LastLocation"),
                values(tmpexits)) == -1){
        tmpexits = backup_direction;
    }
    if(!mapp(tmpexits)) tmpexits = ([]);

    /* Now we try to guess our coordinate from the
     * set coordinate of an adjacent room.
     */
    foreach(string key, mixed val in tmpexits){
        string sub_pre = path_prefix(val);
        string sub_name = last_string_element(val, "/");
        int breakout;
        if(member_array(key, cards) == -1) continue;

        if(WorldMap[sub_pre] && WorldMap[sub_pre][sub_name]){
            mapping cmap = WorldMap[sub_pre][sub_name]["coords"];
            if(val != ROOM_ZERO && cmap["x"] == 0 && cmap["y"] == 0 &&
                    cmap["z"] == 0 ){
                continue;
            }
            last_exit = key;
            switch(key){
                case "north" : WorldMap[prefix][room_name]["coords"]["x"] =
                               WorldMap[sub_pre][sub_name]["coords"]["x"];
                               WorldMap[prefix][room_name]["coords"]["y"] =
                                   WorldMap[sub_pre][sub_name]["coords"]["y"] - 1;
                               WorldMap[prefix][room_name]["coords"]["z"] =
                                   WorldMap[sub_pre][sub_name]["coords"]["z"];
                               breakout = 1;
                               break;
                case "south" : WorldMap[prefix][room_name]["coords"]["x"] =
                               WorldMap[sub_pre][sub_name]["coords"]["x"];
                               WorldMap[prefix][room_name]["coords"]["y"] =
                                   WorldMap[sub_pre][sub_name]["coords"]["y"] + 1;
                               WorldMap[prefix][room_name]["coords"]["z"] =
                                   WorldMap[sub_pre][sub_name]["coords"]["z"];
                               breakout = 1;
                               break;
                case "east" : WorldMap[prefix][room_name]["coords"]["x"] =
                              WorldMap[sub_pre][sub_name]["coords"]["x"] - 1;
                              WorldMap[prefix][room_name]["coords"]["y"] =
                                  WorldMap[sub_pre][sub_name]["coords"]["y"];
                              WorldMap[prefix][room_name]["coords"]["z"] =
                                  WorldMap[sub_pre][sub_name]["coords"]["z"];
                              breakout = 1;
                              break;
                case "west" : WorldMap[prefix][room_name]["coords"]["x"] =
                              WorldMap[sub_pre][sub_name]["coords"]["x"] + 1;
                              WorldMap[prefix][room_name]["coords"]["y"] =
                                  WorldMap[sub_pre][sub_name]["coords"]["y"];
                              WorldMap[prefix][room_name]["coords"]["z"] =
                                  WorldMap[sub_pre][sub_name]["coords"]["z"];
                              breakout = 1;
                              break;
                case "northeast" : WorldMap[prefix][room_name]["coords"]["x"] =
                                   WorldMap[sub_pre][sub_name]["coords"]["x"] - 1;
                                   WorldMap[prefix][room_name]["coords"]["y"] =
                                       WorldMap[sub_pre][sub_name]["coords"]["y"] - 1;
                                   WorldMap[prefix][room_name]["coords"]["z"] =
                                       WorldMap[sub_pre][sub_name]["coords"]["z"];
                                   breakout = 1;
                                   break;
                case "northwest" : WorldMap[prefix][room_name]["coords"]["x"] =
                                   WorldMap[sub_pre][sub_name]["coords"]["x"] + 1;
                                   WorldMap[prefix][room_name]["coords"]["y"] =
                                       WorldMap[sub_pre][sub_name]["coords"]["y"] - 1;
                                   WorldMap[prefix][room_name]["coords"]["z"] =
                                       WorldMap[sub_pre][sub_name]["coords"]["z"];
                                   breakout = 1;
                                   break;
                case "southeast" : WorldMap[prefix][room_name]["coords"]["x"] =
                                   WorldMap[sub_pre][sub_name]["coords"]["x"] - 1;
                                   WorldMap[prefix][room_name]["coords"]["y"] =
                                       WorldMap[sub_pre][sub_name]["coords"]["y"] + 1;
                                   WorldMap[prefix][room_name]["coords"]["z"] =
                                       WorldMap[sub_pre][sub_name]["coords"]["z"];
                                   breakout = 1;
                                   break;
                case "southwest" : WorldMap[prefix][room_name]["coords"]["x"] =
                                   WorldMap[sub_pre][sub_name]["coords"]["x"] + 1;
                                   WorldMap[prefix][room_name]["coords"]["y"] =
                                       WorldMap[sub_pre][sub_name]["coords"]["y"] + 1;
                                   WorldMap[prefix][room_name]["coords"]["z"] =
                                       WorldMap[sub_pre][sub_name]["coords"]["z"];
                                   breakout = 1;
                                   break;
                case "up" : WorldMap[prefix][room_name]["coords"]["x"] =
                            WorldMap[sub_pre][sub_name]["coords"]["x"];
                            WorldMap[prefix][room_name]["coords"]["y"] =
                                WorldMap[sub_pre][sub_name]["coords"]["y"];
                            WorldMap[prefix][room_name]["coords"]["z"] =
                                WorldMap[sub_pre][sub_name]["coords"]["z"] - 1;
                            breakout = 1;
                            break;
                case "down" : WorldMap[prefix][room_name]["coords"]["x"] =
                              WorldMap[sub_pre][sub_name]["coords"]["x"];
                              WorldMap[prefix][room_name]["coords"]["y"] =
                                  WorldMap[sub_pre][sub_name]["coords"]["y"];
                              WorldMap[prefix][room_name]["coords"]["z"] =
                                  WorldMap[sub_pre][sub_name]["coords"]["z"] + 1;
                              breakout = 1;
                              break;
                case "fly" : WorldMap[prefix][room_name]["coords"]["x"] =
                             WorldMap[sub_pre][sub_name]["coords"]["x"];
                             WorldMap[prefix][room_name]["coords"]["y"] =
                                 WorldMap[sub_pre][sub_name]["coords"]["y"];
                             WorldMap[prefix][room_name]["coords"]["z"] =
                                 WorldMap[sub_pre][sub_name]["coords"]["z"] - 1;
                             breakout = 1;
                             break;
                case "sink" : WorldMap[prefix][room_name]["coords"]["x"] =
                              WorldMap[sub_pre][sub_name]["coords"]["x"];
                              WorldMap[prefix][room_name]["coords"]["y"] =
                                  WorldMap[sub_pre][sub_name]["coords"]["y"];
                              WorldMap[prefix][room_name]["coords"]["z"] =
                                  WorldMap[sub_pre][sub_name]["coords"]["z"] + 1;
                              breakout = 1;
                              break;
            }
        }
        if(breakout) break;
    }
    coord = WorldMap[prefix][room_name]["coords"]["x"]+","+
        WorldMap[prefix][room_name]["coords"]["y"]+","+
        WorldMap[prefix][room_name]["coords"]["z"];

    /* Here we try to extrapolate a virtual's coords from a known
     * coord in the same virt directory.
     */
    if(coord == "0,0,0" && name != ROOM_ZERO && sizeof(TmpMap)){
        int a,b,c;
        int a2,b2,c2;
        int x,y,z;
        int x2,y2,z2;
        int xd,yd,zd;
        //tc("hmm","red");
        foreach(mixed key, mixed val in TmpMap){
            if(val){
                if(!val["date"]) continue;
                if(sscanf(key,"%d,%d,%d",a,b,c) != 3){
                    if(sscanf(key,"%d,%d",a,b) != 2) continue;
                    else c = 0;
                }
                if(!val["coords"]) continue;
                else {
                    x = val["coords"]["x"];
                    y = val["coords"]["y"];
                    z = val["coords"]["z"];
                }
                if(!x && !y && !z){
                    map_delete(TmpMap,key);
                    continue;
                }
                else break;
            }
            else map_delete(TmpMap,key);
        }
        if(!x && !y && !z){
            return 0;
        }
        if(!undefinedp(a)){
            string roomname = last_string_element(base_name(ob),"/");
            if(sscanf(roomname,"%d,%d,%d",a2,b2,c2) != 3){
                if(sscanf(roomname,"%d,%d",a2,b2) != 2){
                    return 0;
                }
                else c2 = 0;
            }
            xd = a - a2;
            yd = b - b2;
            zd = c - c2;
            x2 = x - xd;
            y2 = y - yd;
            z2 = z - zd;
            WorldMap[prefix][room_name] = ([]);
            WorldMap[prefix][room_name]["coords"] =
                ([ "x" : x2, "y" :y2, "z" : z2 ]);
            global_manual = 1;
            manual = x2+","+y2+","+z2;
            SetGrid(name, manual, player);
            //tc("delta");
            return ({ x2, y2, z2 });
        }
        else {
        }
        if(WorldMap[prefix] && WorldMap[prefix][room_name] &&
                StrCoord(WorldMap[prefix][room_name]["coords"]) == "0,0,0"){
            WorldMap[prefix][room_name] = 0;
        }
        if(!creator) return 0;
    }
    /* end virt extrapolation */

    if(coord == "0,0,0" && name != ROOM_ZERO){
        true();
    }
    else SetGrid(name, coord, player);
    //tc("epsilon");
    return ({ WorldMap[prefix][room_name]["coords"]["x"],
            WorldMap[prefix][room_name]["coords"]["y"],
            WorldMap[prefix][room_name]["coords"]["z"] });
#else
    //tc("phi");
    return ({ 0, 0, 0 });
#endif
}

string GetCoordinates(mixed ob){
#ifdef __FLUFFOS__ && GRID
    string name, prefix, room_name, ret;
    int err;
    if(objectp(ob)){
        if(clonep(ob)){
            object env = room_environment(ob);
            if(env && !clonep(env)) ob = env;
            else return "";
        }
        name = base_name(ob);
    }
    else if(stringp(ob)){
        if(last(ob,2) == ".c") name = truncate(ob,2);
        else name = ob;
    }
    else {
        return "";
    }
    prefix = path_prefix(name);
    room_name = last_string_element(name, "/");
    if(!WorldMap[prefix] || !WorldMap[prefix][room_name]){
        return "";
    }
    ret = WorldMap[prefix][room_name]["coords"]["x"]+","+
        WorldMap[prefix][room_name]["coords"]["y"]+","+
        WorldMap[prefix][room_name]["coords"]["z"];
    return ret;
#else
    return "";
#endif
} 

mapping GetCoordinateMap(mixed ob){
#ifdef __FLUFFOS__ && GRID
    string name = base_name(ob);
    string prefix = path_prefix(name);
    string room_name = last_string_element(name, "/");
    if(!WorldMap[prefix] || !WorldMap[prefix][room_name]){
        return ([]);
    }
    return ([ "x" : WorldMap[prefix][room_name]["coords"]["x"],
            "y" : WorldMap[prefix][room_name]["coords"]["y"],
            "z" : WorldMap[prefix][room_name]["coords"]["z"], ]);
#else
    return ([]);
#endif
}

mixed GetRoom(mixed ob){
#ifdef __FLUFFOS__ && GRID
    string name, prefix, room_name;
    if(objectp(ob)){
        if(clonep(ob)) return 0;
        name = base_name(ob);
    }
    else if(stringp(ob)) name = ob;
    if(!name) return 0;
    prefix = path_prefix(name);
    room_name = last_string_element(name, "/");
    if(!WorldMap[prefix]) return "No such prefix.";
    if(!WorldMap[prefix][room_name]) return "No such room.";
    return copy(WorldMap[prefix][room_name]); 
#else
    return "Grid is not enabled.";
#endif
}

varargs mixed GetDirectionRoom(mixed origin, string direction, int noclip){
#ifdef __FLUFFOS__ && GRID
    int x, y, z;
    string dir_coords;
    mixed ret, room = GetRoom(origin);
    if(!room || !sizeof(room) || !mapp(room)) return 0;
    if(sizeof(room["exits"]) && room["exits"][direction]){
        return room["exits"][direction];
    }
    if(!noclip || !room["coords"]) return 0;
    x = room["coords"]["x"];
    y = room["coords"]["y"];
    z = room["coords"]["z"];
    switch(direction){
        case "up" : z++;break;
        case "down" : z--;break;
        case "north" : y++;break;
        case "south" : y--;break;
        case "east" : x++;break;
        case "west" : x--;break;
        case "northeast" : y++;x++;break;
        case "northwest" : y++;x--;break;
        case "southwest" : y--;x--;break;
        case "southeast" : y--;x++;break;
        default : return 0;
    }
    dir_coords = x+","+y+","+z;
    ret = GetGrid(dir_coords);
    if(sizeof(ret) && ret["room"]) return ret["room"];
    return 0;
#else 
    return 0;
#endif
}

mixed DroneCache(mixed foo){
    string path;
    object prev, env;
    prev = previous_object();
    if(!prev) return 0;
    env = environment(prev);
    if(!env) return 0;
    path = path_prefix(base_name(env));
    if(time() - cache_timer > 60 || !DroneCache) DroneCache = ([]);
    cache_timer = time();
    if(!DroneCache[path]) DroneCache[path] = ({});
    if(foo) DroneCache[path] = distinct_array(DroneCache[path] + foo );
    else return copy(DroneCache[path]);
}
