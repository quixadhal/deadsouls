#include <lib.h>

inherit LIB_DUMMY; 
inherit LIB_MANIPULATE;
inherit LIB_PRESS; 

int hidden;

int MoveThePainting();
int PushThePainting();
int OpenSesame();
string ShowDali();

static void create() {
    dummy::create();
    SetKeyName("wall");
    SetId("wall");
    SetAdjectives("bedroom");
    SetShort("a bedroom wall");
    SetLong("It is a typical, if ornate, bedroom wall, with a painting on it.");
    SetItems( ([
                ({"painting","beautiful painting"}) :  (: ShowDali :),
                ]) );
    SetManipulate( ([
                ({"painting","beautiful painting"}) : (: MoveThePainting :)
                ]) );
    SetPress( ([
                ({"painting","beautiful painting"}) : (: PushThePainting :)
                ]) );
    hidden = 1;
}

void init(){
    add_action("GetPainting","get");
}

int GetPainting(string str){
    write("It appears firmly attached to the wall.");
    return 1;
}

int MoveThePainting(){
    send_messages("move", "$agent_name $agent_verb the painting on the wall.",
            this_player(), 0, environment(this_player()));
    if(hidden == 1) {
        OpenSesame();
    }
    return 1;
}

int PushThePainting(){
    send_messages("push", "$agent_name $agent_verb the painting on the wall.",
            this_player(), 0, environment(this_player()));
    if(hidden == 1) {
        OpenSesame();
    }
    return 1;
}

string ShowDali(){
    string ret;
    ret = "A beautiful reproduction:\n";
    ret += read_file("/domains/town/txt/pot.txt");
    return ret;
}

int OpenSesame(){
    write("You uncover a secret passageway!");
    say(this_player()->GetName()+" uncovers a secret passageway!");
    AddItem(({"secret passageway","passageway"}) , "A secret passageway you can enter.");
    hidden = 0;
    environment()->OpenPassage();
    return 1;
}
