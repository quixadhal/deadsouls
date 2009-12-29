#include <lib.h>

inherit LIB_DAEMON;

string ReportSyntax(){
    int admin = adminp(this_player());
    string ret = "Syntax: gag [remote or local] <on or off>";
    if(admin) ret += "        gag <player> [on or off]";
    return ret;
}

mixed cmd(string str) {
    string which, type;
    object player;
    string *validtypes = ({ "remote", "local", "all" });
    int admin = adminp(this_player());
    if( !sizeof(str) )
        return ReportSyntax();
    str = lower_case(str);
    sscanf(str,"%s %s", type, which);
    if(!which && type){
        which = type;
        type = "all";
    }
    if(!type){
        if(admin){
            player = find_player(str);
        }
        if(!player) which = str;
        else which = "on";
        type = "all";
    }
    if(admin){
        if(!player) player = find_player(type);
        if(player){
            if(!which) which = "on";
            type = "all";
        }
    }
    if( which != "on" && which != "off" )
        return "You can only turn gag 'on' or 'off'.";
    if( !player && member_array(type, validtypes) == -1 )
        return "Valid gag types are: remote, local, all.";
    if(!player) player = this_player();
    if(admin){
        player->SetGagged(type, (which == "on" ? 2 : 0));
    }
    else {
        player->SetGagged(type, (which == "on"));
    }
    message("system",
            "Gag mode turned "+str+" for "+type+" channels"+
            (player != this_player() ? " for "+player->GetName() : "")+
            ".", this_player() );
    return 1;
}

string GetHelp() {
    int admin = adminp(this_player());
    return "Syntax: gag [remote | local] <on | off>\n"+
        (admin ? "        gag <PLAYER> <on | off>\n" : "")+
        "\nThis command prevents you from talking on channels.\n"
        "Good for when you're hiding or using multiple chars.\n"
        "You can gag yourself on all channels with: gag on\n"
        "You can gag yourself only on intermud channels with: gag remote on\n"
        "See also: mute, env";
}
