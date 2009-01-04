#include <lib.h>
#include <modules.h>
#include <commands.h>

string tmpfile, contents, globalstr, globalstr1, globalstr2, globalstr3;
string my_room_file, other_room_file, tmpfile;
string my_room_contents, other_room_contents;
string *global_array;

varargs int eventReceiveId(object door, string *id, string cote){
    string ret_id = implode(id,":");
    if(!cote) cote = this_object()->eventEvaluateDoorSide(door);
    this_object()->eventProcessDoor(door, "SetId", ret_id, cote);
    return 1;
} 

mapping GetDoorKeys(string str){
    string dir, junk1, junk2;
    string *lines;
    mapping KeyMap = ([]);

    globalstr = str;
    unguarded( (: global_array = this_object()->eventReadLines(read_file(globalstr)) :) );
    lines = global_array;
    if(!sizeof(lines)) return ([]);
    foreach(string line in lines){
        if(grepp(line, "SetKeys")){
            sscanf(line,"%sKeys(%s,%s",junk1, dir, junk2);
                    dir = this_object()->eventCleanString(dir);
                    KeyMap[dir] = line;
                    }
                    }

                    return copy(KeyMap);
                    }

                    varargs mixed eventChangeDoor(mixed door, string property, mixed value, string cote){
                    object porte;
                    string *sides, *props;
                    string map_string = "";
                    int lockable;
                    mapping KeyMap = this_object()->GetDoorKeys(door);

                    if(stringp(door)) porte = load_object(door);
                    else if(objectp(door)) porte = door;

                    if(!porte) return 0;

                    sides = porte->GetSides();

                    foreach(string side in sides){
                        mapping TmpMap1 = ([]);
                        TmpMap1 = porte->GetSide(side);
                        props = keys(TmpMap1);

                        switch(property){
                            case "SetId" : property = "id";break;
                            case "SetLong" : property = "long";break;
                            case "SetShort" : property = "short";break;
                            case "SetCanLock" : property = "lockable";break;
                            case "SetKey" : property = "key";break;
                        }
                        if(member_array(property, props) != -1 ){
                            if(!cote) TmpMap1[property] = value;
                            else if(cote && cote == side && property == "id")
                                TmpMap1[property] = explode(value,":");
                            else if(cote && cote == side) TmpMap1[property] = value;
                        }

                        if(!TmpMap1["lockable"]) lockable = 0;
                        else lockable = TmpMap1["lockable"];
                        map_string += "SetSide(\""+side+"\", ([\"id\" : "+identify(TmpMap1["id"])+",\n";
                        map_string += "\"short\" : \""+TmpMap1["short"]+"\",\n";
                        map_string += "\"long\" : \""+TmpMap1["long"]+"\",\n";
                        map_string +="\"lockable\" : "+lockable+" ]) );\n";
                        if(property == "key") {
                            if(!cote || cote == side) map_string += "SetKeys(\""+side+"\", ({\""+value+"\"}) );\n\n";
                            else if(KeyMap[side])  map_string += KeyMap[side] + "\n\n";
                        }
                        if(property != "key" && KeyMap[side]){
                            map_string += KeyMap[side] + "\n\n";
                        }
                    }

                    return map_string;
                    }

varargs int eventProcessDoor(mixed door, string property, mixed value, string cote){
    object porte;
    int closed, locked;
    string other_room;

    if(!door || ! property ){
        write("Wrong number of arguments to eventProcessDoor");
        return 0;
    }

    if(!value) value = 0;

    if(objectp(door)) door = base_name(door);
    if(last(door,2) != ".c") door += ".c";
    if(!file_exists(door)){
        write("No such door file.");
        return 0;
    }

    porte = load_object(door);
    tmpfile = generate_tmp(porte);

    if(property == "closed" || property == "SetClosed") closed = value;
    else closed = this_object()->eventReadValue(door, "SetClosed");
    if(property == "locked" || property == "SetLocked") locked = value;
    else locked = this_object()->eventReadValue(door, "SetLocked");;

    contents = "#include <lib.h>\n\n";
    contents += "inherit LIB_DOOR;\n\n";
    contents += "static void create() {\n";
    contents += "door::create();\n\n";
    if(cote) contents += this_object()->eventChangeDoor(door, property, value, cote)+"\n";
    else contents += this_object()->eventChangeDoor(door, property, value)+"\n";
    contents += "SetClosed("+closed+");\n";
    contents += "SetLocked("+locked+");\n";
    contents += "}\n";
    unguarded( (: write_file(tmpfile,contents,1) :) );
    this_object()->eventGeneralStuff(tmpfile);
    globalstr2 = door;
    unguarded( (: cp(tmpfile, globalstr2) :) );
    unguarded( (: rm(tmpfile) :) );
    reload(door);
    reload(environment(this_player()));
    other_room = this_object()->GetOtherRoom(door);
    if(other_room) reload(other_room);
    return 1;
}

