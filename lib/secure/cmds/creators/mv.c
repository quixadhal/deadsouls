/*
// This file is part of the TMI Mudlib distribution.
// Please include this header if you use this code.
// Adapted by Buddha(1-18-91) from unknown source.
// Added check for existance of destination file. Plura 930120
 */

#include <lib.h>
inherit LIB_DAEMON;

int cmd(string str);
string GetHelp();


mixed cmd(string str){
    string t1, t2, *tmp;
    int force = 0;
    int dir = 0;
    if(this_player()->GetForced()) {
        write("Someone tried forcing you to mv "+str+"\n");
        return 1;
    }

    if(!str || !sizeof(str)){
        return GetHelp();
    }

    if(sscanf(str,"-f %s %s",t1,t2) == 2) {
        force = 1;
        str = t1+" "+t2;
    }
    if(!str||sscanf(str,"%s %s",t1,t2)!=2) {
        /* We should add checks for flags here. */
        return GetHelp();  
    } else {
        if(!force){
            if(file_size(t2=absolute_path(this_player()->query_cwd(),t2)) > 0)
            {
                notify_fail("mv: "+t2+" already exists.\n");
                return 0;
            }
        }
        t1=absolute_path(this_player()->query_cwd(),t1);
        t2=absolute_path(this_player()->query_cwd(),t2);
        if(directory_exists(t1)) dir = 1;
        else if(file_exists(t1)) dir = 0;
        else {
            write(t1+": no such file or directory.");
            return 1;
        }
        if(!dir && directory_exists(t2)){
            if(last(t2,1) != "/") t2 += "/";
            t2 += last_string_element(t1,"/");
        } 
        rename(t1,t2);
        if((dir && directory_exists(t2) && !directory_exists(t1)) ||
                (!dir && file_exists(t2) && !file_exists(t1)) )
            write("mv: Ok.");
        else write("mv: Failed.");
    }
    return 1;
}

string GetHelp() {
    return "Syntax: mv <file1> <file2|directory>\n\n" 
        "Renames a file or moves it into the directory specified.\n" 
        "The -f flag forces the overwriting of an existing file.\n\n"
        "Examples:\n"+
        "mv -f workroom.bak workroom.c\n"+
        "mv workroom.bak /tmp/\n\n"
        "See also: rm";
}
