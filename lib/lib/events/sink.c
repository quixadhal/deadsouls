#include <position.h>
#include <rooms.h>
#include <medium.h>
#include <message_class.h>

//int direct_sink(){ return 1; }
//int direct_sink_down(){ return 1; }

mixed eventSink(){
    int err;
    object env = environment();
    mixed tmprumbo, rumbo = 0;
    string name = this_object()->GetName();

    if(!living(this_object())) name = this_object()->GetShort();

    if(!env || ( !(rumbo = env->GetExit("down")) && 
        !(sizeof(rumbo = env->GetSinkRoom())))) return 0;
    if(env == rumbo) return 0;
    if(this_object()->GetPosition() == POSITION_SWIMMING){
        return 2;
    }
    if(living(this_object())) this_object()->SetPosition(POSITION_FLOATING);
    if(!rumbo) return 0;
    tmprumbo = rumbo;
    if(stringp(rumbo)) err = catch(rumbo = load_object(rumbo));
    if(err || !rumbo){
        //tc("tmprumbo (sink) for failed roomload: "+identify(tmprumbo));
        err = catch(rumbo = load_object(ROOM_VOID));
    }
    if(err || !rumbo){
        //tc("eventSink failed for "+identify(this_object()),"red");
        //if(environment()) tc("Current env: "+identify(environment()),"red");
        //tc("desired location: "+identify(tmprumbo),"red");
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
