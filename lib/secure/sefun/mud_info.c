//      /adm/simul_efun/mud_info.c
//      from the Dead Souls Mudlib
//      gives information simul_efuns about the mud
//      created by Descartes of Borg

#include <config.h>

string mud_name() { return MUD_NAME; }

string admin_email() { return ADMIN_EMAIL; }

#ifdef MUDOS_VERSION
string version() { return MUDOS_VERSION; }
#endif // MUDOS_VERSION

#ifdef __VERSION__
string version() { return __VERSION__; }
#endif // __VERSION__

#ifdef MUDOS_ARCH
string architecture() { return MUDOS_ARCH; }
#endif // MUDOS_ARCH

#ifdef __ARCH__
string architecture() { return __ARCH__; }
#endif // __ARCH__

string mudlib() { return "Dead Souls"; }

string mudlib_version() { return "2.1a13"; }

int query_host_port() { return __PORT__; }

string query_os_type(){
    string config_file, s1, s2, s3;
    if(function_exists("architecture",load_object("/secure/sefun/sefun"))){
	if(architecture() == "Cygwin-32") return "windows";
	if(!file_exists("/secure/cfg/mudos.cfg")) return architecture();
    }
    if(!file_exists("/secure/cfg/mudos.cfg")) return "";
    config_file = read_matching_line("/secure/cfg/mudos.cfg","mudlib directory :");
    if(!config_file) return "";
    if(sscanf(config_file,"%s:%s:%s",s1,s2,s3) == 3){
	return "windows";
    }
    else return "unix";
}
