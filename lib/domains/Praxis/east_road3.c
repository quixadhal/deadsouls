#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 2);
    SetProperty("no castle", 1);
    SetShort( "the north end of East Road");
    SetLong(
      "East Road comes to a sudden dead end as you find yourself "
      "at the foot of Praxis forest. These outskirts of Praxis aren't "
      "visited very often. The area is very quiet and lonely with "
      "so few people around.  Ahead, at the end of the road you "
      "can see a  discreet sign tacked to a tree. A  small footpath leads "
      "off the road in an eastern direction.");
    SetItems(
      (["road" : "The road that travels the eastern end of Praxis.",
        "forest" : "The forest is dark and forebidding.",
        "intersection" : "There is a restaurant at one of its corners.",
        "alley" : "It is bright and wonderful.",
        "restaurant" : "The Last Dragon Restaurant.",
        "sign" : "A litte white sign, perhaps you could read it."]) );
    SetSkyDomain("town");
    SetExits( ([ 
        "south" : "/domains/Praxis/east_road2",
      ]) );
}

int go_north() {
    if(this_player()->query_level() > 5) {
        write("You try to continue on to the north, but the path "
          "becomes too narrow to accomadate your size.");
        return 0;
    }
    return 1;
}

void init() {
    ::init();
    add_action("read","read");
}

int read(string str) {
    if(str == "sign") {
        write("The sign reads: %^RED%^Welcome all new members to Nightmare! "
          "Off to the north of here lies a special area for all "
          "players new to the town of Praxis.%^RESET%^");
        return 1;
    }
    notify_fail("Read what?\n");
    return 0;
}
