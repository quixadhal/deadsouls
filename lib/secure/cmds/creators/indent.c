#include <lib.h>

inherit LIB_DAEMON;

string cmd(string str) {

    string *files, *paths;
    string where, rulefile, tmp;
    int flag;

    notify_fail("Syntax: <indent [file | * | */*]\n");

    if( !str ) return "You must specify a file or wildcard.";
    else if(str == "*") flag = 1;
    else if(str == "*/*") flag = 2;
    else str = absolute_path(this_player()->query_cwd(), str);
    if( !file_exists(str) && !flag) return "File " + str + " not found.";
    else if( !(tmp = read_file(str)) && !flag)
        return "Unable to read file " + str + ".";

    rulefile = "/tmp/"+this_player()->GetKeyName()+".indent";
    write_file(rulefile, "I",1);

    if(flag){

        where = this_player()->query_cwd();
        if(flag == 1) paths = ({ where });
        else {
            paths = ({});
            files = get_dir(this_player()->query_cwd()+"/");
            if(sizeof(files)) foreach(string s1 in files){
                if(file_size(where + "/" +s1) == -2 ){
                    paths += ({ where + "/" +s1 });
                }
            }
        }
        if(sizeof(paths)) foreach(string path in paths){
            load_object("/secure/cmds/creators/lsed")->cmd(rulefile + " "+ path + "/*.c");
            load_object("/secure/cmds/creators/lsed")->cmd(rulefile + " "+ path + "/*.h");
        }
    }

    else {
        load_object("/secure/cmds/creators/lsed")->cmd(rulefile + " "+str);
    }

    rm(rulefile);
    return "Done."; 
}

string GetHelp() {
    return ("Syntax: indent [file | * | */*]\n\n"
            "Indents a specified file, or all files in a directory."
            "\nSee also: cd, ls, mv, pwd, rm, lsed, grep");
}
