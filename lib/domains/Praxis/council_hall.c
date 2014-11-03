#include <lib.h>
#include ROOMS_H
#include <council.h>
#include <daemons.h>

inherit LIB_ROOM;

int CanReceive(object ob) {
    if((!previous_object()->is_player()) ||
            creatorp(previous_object()) ||
            (-1!=member_array(previous_object()->query_name(), COUNCIL)))
        return ::CanReceive(ob);
    message("my_action", "You are not allowed in the council room. If you are "+
            "a council member, try contacting Nialson, advocate, and law in "+
            "that order.", previous_object());
    return 0; 
}

void create() {
    object ob;

    ::create();
    SetProperties( (["light"       : 2, 
                "indoors"     : 1,
                "no attack"   : 1,
                "no steal"    : 1,
                "no magic"    : 1,
                "no teleport" : 1,
                "no castle"   : 1]) );
    SetShort( "The private meeting hall of the council");
    SetLong( "Welcome to the cavern beneath the square. This is the meeting "+
            "hall of the councils of the classes. " );
    SetExits( (["leave" : "/domains/Praxis/square"]) );

    ob = new("/lib/bboard");
    ob->SetKeyName("board");
    ob->SetId( ({ "board", "pipe dream board", "bulletin board"}) );
    ob->set_board_id("council_board");
    ob->set_max_posts(30);
    ob->set_edit_ok( ({"nialson"}) );
    ob->move(this_object());
    ob->SetShort( "the pipe dream board of class war avoidance");
    ob->SetLong( "This is a collection of ill-conceived threats, and useful "+
            "suggestions intended to increase the orderly flow of "+
            "information and to decrease the level of inter-player "+
            "tension (at least for the immortals).");
    SetSearch("default", "Why, the remains of Archduke Ferdinand are "+
            "concealed here! So that's what happenned to him.");
}

void init() {
    ::init();
    add_action("leave","leave");
}

int leave() {
    string verb;
    string *tmpstrs;
    int val;

    if( this_player()->query_disable() &&
            sizeof(this_player()->query_attackers()) ) {
        write("You can not exit while doing something else.");
        return 1; }
    if (creatorp(this_player()))
        this_player()->eventMoveLiving("/domains/Praxis/adv_inner", "leave");
    else
        this_player()->eventMoveLiving("/domains/Praxis/"+this_player()->query_class()+
                "_hall", "leave");
    return 1;
}
