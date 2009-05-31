// IMPORTANT NOTE
// Zafo does not speak Common. He is a simple human and only
// speaks English. For him to understand what you say, you
// MUST SPEAK IN ENGLISH.
// Or edit this file to make him know Common.

/* This is an example of parsing talk. The CopResponse() function
 * is a bit silly, because it tries to parse words, and as such is
 * doomed to failure, since it is trying to be an language/meaning
 * AI. Really it should be done with phrases. But for educational
 * purposes, this example is here.
 * -Crat 13FEB2008
 */

#include <lib.h>

inherit LIB_SENTIENT;

mixed CopResponse(object who, mixed foo, string message, mixed bar){
    string *message_array, *stupid_array, *cop_array;
    int i, not_stupid;

    //make sure the message string actually exists
    if(!sizeof(message)) return 0;
    //parsing is easier in a single case
    message = lower_case(message);
    //make an array whose elements are the individual words
    //of the message, for example ({ "cops", "are", "not", "stupid" })
    message_array = explode(message," ");

    //Now we take the message array and create a new array called cop_array
    //that contains words in message_array that start with "cop" and the next character
    //is either nothing (cop) or an s (cops) or punctuation(cop.).
    //Note that this would allow something like "copse"
    cop_array = regexp(message_array, "cop(''|s|[!-@])");
    //Now we create an array that does the same thing but with the
    //word "stupid"
    stupid_array = regexp(message_array, "stupid(''|[!-@])");
    //Here we find out whether the word "not" is a memeber of the message
    //array. If it is not a member, i will be -1. Otherwise, i will be
    //the index number of the word. For example, in ({ "cops", "are", "not", "stupid" }),
    //"cop" is 0, and "not" is 2.
    i = member_array("not", message_array);
    //Now we determine if "not" is in the array, by checking to see if the
    //value of i is -1. If it is not -1, then we check to see if there is
    //another word after it in the array (i+1). If there is, we check to see
    //if that word contains the string "stupid". And if it does, then we 
    //assign not_stupid a value of 1.
    if(i != -1 && message_array[i+1] && grepp(message_array[i+1],"stupid")){
        not_stupid = 1;
    }

    //If we have a matching cop word, and a matching stupid word, and
    //if the word "not" is *not* in front of the word stupid, then
    //we'll go ahead and have a reaction.
    if(sizeof(cop_array) && sizeof(stupid_array) && !not_stupid){
        eventForce("laugh");
        eventForce("say LOL you said 'stupid' and 'cop!'");
        return 1;
    }
    //Otherwise we just return an arbitrary value.
    else return 0;
}

static void create() {
    sentient::create();
    SetKeyName("zafo");
    SetId(({"dude"}));
    SetAdjectives(({"punk rock", "punkrock", "rock", "cool"}));
    SetShort("Zafo");
    SetLong("Zafo is a cool dude, and totally punk rock.");
    SetLevel(10);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetInventory( ([
                "/domains/town/armor/jacket" : "wear jacket",
                "/domains/town/armor/pants" : "wear pants",
                "/domains/town/armor/boot_l" : "wear left boot",
                "/domains/town/armor/boot_r" : "wear right boot",
                ]) );
    SetTalkResponses( ([  
                "s up" : "Nothin'.",
                "re you rebelling against" : "Waddaya got?",
                "cop" : (: CopResponse :),
                ]) );
}
void init(){
    sentient::init();
}
