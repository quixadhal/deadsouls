#include <lib.h>
#include <objects.h>
#include <post.h>

inherit LIB_ROOM;

void init() {
    object ob;

    ::init();
    if(!living(this_player()) || !interactive(this_player())) 
        this_player()->eventMoveLiving("/domains/Praxis/alley1", "north");
    add_action("mail", "mail");
}

void create() {
    ::create();
    SetShort( "Praxis post office");
    SetLong(
            "Welcome to the Praxis Post Office!\n"
            "The Praxis post office is a quaint whitewashed building. The floor "
            "is a worn wood, due to all the customers that walk upon its floors. "
            "Red trim lines the walls, and a window in the far corner lets in "
            "a slight breeze. "
            "People come here to read and post mail.  Type <mail> to access "
            "your mailbox, or <mail name> to send mail to player name.  "
            "The Nightmare post office runs on the intermud system, meaning "
            "that you can send mails to players on any other mud on the system.  "
            "Type <muds> to find out who is on the system.  The groups available "
            "can be accessed by typing <g> at the % prompt."    );
    SetProperty("no castle", 1);
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetItems( ([ "list" : "A list of commands you may use at the "
                "post office."]) );
    //exit changed by drakken 17.aug
    AddExit("north","/domains/Praxis/alley1",(: this_object(),"more" :) );
    SetProperty("no attack", 1);
    SetProperty("no steal", 1);
}

int mail(string str) {
    object ob;

    if(this_player()->query_name() == "guest") {
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
    ob->destruct();
    return 1;
}

