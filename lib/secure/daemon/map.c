#include <lib.h>
#include <daemons.h>
#include <save.h>

inherit LIB_DAEMON;
mapping MapMap, MapCache;
int caching = 0;

void create(){
    if( file_size( SAVE_MAP __SAVE_EXTENSION__ ) > 0 )
        unguarded( (: restore_object, SAVE_MAP, 1 :) );
    set_heart_beat(300);
}

void zero(){
    MapMap = ([]);
    MapCache = ([]);
}

void heart_beat(){
    unguarded( (: save_object(SAVE_MAP, 1) :) );
}

int eventDestruct(){
    unguarded( (: save_object(SAVE_MAP, 1) :) );
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

varargs mixed GetMap(mixed args, int size) {
    string ret = "";
    int i,x,line,tempy,tmpres, res = 8;
    mapping Lines = ([]);
    mapping myspot;
    string mycoords;
    mapping start;
    if(!args) args = base_name(environment(this_player()));
    if(objectp(args)) args = base_name(args);
    myspot=ROOMS_D->GetGridMap(args);
    mycoords = myspot["coord"];
    if(size) res = size;
    if(!MapCache) MapCache = ([]);
    if(!myspot["coords"]){
        ret = "%^RED%^Map unavailable.%^RESET%^";
        return ret;
    }
    if(caching && MapCache[mycoords]){
        return MapCache[mycoords];
    }
    start = ([ "x" : myspot["coords"]["x"] - (res/2),
      "y" : myspot["coords"]["y"] + (res/2),
      "z" : myspot["coords"]["z"] ]);
if(!MapMap) MapMap = ([]);
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
            Lines[line] = line + repeat_string(" ",60);
        }
        if(!Lines[line+1]){
            Lines[line+1] = (line+1) + repeat_string(" ",60);
        }
        if(!Lines[line-1]){
            Lines[line-1] = (line-1) + repeat_string(" ",60);
        }

        if(MapMap[coord]){
            mapping gridmap;
            mapping dirs;
            if(!MapMap[coord]["name"]) continue;
            gridmap = ROOMS_D->GetGridMap(MapMap[coord]["name"]);
            if(!sizeof(gridmap)) continue;
            if(gridmap["exits"]) dirs = gridmap["exits"];
            if(mycoords == coord){
                Lines[line][xcount+2] = '@';
            }
            else {
                Lines[line][xcount+2] = '#';  
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
for(i = sizeof(Lines)-1, x = 0;x < i;x++){
    if(Lines[x]) ret += Lines[x]+"\n";
}
ret = replace_string(ret,"@","%^RED%^@%^RESET%^");
MapCache[mycoords] = ret;
return ret; 
}

