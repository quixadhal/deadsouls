/*    /secure/cmds/creator/source.c
 *    from the Dead Souls LPC Library
 *    reads a file and forces you to execute each line as a command
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string file, contents,flags,tmpargs;
    string *tmplines, *lines = ({});
    int queued, gauged;

    if(!args) {
        write("Please specify a file as an argument.");
        return 1;
    }
    if(sscanf(args,"-%s %s",flags, tmpargs) == 2){
        args = tmpargs;
    }
    else flags = "";

    if(grepp(flags, "q")){
        queued = 1;
    }

    if(grepp(flags, "g")){
        if(query_os_type() != "windows"){
            gauged = 1;
        }
    }

    if(file_size(args) > 0 ) file = args;
    else if(file_size(this_player()->query_cwd()+"/"+args) > 0){
        file = this_player()->query_cwd()+"/"+args;
    }
    else {
        write("That is not a valid file.");
        return 1;
    }

    if(!strsrch(file,"/secure/scripts/") ||
            !strsrch(file,user_path(this_player())) ){
        write("Processing "+file+"...");
    }
    else {
        write(file+" is not in a permitted script directory. Aborting.");
        return 1;
    }

    contents = read_file(file);

    if(!contents || !sizeof(contents)) {
        write("Either the file is unreadable or it is empty.");
        return 1;
    }

    tmplines = explode(contents,"\n");

    foreach(string line in tmplines){
        if(!strsrch(line,"title ") || !strsrch(line,"describe ")){
        }
        else {
            line = replace_string(line,"$N",this_player()->GetKeyName());
        }
        if(strsrch(line,"#")) lines += ({ (gauged ? "gauge " + line : line) });
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

string GetHelp() {
    return ("Syntax: source [-gq] <filename>\n\n"
            "If <filename> exists and is readable, this command "
            "will read each line of that file and force you to "
            "execute that line as if you had entered it on the "
            "command line. If there is a file called .profile "
            "in your home directory, it is automatically executed "
            "in this way each time you log in.\n\n"
            "To include system load output, use the -g flag.\n\n"
            "If a set of commands need to occur with some time between "
            "them (for example, to avoid the MAX_COMMANDS_PER_SECOND limit) "
            "use the -q flag. For example:\n "
            "source -q /secure/scripts/crat2.src");
}
