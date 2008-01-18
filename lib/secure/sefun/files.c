/*    /secure/sefun/files.c
 *    from Dead Souls 3.3
 *    functions which handle file operations
 *    created by Descartes of Borg 940926
 */

string gtempname, gfilename;

int file_exists(string str) { 
    if(!str) return 0;
    return (file_size(str) > -1);
}

int directory_exists(string str) { return (file_size(str) == -2); }

string save_file(string who) {
    if( !stringp(who) ) error("Bad argument 1 to save_file().");
    who = convert_name(who);
    return (string)master()->player_save_file(who);
}

int indent_file(string filename){

    string tempname;
    tempname = "/tmp/indent."+time()+".tmp";

    write_file(tempname,"I\n");

    gtempname = tempname;
    gfilename = filename;
    if(!cp(filename,tempname+".dat")){
        write("You don't have read access to "+filename);
        return 0;
    }

    load_object("/secure/cmds/creators/lsed")->cmd(tempname+" "+tempname+".dat");

    if(!unguarded((: cp(gtempname+".dat", gfilename) :)) ){
        write("You don't have write access to "+filename);
        rm(tempname+".dat");
        rm(tempname);
        return 0;
    }

    rm(tempname+".dat");
    rm(tempname);
    return 1;
}

int mkdir_recurse(string path){
    string *path_arr = explode(path,"/");
    string agglutinate = "";

    if(directory_exists(path)) return 0;

    foreach(string element in path_arr){
        agglutinate += "/"+element;
        if(!directory_exists(agglutinate)){
            if(!mkdir(agglutinate)) return 0;
        }
    }
    return 1;
}
