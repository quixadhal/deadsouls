#include <lib.h>
inherit LIB_ROOM;

int found;
string SearchFun(){
    string result;
    if(found == 0){
        result="You search the room and find a gold ring!";
        tell_room(this_object(),this_player()->GetName()+ 
          " searches the room and finds something ",
          ({ this_player() }) );
        if(!new("/domains/town/armor/newbie_ring")->eventMove(this_player()))
            new("/domains/town/armor/newbie_ring")->eventMove(this_object());
        found = 1;
        return result;
    }
    return "You find nothing special.";
}
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Mansion Guest Room West");
    SetLong("You are in the west upstairs guest room of the mansion. "
      "The room is sumptuously appointed and furnished. "
      "It looks as though nobody has dusted here in a "
      "very long time. ");
    SetExits( ([
        "east" : "/domains/town/room/mansion_uhall1"
      ]) );
    SetItems( ([ 
        ({"here","area","place","room"}) : "A guest room in the mansion.",
        "dust" : "There's some dust here, a bit more than one might expect in such a nice place.",
      ]) );
    SetInventory(([
        "/domains/town/obj/gbed" : 1,
      ]) );
    SetSearch( ([ "default"  : (: SearchFun :)  ]) );
    found = 0;

    SetDoor("east", "/domains/town/doors/m2.c");

}
void init(){
    ::init();
    AddSearch( ({"here","area","place","room"})  , (: SearchFun :)  );
}
