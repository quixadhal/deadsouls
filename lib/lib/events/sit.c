/*    /lib/sit.c
 *    From the Dead Souls Mud Library
 *    Handles people sitting down in it
 *    Created by Descartes of Borg 961221
 *    Version: @(#) sit.c 1.1@(#)
 *    Last modified: 96/12/21
 */

#include <position.h>

private int          MaxSitters = 1;
private object array Sitters    = ({});

int GetMaxSitters(){
    return MaxSitters;
}

static int SetMaxSitters(int x){
    return (MaxSitters = x);
}

object array GetSitters(){
    return copy(Sitters);
}

mixed eventReceiveSit(object who){
    mixed furn = who->GetProperty("furniture");
    if(furn && objectp(furn)){
        write("You are already using a piece of furniture.");
        return 1;
    }
    Sitters = ({ Sitters..., who });
    who->SetProperty("furniture", " on "+this_object()->GetShort());
    who->SetProperty("furniture_object", this_object());
    return 1;
}

mixed eventReleaseStand(object who){
    Sitters -= ({ who });
    Sitters = filter(Sitters, (: objectp($1) :) );
    if(who) who->RemoveProperty("furniture");
    if(who) who->RemoveProperty("furniture_object");
    return 1;
}

mixed direct_sit_word_obj(){
    Sitters = filter(Sitters, (: $1 && $1->GetPosition()==POSITION_SITTING :));
    if( sizeof(Sitters) >= MaxSitters ){
        return "There is no room to sit there.";
    }
    if(environment() != environment(this_player())){
        return "That's not available for sitting right now.";
    }
    return 1;
}

mixed direct_sit_down_word_obj(){
    return direct_sit_word_obj();
}

int CanGet(object who){
    object *sitters = this_object()->GetSitters();
    if(sizeof(sitters)){
        foreach(object wer in sitters){
            if(!wer || environment(wer) != environment()) this_object()->eventReleaseStand(wer);
        }         if(sizeof(this_object()->GetSitters())){
            write(this_object()->GetSitters()[0]->GetName()+" is using it right now.");
            return 0;
        }
        else return 1;
    }
    else return 1;
}
