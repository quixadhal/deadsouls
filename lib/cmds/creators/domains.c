/*    /cmds/creators/domains.c
 *    From the Dead Souls V Object Library
 *    Shows the domain statistics information
 *    created by Descartes of Borg 960214
 *    Version: @(#) domains.c 1.3@(#)
 *    Last modified: 96/10/08
 */

#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object who = previous_object();
    int width = (who->GetScreen())[0];
    mapping stats = ([]);
    string array domains;
    mixed array tmp;
    
    foreach(string domain, mixed data in domain_stats()) {
	if( !data ) {
	    continue;
	}
	if( file_size(DOMAINS_DIRS "/" + domain) != -2 ) {
	    continue;
	}
	if( !data["moves"] && !data["heart_beats"] && !data["objects"] ) {
	    continue;
	}
	stats[domain] = data;
    }
    domains = sort_array(keys(stats), function(string a, string b, mapping mp) {
                                          if( mp[a]["moves"] > mp[b]["moves"] ) {
					      return -1;
					  }
					  else {
					      return 1;
					  }
                                      }, stats); 
    tmp = ({ sprintf("%:-15s %:-11s %:-11s %:-11s %:-11s %s", "Domain",
		     "Moves", "Objects", "Errors", "Heart Beats",
		     "Array Size"), sprintf("%'-'" + (width-1) + "s", "") });
    foreach(string domain in domains)  {
	tmp += ({ sprintf("%:-15s %:-11d %:-11d %:-11d %:-11d %d",
			capitalize(domain), stats[domain]["moves"],
			stats[domain]["objects"], stats[domain]["errors"],
			stats[domain]["heart_beats"],
			stats[domain]["array_size"]) });
    }
    who->eventPage(tmp, MSG_SYSTEM);
    return 1;
}
 
string GetHelp() {
    return ("Syntax: <domains>\n\n"
	    "Prints out domain statistics for the mud's domains.\n\n"
	    "See also: realms");
}
