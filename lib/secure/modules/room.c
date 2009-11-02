#include <lib.h>
#include <modules.h>

private string globalstr, globalstr2, globaltmp;
private string globalroom, globalfile;

varargs int eventCreateExit(string dir, string room, string file, int remote);
varargs int eventCreateEnter(string dir, string room, string file, int remote);
int eventRemoveExit(string dir, string filename);
varargs mixed eventProcessExits(string filename, string dir, string location);
varargs mixed eventProcessEnters(string filename, string dir, string location, object room);
string eventCopyRoom(string source, string dest);

string *cardinal_dirs = ( ({"none", "north","south","east","west", "northeast","northwest","southeast","southwest","up","down", "out"}) );

mixed make(string str) {
    int enter;
    int blank = 0;
    string *exits;
    string *enters;
    string foo, current_dir, current_room, this_room, new_room, room_dir;
    string tmp_path, new_file, arg1, arg2, s1, s2;
    object room;
    globalstr = globalstr2 = globaltmp = "";

    if(!str || str == "") {
        write("You'll need to be more specific. Try 'help create'");
        return 1;
    }

    if(environment(this_player())->GetDirectionMap()){
        write("This is a virtual room. It cannot be modified with the QCS.");
        return 1;
    }

    if(sscanf(str," %s %s", s1, s2) == 2){
        arg1 = s1;
        arg2 = s2;
        if(sizeof(opposite_dir(arg1))) enter = 0;
        else enter = 1;
    }
    else {
        write("Usage: create room <direction> <file>");
        return 1;
    }

    if(arg1 == "none") blank = 1;

    foo = ""+time();
    room = environment(this_player());
    current_dir = this_player()->query_cwd();
    current_room = base_name(room);
    if(!blank) room_dir = path_prefix(current_room);
    else room_dir = current_dir;

    if(file_exists(current_room+".c") && !check_privs(this_player(),current_room+".c")){
        write("You do not appear to have write access to this room file. Modification aborted.");
        return 1;
    }

    if(!write_file(room_dir+"/"+foo+".foo","",1)) {
        write("You do not have write privileges to this area.");
        return 1;
    }

    rm(room_dir+"/"+foo+".foo");
    write("It appears you have write access to this area.");
    this_room = read_file(current_room+".c");
    new_room = read_file("/obj/room.c");

    if(!blank){

        if(environment(this_player())->GetNoModify() ){
            write("This should be edited by hand. Change cancelled.");
            write("Please see http://dead-souls.net/ds-creator-faq.html#2.69");
            return 1;
        }

        exits = room->GetExits();
        enters = room->GetEnters();

        if(member_array(arg1,exits) != -1){
            write("This room already has an exit in that direction.");
            return 1;
        }

        if(member_array(arg1,enters) != -1){
            write("This room already has an enter by that name.");
            return 1;
        }
    }

    if(strsrch(arg2,".c") == -1) arg2 += ".c";
    if(strsrch(arg2," ") != -1) arg2 = replace_string(arg2," ","_");

    if(file_exists(arg2)) new_file = arg2;
    else if(strsrch(arg2,"./") != -1) {
        arg2 = replace_string(arg2,"./","");
        new_file = absolute_path(this_player()->query_cwd(), arg2);
    }
    else if(directory_exists(path_prefix(arg2))){
        new_file  = arg2;
    }
    else if(grepp(arg2,"/")){
        new_file = room_dir +"/"+ last_string_element(arg2,"/");
    }

    else {
        new_file = room_dir +"/"+ arg2;
    }

    if(!check_privs(this_player(),new_file)){
        write("Invalid directory.");
        return 1;
    }

    if(new_file[0..7] == "/realms/" && strsrch(new_file,"/area/room/") != -1){
        if(!file_exists(new_file)) cp("/obj/area_room.c",new_file);
    }
    else {
        if(!file_exists(new_file)) cp("/obj/room.c",new_file);
    }

    if(blank){
        reload(new_file);
        this_player()->eventMoveLiving(new_file);
        return 1;
    }

    if(enter == 0) eventCreateExit(arg1, current_room+".c", new_file);
    else eventCreateEnter(arg1, current_room+".c", new_file);
    if(file_exists(new_file) &&
            grepp(read_file(new_file),"SetShort(\"a blank room\");") ) {
        eventCopyRoom(current_room+".c", new_file);
        reload(new_file);
    }
    return 1;
}

