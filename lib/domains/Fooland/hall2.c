// hall2.c
// This room is where players come to be promoted to wizard. The function
// wizqual in this room is the one that determines what it takes to become
// a wizard. You probably want to alter it: we've set the standards rather
// low for our own purposes. More detail on it in the header.

#define NUM_QUESTS 1
#define MIN_SKILL 5
#define TOT_SKILL 100

#include <mudlib.h>
#include <daemons.h>

inherit ROOM ;

void create() {
	::create();
// This room needs EUID so it can load the promotion daemon.
	seteuid(getuid()) ;
	set("light", 1) ;
	set ("short", "The promotion room") ;
	set ("long",
"You are in a small back room of the Fooland adventurer's hall. There is\n"+
"a small gnome sitting behind a desk which is covered with papers. You can\n"+
"go back east to the main hall.\n") ;
	set ("item_desc", ([ "gnome" :
"This fellow is responsible for promoting players into the ranks of wizards.\n"+
"He has an aura of tremendous responsibility and power about him. If you\n"+
"believe you are worthy of promotion, type \"apply\". He can also tell you\n"+
"what the requirements for wizardhood are: if you wish to know them, type\n"+
"\"inquire\".\n",
	"desk" :
"It is a solid oak desk. Strangely, it contains no drawers, yet the gnome\n"+
"behind the desk seems to be pulling items out of it regularly.\n",
	"papers" :
"The papers are written in a strange language that you cannot read.\n"
	]) ) ;
	set ("exits", ([ "east" : "/domains/Fooland/hall" ]) ) ;
}

void init() {
	add_action ("apply", "apply") ;
	add_action ("inquire", "inquire") ;
}

// This is the wizqual function which determines what it takes to get
// promoted to wizard. It comes with the following standards:
//   1. All quests completed.
//   2. No skill below 5.
//   3. Total skills above 100.
// If the player has met the requirements it returns 1, otherwise it
// returns 0.

int wizqual (object ob) {

	mapping solved, skills ;
	string *names ;
	int i, j, sum ;

        solved = ob->query("quest") ;
	if (!solved) return 0 ;
	j = sizeof(solved) ;
	if (j != NUM_QUESTS) return 0 ;
	skills = ob->query_skills() ;
	names = keys(skills) ;
	for (i=0;i<sizeof(skills);i++) {
		j = skills[names[i]] ;
		if (j<MIN_SKILL) return 0 ;
		sum = sum + j ;
	}
	if (sum<TOT_SKILL) return 0 ;
	return 1 ;
}

// This function checks whether a player has met the criteria. If so, it
// gives him the wizard command path, sets his wizard property, and calls
// enable_wizard() on him. Notably, it does NOT give him a directory. If
// you want that done, you'll have to add it.

int apply() {

	string str ;
	int res ;

	if (wizardp(this_player())) {
		notify_fail ("The gnome gives you a dirty look. You are already a wizard!\n") ;
		return 0 ;
	}
	if (wizqual(this_player())==1) {
		res = PROMOTION_D->promote_wiz(this_player()) ;
		if (res==0) {
			write ("Unable to promote you! See an admin.\n") ;
			return 1 ;
		}
		write (
"The old gnome looks you in the eye and says:\n"+
"  Congratulations! You have been promoted to wizard on this MUD. If you\n"+
"have never been a wizard before, the first thing you should do is type\n"+
"\"help new_wiz\" and read the whole thing through. It will give you some\n"+
"idea of what powers wizards have, and what responsibilities wizards must\n"+
"carry out in order to keep the MUD running smoothly. When you have done\n"+
"that, talk to one of the administrators of the MUD and ask them what you\n"+
"should do in order to become a full_fledged wizard.\n") ;
		shout ("A new wizard has been born.\n") ;
		str = this_player()->query("cap_name")+" "+ctime(time()) ;
		log_file ("NEW_WIZARDS", str) ;
		return 1 ;
	} else {
		write ("The gnome looks at you and shakes his head. \"I'm sorry,\" he says, \"but you\n"+
		"are not yet qualified to be promoted to wizard.\"\n") ;
		return 1 ;
	}
}

int inquire() {
	write (
"The old gnome looks at you and says:\n"+
"  To make wizard on this MUD you must meet three qualifications. First,\n"+
"you must complete all the the quests. The Book of Quests downstairs will\n"+
"describe them, and you can type \"quests\" to see how many you have left to\n"+
"do. Second, all of your skills must be 5 or higher. Third, the total of\n"+
"all your skills must be at least 100.\n"+
"\nThen he returns to his paperwork.\n") ;
	return 1 ;
}
