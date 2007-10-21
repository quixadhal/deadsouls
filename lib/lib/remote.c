/*    /lib/remote.c
 *    from the Foundation II LPC Library
 *    handles security for cre access via the remote creator protocol
 *    created by Descartes of Borg 950505
 */

#include <lib.h>

inherit LIB_DAEMON;

private static string Directory, Home;

static void create() {
    daemon::create();
    SetNoClean(1);
    Directory = Home = "/" +
    implode(explode(file_name(this_object()), "/")[0..<3], "/");
    if( strsrch(Directory, REALMS_DIRS) ) call_out( (: Destruct :), 0 );
}

string eventWriteFile(string file, string str) {
    file = absolute_path(Directory, file);
    if( base_name(this_object()) == LIB_REMOTE ) 
	return "50 Failed to write file: " + file;
    if( file_size(file) > -1 && !rm(file) ) 
	return "50 Failed to write file: " + file;
    if( !write_file(file, str) ) return "50 Failed to write file: " + file;
    else return "110 File " + file + " written.";
}

string eventReadFile(string file) {
    string tmp;

    if( base_name(this_object()) == LIB_REMOTE ) return "";
    file = absolute_path(Directory, file);
    if( !(tmp = read_file(file)) ) return "";
    else return tmp;
}

string eventCommand(string cmd, string arg) {
    string *files;
    object ob;
    string tmp;

    if( base_name(this_object()) == LIB_REMOTE ) return 0;
    switch(cmd) {
    case "ls":
	if( !arg || arg == "" ) arg = Directory;
	switch(file_size(arg)) {
	case -2:
	    if( arg[<1] != '/' ) arg += "/";
	    files = ({ arg });
	    files += map(get_dir(arg), 
	      (: (file_size($(arg) + $1) == -2) ? $1 + "/" : $1 :));
	    return implode(files, "\t");
	case -1:
	    return 0;
	default:
	    return arg;
	}
    case "mkdir":
	arg = absolute_path(Directory, arg);
	if( file_size(arg) != -1 ) return 0;
	if( !mkdir(arg) ) return 0;
	else return "Directory created.";
    case "cd":
	arg = absolute_path(Directory, arg);
	if( !arg || arg == "" ) Directory = Home;
	else if( !((int)master()->valid_read(arg, this_object(), "cd")) )
	    return 0;
	else Directory = arg;
	return Directory;
    case "rm":
	arg = absolute_path(Directory, arg);
	if( !rm(arg) ) return 0;
	else return arg + " deleted.";
    case "rmdir":
	arg = absolute_path(Directory, arg);
	if( !rmdir(arg) ) return 0;
	else return arg + " deleted.";
    case "mv":
	files = explode(arg, " ");
	if( sizeof(files) != 2 ) return 0;
	files[0] = absolute_path(Directory, files[0]);
	files[1] = absolute_path(Directory, files[1]);
	if( !rename(files[0], files[1]) ) return 0;
	else return files[0] + " renamed to " + files[1] + ".";
    case "update":
	arg = absolute_path(Directory, arg);
	if( arg[<2..] == ".c" ) arg = arg[0..<3];
	if( ob = find_object(arg) ) {
	    if( !((int)ob->eventDestruct()) ) 
		return "Failed to destruct original object.";
	}
	tmp = catch(call_other(arg, "???"));
	if( tmp ) 
	    return "Error in loading object: " + replace_string(tmp, "\n", "\t");
	else return arg + ": successfully loaded.";
    }
    return 0;
}
