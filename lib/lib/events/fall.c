#include <position.h>
#include <medium.h>
#include <message_class.h>

mixed eventFall() {
    object env = environment();
    mixed rumbo = 0;
    string name = this_object()->GetName();

    if(!living(this_object())) name = this_object()->GetShort();

    if(!env || !(rumbo = env->GetExit("down"))) return 0;
    if(env == rumbo) return 0;
    if(env->GetMedium() != MEDIUM_AIR){
        send_messages("fall", "$agent_name $agent_verb to the ground.",
          this_object(), 0, env);
        if(living(this_object())) this_object()->SetPosition(POSITION_LYING);
        return 1;
    }
    else {
        //tc("rumbo: "+identify(rumbo));
        if(!rumbo) return 0;
        if(stringp(rumbo)) rumbo = load_object(rumbo);
        if(!rumbo){
            //tc("Couldn't load rumbo!","red");
            return 0;
        }
        tell_object(this_object(),"You plummet downward!");
        //tc("I am "+identify(this_object())+" and my location is :"+identify(environment()),"red");
        if(this_object()->eventMove(rumbo)){
            //tc("I am "+identify(this_object())+" and my location is :"+identify(environment()),"green");
            //tc("env is: "+identify(env));
            //tc("rumbo is: "+identify(rumbo));
            //flush_messages();
            //rumbo->eventPrint(this_object()->GetName()+" plummets in from above.",MSG_ENV);
            //tell_room(rumbo,this_object()->GetName()+" plummets in from above.",
            //({ this_object() }));
            //flush_messages();
            tell_room(env,capitalize(name)+" continues "+
              possessive(this_object())+" fall downward.", ({ this_object() }));
            tell_room(rumbo,capitalize(name)+" plummets in from above.",
              ({ this_object() }));

            if(rumbo->GetMedium() == MEDIUM_AIR && !(this_object()->CanFly())){ 
                call_out( "eventFall", 1);
            }
            //flush_messages();
        }
        return 1;
    }
    return 0;
}
