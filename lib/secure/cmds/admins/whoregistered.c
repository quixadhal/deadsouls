#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    string res;
    string *sites;

    if(!archp(previous_object())) return 0;
    if(res=catch(sites = call_other(BANISH_D, "query_registered"))) {
        write("Error in checking sites: "+res+"\n");
        return 1;
    }
    sites = sort_array(sites, "order_sites", this_object());
    if(str) {
        if(member_array(str, sites) == -1)
            write("Site \""+str+"\" is not currently on registration.\n");
        else write("Site \""+str+"\" is currently on registration.\n");
    }
    else {
        write("People from these sites must currently register in order to get characters:\n");
        this_player()->more(explode(format_page(sites, 5), "\n"));
    }
    return 1;
}

int order_sites(string alpha, string beta) {
    string *a, *b;
    int i, x, y;

    a = explode(alpha, ".");
    b = explode(beta, ".");
    for(i=0; i<sizeof(a) && i<sizeof(b); i++) {
        if(!(x = to_int(a[i]))) return -1;
        if(!(y = to_int(b[i]))) return 1;
        if(x > y) return 1;
        if(y > x) return -1;
    }
    if(sizeof(a) < sizeof(b)) return -1;
    else return (sizeof(a) > sizeof(b));
}

string GetHelp(){
    return ("Syntax: whoregistered [site]\n\n"
            "Without an argument, it lists all sites which need to register "
            "in order to create a character on the mud.  Given with a site "
            "as an argument, it will confirm if that site must register. "
            "Sites must be in ip numeric format.\nSee also: "
            "register, unregister, banish, unbanish, whobanished, "
            "watch, unwatch, whowatched");
}
