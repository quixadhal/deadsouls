#include <lib.h>
#include <daemons.h>
#include <logs.h>

inherit LIB_DAEMON;
mapping SpecialLogs = ([]);

static void create() {
    daemon::create();
    SetNoClean(1);
    SpecialLogs = ([
            INTERMUD_D : ({ LOG_I3, LOG_I3ERR }),
            SERVICES_D : ({ LOG_I3, LOG_I3ERR }),
            ]);
}

varargs int RotateLogs(int force){
    string *contents,*paths;
    string fooname,foopath,temppath;
    int maxsize = 50000;
    if(force) maxsize = 0;
    paths = ({"/log/chan/", "/log/","/log/errors/","/secure/log/adm/",
            "/secure/log/","/log/router/","/log/secure/","/log/adm/"});
    foreach(string path in paths){
        contents = get_dir(path);
        foreach(string substr in contents){
            temppath = path+substr;
            if( file_size(temppath) > maxsize) {
                write_file(temppath,"\nEND-OF-LOG\n");
                fooname=substr+"-"+timestamp();
                foopath=path+fooname;
                cp(temppath,foopath);
                if( file_size(path+"archive") != -2 ) mkdir(path+"archive");
                cp(foopath,path+"archive/"+fooname);
                rm(foopath);
                write_file(temppath, "NEW_LOG "+timestamp()+"\n", 1); 
            }
        }
    }
    return 1;
}

#if 1
int LogSpecial(string file, string content){
    int ret;
    object prev = previous_object();
    string prevname, *specials = ({});
    if(!prev) return 0;
    specials = keys(SpecialLogs);
    if(!sizeof(specials)) return;
    prevname = base_name(prev);
    if(!sizeof(prevname) || member_array(prevname, specials) == -1){
        return 0;
    }
    if(member_array(file, SpecialLogs[prevname]) == -1) return 0;
    ret = unguarded( (: write_file($(file), $(content)) :) );
    return ret;
}
#endif
