/*    /verbs/players/marry.c
 *    from the Dead Souls  Object Library
 *    marry
 *    marry LIV to LIV
 *    created by Descartes of Borg 951210
 */

#pragma save_binary

#include <lib.h>
#include <objects.h>
#include "include/marry.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("marry");
    SetRules("LIV to LIV");
    SetErrorMessage("Marry whom to whom?");
    SetHelp("Syntax: <marry PLAYER to PLAYER>\n\n"
	    "Allows people of great faith to join to souls in marriage.  "
	    "In order to marry to people, you must however be in a place "
	    "which you find holy.\n\n"
	    "See also: divorce");
}

mixed can_marry_liv_to_liv() {
    mixed tmp;
    
    if( (int)this_player()->GetSkillLevel("faith") < 15 )
      return "You do not have enough faith to join to people.";
    return 1;;
}

mixed do_marry_liv_to_liv(object spouse1, object spouse2) {
    mixed tmp;
    if( this_player() == spouse1 || this_player() == spouse2 )
        return "You may not perform a marriage on yourself.";
    tmp = (mixed)environment(this_player())->CanMarry(this_player(),
                                              spouse1, spouse2);
    if( !tmp ) {
        this_player()->eventPrint("This place is not holy to you.");
        return 1;
    }
     else if( stringp(tmp) ) {
        this_player()->eventPrint(tmp);
        return 1;
    }
    tmp = (mixed)environment(this_player())->eventMarry(this_player(),
							 spouse1, spouse2);
    if( tmp == 1 ) {
	object ring;

	ring = new(OBJ_WED_RING);
	ring->SetSpouse((string)spouse2->GetCapName());
	ring->eventMove(spouse1);
	ring = new(OBJ_WED_RING);
	ring->SetSpouse((string)spouse1->GetCapName());
	ring->eventMove(spouse2);
    }
}