int eventDeleteDoor(string door){
    string this_room, other_room;
    this_room = base_name(environment(this_player()))+".c";
    other_room = this_object()->GetOtherRoom(door)+".c";
    foreach( string room in ({this_room, other_room}) ){
        globalstr = room;
        unguarded( (: globalstr2 = read_file(globalstr) :) );
        globalstr2 = remove_matching_line(globalstr2, door);
        globalstr3 = generate_tmp(load_object(door));
        unguarded( (: write_file(globalstr3, globalstr2, 1) :) );
        unguarded( (: cp(globalstr3, globalstr) :) );
        unguarded( (: rm(globalstr3) :) );
        reload(room);
    }
    return 1;
}

string GetOtherRoom(string door){
    string other_room, dir_str;
    foreach(string dir in environment(this_player())->GetDoors()){
        if(grepp(environment(this_player())->GetDoor(dir), door)) dir_str =
            dir;
    }
    other_room = environment(this_player())->GetExit(dir_str);
    if(other_room) return other_room;
    else return "";
}

string eventEvaluateDoorSide(object door){
    string *door_array, *door_sides;
    string dir_str, ret_str;

    door_array = environment(this_player())->GetDoors();
    door_sides = door->GetSides();

    foreach(string dir in door_array){
        if(load_object(environment(this_player())->GetDoor(dir)) == door) dir_str = dir;
    }

    if(!dir_str) return "";

    foreach(string side in door_sides){
        if(side == dir_str) ret_str = side;
    }

    return ret_str;
}

int eventCreateDoor(string dir, string filename){
    object my_room, other_room;
    object *everyone_here, *everyone_there;
    string *temp_array = ({});
    string contents, rep_str, opp_dir, new_line;

    tmpfile = generate_tmp(filename);

    my_room = environment(this_player());
    my_room_file = base_name(my_room)+".c";
    other_room_file = my_room->GetExit(dir)+".c";
    if(!file_exists(other_room_file)) {
        tell_room(my_room,"The new door fades away and disappears.");
        return 0;
    }
    unguarded( (: my_room_contents = read_file(my_room_file) :) );
    unguarded( (: other_room_contents = read_file(other_room_file) :) );
    other_room = load_object(other_room_file);
    everyone_here = get_livings(my_room,1);
    everyone_there = get_livings(other_room,1);

    contents = read_file("/obj/door.c");

    if(grepp(contents,"DIR_X")) rep_str = "X";
    else rep_str = "Y";

    contents = replace_string(contents, "DIR_"+rep_str, dir);
    contents = replace_string(contents, rep_str+"_SIDE", dir);

    opp_dir = opposite_dir(dir);
    if(opp_dir && opp_dir != "" && grepp(contents,"DIR_Y")){
        rep_str = "Y";
        contents = replace_string(contents, "DIR_"+rep_str, opp_dir);
        contents = replace_string(contents, rep_str+"_SIDE", opp_dir);
    }

    temp_array = explode(my_room_contents,"\n");
    foreach(string line in temp_array) {
        if(grepp(line,"SetDoor")){
            if(grepp(line,truncate(filename,2)) || grepp(line,"\""+dir+"\"")){
                my_room_contents = remove_matching_line(my_room_contents, line);
            }
        }
    }

    new_line = "SetDoor(\""+dir+"\", \""+filename+"\");";
    my_room_contents = this_object()->eventAppendLast(my_room_contents,"create", "\n"+new_line+"\n");

    temp_array = explode(other_room_contents,"\n");
    foreach(string line in temp_array) {
        if(grepp(line,"SetDoor")){
            if(grepp(line,truncate(filename,2)) || grepp(line,"\""+opp_dir+"\"")){
                other_room_contents = remove_matching_line(other_room_contents, line);
            }
        }
    }

    new_line = "SetDoor(\""+opp_dir+"\", \""+filename+"\");";
    other_room_contents = this_object()->eventAppendLast(other_room_contents,"create","\n"+new_line+"\n");

    globalstr2 = filename;
    globalstr3 = contents;

    unguarded( (: write_file( tmpfile, globalstr3,1 ) :) );
    this_object()->eventGeneralStuff(tmpfile);
    unguarded( (: cp(tmpfile, globalstr2) :) );

    globalstr3 = my_room_contents;
    unguarded( (: write_file( tmpfile, globalstr3,1 ) :) );
    this_object()->eventGeneralStuff(tmpfile);
    unguarded( (: cp(tmpfile, my_room_file) :) );

    globalstr3 = other_room_contents;
    unguarded( (: write_file( tmpfile, globalstr3,1 ) :) );
    this_object()->eventGeneralStuff(tmpfile);
    unguarded( (: cp(tmpfile, other_room_file) :) );

    reload(filename);
    reload(my_room_file);
    reload(other_room_file);

    unguarded( (: rm(tmpfile) :) );

    return 1;


}


