#include <lib.h>
inherit LIB_DAEMON;

static void create() {
    daemon::create();
    SetNoClean(1);
}

int RotateLogs(){
    string *contents,*paths;
    string fooname,foopath,temppath;
    paths = ({"/log/chan/", "/log/","/log/errors/","/secure/log/adm/",
      "/secure/log/","/log/router/","/log/secure/","/log/adm/"});
    foreach(string path in paths){
	contents = get_dir(path);
	foreach(string substr in contents){
	    temppath = path+substr;
	    if( file_size(temppath) > 50000) {
		fooname=substr+"-"+timestamp();
		foopath=path+fooname;
		rename(temppath,foopath );
		if( file_size(path+"archive") != -2 ) mkdir(path+"archive");
		cp(foopath,path+"archive/"+fooname);
		rm(foopath); 
	    }
	}
    }
    return 1;
}
