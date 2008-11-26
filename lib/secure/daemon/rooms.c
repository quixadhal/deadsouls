#include <lib.h>
#include <daemons.h>
#include <rooms.h>
#include <save.h>
#define ROOM_ZERO "/domains/campus/room/start"

inherit LIB_DAEMON;

mapping Workrooms = ([]);

mapping WorldMap = ([]);
mapping WorldGrid = ([]);
static string last_exit;
static int global_manual;
int debugging;
static string *cards = ({ "north", "south", "east", "west",
  "northeast", "northwest", "southeast", "southwest",
  "up", "down" });
static string *unsafes = ({ "/realms/", "/open/", "/estates/" });

void create(){
    if(!WorldMap) WorldMap = ([]);
    if(!WorldGrid) WorldGrid = ([]);
    if( file_size( SAVE_ROOMS __SAVE_EXTENSION__ ) > 0 )
        unguarded( (: restore_object, SAVE_ROOMS, 1 :) );
    set_heart_beat(300);
}

void heart_beat(){
    unguarded( (: save_object(SAVE_ROOMS, 1) :) );
}

string GetRoomZero(){
    return ROOM_ZERO;
}

mixed GenerateNames(int x){
    int first, second, third;
    third = (x & 32767);
    second = (x & 536870911) - third;;
    first = ((x >> 29) << 29);
    //tc("first: "+first);
    //tc("second: "+second);
    //tc("third: "+third);
    //tc("total: "+(first+second+third));
    return ({ first, second, third });
}

int eventDestruct(){
    unguarded( (: save_object(SAVE_ROOMS, 1) :) );
    return daemon::eventDestruct();
}

int SetDebugging(int i){
    if(!this_player() || !adminp(this_player())) return 0;
    if(i) debugging = 1;
    else debugging = 0;
    return debugging;
}

mixed validate_last_room(string room, object player){
    string location_str, current_room_name;
    object location_ob, current_room;
    mapping origin_room = ([]);
    mapping origin_room_exits = ([]);
    mixed tmp, ret;
    if(debugging) tc("validate "+room+"?","blue");
    if(player) location_str=player->GetProperty("LastLocation");
    if(!location_str) return 0;
    if(location_str) location_ob=load_object(location_str);
    if(location_ob) tmp = this_object()->GetCoordinates(location_ob);
    if(tmp) origin_room = this_object()->GetGrid(tmp);
    current_room = room_environment(player);
    current_room_name = base_name(room_environment(player));
    if(debugging){
        tc("Last Location: "+location_str);
        tc("Last Location coords: "+ tmp);
        if(sizeof(tmp) > 1)
            tc("What those coordinates grid as: "+identify(origin_room));
    }

    if(room == ROOM_ZERO || (mapp(origin_room) &&
        origin_room["room"] && origin_room["room"] == location_str)){
        mapping exits = location_ob->GetExitMap();
        if(location_ob->GetFlyRoom()){
            exits["up"] = location_ob->GetFlyRoom();
        }
        foreach(mixed key, mixed val in exits){
            if(val == current_room_name){
                ret = ([ opposite_dir(key) : location_str ]);
            }
        }
        if(debugging) tc("validation passed","blue");
        if(!ret) ret = 1;
    }
    if(debugging) tc("validation failed","red");
    return ret;
}    

void zero(){
    WorldMap = ([]);
    WorldGrid = ([]);
    Workrooms = ([]);
    ROOM_ZERO->init();
}

varargs mapping GetGridMap(string str){
    mapping ret;
    string prefix, room;
    object ob;
    if(!sizeof(str)) return copy(WorldGrid);
    if(catch(ob = load_object(str))){
        return 0;
    }
    prefix = path_prefix(str);
    room = last_string_element(str, "/");
    if(ob) ret = this_object()->GetGrid(this_object()->GetCoordinates(ob));
    if(!ret) ret = ([]);
    //tc("ret: "+identify(ret),"blue");
    //tc("prefix: "+prefix);
    //tc("room: "+room);
    //tc("1: "+identify(WorldMap[prefix]));
    if(WorldMap[prefix] && WorldMap[prefix][room]){
        //tc("2: "+identify(WorldMap[prefix][room]),"red");
        ret += WorldMap[prefix][room];
    }
    return ret;
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
    string room; 
    mixed a, b, c, d, e, f, g, h, i, j, k, l, m, n;
    int p, q, x, y, z;
    mixed xarr, yarr, zarr;
    //string hashed_coord = crypt(coord, "xyz");

    if(this_player() && adminp(this_player())){
        room = arg_room;
    }
    else {
        room = base_name(previous_object());
    }

    if(!player->GetProperty("LastLocation")) return 0;
    if(debugging){
        tc("SetGrid received: "+room+", "+coord+", "+
          identify(player)+", "+unset,"blue");
    }
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
        if(debugging){
            tc("Unsetting: "+identify(WorldGrid[a][b][c][d][e][f][g][h][i]),"red");
        }
        WorldGrid[a][b][c][d][e][f][g][h][i] = 0;
        return 1;
    }
    if(sizeof(WorldGrid[a][b][c][d][e][f][g][h][i]) ||
      sizeof(this_object()->GetCoordinates(coord))){
        if(debugging) tc("SetGrid coord "+coord+
              " is already "+ identify(WorldGrid[a][b][c][d][e][f][g][h][i]));
        return WorldGrid[a][b][c][d][e][f][g][h][i];
    }
    else {
        if(global_manual || validate_last_room(room, player)){ 
            if(debugging) tc("Setting coord "+coord+
                  " as "+room, "green");
            MAP_D->RemoveCache(coord);
            global_manual = 0;
            WorldGrid[a][b][c][d][e][f][g][h][i] =
            ([ "room" : room, "coord" : coord ]);
        }
        return WorldGrid[a][b][c][d][e][f][g][h][i];
    }
}

