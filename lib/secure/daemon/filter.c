#include <lib.h>

inherit LIB_DAEMON;


static void create() {
    daemon::create();
}

string PirateFilter(string str){
    if(!str || !sizeof(str) || !stringp(str)) return "";
    if(grepp(str,"tp://")) return str;
    if(lower_case(str) == "test") str = "ARRR!";
    if(lower_case(str) == "testing") str = "ARRR, I says!";
    if(lower_case(str) == "testing.") str = "ARRR!";
    if(lower_case(str) == "test.") str = "ARRR!";
    if(lower_case(str) == "hmm") str = "YARR!";
    if(lower_case(str) == "hmmm") str = "YARRR!";
    if(lower_case(str) == "?") str = "YARRR?";
    if(lower_case(str) == "...") str = "Y...A...R...R?";
    str = replace_string(str, "that's", "that be");
    str = replace_string(str, "That's", "That be");
    str = replace_string(str, "they're", "they be");
    str = replace_string(str, "They're", "They be");
    str = replace_string(str, "your ", "yer ");
    str = replace_string(str, " gay ", " hamarseckshul ");
    str = replace_string(str, "Your ", "Yer ");
    str = replace_string(str, "you're", "ye be");
    str = replace_string(str, "You're", "Ye be");
    str = replace_string(str, "here's", "here be");
    str = replace_string(str, "Here's", "Here be");
    str = replace_string(str, "who's", "who be");
    str = replace_string(str, "Who's", "Who be");
    str = replace_string(str, "what's", "what be");
    str = replace_string(str, "What's", "What be");
    str = replace_string(str, " his ", " he ");
    str = replace_string(str, "His ", "He ");
    str = replace_string(str, " her ", " she ");
    str = replace_string(str, "Her ", "She ");
    str = replace_string(str, " is ", " be ");
    str = replace_string(str, "Is ", "Be ");
    str = replace_string(str, "thbe", "this");
    str = replace_string(str, "Thbe", "This");
    str = replace_string(str, "the ", "th' ");
    str = replace_string(str, "The ", "Th' ");
    str = replace_string(str, " am ", " be ");
    str = replace_string(str, "I'm ", "I be ");
    str = replace_string(str, "Im ", "I be ");
    str = replace_string(str, "i'm ", "i be ");
    str = replace_string(str, "ing ", "in' ");
    str = replace_string(str, "ing ", "in' ");
    str = replace_string(str, "ing ", "in' ");
    str = replace_string(str, "ing.", "in'.");
    str = replace_string(str, "in'.c", "ing.c");
    str = replace_string(str, "in'.h", "ing.h");
    str = replace_string(str, "ing!", "in'!");
    str = replace_string(str, "there", "thar");
    str = replace_string(str, "There", "Thar");
    str = replace_string(str, "you ", "ye ");
    str = replace_string(str, "You ", "Ye ");
    str = replace_string(str, "it is not ", "'tain't ");
    str = replace_string(str, "It is not ", "'tain't ");
    str = replace_string(str, "it isn't ", "'tain't ");
    str = replace_string(str, "It isn't ", "'tain't ");
    str = replace_string(str, "it's not ", "'tain't ");
    str = replace_string(str, "It's not ", "'tain't ");
    str = replace_string(str, "it is ", "'tis ");
    str = replace_string(str, "It is ", "'tis ");
    str = replace_string(str, "It's ", "'tis ");
    str = replace_string(str, "my ", "me ");
    str = replace_string(str, "My ", "me ");
    str = replace_string(str, "of ", "o' ");
    str = replace_string(str, "Of ", "O' ");
    str = replace_string(str, "huh? ", "ehhhhh? ");
    str = replace_string(str, "Huh? ", "Ehhhhh? ");
    str = replace_string(str, "yes", "aye");
    str = replace_string(str, "Yes", "Aye");
    str = replace_string(str, "ayeterday", "yesterday");
    str = replace_string(str, "Ayeterday", "Yesterday");
    str = replace_string(str, "yepper ", "aye ");
    str = replace_string(str, "Yepper ", "Aye ");
    str = replace_string(str, "yep", "aye");
    str = replace_string(str, "Yep", "Aye");
    str = replace_string(str, "yeah", "aye");
    str = replace_string(str, "Yeah", "Aye");
    str = replace_string(str, "yea", "aye");
    str = replace_string(str, "Yea", "Aye");
    str = replace_string(str, "no ", "nay ");
    str = replace_string(str, "No ", "Nay ");
    str = replace_string(str, "no, ", "nay, ");
    str = replace_string(str, "No, ", "Nay, ");
    str = replace_string(str, "nope", "nay");
    str = replace_string(str, "Nope ", "Nay ");
    str = replace_string(str, "oh ", "ARRRRR ");
    str = replace_string(str, "oh, ", "ARRRRR, ");
    str = replace_string(str, " and ", " 'n' ");
    str = replace_string(str, " them ", " 'em ");
    str = replace_string(str, " were ", " was ");
    str = replace_string(str, " are ", " be ");
    str = replace_string(str, " to ", " t' ");
    str = replace_string(str, "who ", "'oo ");
    str = replace_string(str, "Who ", "'oo ");
    str = replace_string(str, " did ", " done ");
    str = replace_string(str, " said", " says");
    str = replace_string(str, " doesn't ", " don't ");
    str = replace_string(str, " does ", " do ");
    str = replace_string(str, "hehe", "har harr");
    str = replace_string(str, "heh", "harr");
    str = replace_string(str, "they ", "them ");
    str = replace_string(str, "They ", "Them ");
    str = replace_string(str, "hey ", "ahoy ");
    str = replace_string(str, "Hey ", "Ahoy ");
    str = replace_string(str, "hello", "ahoy");
    str = replace_string(str, "Hello", "Ahoy");
    str = replace_string(str, "hi ", "ahoy ");
    str = replace_string(str, "hiya", "ahoy");
    str = replace_string(str, "Hiya", "Ahoy");
    str = replace_string(str, ", guys", ", mateys");
    str = replace_string(str, "guys, ", "mateys, ");
    str = replace_string(str, "Guys, ", "Mateys, ");
    str = replace_string(str, "Hi ", "Ahoy ");
    str = replace_string(str, "hi,", "ahoy,");
    str = replace_string(str, "Hi,", "Ahoy,");
    str = replace_string(str, "howdy,", "ahoy,");
    str = replace_string(str, "Howdy,", "Ahoy,");
    str = replace_string(str, "howdy ", "ahoy ");
    str = replace_string(str, "wow", "shiver me timbers");
    str = replace_string(str, "Wow", "Shiver me timbers");
    str = replace_string(str, "WOW", "Shiver me timbers");
    str = replace_string(str, "omg", "shiver me timbers");
    str = replace_string(str, "OMG", "By Blackbeard's ghost!");
    str = replace_string(str, "omfg", "shiver me fucken' timbers");
    str = replace_string(str, "OMFG", "shiver me fucken' blasted timbers");
    str = replace_string(str, "stfu", "belay that guff");
    str = replace_string(str, "STFU", "Silence, animals!");
    str = replace_string(str, "wtf", "arr?");
    str = replace_string(str, "WTF", "ARR!?");
    str = replace_string(str, "stop", "avast");
    str = replace_string(str, "Stop", "Avast");
    str = replace_string(str, " stuff ", " booty ");
    str = replace_string(str, "there", "thar");
    str = replace_string(str, "There", "Thar");
    str = replace_string(str, "lol", "Yo Ho Ho");
    str = replace_string(str, " for ", " fer ");
    str = replace_string(str, "For ", "Fer ");
    str = replace_string(str, "LOL", "YO HO HO");
    str = replace_string(str, "rofl", "harr harr harr");
    str = replace_string(str, "ROFL", "HARR HARR HARR");
    //str = replace_string(str, "testing", "ARRR!");
    //str = replace_string(str, "test", "ARRR");
    //str = replace_string(str, "a ARRR", "a test");
    //str = replace_string(str, "to ARRR", "to test");
    if(random(100) < 1){
        switch(random(5)){
            case 0 : str = "Avast, me hearties! "+str;break;
            case 1 : str += " ARRRR!!";break;
            case 2 : str = "Well blow me down! "+str;break;
            case 3 : str += " YO HO HO!";break;
            case 4 : str += " Wench! Grog!";break;
        }
    }
    return str;
}

