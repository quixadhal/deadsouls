#include <lib.h>
inherit LIB_ROOM;

int ReadSign();

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("First Village Bank");
    SetLong("You are standing in the new First Village Bank. "
      "It is immaculate with white, blue-veined marble floors "
      "and beautiful sculptures and paintings by local artists. "
      "An expensive red carpet leads from the entrance to the "
      "main counter where a teller can assist you.\n"
      "%^GREEN%^A sign you can read hangs here.%^RESET%^");
    SetInventory( ([
	"/domains/town/npc/zoe" : 1
      ]) );
    SetItems( ([
	"sign" : "A sign you can read.",
	({"bank","First Village Bank"}) : "The primary financial "
	"institution in town.",
	({"floor","floors","marble floor","marble floors"}) :
	"The floor is lustrous, clean, and beautiful.",
	({"sculpture","sculptures"}) : "Graceful marble statues "
	"are here, depicting various characters from an "
	"ancient mythological pantheon.",
	({"painting","paintings"}) : "Extraordinary paintings "
	"hang here, testifying to a brilliant local art community.",
	({"carpet","red carpet"}) : "A thick, fine red carpet "
	"leads from the bank entrance to the main counter.",
	"entrance" : "The entrace of the bank.",
	({"counter","main counter"}) : "An area where a "
	"teller waits to assist you."
      ]) );
    SetRead( ({"sign","sign hanging here"}) , (: ReadSign :) );
    SetExits(([ "west" : "/domains/town/room/south_road1",
	"out" : "/domains/town/room/south_road1",
      ]));
    SetObviousExits("w");
    SetProperty("no attack", 1);
}

int ReadSign(){
    write( @EndText
    - This bank requires a minimum balance to open an account.
      To use any of the bank's services, you must first open an
      account (see below as to how).

    - This bank charges for certain transactions, such as
      currency exchange.

      To conduct business here:

      request account from TELLER
      request balance from TELLER
      ask TELLER to deposit AMOUNT CURRENCY
      ask TELLER to withdraw AMOUNT CURRENCY
      ask TELLER to exchange AMOUNT CURRENCY for CURRENCY

      examples:

      request account from zoe
      request balance from zoe
      ask zoe to deposit 100 silver
      ask zoe to withdraw 10 silver
      ask zoe to exchange 10 gold for silver

EndText
    );
    return 1;
}

//int CanReceive(object ob) {
//	    if(playerp(ob) && !creatorp(ob) ) {
//		         message("info","The bank is closed for Festivus.", ob);
//			            return 0;
//	    }
//	        return 1;
//		    }