mixed GetGrid(string str){
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
        if(debugging) tc("No joy for "+str,"red");
        return ([]);
    }
    if(debugging) tc("Joy! "+str+" is "+
          identify(WorldGrid[a][b][c][d][e][f][g][h][i]),"white");
    return copy(WorldGrid[a][b][c][d][e][f][g][h][i]);
}

int UnSetRoom(object arg_ob){
    string creator, name, prefix, room_name;
    string grid;
    object ob;

    if(this_player() && adminp(this_player())){
        ob = arg_ob;
    }
    else{
        ob = previous_object();
    }

    name = base_name(ob);
    grid = this_object()->GetCoordinates(ob);
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
}

varargs int SetRoom(object arg_ob, object player, string manual){
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

    //tc("ob: "+identify(ob));

    if(!(last_str = player->GetProperty("LastLocation"))){
        //tc("fail 0");
        return 0;
    }
    name = base_name(ob);
    prefix = path_prefix(name);

    /* Still need to figure out exclusions for
     * an island or continent perimeter :(
     */
    if(prefix == "/domains/town/virtual/surface"){
        return 0;
    }

    room_name = last_string_element(name, "/");
    if(!sizeof(WorldMap)){
        if(name == ROOM_FURNACE){
            return 0;
        }
    }
    if(!manual && WorldMap[prefix] && WorldMap[prefix][room_name]){
        //tc("hmm");
        if(StrCoord(WorldMap[prefix][room_name]["coords"]) == "0,0,0" &&
          name != ROOM_ZERO){
            //tc("HMM!");
            UnSetRoom(ob);
        }
        else{
            //tc("fail 1");
            return 0;
        }
    }
    sscanf(name,"/realms/%s/workroom",creator);
    foreach(string element in unsafes){
        if(!strsrch(last_str, element)){
            if(strsrch(name, element) && !creator)
                //tc("fail 2");
                return 0;
        }
    }
    if(!WorldMap[prefix]){
        if(debugging) tc("Creating WorldMap["+identify(prefix)+"]","yellow");
        WorldMap[prefix] = ([]);
    }
    if(!WorldMap[prefix][room_name]){
        if(debugging) tc("Creating WorldMap["+identify(prefix)+"]"+
              "["+identify(room_name)+"]","cyan");
        WorldMap[prefix][room_name] = ([ "date" : time(), 
          "coords" : ([ "x" : 0, "y" : 0, "z" : 0 ]),
          "exits" : ob->GetExitMap(),
        ]);
    }
    if(debugging){
    }
    if(creator){
        int x,y,z,next = sizeof(Workrooms);
        if(Workrooms[creator]){
            return 1;
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
            if(debugging) tc("Setting manually.","red");
            WorldMap[prefix][room_name]["coords"] =
            ([ "x" : x, "y" :y, "z" : z ]);
            global_manual = 1;
            SetGrid(name, manual, player);
            return 1;
        }
    }

    if(ob->GetVirtual() && WorldMap[prefix]) TmpMap = WorldMap[prefix];

    // && (!WorldMap[prefix][room_name] ||
    //!WorldMap[prefix][room_name]["date"])) TmpMap = WorldMap[prefix];

    if( !(backup_direction = validate_last_room(name, player)) ||
      sizeof(TmpMap)){ 
        int a,b,c;
        int a2,b2,c2;
        int x,y,z;
        int x2,y2,z2;
        int xd,yd,zd;
        //tc("sizeof TmpMap: "+sizeof(TmpMap));
        foreach(mixed key, mixed val in TmpMap){
            //tc("key: "+identify(key)+", cal: "+identify(val),"white");
            if(val){
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
        if(!undefinedp(a)){
            string roomname = last_string_element(base_name(ob),"/");
            if(sscanf(roomname,"%d,%d,%d",a2,b2,c2) != 3){
                if(sscanf(roomname,"%d,%d",a2,b2) != 2){
                    //tc("fail 3");
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
            //tc("I am "+roomname);
            //tc("I found "+a+","+b+","+c);
            //tc("which is "+x+","+y+","+z);
            //tc("different by "+xd+","+yd+","+zd);
            //tc("meaning I am "+x2+","+y2+","+z2);
            WorldMap[prefix][room_name] = ([]);
            WorldMap[prefix][room_name]["coords"] =
            ([ "x" : x2, "y" :y2, "z" : z2 ]);
            global_manual = 1;
            manual = x2+","+y2+","+z2;
            SetGrid(name, manual, player);
            return 1;
        }
        else {
            //tc(":(");
        }
        if(debugging) tc("Unknown origin. Not mapping.","red");
        if(WorldMap[prefix] && WorldMap[prefix][room_name] &&
          StrCoord(WorldMap[prefix][room_name]["coords"]) == "0,0,0"){
            WorldMap[prefix][room_name] = 0;
        }
        if(!creator) return 0;
    }
    tmpexits = WorldMap[prefix][room_name]["exits"];
    if(!sizeof(tmpexits) || member_array(player->GetProperty("LastLocation"),
        values(tmpexits)) == -1){
        if(debugging)
            tc("USING BACKUP DIRECTION: "+identify(backup_direction),"red");
        tmpexits = backup_direction;
    }
    else {
        if(debugging){
            tc("Using tmpexits from WorldMap[\""+prefix+"\"][\""+
              room_name+"\"] which is: "+identify(WorldMap[prefix][room_name]["exits"]),"white");
        }
    }
    if(!mapp(tmpexits)) tmpexits = ([]);
    if(debugging) tc("tmpexits: "+identify(tmpexits),"white");
    foreach(string key, mixed val in tmpexits){
        string sub_pre = path_prefix(val);
        string sub_name = last_string_element(val, "/");
        int breakout;
        if(member_array(key, cards) == -1) continue;
        if(debugging){
            tc("sub_pre: "+sub_pre);
            tc("sub_name: "+sub_name);
            if(WorldMap[sub_pre] && WorldMap[sub_pre][sub_name])
                tc("WorldMap["+identify(sub_pre)+"]["+identify(sub_name)+
                  "]: "+identify(WorldMap[sub_pre][sub_name]));
            tc("prefix: "+prefix,"yellow");
            tc("room_name: "+room_name,"yellow");
            if(WorldMap[prefix] && WorldMap[prefix][room_name])
                tc("WorldMap["+identify(prefix)+"]["+identify(room_name)+
                  "]: "+identify(WorldMap[prefix][room_name]),"yellow");

        }

        if(WorldMap[sub_pre] && WorldMap[sub_pre][sub_name]){
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
            }
        }
        if(breakout) break;
    }
    coord = WorldMap[prefix][room_name]["coords"]["x"]+","+
    WorldMap[prefix][room_name]["coords"]["y"]+","+
    WorldMap[prefix][room_name]["coords"]["z"];

    if(debugging){
        tc("SetRoom About to send: SetGrid("+identify(name)+", "+
          identify(coord)+", "+identify(player)+")");
    }
    SetGrid(name, coord, player);
    return 1;
}

string GetCoordinates(mixed ob){
    string name, prefix, room_name, ret;
    int err;
    if(clonep(ob)){
        object env = room_environment(ob);
        if(env && !clonep(env)) ob = env;
        else return "";
    }
    name = base_name(ob);
    prefix = path_prefix(name);
    room_name = last_string_element(name, "/");
    if(!WorldMap[prefix] || !WorldMap[prefix][room_name]){
        return "";
    }
    ret = WorldMap[prefix][room_name]["coords"]["x"]+","+
    WorldMap[prefix][room_name]["coords"]["y"]+","+
    WorldMap[prefix][room_name]["coords"]["z"];
    return ret;
} 

mapping GetCoordinateMap(mixed ob){
    string name = base_name(ob);
    string prefix = path_prefix(name);
    string room_name = last_string_element(name, "/");
    if(!WorldMap[prefix] || !WorldMap[prefix][room_name]){
        return ([]);
    }
    return ([ "x" : WorldMap[prefix][room_name]["coords"]["x"],
      "y" : WorldMap[prefix][room_name]["coords"]["y"],
      "z" : WorldMap[prefix][room_name]["coords"]["z"], ]);
}

mixed GetRoom(mixed ob){
    string name, prefix, room_name;
    if(clonep(ob)) return 0;
    name = base_name(ob);
    prefix = path_prefix(name);
    room_name = last_string_element(name, "/");
    if(!WorldMap[prefix]) return "No such prefix.";
    if(!WorldMap[prefix][room_name]) return "No such room.";
    return ([ name : copy(WorldMap[prefix][room_name]) ]); 
}
