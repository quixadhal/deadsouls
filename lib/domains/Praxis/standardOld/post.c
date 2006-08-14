#include <std.h>
#include <objects.h>
#include <post.h>

inherit ROOM;

void init() {
    object ob;

    ::init();
    if(!living(this_player()) || !interactive(this_player())) 
	this_player()->move_player("/domains/Praxis/alley1", "north");
    add_action("mail", "mail");
}

void create() {
    ::create();
    set_short( "Praxis Post Office");
    set_long(
      "Welcome to the Praxis Post Office!\n"
      "People come here to read and post mails.  Type <mail> to access "
      "your mailbox, or <mail name> to send mail to player name.  "
      "The Nightmare post office runs on the intermud system, meaning "
      "that you can send mails to players on any other mud on the system.  "
      "Type <muds> to find out who is on the system.  The groups available "
      "can be accessed by typing <g> at the % prompt."    );
    set_property("no castle", 1);
    set_property("light", 2);
    set_property("indoors", 1);
    set_items( ([ "list" : "A list of commands you may use at the "
	"post office."]) );
    //exit changed by drakken 17.aug
    add_exit("north","/domains/Praxis/alley1",(: this_object(),"more" :) );
    set_property("no attack", 1);
    set_property("no steal", 1);
}

int mail(string str) {
    object ob;

    if((string)this_player()->query_name() == "guest") {
	notify_fail("Guest may not use the mailer.\n");
	return 0;
    }
    ob = new(OB_POSTAL);
    ob->move(this_player());
    ob->start_post(str);
    return 1;
}

int more() {
    object ob;

    if(!(ob = present(POSTAL_ID, this_player()))) return 1;
    ob->remove();
    return 1;
}

