#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string which, type;
    string *validtypes = ({ "remote", "local", "all" });
    if( !sizeof(str) )
        return "Syntax: mute <on or off> [remote or local]";
    str = lower_case(str);
    sscanf(str,"%s %s",which, type);
    if(!type) type = "all";
    else str = which;
    if( str != "on" && str != "off" )
        return "You can only turn mute 'on' or 'off'.";
    if( member_array(type, validtypes) == -1 )
        return "Valid mute types are: remote, local, all.";
    this_player()->SetMuted(type, (str == "on"));
    message("system",
            "Mute mode turned "+str+" for "+type+" channels.",
            this_player() );
    return 1;
}

string GetHelp() {
    return ("Syntax: mute on [ remote | local ]\n"
            "        mute off [ remote | local ]\n\n"
            "This command allows you to avoid or listen to channel messages.\n"
            "You can mute all channels with: mute on\n"
            "You can mute only intermud channels with: mute remote on\n"
            "See also: gag, env");
}
