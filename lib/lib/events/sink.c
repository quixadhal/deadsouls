#include <position.h>
#include <daemons.h>
#include ROOMS_H
#include <medium.h>
#include <message_class.h>

int NoSink;

int SetNoSink(int i){
    return NoSink = i;
}

int GetNoSink(){
    return NoSink;
}

mixed eventSink(){
    int err;
    string rvoid = ROOMS_D->GetVoid(this_object());
    object env = environment();
    mixed tmprumbo, rumbo = 0;
    string name = this_object()->GetName();

    if(!living(this_object())) name = this_object()->GetShort();

    if(!env || ( !(rumbo = env->GetExit("down")) && 
                !(sizeof(rumbo = env->GetSinkRoom())))) return 0;
    if(NoSink || env->GetNoSink()) return 0;
    if(env == rumbo) return 0;
    if(this_object()->GetPosition() == POSITION_SWIMMING){
        return 2;
    }
    if(living(this_object())) this_object()->SetPosition(POSITION_FLOATING);
    if(!rumbo) return 0;
    tmprumbo = rumbo;
    if(stringp(rumbo)) err = catch(rumbo = load_object(rumbo));
    if(err || !rumbo){
        log_file("runtime","\n"+timestamp()+" "+identify(this_object())+
                " could not load "+identify(rumbo)+" to sink into.\n");
        err = catch(rumbo = load_object(rvoid));
    }
    if(err || !rumbo){
        return 0;
    }

    tell_object(this_object(),"You sink downward!");
    if(this_object()->eventMove(rumbo)){
        tell_room(env,capitalize(name)+" continues "+
                "to sink.", ({ this_object() }));
        tell_room(rumbo,capitalize(name)+" floats in from above.",
                ({ this_object() }));

        if(rumbo || sizeof(rumbo->GetSinkRoom())){ 
            call_out( "eventSink", 2);
        }
        return 3;
    }
    return 0;
}
