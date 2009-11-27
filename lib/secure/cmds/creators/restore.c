#include <lib.h>
#include <dirs.h>
#include <commands.h>

inherit LIB_DAEMON;
string globalstr1, globalstr2;
int globalint;

mixed cmd(string str) {
    string *sorted_array, *sub_arr, *new_arr, *bkarr;
    string line, s1, bkname, bkcontents;
    int number;
    object here;

    if( !str ) return "You must specify a file to restore.";
    if(str && sscanf(str,"%s %d",s1, number) > 1) str = s1;
    else false();

    new_arr = ({});
    sub_arr = ({});

    bkname = homedir(this_player())+"/bak/bk.db";

    if(!file_exists(bkname)) {
        write("The backup database file does not exist. Aborting.");
        return 1;
    }

    bkcontents = read_file(bkname);
    bkarr = explode(bkcontents,"\n");

    foreach(string zline in bkarr){
        if(!strsrch(zline,str)) new_arr += ({ zline });
    }

    if(!sizeof(new_arr) && str == "here"){
        here = environment(this_player());
        str = last_string_element(base_name(here),"/");
        foreach(string zline in bkarr){
            if(!strsrch(zline,str)) new_arr += ({ zline });
        }
    }

    if(!sizeof(new_arr)){
        write("You haven't backed up a file by that name."); 
        return 1;
    }

    if(str == "workroom.orig") {
        unguarded( (: globalint = cp(homedir(this_player())+"/bak/workroom.orig",
                        homedir(this_player())+"/workroom.c") :) );
        if(globalint) {
            write("Workroom file restored.");
            load_object(CMD_UPDATE)->cmd("-a "+homedir(this_player())+"/workroom.c");
        }
        else write("Workroom could not be restored.");
        return 1;
    }

    foreach(string zline in bkarr){
        if(strsrch(zline,str) != -1) new_arr += ({ zline });
    }

    if(!number || number < 1) number = 1;
    sorted_array = sort_array(new_arr, -1);
    if(number > sizeof(sorted_array)) number = sizeof(sorted_array);
    line = sorted_array[(number - 1)];

    sub_arr = explode(line," : ");
    if(!file_exists(REALMS_DIRS + "/" + this_player()->GetKeyName()+"/bak/"+sub_arr[0])){
        write("That file has an entry in the backup database but can't be found in your backup directory.");
        return 1;
    }
    globalstr1 = REALMS_DIRS + "/" + this_player()->GetKeyName()+"/bak/"+sub_arr[0];
    globalstr2 = sub_arr[1];
    cp(globalstr1, globalstr2);
    if(here) reload(here);
    write("File restored.");
    return 1;
}

string GetHelp(){
    return ("Syntax: restore <STRING> [NUMBER]\n\n"
            "Restores the specified file  from your bak/ directory. "
            "If you specify a number, the command will try to find "
            "the Nth newest file to restore. Examples:\n"
            "To restore the most recent version of file.c:\n"
            "restore file.c\n"
            "To restore the version you backed up before the most "
            "recent version:\n"
            "restore file.c 2\n"
            "And so on.\n"
            "\"restore here\" will attempt to restore a backup of the room "
            "you are currently standing in, and will reload it, if possible. "
            "This is the only case in which the command will automatically "
            "reload a restored object.\n"
            "See also: bk");
}
