//      /bin/dev/_rm.c
//      from the Dead Souls Mudlib
//      the command to rm files (wild cards without prompt!!!!, be careful)
//      created by Descartes of Borg 02 august 1993

#include <lib.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string *files;
    string tmp;
    object ob;
    int i;

    if(!str) {
        notify_fail("Correct syntax: <rm [file]>\n");
        return 0;
    }
    if((ob=previous_object())->GetForced()) {
        write("Someone attempted to force you to rm "+str+".");
        return 1;
    }
    tmp = absolute_path((string)ob->query_cwd(), str);
    if(!archp(ob) || !sizeof(files = (string *)ob->wild_card(tmp)))
      files = ({ tmp });
    if(!(i=sizeof(files))) {
        message("system", "File "+tmp+" does not exist.", this_player());
        return 1;
    }
    while(i--) {
        switch(file_size(files[i])) {
            case -1: message("system", "File "+files[i]+" does not exist.",
			     this_player()); break;
            case -2: message("system", files[i]+" is a directory.", 
			     this_player()); break;
            default: message("system", rm(files[i]) ? files[i]+": Ok." :
			     files[i]+": Permission denied.", this_player());
	      break;
        }
    }
    return 1;
}

void help() {
    message("help", "Syntax: <rm [file]>\n\n"
	    "The command deletes the file named.  Once deleted, the file "
	    "cannot be recovered.", this_player());
    if(archp(this_player()))
      message("help", "Note for arches: wild cards work with this command, "
	      "but there is no prompt, so be VERY CAREFULL!!!!", 
	      this_player());
}
