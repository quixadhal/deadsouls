// magic_guild.c
// The magician's guild in the town of Footown.
// This inherits the standard guild object. The standard object has the
// interesting comments on how it all works.
// Written by Mobydick@TMI-2, 11-16-92
// Please leave this header intact.

inherit "/std/guild" ;

void create() {
	::create();
	seteuid(getuid()) ;
	set ("objects", ([ "bozo" : "/domains/Fooland/bozo" ]) ) ;
	reset() ;
	set("short", "The mansion of Bozo the archmage") ;
	set("long", 
"This is the mansion of the famous archmage Bozo. For a small fee, Bozo\n"+
"is happy to train you in the arts of magic. You can \"list\" the skills\n"+
"in which he can train you, and you can \"train\" in a given skill. He\n"+
"will also teach you the read spell, free of charge, if you type \"learn\n"+
"read\". This will let you learn other spells from scrolls.\n") ;
	set ("exits", ([
		"east" : "/domains/Fooland/wbazz"
	]) ) ;
	set ("light", 1) ;
	 set_trainable_skills ( ({ "Combat spells", "Healing spells", "Divinations spells" }) ) ;
}

void init() {
	::init() ;
	add_action ("learn_read_magic", "learn") ;
}

int learn_read_magic (string str) {

	string *known ;

	if (!str || str!="read") return 0 ;
	known = this_player()->query("spells") ;
	if (known && member_array("read", known)!=-1) {
		write ("You already know the read spell!\n") ;
		return 1 ;
	}
// If a player doesn't know read, he should not know any other spells
// either. However, strange things have happened before, so we allow
// for the possibility that he does know some already.

	if (!known) {
		known = ({ "read" }) ;
	} else {
		known += ({ "read" }) ;
	}
	this_player()->set("spells", known) ;
	write ("Bozo smiles at you kindly and teaches you the read spell.\n") ;
	say (this_player()->query("cap_name")+" learns the read spell from Bozo.\n") ;
	return 1 ;
}