varargs int eventCreateExit(string dir, string room, string file, int remote){
    string param;
    string *file_arr;

    if(file_exists(room) && !check_privs(this_player(),room)){
        write("You do not appear to have write access to this room file. Modification aborted.");
        return 1;
    }

    if(member_array(dir,cardinal_dirs) == -1) {
        this_object()->eventCreateEnter(dir, room, file, remote);
        return 1;
    }

    globalstr = room;
    unguarded( (: globaltmp = read_file(globalstr) :) );
    file_arr = explode(globaltmp,"\n");

    if(remote && member_array(dir,load_object(room)->GetExits()) != -1) return 0;

    if(strsrch(globaltmp,"SetExits") != -1) param = "SetExits";
    else if(strsrch(globaltmp,"AddExit") != -1) param = "AddExit";
    else if(strsrch(globaltmp,"SetLong") != -1) param = "SetLong";
    else {
    }
    globaltmp = remove_matching_line(globaltmp,"SetObviousExits",1);
    globaltmp = remove_matching_line(globaltmp,"//extras",1);
    unguarded( (: write_file(globalstr,globaltmp,1) :) );
    reload(room);
    eventProcessExits(room, dir, file);

    if(!remote) {
        eventCreateExit(opposite_dir(dir), file, room, 1 );
        write("You begin uttering a magical incantation.");
        say(this_player()->GetCapName()+" begins uttering a magical incantation.");
        this_object()->eventGeneralStuff(room);
    }
    this_object()->eventAddInit(room);
    if(remote){
        this_object()->eventGeneralStuff(room);
        write("You wave your hand, and a new exit appears.");
        say(this_player()->GetCapName()+" waves "+possessive(this_player())+" hand and a new exit appears.");
    }
    return 1;
}

int eventRemoveExit(string dir, string filename){
    string contents, search_str, map_str, key, val, new_file, tmpfile;
    string *file_arr;
    mapping PointlessMap;
    mapping ExitsMap = load_object(filename)->GetExitMap();

    if(file_exists(filename) && !check_privs(this_player(),filename)){
        write("You do not appear to have write access to this room's file. Modification aborted.");
        return 1;
    }

    tmpfile = generate_tmp(load_object(filename));
    globalstr = filename;
    globaltmp = tmpfile;
    unguarded( (: globalstr2 = read_file(globalstr) :) );
    contents = globalstr2;
    PointlessMap = ([]);

    PointlessMap = load_object(filename)->GetFullExitData();

    if( member_array(dir,load_object(filename)->GetExits()) != -1 
            && !functionp(PointlessMap[dir]["pre"]) 
            && !functionp(PointlessMap[dir]["post"]) ) {
        this_object()->eventReadMapping(filename,({"SetExits"}), 1);
        map_delete(ExitsMap,dir);
        map_str = "SetExits( ([ \n";
        foreach( key, val in ExitsMap){
            if(!functionp(PointlessMap[key]["pre"]) 
                    && !functionp(PointlessMap[key]["post"])) map_str += "\""+key+"\" : \""+val+"\",\n";
        }
        map_str += "]) );";

        unguarded( (: globalstr2 = read_file(globalstr) :) );
        contents = globalstr2;

        new_file = remove_matching_line(contents, "SetExits", 1); 
        new_file = this_object()->eventAppend(new_file,({"SetItems","SetLong","SetDayLong","SetNightLong"}),"\n"+map_str+"\n");
        new_file = remove_matching_line(new_file,"SetObviousExits");
        new_file = remove_matching_line(new_file,"//extras");
        new_file = remove_matching_line(new_file,"AddExit(", 1, ":)");
        globalstr2 = new_file;
        unguarded( (: write_file(globaltmp,globalstr2,1) :) );
        this_object()->eventGeneralStuff(tmpfile);
        globalstr = tmpfile;
        globalstr2 = filename;
        unguarded( (: cp(globalstr, globalstr2) :) );
        reload(filename);
        rm(tmpfile);
        write("With a puff of smoke, an exit vanishes!");
        return 1;
    }


    if(member_array(dir,load_object(filename)->GetExits()) != -1){
        globalstr = filename;
        new_file = read_file(filename);
        file_arr = explode(read_file(filename),"\n");
        foreach(string linea in file_arr){
            if(strsrch(linea,"AddExit") != -1 && strsrch(linea,dir) != -1) {
                search_str = linea;
            }
        }
        globalstr = tmpfile;
        globalstr2 = filename;
        globaltmp = remove_matching_line(new_file, search_str);
        unguarded( (: write_file(globalstr,globaltmp,1) :) );
        unguarded( (: cp(globalstr, globalstr2) :) );
        eventProcessExits(filename);
        reload(filename);
        rm(tmpfile);
        write("With a puff of smoke, an exit vanishes!");
        return 1;
    }

    write("This room's SetExits does not contain that direction.");
    return 1;
}

