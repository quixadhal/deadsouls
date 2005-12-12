#include <lib.h>
#include <dirs.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string *arr;
    string *file_arr;
    string tmp, filename;
    int time;

    if( !str ) return "You must specify a file to backup.";
    else str = absolute_path((string)this_player()->query_cwd(), str);
    if( !file_exists(str) ) str += ".c";
    if( !file_exists(str) ) return "File not found.";
    else if( !(tmp = read_file(str)) )
	return "Unable to read file " + str + ".";

    else {
	time = time();
	file_arr = explode(str,"/");
	filename = REALMS_DIRS + "/" + this_player()->GetKeyName();
	filename += "/bak/"+file_arr[sizeof(file_arr)-1]+"."+time;
	write_file(REALMS_DIRS + "/" + this_player()->GetKeyName()+
	  "/bak/bk.db",file_arr[sizeof(file_arr)-1]+"."+time+" : "+str+"\n");
	load_object("/secure/cmds/creators/cp")->cmd(str + " " +filename);
    }
    return 1;
}

int help() {
    message("help", "Syntax: <bk [file]>\n\n"
      "Copies the specified file into your bak/ directory, and "
      "appends a unique identifying number to the backup file name.",
      this_player());
}
