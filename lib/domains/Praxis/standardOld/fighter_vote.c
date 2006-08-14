#include <std.h>
#include <voting.h>
#include <daemons.h>

inherit "/std/vote_room";

void create() {
    ::create();
    set_property("light", 3);
    set_property("no castle", 1);
    set_property("indoors", 1);
    set_short( "Voting hall of the fighters");
    set_long( (: this_object(), "new_long" :));
    set_items(
      (["list" : "During election time, it lists all candidates."]) );
    set_exits( 
      (["west" : "/domains/Praxis/fighter_hall"]) );
}

string new_long() {
    if(VOTING_D->election_day()) 
	return "This is the room where Fighters can put in their bid for office. "
	"If you wish to run for office, just type in the command "
	"'enter' and you will be added to the list.  A list of candidates "
	"is posted on the wall.  Type 'list candidates' to read it.";
    else return "This is the voting chamber of the Hall of Fighters.  "
	"Fighters will gather here for the next election: "+(string)VOTING_D->query_vote_date()+".";
}
