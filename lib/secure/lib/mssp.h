#ifdef __DGD__
#define keys(X) map_indices(X)
#endif

string mssp_reply(){
    mapping reply, notes;
    string *k;
    int i;
    string ret;

    reply = ([
#ifdef __DSLIB__
            "NAME"             : mud_name(),
            "PLAYERS"          : itoa(sizeof(users())),
            "UPTIME"           : itoa(time() - uptime()),
            "PORT"             : itoa(query_host_port()),
            "CODEBASE"         : mudlib()+" "+mudlib_version(),
            "CONTACT"          : ADMIN_EMAIL,
            "WHO"              : implode(map(filter(users(),
            (: (environment($1) && !($1->GetInvis())) :)), 
            (: $1->GetCapName():)), "\t"),
#else
            "NAME"             : "Generic LP Mud",
            "PLAYERS"          : "0",
            "UPTIME"           : "0",
            "PORT"             : "8000",
            "CODEBASE"         : "Dead Souls 2.9",
            "CONTACT"          : "cratylus@comcast.net",
#endif
            "HOSTNAME"         : "dead-souls.net",
            "DESCRIPTION"      : "A fun mud!",
            "CREATED"          : "2006",
            "ICON"             : "http://lpmuds.net/favicon.ico",
            "IP"               : "66.197.134.110",
            "LANGUAGE"         : "English",
            "LOCATION"         : "United States",
            "MINIMUM AGE"      : "13",
            "WEBSITE"          : "http://dead-souls.net",
            "FAMILY"           : "LPMud",
            "GENRE"            : "Adventure",
            "SUBGENRE"         : "Adventure",
            "GAMEPLAY"         : "Adventure",
            "GAMESYSTEM"       : "Custom",
            "INTERMUD"         : "I3\tIMC2",
            "STATUS"           : "ALPHA",
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
            "PORT-NOTES"             : "player port is 6666",
            "CODEBASE-NOTES"         : "release a16",
            "FAMILY-NOTES"           : "descendant of Nightmare",
            "STATUS-NOTES"           : "currently a code demo mud",
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

