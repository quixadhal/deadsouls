#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string which, type;
    string *validtypes = ({ "remote", "local", "all" });
    if( !sizeof(str) )
        return "Syntax: gag <on or off> [remote or local]";
    str = lower_case(str);
    sscanf(str,"%s %s",which, type);
    if(!type) type = "all";
    else str = which;
    if( str != "on" && str != "off" )
        return "You can only turn gag 'on' or 'off'.";
    if( member_array(type, validtypes) == -1 )
        return "Valid gag types are: remote, local, all.";
    this_player()->SetGagged(type, (str == "on"));
    message("system",
            "Gag mode turned "+str+" for "+type+" channels.",
            this_player() );
    return 1;
}

void help() {
    message("help",
            "Syntax: gag on [ remote | local ]\n"
            "        gag off [ remote | local ]\n\n"
            "This command prevents you from talking on channels.\n"
            "Good for when you're hiding or using multiple chars.\n"
            "You can gag yourself on all channels with: gag on\n"
            "You can gag yourself only on intermud channels with: gag remote on\n"
            "See also: mute, env\n\n",
            this_player() );
}
