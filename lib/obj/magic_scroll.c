// magic_scroll.c
// This scroll contains a spell, which a player can learn by casting
// the read spell on the scroll.
// Mobydick@TMI-2, 3-24-93.

#include <mudlib.h> 

inherit OBJECT ;

void create() {
    ::create();
    set ("id", ({ "scroll" }) ) ;
    set ("short", "a magical scroll") ;
    set ("long",
            "This is a magic scroll on which spells can be written. At the moment
            "+"it is blank.\n") ;
    set ("mass", 10) ;
    set ("bulk", 4) ;
}

void init() {
    add_action ("write_scroll", "write") ;
    add_action ("read_scroll", "read") ;
    add_action ("help", "help") ;
}

void set_spell (string str) {
    //if (member_array(str,SPELLS)==-1) {
    //		write ("Error: tried to set an unknown spell.\n") ;
    //		return ;
    //	}
    //	set ("spell", str) ;
    //	set ("short", "A scroll of "+str) ;
    set ("long",
            "This is a magic scroll with the "+str+" spell written on it.\n") ;
}

int write_scroll (string str) {

    string *known ;

    //	known = this_player()->query("spells") ;
    //	if (member_array(str,known)==-1) {
    //		notify_fail ("You don't know any "+str+" spell to write.\n") ;
    //		return 0 ;
    //	}
    write ("You write the "+str+" spell onto the scroll.\n") ;
    //	set_spell(str) ;
    return 1 ;
}

int read_scroll (string str) {
    if (!query("spell")) {
        write("You haven't written any spells on the scroll.\n");
        return 1;
    }
    //	if (str!="scroll" && str!="scroll of "+query("spell")) return 0 ;
    //	write ("You begin reading the scroll....\n") ;
    //	write ("You begin casting the "+query("spell")+" spell!\n") ;
    //	write ("On whom do you wish to cast it? (CR for yourself or current attacker) : ") ;
    //	input_to ("cast_args") ;
    return 1 ;
}

int cast_args (string str) {

    string name ;

    //	if (!str || str == "") str="NONE" ;
    //	name = PATH+query("spell")+EXTENSION ;
    //	name->cast(str, 1) ;
    //	remove() ;
    return 1 ;
}

int help (string str) {
    if (str!="scroll") return 0 ;
    write ("If you know the read spell, you can cast it on the scroll and\n"+
            "you will learn the spell written on the scroll, if there is one.\n"+
            "If you don't know the read spell, you should find someone who can\n"+
            "teach it to you.\n"+
            "You can also simply \"read\" the scroll and cast the spell written\n"+
            "on it, but if you do that you will not learn the spell.\n") ;
    return 1 ;
}
