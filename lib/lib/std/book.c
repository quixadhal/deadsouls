#include <lib.h>

inherit LIB_ITEM;

string array chapters, globalheader;
string Title = "Generic Book";
string Source = "/obj/book_source";
static mapping BookItems = ([]);
static mapping BookReads = ([]);

string globalstr, globalstr2;

void LoadBook();

void SetTitle(string title){
    if(title) Title = title;
}

mixed GetTitle(){
    if(Title) return Title;
    else return 0;
}

void SetSource(string source){
    if(source && directory_exists(source)) Source = source;
}

string GetSource(){
    if(Source) return Source;
    else return "";
}

string array eventExtractChapterName(string path){
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
    else if(sscanf(line,"chapter %s %s", chap, foo) != 2) chap = "unknown";
    if(sscanf(chap,"%d",num) != 1) num = 0;
    header = ({ "chapter "+chap, "chapter "+cardinal(num), chap });
    return ({ header,  foo   });
}

mapping *eventLoadChapters(){
    string this_path;

    foreach(string chap in get_dir(Source+"/")){
	this_path = Source+"/"+chap;
	globalheader = eventExtractChapterName(this_path);

	if(sizeof(globalheader) > 1){
	    BookItems[globalheader[0]] = globalheader[1];
	    globalstr = this_path;
	    unguarded( (: BookReads[globalheader[0]] = read_file(globalstr) :) ); 
	}
    }
    return ({ copy(BookItems), copy(BookReads) });
}

string eventLoadIndex(){
    int i;
    string chapter_index = "\t\t"+this_object()->GetTitle()+"\n\n";
    for(i=1; i<300; i++){
	if(this_object()->GetItem("chapter "+i))  {
	    chapter_index += "Chapter "+i+":\t";
	    chapter_index += this_object()->GetItem("chapter "+i)+"";
	}
    }
    return chapter_index;
}


void create(){
    ::create();
    call_out( (: LoadBook :), 1);
    SetDefaultRead("Try \"read chapter 1 in book\" or "
      "\"read index in book\"");
}

void init(){
    ::init();
}

void LoadBook(){
    mapping *map_array = this_object()->eventLoadChapters();
    SetItems(map_array[0]);
    SetReads(map_array[1]);
    AddItem( "index", "This is a list of the chapters in this book.");
    SetRead("index", this_object()->eventLoadIndex());
}
