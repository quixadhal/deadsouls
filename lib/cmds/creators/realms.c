/*    /cmds/creators/realms.c
 *    from the Dead Souls  Library
 *    created by Blitz@Dead Souls
 */

static mapping Stats = ([ ]);

int sortFunction(string a, string b) {
    if( Stats[a]["moves"] > Stats[b]["moves"] ) return -1;
    else if( Stats[a]["moves"] < Stats[b]["moves"] ) return 1;
    else return 0;
}

mixed cmd(string str) {
    string *names, *text;

    Stats = author_stats();
    names = sort_array(keys(Stats), (: sortFunction :));
    text  = ({ "\t[ %^CYAN%^Creator Area Statistics...%^RESET%^ ]" }); 
    text  += ({ "Creator        Moves    Size     Errors   Objs   HBs" });
    text  += ({ "-----------------------------------------------------" });
    foreach(string name in names) {
        mapping foo = Stats[name];
        text += ({ sprintf("%-14s %-8d %-8d %-8d %-6d %d", name, 
                   foo["moves"], foo["array_size"], foo["errors"], 
                   foo["objects"], foo["heart_beats"]) });
    }
    this_player()->eventPage(text);
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: <realms>\n\n"
            "A creator command that displays individual author "
            "statistics relative to the \"areas\" found within "
            "their directories.\n\n"
            "See also: man author_stats");
}
