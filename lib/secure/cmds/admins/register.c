//	/bin/adm/_register.c
//	from the Dead Souls mudlib
//	for restricting access for certain sites
//	created by Shadowwolf@Dead Souls?
// Edit stuff added by Manny@Dead Souls 940819

#include <lib.h>
#include <daemons.h>
#include <dirs.h>

inherit LIB_DAEMON;

void end_edit(string site);
void abort();

int cmd(string str) {
    if(!archp(previous_object())) return 0;
    if(!str) {
        notify_fail("Syntax: register <site-ip>\n\n");
        return 0;
    }
    write("%^RED%^Enter all applicable information, including names, "
            "offenses and email addresses.");
    rm(DIR_TMP+"/"+this_player()->GetKeyName()+".tmp");
    this_player()->eventEdit( DIR_TMP "/" + this_player()->GetKeyName(),
            (: end_edit($(str)) :));
    return 1;
}

void end_edit(string site) {
    string *lines;
    string tmpfile, res, str;
    int x, num;

    tmpfile = read_file(DIR_TMP+"/"+this_player()->GetKeyName());
    rm(DIR_TMP+"/"+this_player()->GetKeyName());
    if(tmpfile){
        tmpfile = replace_string(tmpfile, "\n", " ");
    }
    else {
        write("Nothing written. Registration operation cancelled.");
        return;
    }
    num = sizeof(lines = explode(wrap(tmpfile, 60), "\n"));
    str = " - "+site+" placed on Registration\n";
    str += "   by "+previous_object()->GetName()+": "+
        ctime(time())+"\n";
    for(x=0; x<num; x++) 
        str +=  "      * "+lines[x]+"\n";

    if(res = catch(call_other(BANISH_D, "register_site", site))) 
        write("Error in registering site: "+res+"\n");

    log_file("watch/register", "\n" + str);
    write("%^RED%^%^BOLD%^"+site+" is now on registration!");
}

void abort() {
    rm(DIR_TMP+"/"+this_player()->GetKeyName()+".tmp");
    write("%^RED%^Site not registered!");
}

string GetHelp(){
    return "Syntax: register <site>\n\n"
        "Makes a given site needs to register their characters with "
        "law before being able to play the mud.  A reason must be given "
        "and it must be good.  In general, only law should be using "
        "this command.  And if you are not law, you better mail law. "
        "This command only works with numeric addresses, ie:\n"
        "    register 134.181.*                ok\n"
        "    register 134.181.1.12             ok\n"
        "    register orlith.bates.edu         BAD\n"
        "See also: unbanish, whobanished, whoregistered, whowatched";
}