string CurseFilter(string str){
    string orig, lowered;
    int i;
    string *curses;

    if(!sizeof(str)) return "";
    curses = ({"bullshit", "motherfucking", "motherfuck",
            "shitty", "fucking", "fucken", "mother fucking", "mother fuck",
            "damned", "god damned", "goddamned", "christing", "cunting",
            "assfuck", "buttfuck", "butt fuck", "shitlick", "shit lick",
            "cocklick", "cock lick", "dicklick", "dick lick", "cuntlick",
            "cunt lick", "shithead", "fuckhead", "shit head", "fuck head", 
            "shit", "fuck", "cunt", "slut", "bastard", "asshole",
            "cocksuck", "cock suck", "dicksuck", "dick suck", "suck dick",
            "suck cock", "pussy", "twat", "jizz", "piss", "blowjob",
            "buttsex", "butt sex", "anal sex", "faggot", "nigger", "kike",
            "jap", "rimjob", "nafe", "handjob", "god damn", "goddamn", "damn",
            "goddam", "whore", "son of a bitch", "bitch", "felch", "feltch",
            "bukkake", "rape", "incest", "sodomy", "dildo", "christ", "jesus",
            "heterosexual intercourse in the missionary position" });

    lowered = lower_case(str);
    orig = str;

    foreach(string curse in curses){
        i = sizeof(curse);
        lowered = replace_string(lowered, curse, repeat_string("*", i));
    }
    for(i = 0; i < sizeof(str); i++){
        if(lowered[i..i] == "*") str[i..i] = "*";
    }
    return str;
} 

varargs string eventFilter(string str, string type){
    if(!str || !sizeof(str) || !stringp(str)) return "";
    if(!type) return str;
    if(type == "curse") str = CurseFilter(str);
    if(type == "pirate") str = PirateFilter(str);
    return str;
}