varargs mixed eventProcessExits(string filename, string dir, string location){
    string map_str, key, val, new_file;
    mapping PointlessMap;

    mapping ExitsMap = load_object(filename)->GetExitMap();

    if(file_exists(filename) && !check_privs(this_player(),filename)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }

    PointlessMap =  load_object(filename)->GetFullExitData();
    map_str = "SetExits( ([\n";

    this_object()->eventReadMapping(filename,({"SetExits"}), 1);
    new_file = remove_matching_line(read_file(filename),"SetObviousExits");
    new_file = remove_matching_line(new_file,"//extras");
    new_file = remove_matching_line(new_file, "AddExit(", 1, ":)");

    foreach( key, val in ExitsMap){
        if(!functionp(PointlessMap[key]["pre"]) &&
                !functionp(PointlessMap[key]["post"])) map_str += "\""+key+"\" : \""+val+"\",\n";
    }
    if(dir && location && member_array(dir,load_object(filename)->GetExits()) == -1){
        map_str += "\""+dir+"\" : \""+location+"\",\n";
    }
    map_str += "]) );";

    globalstr = filename; 
    globaltmp = generate_tmp();

    globalstr2 = this_object()->eventAppend(new_file,({"SetItems","SetLong","SetDayLong","SetNightLong","SetShort","create()","create"}),"\n"+map_str+"\n");
    unguarded( (: write_file(globaltmp,globalstr2,1) :) );
    unguarded( (: cp(globaltmp,globalstr) :) );
    reload(filename);
    rm(globaltmp);
    return 1;
}

