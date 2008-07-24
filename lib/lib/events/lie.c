#include <position.h>
#include <lib.h>
inherit LIB_SIT;

private int          MaxLiers = 1;
private object array Liers    = ({});

int GetMaxLiers(){
    return MaxLiers;
}

static int SetMaxLiers(int x){
    return (MaxLiers = x);
}

object array GetLiers(){
    return copy(Liers);
}

mixed eventReceiveLay(object who){
    mixed furn = who->GetProperty("furniture");
    if(furn && objectp(furn)){
        write("You are already using a piece of furniture.");
        return 1;
    }
    Liers = ({ Liers..., who });
    who->SetProperty("furniture", " on "+this_object()->GetShort());
    who->SetProperty("furniture_object", this_object());
    return 1;
}

mixed eventReleaseStand(object who){
    Liers -= ({ who });
    Liers = filter(Liers, (: objectp($1) :) );
    sit::eventReleaseStand(who);
    return 1;
}

mixed direct_lie_word_obj(){
    Liers = filter(Liers, (: $1 && $1->GetPosition()==POSITION_LYING :));
    if( sizeof(Liers) >= MaxLiers ){
        return "There is no room to lie there.";
    }
    if(environment() != environment(this_player())){
        return "That's not available for sitting right now.";
    }
    return 1;
}

mixed direct_lie_down_word_obj(){
    return direct_lie_word_obj();
}

int CanGet(object who){
    object *liers = this_object()->GetLiers();     if(sizeof(liers)){
        foreach(object wer in liers){             if(!wer || environment(wer) != environment()) this_object()->eventReleaseStand(wer);
        }
        if(sizeof(this_object()->GetLiers())){
            write(this_object()->GetLiers()[0]->GetName()+" is using it right now.");
            return 0;
        }
        else return sit::CanGet(who);
    }
    else return sit::CanGet(who);
}
