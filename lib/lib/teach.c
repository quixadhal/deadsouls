#include <lib.h>

private static mapping Teaching = ([]);
private static int OfferExpires = 60;

int direct_teach_str_to_liv(){ return 1;}
int direct_teach_liv_to_str(){ return 1;}

varargs int CanTeach(object whom, string what){
    if(whom == this_object()){
        write("You are not a member of the autodidact guild.");
        return 0;
    }
    if(member_array(what, keys(this_object()->GetSpellBook())) != -1){
        int magpoint = this_object()->GetMagicPoints();
        int maxmagpoint = this_object()->GetMaxMagicPoints();
        if(magpoint < maxmagpoint){
            write("You must have your full mana to teach magic.");
            return 0;
        }
        return 1;
    }
    return 0;
}

int eventOfferTeaching(object who, string what){
    if(!CanTeach(who, what)){
        write("You are unable to teach that.");
        return 0;
    }
    //if(!Teaching[who] || !Teaching[who][0] || Teaching[who][0] != what){
    tell_player(who,this_object()->GetName()+
            " offers to teach you "+what+".");
    tell_player(this_object(),"You offer to teach "+what+
            " to "+who->GetName()+".");
    //}
    Teaching[who] = ({ what, time() });
    return 1;
}

int eventTeach(object who, string what){
    int magpoint = this_object()->GetMagicPoints();
    if(!CanTeach(who, what)){
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
    if( !who->eventLearnSpell(what) ){
        write("You are not prepared for that spell!");
        return 0;
    }

    who->eventPrint(this_object()->GetName() + " touches your forehead and gives "
            "you knowledge of " + what + ".");
    environment()->eventPrint(this_object()->GetName() + " touches " +
            possessive_noun(who) +
            " forehead and gives " +
            objective(who) + " knowledge of " +
            what + ".", ({ who, this_object() }));
    this_object()->eventPrint("You touch "+possessive_noun(who) + 
            " forehead and give " + objective(who) + " knowledge of " +
            what + ".");
    if(!creatorp(this_object())){
        this_object()->AddMagicPoints(-magpoint);
    }
    map_delete(Teaching,who);
    return 1;
}
