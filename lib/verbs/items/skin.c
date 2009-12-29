/*    /verbs/items/skin.c
 *    From the Dead Souls Mud Library
 *    Verb for skinging things
 *    Created by Descartes of Borg 951030
 *    Version: @(#) skin.c 1.2@(#)
 *    Last modified: 97/01/01
 */

#include <lib.h>
#include <rounds.h>
#include <position.h>

inherit LIB_VERB;

string skin; 

static void create() {
    verb::create();
    SetVerb("skin");
    SetRules("STR with OBJ", "OBJ with OBJ");
    SetErrorMessage("Skin with what?  Is there anything in particular to skin?");
    SetHelp("Syntax:  <skin THING with TOOL>\n\n"
            "Allows you to skin with skinning tools.  Sometimes you may "
            "be required to specify what it is you wish to skin.  In "
            "that case, you can use the thing field to specify what it is "
            "you wish to skin.\n\n"
            "See also: fillet");
}


static void eventSkin(object who, object tool, object target, object check) {
    if( !who || check != environment(who) ) {
        return;
    }
    if( !tool || (environment(tool) != who) ) {
        who->eventPrint("You have lost your equipment.");
        return;
    }
    skin = target->GetSkin();
    if(skin){
        // write( "You Skin a " + object->GetShort() + " with a " + tool->GetShort() + ".");
        new(skin)->eventMove(environment(who));
    }
}

varargs static void eventPrepareSkin(object who, object tool, object what) {
    function f = (: eventSkin($(who), $(tool), $(what), environment($(who))) :);
    if( this_player()->GetInCombat() ) {
        send_messages("start", "$agent_name $agent_verb to skin with " +
                tool->GetShort() + ".", who, 0, environment(who));
        who->SetAttack(0, f, ROUND_OTHER);
    }
    else {
        evaluate(f);
    }
}

mixed can_skin_with_obj(object tool) {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything!";
    }
    if( this_player()->GetPosition() != POSITION_STANDING ) {
        return "skin when you are not standing?";
    }
    return this_player()->CanManipulate();
}

mixed can_skin_obj_with_obj(object corpse, object tool) {
    return can_skin_with_obj(tool);
}

mixed do_skin_obj_with_obj(object what, object ob, string id) {
    eventPrepareSkin(this_player(), ob, what);
    return 1;
}
