/*  Lets you replay the login news.
 *  created by Rush@Dead Souls & Kalinash@Dead Souls
 */

#include <lib.h>
#include "include/news.h"

inherit LIB_VERB;

static void create() {
    ::create();
    SetVerb("news");
    SetRules("", "STR");
    SetErrorMessage("See \"help news\" for more info on this command.");
    SetHelp("Syntax: news [<classname>|general]\n\n"    
            "This command allows you to replay the login news.");
}

mixed can_news() { return can_news_str("general"); }

mixed can_news_str(string str) {
    if(!str) return 0;
    if(!file_exists(DIR_NEWS + "/" + str)) return "No " + str + " news.";
    if(str == "general" || this_player()->ClassMember(str)) return 1;
    if(str == "welcome") return 1;
    if(str == "creator" && creatorp(this_player())) return 1;
    if(archp(this_player())) return 1;
    return "The " + str + " news is not for your eyes.";
}

mixed do_news() { return do_news_str("general"); }

mixed do_news_str(string str) {
    this_player()->eventPage(DIR_NEWS + "/" + str);
    return 1;
}
