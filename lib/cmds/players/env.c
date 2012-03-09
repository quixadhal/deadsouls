#include <lib.h>

inherit LIB_DAEMON;

string GetMuted(){
    string ret;
    if(this_player()->GetMutedType("local")){
        ret = "local: %^RED%^MUTED%^RESET%^";
    }
    else {
        ret = "local: %^GREEN%^listening%^RESET%^";
    }
    if(this_player()->GetMutedType("remote")){
        ret += ", remote: %^RED%^MUTED%^RESET%^";
    }
    else {
        ret += ", remote: %^GREEN%^listening%^RESET%^";
    }
    return ret;
}

string GetGagged(){
    string ret;
    if(this_player()->GetGaggedType("local")){
        ret = "local: %^RED%^GAGGED%^RESET%^";
    }
    else {
        ret = "local: %^GREEN%^talking%^RESET%^";
    }
    if(this_player()->GetGaggedType("remote")){
        ret += ", remote: %^RED%^GAGGED%^RESET%^";
    }
    else {
        ret += ", remote: %^GREEN%^talking%^RESET%^";
    }
    return ret;
}

mixed cmd(string args) {
    object player = this_player();
    mixed replee = player->GetProperty("reply");
    string ret = "";
    string *tmp;
    ret += "Screen: \t\t"+identify(this_player()->GetScreen())+"\n";
    ret += "Terminal: \t\t"+this_player()->GetTerminal()+"\n";
    ret += "Brief mode: \t\t"+ ( (this_player()->GetBriefMode()) ? "on" : "off" )+"\n";
    ret += "Cursefilter: \t\t"+ ( (this_player()->GetParanoia("cursefilter")) ? "on" : "off" )+"\n";
    ret += "Channel message colors: "+ ( (this_player()->GetNoChanColors()) ? "off" : "on" )+"\n";
    ret += "Playerkiller mode: \t"+ ( (this_player()->GetPK()) ? "on" : "off" )+"\n";
    ret += "Mute mode: \t\t"+ GetMuted()+" \n";
    ret += "Gag mode: \t\t"+ GetGagged()+" \n";
    ret += "Wimpy mode: \t\t"+ ( (this_player()->GetWimpy()) ? "on" : "off" )+"\n";
#if efun_defined(compressedp)
    ret += "MCCP mode: \t\t"+(compressedp(this_player()) ? "on" : "off")+"\n";
#endif
#if MINIMAP
    ret += "Minimap mode: \t\t"+ ( this_player()->GetProperty("minimapping") ? "on" : "off" )+"\n";
#endif
    ret += "Annoyblock: \t\t"+ ( (this_player()->GetAnnoyblock()) ? "on" : "off" )+"\n";
    ret += "Reprompt mode: \t\t"+ ( this_player()->GetProperty("reprompt") ? "on" : "off" )+"\n";
    ret += "Screenlock mode: \t"+ ( this_player()->GetProperty("screenlock") ? "on" : "off" )+"\n";
    ret += "Timezone: \t\t"+ ( this_player()->GetProperty("timezone") ? 
            this_player()->GetProperty("timezone") : "None specified" )+"\n";
#ifdef __GET_CHAR_IS_BUFFERED__
    ret += "Charmode: \t\t"+ 
        //( (query_charmode(player) > 0) ? "on" : "off" )+"\n";
        ( (player->GetCharmode()) ? "on" : "off" )+"\n";
#endif
    ret += "Commandecho: \t\t"+ ( this_player()->GetProperty("commandecho") ? this_player()->GetProperty("commandecho") : "off" )+"\n";
#ifdef __DSLIB__
    ret += "Keepalive mode: \t"+ ( this_player()->GetProperty("keepalive") 
            ? this_player()->GetProperty("keepalive") : "off" )+"\n";
#endif

    if(creatorp(this_player())){ 
        ret += "Homedir: \t\t"+user_path(this_player())+"\n";
        if(this_player()->GetParanoia("homeroom")){
            ret += "Homeroom: \t\t"+this_player()->GetParanoia("homeroom")+"\n";
        }
        ret += "Debug mode: \t\t"+ ( (this_player()->GetProperty("debug")) ? "on" : "off" )+"\n";
        ret += "Godmode: \t\t"+ ( (this_player()->GetGodMode()) ? "on" : "off" )+"\n";
        ret += "Wizvision: \t\t"+ ( (this_player()->GetWizVision()) ? "on" : "off" )+"\n";
#if GRID
        ret += "Showgrid: \t\t"+ ( (this_player()->GetVisibleGrid()) ? "on" : "off" )+"\n";
        ret += "Wizmap mode: \t\t"+ ( this_player()->GetProperty("wizmapping") ? "on" : "off" )+"\n";
        ret += "Noclip mode: \t\t"+ ( this_player()->GetProperty("noclip") ? "on" : "off" )+"\n";
        ret += "Anchor: \t\t"+ ( this_player()->GetAnchored() ? "on" : "off" )+"\n";
#endif
    }
    if(replee){
        ret += "Reply target: \t\t"+replee+"\n";
    }
    tmp = sort_array(explode(ret, "\n"),1);
    ret = implode(tmp, "\n");
    write(ret);
    return 1;
}

string GetHelp() {
    string *see_also = ({});
    if(creatorp(this_player())){
        see_also = ({ "debug" , "showgrid",
                "wizvision", "godmode", "wizmap", "noclip" });
    }
    see_also += ({ "brief", "chancolors", "commandecho", "cursefilter",
            "terminal", "screen", "pk", "mute", "gag", "wimpy", "minimap",
            "annoyblock", "reprompt", "charmode", "keepalive", "timezone",
            "screenlock" });
    return ("Syntax: env\n\n"
            "Displays some basic interface and play settings.\n" +
            "See also: "+implode(sort_array(see_also, 1), ", "));
}
