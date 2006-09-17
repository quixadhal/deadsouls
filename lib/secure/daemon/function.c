#include <lib.h>
#include <config.h>
#include <daemons.h>
#include <runtime_config.h>

inherit LIB_DAEMON;

static string globaltmp;
static string *files = ({});
static string SaveFile = "/secure/save/functions.o";
static int seeking = 0;
static int count = 0;
mapping FileSize = ([]);
mapping FunctionCache = ([]);

static private void validate() {
    if(!this_player()) return 0;
    if( !((int)master()->valid_apply(({ "ASSIST" }))) )
	error("Illegal attempt access FUNCTIONS_D: "+get_stack()+" "+identify(previous_object(-1)));
}

void heart_beat(){
    if(seeking){
	foreach(mixed arr in call_out_info()){
	    if(arr[0] == this_object()){
		return ;
	    }
	}
	//tc("functions load complete");
	save_object(SaveFile);
	seeking = 0;
    }
    count++;
    if(count > 700){
	save_object(SaveFile);
	count = 0;
    }
}

mixed SendFiles(string *arr){
    foreach(string sub in arr){
	load_object("/secure/cmds/creators/showfuns")->cmd(sub);
    }
    return 1;
}

mixed ReadFuns(string str){
    int interval = 0;
    string *subfiles = ({});
    validate();

    if(seeking){
	//tc("Already seeking");
	//return 1;
    }

    seeking = 1;
    if(!str || !sizeof(str)) str = "/lib";

    files = FILE_D->GetFiles();
    globaltmp = str;
    files = filter(files, (: (!strsrch($1, globaltmp) && last($1,2) == ".c" ) :) );
    //tc("files: "+implode(files, ", "),"red");
    files = filter(files, (: (!sizeof(FunctionCache[$1]) || stat($1)[0] != FileSize[$1]) :) ); 
    //tc("files: "+implode(files, ", "),"yellow");
    //foreach(string file in files){
    //tc("FileSize["+file+"]: "+FileSize[file]);
    //tc("Actual file size: "+stat(file)[0]+"\n","yellow");
    //}
    while(sizeof(files) > 0){
	interval++;
	subfiles = ({ files[0] }) ;
	files -= subfiles;
	call_out( (: SendFiles, subfiles :), interval);
    }
    return 1;
}

static void create() {
    daemon::create();
    if(!file_exists(SaveFile)) save_object(SaveFile);
    else restore_object(SaveFile);
    call_out((: ReadFuns,"/lib/" :), 1);
    call_out((: ReadFuns,"/secure/sefun/" :), 30);
    call_out((: ReadFuns,"/secure/lib/" :), 60);
    call_out((: ReadFuns,"/verbs" :), 90);
    call_out((: ReadFuns,"/spells" :), 120);
    set_heart_beat(1);
}

int ReceiveFunctionData(string file, string funs, int fsize){
    tc("file: "+file+", size: "+fsize,"cyan");
    FunctionCache[file] = funs;
    FileSize[file] = fsize;
    return fsize;
}

mixed GetFunctions(string str){
    if(this_player() && (!FileSize[str] || stat(str)[0] != FileSize[str])) return 0;
    return FunctionCache[str];
}

int eventDestruct(){
    save_object(SaveFile);
    return ::eventDestruct();
}

varargs mixed GetInstances(string str, string where){
    string cooked_list = "";
    if(!where || ! sizeof(where)) where = "/lib/";
    //tc("where: "+where);
    foreach(string key, string val in FunctionCache){
	string funex;
	if(grepp(val, str) && !strsrch(key,where)){
	    funex = function_exists(str,load_object(key),1);
	    if(funex && !grepp(cooked_list,funex+"\n")){
		cooked_list += funex+"\n";
	    }
	}
    }
    return cooked_list;
}