string eventCopyRoom(string source, string dest){
    string homedir, areadir, tmpsource, map_str, new_file;
    mapping DestExits;

    if(file_exists(source) && (!check_privs(this_player(),source) &&
                strsrch(source,"/obj/"))){
        write("You do not appear to have write access to this file. Modification aborted.");
        return "";
    }

    if(file_exists(dest) && !check_privs(this_player(),dest)){
        write("You do not appear to have write access to that file. Modification aborted.");
        return "";
    }

    tmpsource = generate_tmp(source);
    globalstr2 = source;
    globalstr = tmpsource;
    unguarded( (: cp(globalstr2, globalstr) :) );
    globalstr2 = dest;
    if(!file_exists(source)) return "Source read has failed.";
    if(!file_exists(tmpsource)) return "Read failed.";
    if(!file_exists(dest)) return "Destination read failed.";
    homedir = "/realms/"+this_player()->GetKeyName();
    areadir = homedir+"/area";

    unguarded( (: this_object()->eventReadMapping(globalstr,({"SetExits"}),1) :) ); 
    unguarded( (: this_object()->eventReadMapping(globalstr2,({"SetExits"}),1) :) ); 
    DestExits = load_object(dest)->GetExitMap();

    unguarded( (: globaltmp = read_file(globalstr) :) ); 

    map_str = "SetExits("+this_object()->eventStringifyMap(DestExits)+");\n";
    new_file = this_object()->eventAppend(globaltmp,({"SetItems","SetLong","SetDayLong"}),"\n"+map_str+"\n");
    new_file = replace_line(new_file,({"SetShort(",");"}),"SetShort(\"copy of "+last_string_element(source,"/")+"\");");
    replace_string(new_file,"\n\n\n","\n\n");
    new_file = remove_matching_line(new_file, "SetDoor", 1);

    if(query_verb() == "copy" && grepp(new_file, "customdefs\.h")){
        string *tmparr = explode(new_file,"\n");
        string *tmparr2 = explode(read_file(dest),"\n");
        string bad_def = "";
        string good_def = "";
        foreach(string element in tmparr2){
            if(!strsrch(element,"#include")){
                if(grepp(element,"customdefs.h")){
                    good_def = element;
                }
            }
        }
        tmparr2 = ({});
        foreach(string element in tmparr){
            if(!strsrch(element,"#include")){
                if(grepp(element,"customdefs.h")) element = good_def;
            }
            tmparr2 += ({ element });
        }
        new_file = implode(tmparr2,"\n");
    }

    write_file(tmpsource,new_file,1);

    globalstr = tmpsource;
    globalstr2 = dest;

    if(!unguarded( (: cp(globalstr, globalstr2) :) )){
        return "Write failed.";
    }

    this_object()->eventAddInit(dest);
    this_object()->eventGeneralStuff(dest);
    rm(tmpsource);
    return "Room data copy complete.";
}

varargs int eventCreateEnter(string dir, string room, string file, int remote){

    if(file_exists(room) && !check_privs(this_player(),room)){
        write("You do not appear to have write access to this room file. Modification aborted.");
        return 1;
    }

    if(!present(dir,environment(this_player()))){ 
        write("This room needs a corresponding SetItem to make a SetEnter ");
        write("of "+dir);
        return 1;
    }

    globalroom = room;
    globalfile = file;
    globaltmp = "";

    if(remote) unguarded( (: globaltmp = read_file(globalroom) :) );
    else unguarded( (: globaltmp = read_file(globalfile) :) );

    if(remote && member_array("out",load_object(room)->GetExits()) != -1) return 0;

    globaltmp = remove_matching_line(globaltmp,"SetEnters",1);
    globaltmp = remove_matching_line(globaltmp,"//extras",1);
    unguarded( (: write_file(globalfile,globaltmp,1) :) );

    if(!remote) {
        eventProcessEnters(room, dir, file);
        this_object()->eventCreateExit("out", file, room, 1 );
        say(this_player()->GetCapName()+" waves "+possessive(this_player())+" hand and a new enter appears.");
        this_object()->eventGeneralStuff(room);
    }
    this_object()->eventAddInit(room);
    if(remote){
        this_object()->eventGeneralStuff(room);
        write("You begin uttering a magical incantation.");
        write("You wave your hand, and a new enter appears.");
        say(this_player()->GetCapName()+" begins uttering a magical incantation.");

    }
    reload(room);
    reload(file);
    room = "";
    file = "";
    globalroom = "";
    globalfile = "";
    globaltmp ="";
    return 1;
}

