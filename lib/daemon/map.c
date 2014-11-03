#include <lib.h>
#include <daemons.h>
#include <save.h>
#define FLOW_DEBUGGING 0
#ifndef MAP_CACHE
#define MAP_CACHE 1
#endif

inherit LIB_DAEMON;
mapping MapMap, MapCache;
static int heart_count = 1, sweepflag, roomcount, caching = 0;
static string SaveFile;
static string *sweeprooms = ({});

void create(){
#if WIZMAP
    SaveFile = save_file(SAVE_MAP);
    if(!file_exists(SaveFile) && file_exists(old_savename(SaveFile))){
        cp(old_savename(SaveFile), SaveFile);
    }
    if(file_exists(SaveFile)){
        RestoreObject(SaveFile, 1);
    }
    set_heart_beat(1);
#endif
}

void zero(){
    MapMap = ([]);
    MapCache = ([]);
    SaveObject(SaveFile, 1);
}

void heart_beat(){
    if(!(heart_count % 900)){ //Every 15 minutes save the cache
        SaveObject(SaveFile, 1);
    }
    if(!(heart_count % 64800)){ //Every 18 hours refresh the cache
        if(!sweepflag){
            sweepflag = 1;
            sweeprooms = keys(MapMap);
        }
    }
    if(sweepflag){
        if(sizeof(sweeprooms) <= roomcount){
            sweepflag = 0;
            roomcount = 0;
            return;
        }
        if(MapMap[sweeprooms[roomcount]]["name"]){
            this_object()->GetMap(MapMap[sweeprooms[roomcount]]["name"], 8, 1);
        }
        else {
            map_delete(MapMap, sweeprooms[roomcount]);
            map_delete(MapCache, sweeprooms[roomcount]);
        }
        roomcount++;
    }
    heart_count++;
}

int eventDestruct(){
    SaveObject(SaveFile, 1);
    return daemon::eventDestruct();
}

int RemoveCache(string str){
    if(MapCache && MapCache[str]){
        MapCache[str] = 0;
        return 1;
    }
    return 0;
}

int SetCaching(int x){
    if(x) caching = 1;
    else caching = 0;
    return caching;
}

int GetCaching(){
    return caching;
}

