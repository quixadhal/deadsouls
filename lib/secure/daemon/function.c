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
mixed *Functions = ({});
mapping LCFunctions = ([]);

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
    if(query_os_type() == "windows" || !(MASTER_D->GetPerfOK())){
        return 0;
    }
    validate();
    foreach(string sub in arr){
        load_object("/secure/cmds/creators/showfuns")->cmd(sub);
    }
    return 1;
}

mixed ReadFuns(string str){
    int interval = 0;
    string *subfiles = ({});
    if(query_os_type() == "windows" || !(MASTER_D->GetPerfOK())){
        return 0;
    }
    validate();
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
    restore_object(SaveFuns);
    if(query_os_type() == "windows" || !(MASTER_D->GetPerfOK())){
        return;
    }
    validate();
    call_out((: ReadFuns,"/lib/" :), 1);
    call_out((: ReadFuns,"/secure/sefun/" :), 30);
    call_out((: ReadFuns,"/secure/lib/" :), 60);
    call_out((: ReadFuns,"/verbs" :), 90);
    call_out((: ReadFuns,"/spells" :), 120);
    set_heart_beat(1);
}

varargs int ReceiveFunctionData(string file, string funs, int fsize, mixed fs){
    FunctionCache[file] = funs;
    FileSize[file] = fsize;
    if(sizeof(fs) && arrayp(fs)){
        if(!Functions) Functions = ({});
        if(!LCFunctions) LCFunctions = ([]);
        if((sizeof(Functions) + sizeof(fs)) < 64000){
            Functions += fs;
            Functions = distinct_array(Functions);
            foreach(string fun in fs){
                string lname = lower_case(fun);
                if(!LCFunctions[lname]) LCFunctions[lname] = ({});
                LCFunctions[lname] += ({ fun });
                LCFunctions[lname] = distinct_array(LCFunctions[lname]);
            }
        }
    }
    return fsize;
}

varargs mixed GetFunctions(string str){
    if(!str) return copy(Functions);
    if(this_player() && (!FileSize[str] || stat(str)[0] != FileSize[str])) return 0;
    return FunctionCache[str];
}

varargs mixed GetLCFunctions(string str, int subs){
    string *ret = ({}), *l_funs, *e_funs, *se_funs;
    if(!LCFunctions) LCFunctions = ([]);
    if(!str) return copy(LCFunctions);
    str = lower_case(str);
    if(!subs){
        return LCFunctions[str];
    }
    l_funs = regexp(keys(LCFunctions), str);
    foreach(mixed foo in l_funs){
        if(sizeof(LCFunctions[foo]))
            ret += LCFunctions[foo];
    }
    ret = distinct_array(ret);
    e_funs = regexp(efuns(), str);
    se_funs = regexp(sefuns(), str);
    if(sizeof(e_funs) + sizeof(ret) < 64000) ret += e_funs;
    if(sizeof(se_funs) + sizeof(ret) < 64000) ret += se_funs;
    ret = sort_array(distinct_array(ret), 1);
    return ret;
}

int eventDestruct(){
    unguarded( (: save_object(SaveFuns) :) );
    return ::eventDestruct();
}

varargs mixed GetInstances(string str, string where){
    string cooked_list = "";
    if(!where || ! sizeof(where)) where = "/lib/";
    if(!sizeof(FunctionCache)) return 0;
    foreach(string key, string val in FunctionCache){
        string funex;
        object cle;
        if(grepp(val, str) && !strsrch(key,where)){
            if(!sizeof(key) || catch(cle = load_object(key)) || !cle) continue;
            funex = function_exists(str, cle,1);
            if(funex && !grepp(cooked_list,funex+"\n")){
                cooked_list += funex+"\n";
            }
        }
    }
    return cooked_list;
}
