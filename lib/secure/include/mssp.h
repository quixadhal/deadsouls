#include <network.h>
mapping reply, notes;

string mssp_reply(){
    string *k;
    int i;
    string ret, ip = query_intermud_ip();
    string intermud = "";
   
    if(find_object(INTERMUD_D)) intermud += "i3";
    if(find_object(IMC2_D)){
        if(sizeof(intermud)) intermud += "\t";
        intermud += "IMC2";
    }
    if(!sizeof(intermud)) intermud = "0";

    reply = ([
            "NAME"             : mud_name(),
            "PLAYERS"          : itoa(sizeof(users())),
            "UPTIME"           : itoa(time() - uptime()),
            "PORT"             : itoa(query_host_port()),
            "CODEBASE"         : mudlib()+" "+mudlib_version(),
            "CONTACT"          : ADMIN_EMAIL,
            "WHO"              : implode(map(filter(users(),
            (: (environment($1) && !($1->GetInvis())) :)), 
            (: $1->GetCapName():)), "\t"),
            "HOSTNAME"         : ip,
            "DESCRIPTION"      : "A fun mud!",
            "CREATED"          : itoa(local_time()[5]),
            "ICON"             : "http://"+ip+":"+PORT_HTTP+"/favicon.ico",
            "IP"               : ip,
            "LANGUAGE"         : "English",
            "LOCATION"         : "United States",
            "MINIMUM AGE"      : "13",
            "WEBSITE"          : "http://"+ip+":"+PORT_HTTP,
            "FAMILY"           : "LPMud",
            "GENRE"            : "Adventure",
            "SUBGENRE"         : "Adventure",
            "GAMEPLAY"         : "Adventure",
            "GAMESYSTEM"       : "Custom",
            "INTERMUD"         : intermud,
            "STATUS"           : "Alpha",
            "AREAS"            : "7",
            "HELPFILES"        : "N/A",
            "MOBILES"          : "150",
            "OBJECTS"          : "300",
            "ROOMS"            : "300",
            "RESETS"           : "N/A",
            "MUDPROGS"         : "N/A",
            "MUDTRIGS"         : "N/A",
            "CLASSES"          : "5",
            "LEVELS"           : "300",
            "RACES"            : "72",
            "SKILLS"           : "N/A",
            "DBSIZE"           : "N/A",
            "EXITS"            : "N/A",
            "EXTRA DESCRIPTIONS" : "N/A",
            "ADULT MATERIAL"   : "0",
            "MULTICLASSING"    : "0",
            "PLAYER CITIES"    : "0",
            "PLAYER CLANS"     : "0",
            "PLAYER CRAFTING"  : "0",
            "PLAYER GUILDS"    : "0",
            "EQUIPMENT SYSTEM" : "N/A",
            "MULTIPLAYING"     : "Full",
            "PLAYERKILLING"    : "Full",
            "QUEST SYSTEM"     : "Automated\tIntegrated",
            "ROLEPLAYING"      : "None",
            "TRAINING SYSTEM"  : "Skill",
            "WORLD ORIGINALITY": "Mostly Original",
            "ANSI"             : "1",
            "MCCP"             : "0",
            "SSL"              : "0",
            "MCP"              : "0",
            "MSP"              : "0",
            "MXP"              : "0",
            "PUEBLO"           : "0",
            "VT100"            : "0",
            "XTERM 256 COLORS" : "0",
            "PAY TO PLAY"      : "0",
            "PAY FOR PERKS"    : "0",
            "HIRING BUILDERS"  : "1",
            "HIRING CODERS"    : "1",
            ]);

    notes = ([
            "FAMILY-NOTES"           : "descendant of Nightmare",
            "HELPFILES-NOTES"        : "each command, and hundreds of other docs",
            "MOBILES-NOTES"          : "npc's can be cloned, so there can be thousands",
            "OBJECTS-NOTES"          : "objects can be cloned, so there can be thousands",
            "ROOMS-NOTES"            : "areas have generated rooms, could be millions",
            "RESETS-NOTES"           : "doesn't apply, LPMud",
            "MUDPROGS-NOTES"         : "doesn't apply, LPMud",
            "MUDTRIGS-NOTES"         : "doesn't apply, LPMud",
            "RACES-NOTES"            : "not all are playable",
            "SKILLS-NOTES"           : "many, but not like Diku",
            ]);

    ret = "MSSP-REPLY-START\r\n";
    k = keys(reply);

#ifdef __DSLIB__
    k = sort_array(k, -1);
#endif

    for(i = (sizeof(k) - 1); i > -1; i--){
        ret += k[i];
        ret += "\t";
        ret += reply[k[i]];
        ret += "\r\n";
    }

    k = keys(notes);

#ifdef __DSLIB__
    k = sort_array(k, -1);
#endif

    for(i = (sizeof(k) - 1); i > -1; i--){
        ret += k[i];
        ret += "\t";
        ret += notes[k[i]];
        ret += "\r\n";
    }

    ret += "MSSP-REPLY-END\r\n";

    return ret;
}

mapping mssp_map(){
    string text = mssp_reply();
    mapping ret = ([]), tmp = add_maps(reply, notes);
    foreach(mixed key, mixed val in tmp){
        if(undefinedp(val)) continue;
        if(grepp(val, "\t")){
            ret[key] = explode(val, "\t");
        }
        else ret[key] = val;
    }
    return ret;
}
    