int eventRemoveEnter(string dir, string filename){
    string contents, search_str, map_str, new_file, tmpfile;
    mixed *key;
    mixed val;
    string *file_arr;
    mixed *key_arr;
    mapping PointlessMap;
    mapping EntersMap = load_object(filename)->GetEnterMap();

    if(file_exists(filename) && !check_privs(this_player(),filename)){
        write("You do not appear to have write access to this room's file. Modification aborted.");
        return 1;
    }

    tmpfile = generate_tmp(load_object(filename));
    globalstr = filename;
    globaltmp = tmpfile;
    unguarded( (: globalstr2 = read_file(globalstr) :) );
    contents = globalstr2;
    PointlessMap = ([]);

    PointlessMap = load_object(filename)->GetEnterMap();

    if( member_array(dir,load_object(filename)->GetEnters()) != -1) { 
        this_object()->eventReadMapping(filename,({"SetEnters"}), 1);
        foreach(key,val in EntersMap){
            if(arrayp(key) && member_array(dir,key) != -1) 
                key_arr = key;
            true(); 
        }
        map_delete(EntersMap,key_arr);
        map_str = "SetEnters( ([ \n";
        foreach( key, val in EntersMap){
            map_str += "\""+key[0]+"\" : \""+val+"\",\n";
        }
        map_str += "]) );";

        unguarded( (: globalstr2 = read_file(globalstr) :) );
        contents = globalstr2;

        new_file = remove_matching_line(contents, "SetEnters", 1); 
        new_file = this_object()->eventAppend(new_file,({"AddItem","SetItems"}),"\n"+map_str+"\n");
        new_file = remove_matching_line(new_file,"SetObviousEnters");
        new_file = remove_matching_line(new_file,"//extras");
        new_file = remove_matching_line(new_file,"AddEnter(", 1);
                globalstr2 = new_file;
                unguarded( (: write_file(globaltmp,globalstr2,1) :) );
                this_object()->eventGeneralStuff(tmpfile);
                globalstr = tmpfile;
                globalstr2 = filename;
                unguarded( (: cp(globalstr, globalstr2) :) );
                rm(tmpfile);
                reload(filename);
                write("With a puff of smoke, an enter vanishes!");
                return 1;
                }


                if(member_array(dir,load_object(filename)->GetEnters()) != -1){
                globalstr = filename;
                new_file = read_file(filename);
                file_arr = explode(read_file(filename),"\n");
                foreach(string linea in file_arr){
                if(strsrch(linea,"AddEnter") != -1 && strsrch(linea,dir) != -1) {
                search_str = linea;
                }
                }
                globalstr = tmpfile;
                globalstr2 = filename;
                globaltmp = remove_matching_line(new_file, search_str);
                unguarded( (: write_file(globalstr,globaltmp,1) :) );
                unguarded( (: cp(globalstr, globalstr2) :) );
                eventProcessEnters(filename);
                reload(filename);
                rm(tmpfile);
                write("With a puff of smoke, an enter vanishes!");
                return 1;
                }

                write("This room's SetEnters does not contain that direction.");
                return 1;
}

varargs mixed eventProcessEnters(string filename, string dir, string location, object room) {
    string map_str, key, val, new_file;
    string *id_array;
    object *dummies;
    mapping PointlessMap;

    mapping EntersMap = load_object(filename)->GetEnterMap();
    id_array = ({});
    dummies = load_object(filename)->GetDummyItems();
    if(sizeof(dummies)) foreach(object dumdum in dummies){
        id_array += dumdum->GetId();
    }

    if(file_exists(filename) && !check_privs(this_player(),filename)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }

    PointlessMap =  load_object(filename)->GetEnterMap();
    map_str = "SetEnters( ([\n";
    globalstr = filename;

    unguarded( (: this_object()->eventReadMapping(globalstr,({"SetEnters"}), 1) :) );
    new_file = remove_matching_line(unguarded( (: read_file(globalstr) :) ),"SetObviousExits");
    new_file = remove_matching_line(new_file,"//extras");
    new_file = remove_matching_line(new_file, "AddEnter(", 1, ":)");

    foreach( key, val in EntersMap){
        map_str += "\""+key[0]+"\" : \""+val+"\",\n";
    }

    if(dir && location && member_array(dir,id_array) != -1){
        map_str += "\""+dir+"\" : \""+location+"\",\n";
    }
    map_str += "]) );";

    globalstr = filename; 
    globaltmp = generate_tmp(load_object(filename));

    globalstr2 = this_object()->eventAppend(new_file,({"AddItem","SetItems","SetExits","AddExit"}),"\n"+map_str+"\n");
    unguarded( (: write_file(globaltmp,globalstr2,1) :) );
    unguarded( (: cp(globaltmp,globalstr) :) );
    reload(filename);
    rm(globaltmp);
    return 1;
}
