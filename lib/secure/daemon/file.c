#include <lib.h>
#include <config.h>
#include <daemons.h>
#include <runtime_config.h>

inherit LIB_DAEMON;
string *all_dirs = ({});
string *all_files = ({});
int ftilt, dtilt;
string globaltemp;

static mixed Report();

static private void validate() {
    if(!this_player()) return 0;
    if( !((int)master()->valid_apply(({ "ASSIST" }))) )
	error("Illegal attempt to access FILE_D: "+get_stack()+" "+identify(previous_object(-1)));
}

void heart_beat(){
    foreach(mixed arr in call_out_info()){
	if(arr[0] == this_object()){
	    return ;
	}
    }
    //shout("SYSTEM: File scan complete.");
    Report();
    set_heart_beat(0);
}

mixed ReadDir(string str){
    string *current_level_dirs = ({});
    int iteration = 1;

    validate();

    if(!str ||  !sizeof(str)) str = "/";

    if(!query_heart_beat()) set_heart_beat(1);
    if(last(str,1) != "/") str += "/";
    if(directory_exists(str)){
	foreach(string element in get_dir(str)){
	    if(file_exists(str+element)) all_files += ({ str+element });
	    if(directory_exists(str+element)) {
		all_dirs += ({ str+element });
		current_level_dirs += ({ str+element });
	    }
	}
	if(sizeof(current_level_dirs)){
	    foreach(string element in current_level_dirs){
		iteration++;
		globaltemp = element;
		call_out( (: ReadDir, globaltemp :), iteration );
	    }
	}
    }
    return 1;
}

static mixed Report(){
    foreach(mixed arr in call_out_info()){
	if(arr[0] == this_object()){
	    write("File scan is not complete.");
	    return 1;
	}
    }
    all_dirs = sort_array(all_dirs,1);
    all_dirs = singular_array(all_dirs);
    all_files = sort_array(all_files,1);
    all_files = singular_array(all_files);
    rm("/secure/tmp/dirs.txt");
    rm("/secure/tmp/files.txt");
    foreach(string dir in all_dirs){
	write_file("/secure/tmp/dirs.txt",dir+"\n");
    }
    foreach(string dir in all_files){
	write_file("/secure/tmp/files.txt",dir+"\n");
    }
    return 1;
}

string *GetFiles(){
    return all_files;
}

string *GetDirs(){
    return all_dirs;
}

static void create() {
    object fun_d = find_object(FUNCTION_D);
    daemon::create();
    call_out((: ReadDir,"/" :), 1);
    if(!fun_d) fun_d = load_object(FUNCTION_D);
}

