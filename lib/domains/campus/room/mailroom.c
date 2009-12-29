#include <lib.h>
#include <objects.h>
#include <post.h>
#include "/lib/include/post_office.h"

inherit LIB_ROOM;

static void create(){
    room::create();
    SetTown("campus");
    SetClimate("indoors");
    SetAmbientLight(40);
    SetShort("the campus mail room");
    SetLong("This is a small office containing rows of mailboxes and "+
            "various other postal implements. There is a sign on the wall behind the "+
            "counter, outlining instructions for how to mail other users.");
    SetItems( ([ 
                ({"box","boxes","mailboxes","mailbox"}) : "Rows of mailboxes for "+
                "the denizens of LPC University.",
                "sign" : "This is a sign on the wall describing how to mail messages.",
                ({"wall","walls"}) : "Gray-painted institutional walls of the kind you'd "+
                "expect in a post office.",
                "implements" : "Ink, paper, etc.",
                "instructions" : "Try reading them.",
                "counter" : "A counter folks use to lean on while writing messages." ]) );
    SetExits( ([ 
                ]) );
    SetProperty("no attack", 1);
}
void init(){
    ::init();
    add_action("instr","read");
}
int instr(string str){
    if(str=="instructions"||str=="sign"){
        write("To mail someone, type mail <person's name here>.\n"+
                "Enter a subject line.\n"+
                "Enter your message.\n"+
                "Once you've finished, enter a period (.) on a blank line.\n"+
                "Hit x, then s to send it. You're done!\n");
        return 1;
    }
}
mixed CanMail(object who, string args) {
    if( who && !interactive(who) ) return 0;
    //Normally only people from this town should be able to
    //do mail stuff here, but this is a newbie area, and
    //it's a little too confusing to newcomers not to be able
    //to send mail from a post office.
    //if( GetTown() != who->GetTown() )
    //    return "Any mail you might have will be at your home post office.";
    return 1;
}
mixed eventMail(object who, string args) {
    object ob;

    if( !(ob = new(OBJ_POST)) ) {
        who->eventPrint("Failed to load postal object!");
        return 1;
    }
    if( !(ob->eventMove(who)) ) {
        who->eventPrint("You can't seem to carry the postal object.");
        return 1;
    }
    ob->start_post(args);
    return 1;
}
int CanReceive(object ob) {
    if( !ob || (living(ob) && !interactive(ob)) ) return 0;
    else return room::CanReceive(ob);
}
int eventReleaseObject() {
    object ob;

    if(!ob) return 0;
    if( !(ob = previous_object()) ) return room::eventReleaseObject();
    if( !room::eventReleaseObject() ) return 0;
    if( (ob = present(POSTAL_ID, ob)) ) ob->eventDestruct();
    return 1;
}
