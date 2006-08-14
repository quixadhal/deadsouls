#include <lib.h>
#include <config.h>

inherit LIB_DAEMON;
string *all_dirs = ({});
string *dirs = ({});
string dir_list = "";
string file_list = "";
string *all_files = ({});

static private void validate() {
    if(!this_player()) return 0;
    if( !((int)master()->valid_apply(({ "ASSIST" }))) )
	error("Illegal attempt to access FILE_D: "+get_stack()+" "+identify(previous_object(-1)));
}

mixed eventHarvestFiles(){
    string *harvest;
    string *recolte;

    validate();

    all_files = ({});
    file_list = "";
    if(!sizeof(all_dirs)) this_object()->eventHarvestDirs();
    harvest = all_dirs;
    recolte = all_dirs[0..9];

    while(sizeof(recolte)){
	foreach(string dir in recolte){
	    foreach(string file in get_dir(dir)){
		if(file_exists(dir+file)){
		    all_files += ({ dir+file });
		    file_list += dir+file+"\n";
		}
	    }
	}
	harvest -= harvest[0..9];
	recolte =  harvest[0..9];
    }
    //write_file("/tmp/files.txt",file_list);
    return "fee";
}


mixed eventHarvestDirs(){
    validate();

    all_dirs = ({});
    dirs = ({});
    dir_list = "";

    foreach(string dir in get_dir("/")){
	if( directory_exists("/"+dir) ) {
	    dir_list += "/"+dir+"/\n";
	    dirs += ({ "/"+dir+"/" });
	}
    }
    all_dirs = dirs;

    //write_file("/tmp/dirs.txt",dir_list,1);
    //tc("dirs: "+identify(dirs));

    while(sizeof(dirs)){
	string *tmp_dirs = ({});
	foreach(string dir in dirs){
	    foreach(string sub in get_dir(dir)){
		if( directory_exists(dir+sub) ) {
		    dir_list += dir+sub+"/\n";
		    tmp_dirs += ({ dir+sub+"/" });
		}
	    }
	}
	all_dirs += tmp_dirs;
	dirs = tmp_dirs;
    }

    //write_file("/tmp/dirs.txt",dir_list,1);

    //eventHarvestFiles();
    return "foo";
}

string *GetFiles(){
    if(!sizeof(all_files)) {
	eventHarvestDirs();
	eventHarvestFiles();
    }
    return all_files;
}

string *GetDirs(){
    if(!sizeof(all_dirs)) eventHarvestDirs();
    return all_dirs;
}

static void create() {
    daemon::create();
    call_out((: eventHarvestDirs :), 1);
    call_out((: eventHarvestFiles :), 5);
}

