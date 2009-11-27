#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string tmpfile;
    string ret = "";
    object *obs;

    if(args) args = replace_string(args,"\"","");

    if(!args) obs = ({ this_player() });

    else obs = findobs(args);

    if(!sizeof(obs)) {
        write("No such objects found.");
        return 1;
    }

    write(sizeof(obs)+" matches found. They are:\n\n");

    foreach(object ob in obs){
        string name;
        string str = "%^BOLD%^BLUE%^Object: "+identify(ob)+", ";
        if(args != "door" && name = ob->GetName()) str += "%^BOLD%^GREEN%^name: "+name+", ";
        str += "%^B_BLACK%^BOLD%^WHITE%^";
        if(environment(ob)) str += "environment: "+identify(environment(ob))+".\n";
        else str += "environment: None.\n";
        str += "%^RESET%^\n";
        ret += str;
    }

    tmpfile = generate_tmp();
    write_file(tmpfile,ret);
    this_player()->eventPage(tmpfile);
    rm(tmpfile);
    return 1;
}

string GetHelp() {
    return ("Syntax: findobj <STRING>\n\n"
            "Displays matching objects that are loaded into memory.\n\n"
            "Examples: \n\nfindobj staff\nfindobj /lib/dummy");
}
