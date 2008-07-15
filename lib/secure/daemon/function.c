#include <lib.h>
#include <config.h>
#include <daemons.h>
#include <runtime_config.h>

inherit LIB_DAEMON;

static string globaltmp;
static string *files = ({});
static string SaveFuns = "/secure/save/functions.o";
static int seeking = 0;
static int count = 0;
mapping FileSize = ([]);
mapping FunctionCache = ([]);

static private void validate() {
    if(!this_player()) return 0;
    if( !((int)master()->valid_apply(({ "ASSIST" }))) )
        error("Illegal attempt access FUNCTIONS_D: "+get_stack()+" "+identify(previous_object(-1)));
    if(query_os_type() == "windows"){
        error("The functions daemon has been disabled for your mud "+
          "because it is running on windows. Intensive file operations "+
          "in windows are not yet supported on Dead Souls.");
    }
}

void heart_beat(){
    if(seeking){
        foreach(mixed arr in call_out_info()){
            if(arr[0] == this_object()){
                return ;
            }
        }
        unguarded( (: save_object(SaveFuns) :) );
        seeking = 0;
    }
    count++;
    if(count > 700){
        unguarded( (: save_object(SaveFuns) :) );
        count = 0;
    }
}

mixed SendFiles(string *arr){
    validate();
    if(query_os_type() == "windows") return 0;
    foreach(string sub in arr){
        load_object("/secure/cmds/creators/showfuns")->cmd(sub);
    }
    return 1;
}

mixed ReadFuns(string str){
    int interval = 0;
    string *subfiles = ({});
    validate();
    if(query_os_type() == "windows") return 0;
    seeking = 1;
    if(!str || !sizeof(str)) str = "/lib";

    files = FILE_D->GetFiles();
    globaltmp = str;
    files = filter(files, (: (!strsrch($1, globaltmp) && last($1,2) == ".c" ) :) );
    files = filter(files, (: (!sizeof(FunctionCache[$1]) || 
          ((sizeof(stat($1)) > 1) ? stat($1)[0] : FileSize[$1]) 
          != FileSize[$1]) :) ); 
    foreach(string file in files){
    }
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
    if(query_os_type() == "windows") return;
    if(!file_exists(SaveFuns)){
        unguarded( (: save_object(SaveFuns) :) );
    }
    else restore_object(SaveFuns);
    call_out((: ReadFuns,"/lib/" :), 1);
    call_out((: ReadFuns,"/secure/sefun/" :), 30);
    call_out((: ReadFuns,"/secure/lib/" :), 60);
    call_out((: ReadFuns,"/verbs" :), 90);
    call_out((: ReadFuns,"/spells" :), 120);
    set_heart_beat(1);
}

int ReceiveFunctionData(string file, string funs, int fsize){
    FunctionCache[file] = funs;
    FileSize[file] = fsize;
    return fsize;
}

mixed GetFunctions(string str){
    if(this_player() && (!FileSize[str] || stat(str)[0] != FileSize[str])) return 0;
    return FunctionCache[str];
}

int eventDestruct(){
    unguarded( (: save_object(SaveFuns) :) );
    return ::eventDestruct();
}

varargs mixed GetInstances(string str, string where){
    string cooked_list = "";
    if(!where || ! sizeof(where)) where = "/lib/";
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
