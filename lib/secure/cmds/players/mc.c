#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

#define MONEY_DIR "/accounts/"

mapping account;

mixed cmd(string args) {
    string file;
    float net_worth, max_worth;
    
    file = MONEY_DIR + (string)previous_object()->GetKeyName();
    account = ([]);
    if( !file_exists(file + __SAVE_EXTENSION__) )
      return "You have no bank account on the old Dead Souls.";
    restore_object(file);
    foreach(string bank, mapping data in account) {
	foreach(string curr, int val in data) {
	    float tmp;
	    
	    if( curr == "time" || curr == "transaction" ) continue;
	    if( (tmp = currency_rate(curr)) < 1 ) continue;
	    net_worth += val / tmp;
	}
    }
    max_worth = (int)previous_object()->GetLevel() * 2000;
    if( net_worth  > max_worth ) net_worth = max_worth;
    if( previous_object()->AddBank("Mariner's Bank of Praxis", "electrum",
			       to_int(currency_value(net_worth, "electrum")))
       < 0 ) return "An error occurred in conversion.";
    unguarded((: rm, file + __SAVE_EXTENSION__ :));
    previous_object()->eventPrint("You should now save, since a game crash "
				  "between now and your next save will "
				  "lose all money frm your newly created "
				  "Praxis account without allowing you to "
				  "convert again.", MSG_SYSTEM);
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: <mc>\n\n"
	    "For people with old Dead Souls bank accounts, this allows "
	    "you to convert that bank account over into "
	    "the Mariner's Bank of Praxis.");
}

			       
