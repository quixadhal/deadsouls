//      /adm/simul_efun/mud_info.c
//      from the Dead Souls Mudlib
//      gives information simul_efuns about the mud
//      created by Descartes of Borg

#include <daemons.h>

string mud_name(){ 
    string ret;
    if(uptime() < 60) return MUD_NAME; 
    if(!(ret = MASTER_D->GetMudName())) return MUD_NAME;
    return ret;
}

varargs string imc2_mud_name(string name){ 
    string mudname;

    if(name) mudname = name;
    else mudname = mud_name();

    return replace_string(mudname," ","_");
}

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

string mudlib_version() { return "3.7a6"; }

int query_host_port() { return __PORT__; }

string query_os_type(){
    string config_file, s1, s2, s3;
    if(function_exists("architecture",load_object("/secure/sefun/sefun"))){
        string arch = lower_case(architecture());
        if(grepp(arch,"windows") || grepp(arch,"cygwin")) return "windows";
        else return "unix";
    }
    if(!file_exists("/secure/cfg/mudos.cfg")) return "unknown";
    config_file = read_matching_line("/secure/cfg/mudos.cfg","mudlib directory :");
    if(!config_file) return "";
    if(sscanf(config_file,"%s:%s:%s",s1,s2,s3) == 3){
        return "windows";
    }
    else return "unix";
}

int query_windows(){
    string arch = lower_case(architecture());
#ifdef __WIN32__
    return 1;
#else
    if(grepp(arch,"windows") || grepp(arch,"cygwin")) return 1;
#endif
    return 0;
}

    string query_intermud_ip(){
        if(INTERMUD_D->GetMudList()[mud_name()])
            return INTERMUD_D->GetMudList()[mud_name()][1];
        else return "";
    }
