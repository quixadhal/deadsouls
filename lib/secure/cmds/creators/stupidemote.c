/*    /secure/cmds/admins/stupidemote.c
 *    From the Dead Souls Mud Library
 *    Adds lame emotes
 *    Created by Descartes of Borg 961214
 *    Version: @(#) stupidemote.c 1.4@(#)
 *    Last modified: 96/12/15
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args){
    string array rules = ({ "", "STR" });
    string array adverbs;
    string emote, xtra, prep;
    if( !(master()->valid_apply(({ "ASSIST" }))) &&
            !member_group(this_player(), "EMOTES") ){
        write("You are not admin, nor a member of the EMOTES group.");
        return 1;
    }

    if( !args || args == "" ) {
        return "Add which emote?";
    }
    if( sscanf(args, "%s %s", emote, xtra) != 2 ) {
        emote = args;
        xtra = 0;
    }
    if( xtra ) {
        if( xtra == "LIV" || xtra == "LVS" ) {
            prep = 0;
        }
        else {
            string token;

            if( strlen(xtra) < 5 ) {
                return "The expression " + xtra + " makes no sense.";
            }
            prep = xtra[0..<5];
            token = xtra[<3..];
            if( member_array(prep, master()->parse_command_prepos_list()) ==
                    -1 ) {
                return "The preposition " + prep + " is not a valid.";
            }
            if( member_array(token, ({ "LIV", "LVS" })) == -1 ) {
                return "The token " + token + " is invalid.";
            }
            rules = ({ rules..., token });
        }
        rules = ({ rules..., xtra, "STR " + xtra, xtra + " STR" });
    }
    else {
        prep = 0;
    }
    if( !SOUL_D->AddVerb(emote, capitalize(emote) + " how?") ) {
        previous_object()->eventPrint("Failed to create emote.");
        return 1;
    }
    foreach(string rule in rules) {
        string msg;

        if( rule == "" ) {
            msg = "$agent_name $agent_verb.";
            adverbs = 0;
        }
        else if( rule == "STR" ) {
            msg = "$agent_name $agent_verb $adverb.";
            adverbs = ({ "-" });
        }
        else if( rule == "LIV" || rule == "LVS" || rule == xtra ) {
            if( prep ) {
                msg = "$agent_name $agent_verb " + prep + " $target_name.";
            }
            else {
                msg = "$agent_name $agent_verb $target_name.";
            }
        }
        else {
            if( prep ) {
                msg = "$agent_name $agent_verb $adverb " + prep +
                    " $target_name.";
            }
            else {
                msg = "$agent_name $agent_verb $target_name $adverb.";
            }
        }
        if( adverbs ) {
            SOUL_D->AddRule(emote, rule, ({ ({ emote }), msg }), adverbs);
        }
        else {
            SOUL_D->AddRule(emote, rule, ({ ({ emote }), msg }));
        }
    }
    previous_object()->eventPrint("Stupid emote added.");
    return 1;
}

string GetHelp(){
    return ("Syntax: stupidemote <EMOTE> [RULE]\n\n"
            "You must be admin or member of the EMOTES group to "
            "use this command.\n"
            "This command allows you to add the most common kinds of "
            "emote straight from the command line.  Using the first syntax, "
            "You can add a simple, untargetted emote.  For example, "
            "if you did <stupidemote cheese>, this would create an emote "
            "that would allow people to do <cheese> and <cheese adverb> "
            "with a message that looks like \"Descartes cheeses.\"\n"
            "The second syntax is for targetted emotes.  You specify a "
            "targetting rule.  Targetting rules can be one of the following:\n"
            "\t* LIV\n"
            "\t* LVS\n"
            "\t* preposition LIV\n"
            "\t* preposition LVS\n"
            "The difference between LIV and LVS just determines whether the "
            "emote can be targetted at a single living being or one or more "
            "living beings.  So, to add the \"smile\" emote, you would "
            "do:\n"
            "> stupidemote smile at LVS\n"
            "and that would give you all the functionality of the \"smile\" "
            "emote that exists on this mudlib.\n\n"
            "See also: addadverb, addemote, removeadverb, removeemote");
}
