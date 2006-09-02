// shrubbery.c
// A quest item for the default mudlib quest.
// Please remove the quest once you understand it : it is too easy.
// Mobydick@TMI-2, 11-12-92. My birthday!

#include <mudlib.h>

inherit OBJECT ;

void create() {
        ::create();
	set ("short", "a shrubbery") ;
	set ("long", "This shrubbery has a little path down the middle, with one side a little\nhigher so as to get a nice two-level effect.\n") ;
	set ("id", ({ "shrubbery" }) ) ;
	set ("mass", 100) ;
	set ("bulk", 50) ;
// Any item involved in a quest should set the "quest_recognize" property
// so that it can be recognized uniquely for getting credit for the quest.
// This prevents people from getting another item with the same ID and
// getting credit for the quest.
// It is probably best to set the property to a short string which identifies
// the quest.
	set ("quest_recognize", "shrub") ;
}
