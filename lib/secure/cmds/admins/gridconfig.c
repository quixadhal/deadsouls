#include <lib.h>
#include <daemons.h>
#include <rooms.h>

inherit LIB_DAEMON;

mixed findit(string str){
    string file, tmpfile;
    object room;
    int err;
    if(str == "here"){
        tmpfile = base_name(environment(this_player()));
        file = lpc_file(base_name(environment(this_player())));
    }
    else {
        file = lpc_file(str);
        tmpfile = str;
    }
    if(!file_exists(file) && str != "here"){
        file = lpc_file(this_player()->query_cwd()+"/"+str);
        if(!directory_exists(path_prefix(str))){
            tmpfile = this_player()->query_cwd()+"/"+str;
        }
    }
    if(file_exists(file)) return file;
    err = catch(room = load_object(tmpfile));
    if(room) return room;
    return 0;
}

static mixed ResetGrid(){
    object drone;
    object *drones = ({});
    string *starts = ({});
    string start;
    int err = 0;
    drones = findobs("/domains/default/npc/drone3");
    starts = ({ROOM_START+".c", "/domains/Ylsrim/room/tower.c",
            "/domains/Praxis/square.c", "/domains/campus/room/tunnel"});
    foreach(mixed arr in get_dir("/domains/", -1)){
        if(arr[1] == -2) start = "/domains/"+arr[0]+"/room/start.c";
        if(sizeof(start) && file_exists(start)) starts += ({ start });
        start = "";
    } 
    drones->eventDestruct();
    MAP_D->zero();
    ROOMS_D->zero();
    ROOMS_D->eventDestruct();
    MAP_D->eventDestruct();
    foreach(start in distinct_array(starts)){
        mixed lerr = 0;
        lerr = catch( reload(start, 0, 1));
        if(lerr){ 
            err++;
            write("Error reloading "+start+": "+lerr);
            continue;
        }
        drone = new("/domains/default/npc/drone3");
        lerr = catch( drone->eventMove(start) );
        if(lerr){ 
            err++;
            write("Error moving drone to "+start+": "+lerr);
        }
        else write("Assigning mapper drone to: "+start);
    }
    write("\nRemapping in progress. "+err+" errors encountered.");
    return 1;
}

mixed cmd(string args) {
    string s1, s2, s3, coord, cmd;
    mixed file;
    object room;
    int err;
    if(!this_player() || !archp(this_player())) return "lol";
    if(!args){
        write("Try: help gridconfig");
        return 1;
    }
    if(args == "reset"){
        return ResetGrid();
    }
    if(sscanf(args,"%s %s %s",s1,s2,s3) == 3){
        file = findit(s2);
        cmd = s1;
        coord = s3;
        if(!file){
            write("Room not found.");
            return 1;
        }
        if(objectp(file)) room = file;
        if(!room) err = catch(room = load_object(file));
        if(err || !room){
            write("Room not loadable.");
            return 1;
        }
        if(cmd != "set"){
            write("Try: help gridconfig");
            return 1;
        }
        if(sscanf(coord,"%s,%s,%s",s1,s2,s3) != 3){
            s3 = 0;
            if(sscanf(coord,"%s,%s",s1,s2) != 2){
                write("Type: help gridconfig");
                return 1;
            }
        }
        coord = coord + (s3 ? "" : ",0");
        ROOMS_D->SetRoom(room, this_player(), coord);
        write("Coordinates for "+file+" are: "+
                ROOMS_D->GetCoordinates(room));
        return 1;
    }
    if(sscanf(args,"%s %s",s1,s2) == 2){
        file = findit(s2);
        cmd = s1;
        if(!file){
            write("Room not found.");
            return 1;
        }
        if(objectp(file)) room = file;
        if(!room) err = catch(room = load_object(file));
        if(err || !room){
            write("Room not loadable.");
            return 1;
        }
        if(cmd != "unset"){
            write("Try: help gridconfig");
            return 1;
        }
        ROOMS_D->SetRoom(room, this_player(), "0,0,0");
        write("Coordinates for "+file+" are: "+
                ROOMS_D->GetCoordinates(room));
        return 1;
    }
    write("You fail to use this command properly.");
    return 1;
}

string GetHelp(string args) {
    return ("Syntax: gridconfig set <room> <coordinates>\n"
            "        gridconfig unset <room>\n"
            "        gridconfig reset\n\n"
            "Modifies the grid coordinate table in ROOMS_D for the "
            "specified room. If \"reset\" is the argument, the rooms "
            "daemon and map daemon are purged and slowly rebuilt using "
            "mapper drones. Note that this may lag your mud. Note also "
            "that the rooms daemon will disregard coordinate setting "
            "of rooms in /realms/ and /open/ .\n"
            "See also: mudconfig, admintool");
}
