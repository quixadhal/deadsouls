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
    write("Screen: \t\t"+identify(this_player()->GetScreen()));
    write("Terminal: \t\t"+this_player()->GetTerminal());
    write("Brief mode: \t\t"+ ( (this_player()->GetBriefMode()) ? "on" : "off" ));
    write("Channel message colors: "+ ( (this_player()->GetNoChanColors()) ? "off" : "on" ));
    write("Playerkiller mode: \t"+ ( (this_player()->GetPK()) ? "on" : "off" ));
    write("Mute mode: \t\t"+ GetMuted() );
    write("Gag mode: \t\t"+ GetGagged() );
    write("Wimpy mode: \t\t"+ ( ((int)this_player()->GetWimpy()) ? "on" : "off" ));
#if MINIMAP
    write("Minimap mode: \t\t"+ ( this_player()->GetProperty("minimapping") ? "on" : "off" ));
#endif
    write("Annoyblock: \t\t"+ ( (this_player()->GetAnnoyblock()) ? "on" : "off" ));
    write("Reprompt mode: \t\t"+ ( this_player()->GetProperty("reprompt") ? "on" : "off" ));
    write("Timezone: \t\t"+ ( this_player()->GetProperty("timezone") ? 
                this_player()->GetProperty("timezone") : "None specified" ));
#ifdef __GET_CHAR_IS_BUFFERED__
    write("Charmode: \t\t"+ 
            //( (query_charmode(player) > 0) ? "on" : "off" ));
        ( (player->GetCharmode()) ? "on" : "off" ));
#endif
    write("Commandecho: \t\t"+ ( this_player()->GetProperty("commandecho") ? this_player()->GetProperty("commandecho") : "off" ));
#ifdef __DSLIB__
    write("Keepalive mode: \t"+ ( this_player()->GetProperty("keepalive") 
                ? this_player()->GetProperty("keepalive") : "off" ));
#endif

    if(creatorp(this_player())){ 
        write("Debug mode: \t\t"+ ( (this_player()->GetProperty("debug")) ? "on" : "off" ));
        write("Godmode: \t\t"+ ( (this_player()->GetGodMode()) ? "on" : "off" ));
        write("Wizvision: \t\t"+ ( (this_player()->GetWizVision()) ? "on" : "off" ));
#if GRID
        write("Showgrid: \t\t"+ ( (this_player()->GetVisibleGrid()) ? "on" : "off" ));
        write("Wizmap mode: \t\t"+ ( this_player()->GetProperty("wizmapping") ? "on" : "off" ));
        write("Noclip mode: \t\t"+ ( this_player()->GetProperty("noclip") ? "on" : "off" ));
#endif
    }
    return 1;
}

void help() {
    string de_bug;
    if(creatorp(this_player())) de_bug = ", debug, showgrid, "+
        "wizvision, godmode, wizmap, noclip.";
    else de_bug = ".";
    message("help", "Syntax: <env>\n\n"
            "Displays some basic interface and play settings. " +
            "See also: brief, terminal, screen, chancolors, pk, mute, gag, wimpy, minimap, annoyblock, reprompt, charmode, keepalive, commandecho, timezone"+de_bug, this_player());
}
