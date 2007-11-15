#include <lib.h>

private static mapping Teaching = ([]);
private static int OfferExpires = 60;

int direct_teach_str_to_liv(){ return 1;}
int direct_teach_liv_to_str(){ return 1;}

int CanTeach(){
    return 1;
}

int eventOfferTeaching(object who, string what){
    if(!CanTeach()){
        write("You can't teach that.");
        return 0;
    }
    tell_player(who,this_object()->GetName()+" offers to teach you "+what+".");
    Teaching[who] = ({ what, time() });
    return 1;
}

int eventTeach(object who, string what){
    if(!CanTeach()){
        write("They can't teach that.");
        return 0;
    }
    if(!Teaching[who]){
        write("They're not interested in teaching you anything.");
        return 0;
    }
    if(time() - Teaching[who][1] > OfferExpires){
        write("It's too late. Their offer to teach you expired.");
        map_delete(Teaching,who);
        return 0;
    }
    if( !who->eventLearnSpell(what) ) {
        write("You are not prepared for that spell!");
        return 0;
    }

    who->eventPrint(this_object()->GetName() + " touches your forehead and gives "
      "you knowledge of " + what + ".");
    environment()->eventPrint(this_object()->GetName() + " touches " +
      possessive_noun(who) +
      " forehead and gives " +
      objective(who) + " knowledge of " +
      what + ".", who);
    map_delete(Teaching,who);
    return 1;
}