varargs mixed GetMap(mixed args, int size, int forced){
#if WIZMAP
    string ret = "";
    int i,x,line,tempy,tmpres, res = size;
    mapping Lines = ([]);
    mapping myspot;
    string mycoords;
    mapping start;
    if(!(MASTER_D->GetPerfOK())){
        return ret;
    }
    if(!size) size = 4;
    if(size > 15 && !creatorp(this_player())) size = 6;
    if(!args) args = base_name(environment(this_player()));
    if(objectp(args)) args = base_name(args);
    myspot=ROOMS_D->GetGridMap(args);
    if(!myspot) return 0;
    mycoords = myspot["coord"];
    res = size;
    if(!MapCache) MapCache = ([]);
    if(!myspot["coords"] || ( mycoords == "0,0,0" &&
                ROOMS_D->GetRoomZero() != args )){
        ret = "%^RED%^Map unavailable.%^RESET%^";
        return ret;
    }
    if(!forced && caching && MapCache[mycoords]){
        return MapCache[mycoords];
    }
    start = ([ "x" : myspot["coords"]["x"] - (res/2),
            "y" : myspot["coords"]["y"] + (res/2),
            "z" : myspot["coords"]["z"] ]);
    if(!MapMap) MapMap = ([]);
    if(sizeof(MapMap) >= 64000) MapMap = ([]);
    for(tempy = start["y"]; tempy > start["y"] - res; tempy--){
        mapping ppos = ([]);
        int xcount;
        line+=2;
        for(tmpres = start["x"];tmpres < start["x"] + res;tmpres++){
            string room, coord = tmpres+","+tempy+","+start["z"];
            mixed tmp = ROOMS_D->GetGrid(coord);
            xcount+=2;
            if(tmp){
                room = tmp["room"];
                MapMap[coord] = ([ "name" : room ]);
            }

            if(!Lines[line]){
                string pre = (tempy > -1 ? " " : "");
                Lines[line] = pre + (tempy % 10) + repeat_string(" ",60);
            }
            if(!Lines[line+1]){
                Lines[line+1] = " " + repeat_string(" ",60);
            }
            if(!Lines[line-1]){
                Lines[line-1] = " " + repeat_string(" ",60);
            }
            if(MapMap[coord]){
                mapping gridmap;
                mapping dirs;
                object croom;
                if(!MapMap[coord]["name"]) continue;
                gridmap = ROOMS_D->GetGridMap(MapMap[coord]["name"]);
                if(!sizeof(gridmap)) continue;
                if(gridmap["exits"]) dirs = gridmap["exits"];
                if(mycoords == coord){
                    Lines[line][xcount+2] = '@';
                }
                else {
#if FLOW_DEBUGGING
                    object flow;
                    croom = find_object(MapMap[coord]["name"]);
                    if(croom && flow = present("flow",croom)){
                        int press = flow->GetPressure();
                        if(press == 1) Lines[line][xcount+2] = '1';
                        if(press == 2) Lines[line][xcount+2] = '2';
                        if(press == 3) Lines[line][xcount+2] = '3';
                        if(press == 4) Lines[line][xcount+2] = '4';
                        if(press > 4) Lines[line][xcount+2] = '5';
                    }
                    else Lines[line][xcount+2] = '#';  
#else
                    Lines[line][xcount+2] = '#';
#endif
                }
                Lines[line][xcount+3] = ' ';  
                if(sizeof(dirs)){
                    foreach(string dir in keys(dirs)){
                        switch(dir){
                            case "north" : 
                                if(Lines[line-1])
                                    Lines[line-1][xcount+2] = '|';
                                break;
                            case "south" :
                                if(Lines[line+1])
                                    Lines[line+1][xcount+2] = '|';
                                break;
                            case "east" :
                                Lines[line][xcount+3] = '-';
                                break;
                            case "west" :
                                Lines[line][xcount+1] = '-';
                                break;
                            case "northeast" :
                                if(Lines[line-1]){
                                    if(Lines[line-1][xcount+3] == '\\')
                                        Lines[line-1][xcount+3] = 'X';
                                    else
                                        Lines[line-1][xcount+3] = '/';
                                }
                                break;
                            case "northwest" :
                                if(Lines[line-1]){
                                    if(Lines[line-1][xcount+1] == '/')
                                        Lines[line-1][xcount+1] = 'X';
                                    else
                                        Lines[line-1][xcount+1] = '\\';
                                }
                                break;
                            case "southeast" :
                                if(Lines[line+1]){
                                    if(Lines[line+1][xcount+3] == '/')
                                        Lines[line+1][xcount+3] = 'X';
                                    else
                                        Lines[line+1][xcount+3] = '\\';
                                }
                                break;
                            case "southwest" :
                                if(Lines[line+1]){
                                    if(Lines[line+1][xcount+1] == '\\')
                                        Lines[line+1][xcount+1] = 'X';
                                    else
                                        Lines[line+1][xcount+1] = '/';
                                }
                                break;
                        }
                    }
                }        
            }
            else {
            }
        }
        xcount = 0;
    }
    if(line == size * 2 ){
        int xcount = size;
        string pre;
        x = start["x"]-1; 
        Lines[line] = repeat_string(" ",60);
        Lines[line+1] = "   ";
        while(xcount){
            x++;
            if(x < 0) pre = "-";
            else pre = " ";
            Lines[line+1] += pre + abs((x % 10));
            xcount--;
        }
        Lines[line+2] = repeat_string("+2",30);
    }

    for(i = sizeof(Lines), x = 0;x < i;x++){
        if(Lines[x]) ret += Lines[x]+"\n";
    }
    ret = replace_string(ret,"@","%^RED%^@%^RESET%^");
#if FLOW_DEBUGGING
    ret = replace_string(ret,"1","%^GREEN%^A%^RESET%^");
    ret = replace_string(ret,"2","%^YELLOW%^B%^RESET%^");
    ret = replace_string(ret,"3","%^CYAN%^C%^RESET%^");
    ret = replace_string(ret,"4","%^BLUE%^D%^RESET%^");
    ret = replace_string(ret,"5","%^MAGENTA%^F%^RESET%^");
#endif
    MapCache[mycoords] = ret;
    return ret; 
#else
    /* Wizmapping not enabled in config.h */
    return "";
#endif
}
