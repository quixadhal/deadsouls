#include <position.h>
#include <medium.h>
#include <message_class.h>

mixed eventSink() {
    object env = environment();
    mixed rumbo = 0;
    string name = this_object()->GetName();

    if(!living(this_object())) name = this_object()->GetShort();

    if(!env || ( !(rumbo = env->GetExit("down")) && 
        !(sizeof(rumbo = env->GetSinkRoom())))) return 0;
    if(env == rumbo) return 0;
    if(this_object()->GetPosition() == POSITION_SWIMMING){
        return 1;
    }
    if(living(this_object())) this_object()->SetPosition(POSITION_FLOATING);
    if(!rumbo) return 0;
    if(stringp(rumbo)) rumbo = load_object(rumbo);
    if(!rumbo){
        return 0;
    }
    tell_object(this_object(),"You sink downward!");
    if(this_object()->eventMove(rumbo)){
        tell_room(env,capitalize(name)+" continues "+
          "to sink.", ({ this_object() }));
        tell_room(rumbo,capitalize(name)+" floats in from above.",
          ({ this_object() }));

        if(sizeof(rumbo->GetSinkRoom())){ 
            call_out( "eventSink", 2);
        }
        return 1;
    }
    return 0;
}
