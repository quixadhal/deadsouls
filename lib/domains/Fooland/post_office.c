// post_office.c
// A room which allows access to Huthar's mailer.
// Written by Mobydick@TMI-2, 9-30-92
// Most of the code was taken from the _mail.c command, written by
// Huthar and Buddha.
// The room uses add_action() to define a mail command. If the player types
// that command, a clone of the portable mailer is give to him, and he
// enters the mailer. When he quits the mailer, it destructs itself: it
// also is destructed if he leaves the post office by going north, via
// the pre_exit_func property.

#include <mailer.h>
#include <mudlib.h>

inherit ROOM ;

void create() {
        ::create();
	seteuid(getuid()) ;
	set("short", "The post office") ;
	set("long", "You are in the Footown post office. You can send mail from here, and\n"+
	"if other people send you mail you can read it here.\n"+
	"There is a sign on the wall that you could read also.\n") ;
	set("light",1) ;
	set ("exits", ([ "north" : "/domains/Fooland/ebazz" ]) ) ;
	set ("pre_exit_func", ([ "north" : "dest_mailer" ]) ) ;
	set ("item_func", ([ "sign" : "read_sign" ]) ) ;
}

void init() {
#if 0
	add_action("mail", "mail") ;
#endif
        add_action("help", "help");
	add_action ("read_sign", "read") ;
}

int mail(string arg) {

   object ob;

   seteuid("TEMP");

   ob = new(MAILER);
   ob->move(this_player());
   ob->start_mail(arg);
   return 1;
}

int help(string str) {
    if (str == "mail") {
	this_player()->more(MAILER_LONG);
	return 1;
    } else return 0;
}

int read_sign (string str) {
	if (str && str!="sign") return 0 ;
	write (
"The sign on the wall reminds you that you can still send in a postcard\n"+
"saying whether you prefer the \"Old Buddha\" stamp or the \"Young Buddha\"\n"+
"stamp. It also says that you can type \"mail\" to read your mail, and you\n"+
"can type \"mail <name>\" to send mail to another player.\n") ;
	return 1 ;
}

int dest_mailer() {

	object ob ;

	ob = present("mailer",this_player()) ;
	if (ob) ob->remove() ;
	return 0 ;
}
