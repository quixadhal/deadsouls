//      /bin/user/_where.c
//      from the Dead Souls mudlib
//      created 30 March 1993 by Billbo Baggins esq.
//      redone to use mapping, by Valodin, July 1, 1993

#include <cfg.h>
#include <lib.h>

inherit LIB_DAEMON;

private mapping Sites;

void create() {
    string str, line;

    daemon::create();
    Sites = ([]);
    if( !(str = read_file(CFG_WHERE)) ) return;
    foreach(line in explode(str, "\n")) {
	string site, where;
	if( sscanf(line, "%s:%s", site, where) == 2 )
	  Sites[site] = where;
    }
}

static string GetWhere(object ob) {
    string *chunk;
    int i;

    chunk = explode(query_ip_number(ob), ".");
    for(i = 4; i > 0; i--) {
	string ip;

	if( i < 4 ) chunk[i] = "*";
	ip = implode(chunk, ".");
	if( Sites[ip] ) return Sites[ip];
    }
    return "Unknown";
}

mixed cmd(string arg) {
    string *arr;
    object ob;
    int priv;

    if( arg == "block" ) {
        if( (int)this_player()->GetWhereBlock() )
	  write("Location blocking off.");
        else write("Now blocking location information.");
        this_player()->SetWhereBlock();
        return 1;
    }
    priv = archp(this_player());
    arr = ({});
    foreach(ob in users()) {
	string str;

	if( (int)ob->GetInvis(this_player()) ) continue;
	if( (int)ob->GetWhereBlock() ) {
	    if( !priv ) str = sprintf("%:-15s Unknown", (string)ob->GetName());
	    else str = sprintf("%:-15s [%s]", (string)ob->GetName(),
			       GetWhere(ob));
	}
	else str = sprintf("%:-15s %s", (string)ob->GetName(), GetWhere(ob));
	arr += ({ str });
    }
    this_player()->eventPage(sort_array(arr, 1));
    return 1;
}

void help() {
    write("Syntax: <where>\n\n"
        "Lists all players online and the towns from which those players\n"
       "whose machine site locations are known.  If your site is marked\n"
        "<mail superuser> with the town your *machine* is located in and\n"
        "the ip # (four numbers separated by a period).\n"
        "\nOptional: <where [block]>\n\n"
        "Allows you to keep your location anonymous.\n"
        "See also: users, who\n"
    );
}
