#include <lib.h>
#include <save.h>

inherit LIB_DAEMON;

mapping Books = ([]);

static string *globalheader;
static string globalstr, globalstr2;
static string SaveFile = save_file(SAVE_BOOKS);

static void create() {
    daemon::create();
    if(unguarded( (: file_exists(SaveFile) :) ) ){
        RestoreObject(SaveFile);
    }
    if(!Books) Books = ([]);
    SetNoClean(1);
    SaveObject(SaveFile);
}

int eventDestruct(){
    SaveObject(SaveFile);
    return daemon::eventDestruct();
}

string SetBookTitle(string path, string title){
    if(!Books[path]) Books[path] = ([]);
    Books[path]["title"] = title;
    return title;
}

string GetBookTitle(string path){
    if(Books[path] && Books[path]["title"]) return Books[path]["title"];
    return "Untitled";
}

string array ExtractChapterName(string path){
    string *header;
    string chap, foo, line;
    int num;

    if(!path) return ({""});
    if(first(last_string_element(path,"/"),1) == ".") return ({""});

    if(path && file_size(path) > 0){
        globalstr = "";
        globalstr2 = path;
        if(!file_exists(globalstr2)) globalstr2 == "";
        if(sizeof(globalstr2)) unguarded( (: globalstr = read_file(globalstr2, 1, 1) :) ); 
        line = globalstr;
    }
    if(!line) chap = "unknown";
    if(!strsrch(line,"Chapter")) line=replace_string(line,"Chapter","chapter",1);
    if(sscanf(line,"chapter %s %s", chap, foo) != 2) chap = "unknown";
    if(sscanf(chap,"%d",num) != 1) num = 0;
    header = ({ "chapter "+chap, "chapter "+cardinal(num), chap });
    return ({ copy(header), foo });
}

mixed *LoadChapters(string Source){
    string this_path;
    if(!Books[Source]) Books[Source] = ([]);
    if(!Books[Source]["object"]){
        if(previous_object() && base_name(previous_object()) != LIB_BOOK)
            Books[Source]["object"] = base_name(previous_object());
        else Books[Source]["object"] = "null";
    }
    if(!Books[Source]["items"]) Books[Source]["items"] = ([]);
    if(!Books[Source]["reads"]) Books[Source]["reads"] = ([]);

    foreach(string chap in get_dir(Source+"/")){
        mixed *statinfo;
        this_path = Source+"/"+chap;
        statinfo = stat(this_path);
        if(sizeof(statinfo) != 3 || !intp(statinfo[1]) ||
                (Books[Source][this_path] && Books[Source][this_path] 
                 == statinfo[1])){
            continue;
        }
        else {
            Books[Source][this_path] = statinfo[1]; 
            Books[Source]["index"] = 0;
        }

        globalheader = ExtractChapterName(this_path);

        if(sizeof(globalheader) > 1){
            Books[Source]["items"][globalheader[0]] = globalheader[1];
            globalstr = this_path;
            globalstr2 = Source;
            unguarded( (: Books[globalstr2]["reads"][globalheader[0]] = 
                        read_file(globalstr) :) ); 
        }
    }
    this_object()->LoadBookIndex(Source);
    return copy(Books[Source]["items"]);
}

string LoadBookIndex(string Source){
    int i, items;
    string chapter_index;
    if(!Books[Source]) return 0;
    if(Books[Source]["index"]) return Books[Source]["index"];
    if(!Books[Source]["title"]) Books[Source]["title"] = "Untitled";
    items = sizeof(Books[Source]["items"])+1;
    chapter_index = "\t\t"+Books[Source]["title"]+"\n\n";
    for(i=1; i<items; i++){
        foreach(mixed key, mixed val in Books[Source]["items"]){
            if(member_array("chapter "+i, key) != -1){
                chapter_index += "Chapter "+i+":\t";
                chapter_index += val+"\n";
                break;
            }
        }
    }
    Books[Source]["index"] = chapter_index;
    return chapter_index;    
}

string GetBookIndex(string Source){
    if(!Books[Source] || !Books[Source]["index"]) return "No index.";
    return Books[Source]["index"];
}

string ReturnRead(string file, string what){
    string ret;
    if(Books[file] && Books[file]["reads"]){
        foreach(mixed key, mixed val in Books[file]["reads"]){
            if(member_array(what, key) != -1){
                ret = val;
                break;
            }
        }
        if(ret) return ret;
    }
    return "There is no such thing to read.";
}

string GetBookRead(string name, mixed arg){
    if(arrayp(arg) && sizeof(arg)) arg = arg[0];
    if(!sizeof(arg) || !Books[name] || !Books[name]["reads"]){
        return "There is no such thing to read there.";
    }
    foreach(mixed key, mixed val in Books[name]["reads"]){
        if(member_array(arg, key) != -1) return val;
    }
    return "There is no such thing to read there.";
}

int zero(){
    if(this_player() && archp(this_player())){
        Books = ([]);
    }
    return 0;
}

mixed GetBooks() { return copy(Books); }

