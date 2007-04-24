/*    /secure/cmds/creator/source.c
 *    from the Dead Souls LPC Library
 *    reads a file and forces you to execute each line as a command
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string file;
    string *lines;
    int queued;

    if(!args) {
	write("Please specify a file as an argument.");
	return 1;
    }

    if(grepp(args, "-q ")){
	queued = 1;
	args = replace_string(args, "-q ", "");
    }

    if(file_size(args) > 0 ) file = args;
    else if(file_size(this_player()->query_cwd()+"/"+args) > 0){
	file = this_player()->query_cwd()+"/"+args;
    }
    else {
	write("That is not a valid file.");
	return 1;
    }

    lines = explode(read_file(file),"\n");
    if(!sizeof(lines)) {
	write("Either the file is unreadable or it is empty.");
	return 1;
    }

    if(queued){
	foreach( string line in lines ){
	    if(line && line != "") write("queuing command: "+line);
	    this_player()->eventQueueCommand(line);
	}
	this_player()->eventExecuteQueuedCommands();
	write("Commands queued.");
    }
    else {
	foreach( string line in lines ){
	    if(line && line != "") write("sourcing command: "+line);
	    this_player()->eventForce(line);
	}
	write("Sourcing complete.");
    }
    return 1;

}

void help() {
    message("system", "Syntax: <source [filename]>\n\n"
      "If [filename] exists and is readable, this command "
      "will read each line of that file and force you to "
      "execute that line as if you had entered it on the "
      "command line. If there is a file called .profile "
      "in your home directory, it is automatically executed "
      "in this way each time you log in.\n\n"
      "If a set of commands need to occur with some time between "
      "them (for example, to avois the MAX_COMMANDS_PER_SECOND limit) "
      "use the -q flag. For example:\n source -q /secure/scripts/crat2.src\n\n"
      "", this_player());
}
