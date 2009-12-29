#include <lib.h>
#include <dirs.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string *file_arr;
    string tmp, filename, str2, orig;
    int time;
    object ob;

    orig = str;
    if( !str ) return "You must specify a file to backup.";
    else str = absolute_path(this_player()->query_cwd(), str);
    if( !file_exists(str) ) str2 = str+".c";
    if( !file_exists(str) && !file_exists(str2)) {
        if(str == "here" || str == "room") ob = environment(this_player());
        else {
            ob = present(orig,environment(this_player()));
            if(!ob) ob = present(orig, this_player());
            if(!ob) return "File not found.";
        }
        str2 = base_name(ob)+".c";
    }
    if(str2 && !file_exists(str2)) return "File unfound.";
    else if(str2) str = str2;
    if( !(tmp = read_file(str)) )
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

string GetHelp(){
    return ("Syntax: bk [file]\n"
            "        bk here\n\n"
            "Copies the specified file into your bak/ directory, and "
            "appends a unique identifying number to the backup file name.\n"
            "See also: restore, find, reload");
}
