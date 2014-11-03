#include ROOMS_H
#include <daemons.h>
#include <position.h>
#include <medium.h>
#include <message_class.h>

mixed eventFall(){
    string rvoid = ROOMS_D->GetVoid(this_object());
    object env = environment();
    mixed rumbo = 0;
    mixed tmprumbo = 0;
    string name = this_object()->GetName();

    if(!living(this_object())) name = this_object()->GetShort();

    if(!env || !(rumbo = env->GetExit("down"))){
        if(!sizeof((rumbo = env->GetSinkRoom()))){
            return 0;
        }
    }
    if(env == rumbo){
        return 0;
    }
    if(env->GetMedium() != MEDIUM_AIR){
        send_messages("crash", "$agent_name $agent_verb down!",
                this_object(), 0, env);
        if(living(this_object())) this_object()->SetPosition(POSITION_LYING);
        remove_call_out("eventFall");
        return 1;
    }
    else {
        mixed err;
        if(!rumbo){
            return 0;
        }
        tmprumbo = rumbo;
        if(stringp(rumbo)){
            if(grepp(rumbo, "#")) err = "Let's not load a clone.";
            else err = catch(rumbo = load_object(rumbo));
        }
        if(err || !rumbo){
            log_file("runtime","\n"+timestamp()+" "+identify(this_object())+
                    " could not load "+identify(rumbo)+" to fall into.\n");
            err = catch(rumbo = load_object(rvoid));
        }
        if(err || !rumbo){
            this_object()->eventMove(ROOM_START);
            return 0;
        }
        tell_object(this_object(),"You plummet downward!");
        if(err = this_object()->eventMove(rumbo)){
            tell_room(rumbo,capitalize(name)+" plummets in from above.",
                    ({ this_object() }));
            tell_room(env,capitalize(name)+" continues "+
                    possessive(this_object())+" fall downward.", ({ this_object() }));

            if(rumbo->GetMedium() == MEDIUM_AIR && !(this_object()->CanFly())){ 
                if(!this_object()->GetFalling() && find_call_out("eventFall") == -1){
                    call_out( "eventFall", 1);
                }
            }
        }
        return 1;
    }
    return 0;
}
