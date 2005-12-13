#include <lib.h>
#include <modules.h>

inherit LIB_DAEMON;

string globalstr, globalstr2;

varargs int eventCreateExit(string dir, string room, string file, int remote);
int eventRemoveExit(string dir, string filename);
varargs mixed eventProcessExits(string filename, string dir, string location);
string eventCopyRoom(string source, string dest);

mixed make(string str) {
    int enter;
    string *dir_array;
    string *exits;
    string *enters;
    string foo, current_dir, current_room, this_room, new_room, room_dir;
    string new_file, arg1, arg2, s1, s2;
    object room;

    if(!str || str == "") {
	write("You'll need to be more specific. Try 'help create'");
	return 1;
    }


    foo = ""+time();
    room = environment(this_player());
    current_dir = this_player()->query_cwd();
    current_room = base_name(room);
    dir_array = explode(current_room, "/");
    dir_array -= ({ dir_array[sizeof(dir_array) - 1] });
    room_dir = "/"+implode(dir_array,"/");


    if(file_exists(current_room+".c") && !check_privs(this_player(),current_room+".c")){
	write("You do not appear to have access to this room file. Modification aborted.");
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


    if(sscanf(str," enter %s %s", s1, s2) == 2){ 
	arg1 = s1;
	arg2 = s2;
	enter = 1;
    }
    else if(sscanf(str," %s %s", s1, s2) == 2){ 
	arg1 = s1;
	arg2 = s2;
	enter = 0;
    }
    else {
	write("Usage: create room <direction> <file>");
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

    if(strsrch(arg2,".c") == -1) arg2 += ".c";
    if(strsrch(arg2," ") != -1) arg2 = replace_string(arg2," ","");

    if(file_exists(arg2)) new_file = arg2;
    else{
	if(strsrch(arg2,"/") != -1) arg2 = replace_string(arg2,"/","");
	new_file = room_dir +"/"+ arg2;
    }


    if(new_file[0..7] == "/realms/" && strsrch(new_file,"/area/") != -1){
	if(file_size(new_file) < 0) cp("/obj/area_room.c",new_file);
    }
    else if(file_size(new_file) < 0) cp("/obj/room.c",new_file);

    eventCreateExit(arg1, current_room+".c", new_file);
    if(file_exists(new_file) &&
      grepp(read_file(new_file),"SetShort(\"a blank room\");") ) {
	eventCopyRoom(current_room+".c", new_file);
	load_object("/secure/cmds/creators/update")->cmd("-a "+new_file);
    }
    //write("You mutter an incantation and a new room appears!");
    return 1;
}

varargs int eventCreateExit(string dir, string room, string file, int remote){
    string this_room, param, line, newfile;
    string *file_arr;
    object *players;

    if(file_exists(room) && !check_privs(this_player(),room)){
	write("You do not appear to have access to this room file. Modification aborted.");
	return 1;
    }

    this_room = read_file(room);
    file_arr = explode(this_room,"\n");

    if(remote && member_array(dir,load_object(room)->GetExits()) != -1) return 0;

    if(strsrch(this_room,"SetExits") != -1) param = "SetExits";
    else if(strsrch(this_room,"AddExit") != -1) param = "AddExit";
    else if(strsrch(this_room,"SetLong") != -1) param = "SetLong";
    else {
	write("The room you are in is screwed up. Creation process halted.");
    }
    newfile = remove_matching_line(this_room,"SetObviousExits",1);
    newfile = remove_matching_line(newfile,"//extras",1);
    write_file(room,newfile,1);
    players = get_livings(environment(this_player()),1);
    load_object("/secure/cmds/creators/update")->cmd("-a "+room);
    eventProcessExits(room, dir, file);

    if(!remote) {
	//call_out( (: eventCreateExit, opposite_dir(dir), file, room, 1 :), 1 );
	eventCreateExit(opposite_dir(dir), file, room, 1 );
	players->eventMove(load_object(room));
	write("You begin uttering a magical incantation.");
	say(this_player()->GetCapName()+" begins uttering a magical incantation.");
	load_object(MODULES_GENERIC)->eventGeneralStuff(room);
    }
    load_object(MODULES_FILE)->eventAddInit(room);
    if(remote){
	load_object(MODULES_GENERIC)->eventGeneralStuff(room);
	write("You wave your hand, and a new exit appears.");
	say(this_player()->GetCapName()+" waves "+possessive(this_player())+" hand and a new exit appears.");
    }
    return 1;
}

int eventRemoveExit(string dir, string filename){
    string search_str, map_str, key, val, new_file, tmpfile;
    string *file_arr;
    object *players;
    mapping PointlessMap;
    mapping ExitsMap = load_object(filename)->GetExitMap();

    //if(!sizeof(ExitsMap)) tc("Exitsmap is nothing");
    // else tc("ExitsMap: "+identify(ExitsMap));

    if(file_exists(filename) && !check_privs(this_player(),filename)){
	write("You do not appear to have access to this room's file. Modification aborted.");
	return 1;
    }

    tmpfile = generate_tmp(load_object(filename));
    PointlessMap = ([]);

    PointlessMap = load_object(filename)->GetFullExitData();

    //if(!sizeof(PointlessMap)) write("PointlessMap is nothing");
    //  else write("PointlessMap: "+identify(PointlessMap));

    //if(file_exists(filename)) tc(filename+" exists.");
    //  else tc(filename+" doesn't exist.");

    if( member_array(dir,load_object(filename)->GetExits()) != -1 
      && !functionp(PointlessMap[dir]["pre"]) ) {
	load_object(MODULES_FILE)->eventReadMapping(filename,({"SetExits"}), 1);
	map_delete(ExitsMap,dir);
	map_str = "SetExits( ([ \n";
	foreach( key, val in ExitsMap){
	    if(!functionp(PointlessMap[key]["pre"])) map_str += "\""+key+"\" : \""+val+"\",\n";
	}
	map_str += "]) );";

	new_file = load_object(MODULES_FILE)->eventAppend(read_file(filename),({"SetLong"}),"\n"+map_str+"\n");
	new_file = remove_matching_line(new_file,"SetObviousExits");
	new_file = remove_matching_line(new_file,"//extras");
	new_file = remove_matching_line(new_file,"AddExit(", 1, ":)");
	write_file(tmpfile,new_file,1);
	load_object(MODULES_GENERIC)->eventGeneralStuff(tmpfile);
	cp(tmpfile,filename);
	players = get_livings(environment(this_player()),1);
	load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
	players->eventMove(load_object(filename));
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
	new_file = remove_matching_line(new_file, search_str);
	write_file(tmpfile,new_file,1);
	cp(tmpfile, filename);
	players = get_livings(environment(this_player()),1);
	load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
	eventProcessExits(filename);
	players->eventMove(load_object(filename));
	rm(tmpfile);
	write("With a puff of smoke, an exit vanishes!");
	return 1;
    }

    write("This room's SetExits does not contain that direction.");
    return 1;
}

varargs mixed eventProcessExits(string filename, string dir, string location){
    string tmpexit, search_str, map_str, key, val, new_file;
    string *file_arr;
    object *players;
    mapping PointlessMap;

    mapping ExitsMap = load_object(filename)->GetExitMap();

    if(file_exists(filename) && !check_privs(this_player(),filename)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }

    PointlessMap =  load_object(filename)->GetFullExitData();
    map_str = "SetExits( ([\n";

    load_object(MODULES_FILE)->eventReadMapping(filename,({"SetExits"}), 1);
    new_file = remove_matching_line(read_file(filename),"SetObviousExits");
    new_file = remove_matching_line(new_file,"//extras");
    new_file = remove_matching_line(new_file, "AddExit(", 1, ":)");

    foreach( key, val in ExitsMap){
	if(!functionp(PointlessMap[key]["pre"])) map_str += "\""+key+"\" : \""+val+"\",\n";
    }
    if(dir && location && member_array(dir,load_object(filename)->GetExits()) == -1){
	map_str += "\""+dir+"\" : \""+location+"\",\n";
    }
    map_str += "]) );";

    globalstr = filename; 
    tmpexit = generate_tmp(load_object(filename));

    new_file = load_object(MODULES_FILE)->eventAppend(new_file,({"SetLong","SetDayLong","SetNightLong","SetShort","create()","create"}),"\n"+map_str+"\n");
    write_file(tmpexit,new_file,1);
    cp(tmpexit,globalstr);
    load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
    rm(tmpexit);
    return 1;
}

string eventCopyRoom(string source, string dest){
    string homedir, areadir, tmpsource, map_str, new_file, foo;
    mapping SourceExits, DestExits;

    if(file_exists(source) && !check_privs(this_player(),source)){
	write("You do not appear to have access to this file. Modification aborted.");
	return "";
    }

    if(file_exists(dest) && !check_privs(this_player(),dest)){
	write("You do not appear to have access to that file. Modification aborted.");
	return "";
    }

    tmpsource = generate_tmp(load_object(dest));
    cp(source, tmpsource);
    if(!file_exists(source)) return "Source read failed.";
    if(!file_exists(tmpsource)) return "Read failed.";
    if(!file_exists(dest)) return "Destination read failed.";
    homedir = "/realms/"+this_player()->GetKeyName();
    areadir = homedir+"/area";

    if(grepp(dest, areadir)) foo = replace_line(read_file(tmpsource),
	  ({"customdefs.h"}), "#include \"../../customdefs.h\"" ); 
    else if(path_prefix(dest) == homedir) foo = replace_line(read_file(tmpsource),
	  ({"customdefs.h"}), "#include \"./customdefs.h\"" );
    else foo = replace_line(read_file(tmpsource), ({"customdefs.h"}), "#include \""+homedir(this_player())+"/customdefs.h\"");
    foo = replace_line(foo,({"SetObviousExits"}),"");
    write_file(tmpsource,foo,1);

    SourceExits = load_object(MODULES_FILE)->eventReadMapping(tmpsource,({"SetExits"}),1); 
    DestExits = load_object(dest)->GetExitMap();

    map_str = "SetExits("+load_object(MODULES_MAPPING)->eventStringifyMap(DestExits)+");\n";
    new_file = load_object(MODULES_FILE)->eventAppend(read_file(tmpsource),({"SetLong"}),"\n"+map_str+"\n");
    new_file = replace_line(new_file,({"SetShort(",");"}),"SetShort(\"copy of "+source+"\");");
    replace_string(new_file,"\n\n\n","\n\n");

    write_file(tmpsource,new_file,1);
    load_object(MODULES_GENERIC)->eventGeneralStuff(tmpsource);
    globalstr = tmpsource;
    globalstr2 = dest;

    if(!unguarded( (: cp(globalstr, globalstr2) :) )){
	return "Write failed.";
    }

    load_object(MODULES_FILE)->eventAddInit(dest);
    rm(tmpsource);
    return "Room data copy complete.";
}
