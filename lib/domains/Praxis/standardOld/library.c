//	Added <read all>, and updated set_properties, set_short, set_long
//	Nialson, Apr 19, 1994
#include <std.h>
#include <daemons.h>
#include <dirs.h>

inherit ROOM;

void init() {
    ::init();
    add_action("help", "help");
    add_action("read", "read");
    add_action("cmd_law", "law");
}

void create() {
    ::create();
    set_properties( (["light"     : 2, 
	"indoors"   : 1,
	"no attack" : 1,
	"no steal"  : 1,
	"no castle" : 1]) );
    set_short("Praxis Library");
    set_long(
      "Welcome to the Praxis Library of adventuring!\n"
      "The community of the Nightmare reality comes here to learn.  "
      "Type <read books> to see what books are on the shelves.  "
      "Type <read [title]> to read a particular book.");
    set_items(
      (["library" : "The laws of reality are explained in here.",
	"shelf" : "It has books on it.",
	"shelves" : "Books fill them.",
	"book" : "One of many.",
	"books" : "They are everywhere on the shelves."]) );
    set_exits( 
      (["west" : "/domains/Praxis/east_road1"]) );
}



varargs void display_policy( mapping policy, int as_law )
{
    int then;

    if( as_law )
    {
	then = policy["Time"];
	printf( "As of %s, %d %s, %d NM;  at the urging of %s\n\n\t\t\%s\n\n%s",
	  day( then ), date( then ), month( then ), year( then ),
	  capitalize( policy["Proposer"] ), policy["Title"], policy["Text"] );
	return;
    }
    printf(
      "Title: %s\nProposer: %s\n\n%s",
      policy["Title"],
      capitalize( policy["Proposer"] ),
      policy["Text"],
    );
    if( sizeof( policy["Aye"] ) )
	write( wrap( "In favour: " + implode( policy["Aye"], ", " ) ) );
    if( sizeof( policy["Nay"] ) )
	write( wrap( "Against: " + implode( policy["Nay"], ", " ) ) );
    if( sizeof( policy["Abs"] ) )
	write( wrap( "Abstentions: " + implode( policy["Abs"], ", " ) ) );
}
int cmd_law( string arg )
{
    mapping policy;

    if( arg )
    {
	if( undefinedp( policy = (mapping)POLITICS_D-> query_policy( arg )
	  ) )
	    write( "There is no law: " + arg + "\n" );
	else
	    display_policy( policy, 1 );
    } else {
	printf( "The book of law contains the following:\n  %s\n",
	  wrap( implode( keys( POLITICS_D-> query_policies() ), "\n  " ) ) );
    }
    return 1;
}
int read(string str) {
    string *dir;
    string *tmp;
    string tmp_str;
    int i,j,k;

    if(!str) {
	notify_fail("Read what?\n");
	return 0;
    }
    if(str == "volume" || str == "book") {
	notify_fail("Which "+str+"?\n");
	return 0;
    }
    if(str == "volumes" || str == "books") {
	message("info", "The books available for reading are:",this_player());
	dir = get_dir(DIR_LIBRARY + "/");
	for(i=0; i<sizeof(dir); i++) dir[i] = replace_string(dir[i], "_", " ");
	message("info", replace_string(format_page(dir, 3), "\n", " "),
	  this_player());
	message("info", "Type <read [book name]> to read a particular book.", this_player());
	message("info", "Type <read all> to read all of them.", this_player());
	return 1;
    }
    if(str == "all") {
	dir = get_dir(DIR_LIBRARY + "/");
	for(i=0; i<sizeof(dir); i++) {
	    if(!creatorp(this_player()))
		message("info", "--==** Book: "+
		  replace_string(dir[i], "_", " ")+" **==--", this_player());
	    this_player()->more(DIR_LIBRARY + "/"+dir[i]);
	}
	return 1;
    }
    str = replace_string(str, " ", "_");
    if(file_size(DIR_LIBRARY + "/"+str) == -1) {
	notify_fail("There is no book here by that title.\n");
	return 0;
    }
    this_player()->more(DIR_LIBRARY + "/"+str);
    return 1;
}

int help(string str) {
    if(!str) return 0;
    if(str != "library") {
	notify_fail("You help "+str+".\n");
	return 0;
    }
    write("You can read what is in the library.\nType <read books>.\n");
    return 1;
}
