/*    /daemon/emote.c
 *    From the Dead Souls Object Library
 *    A centralized soul handler
 *    Created by Descartes of Borg 961207
 *    Version: @(#) soul.c 1.11@(#)
 *    Last modified: 96/12/15
 */

#include <lib.h>
#include <pov.h>
#include <privs.h>
#include <save.h>
#include <daemons.h>

inherit LIB_DAEMON;

class emote {
    string ErrorMessage;
    mapping Rules;
}

class rule {
    mixed array Message;
    string array Adverbs;
}

private mapping Emotes = ([]);
private string array Adverbs = ({});

static private void validate() {
    if(!this_player()) return 0;
    if( !(master()->valid_apply(({ "ASSIST" }))) && 
            !member_group(this_player(), "EMOTES") )
        error("Illegal attempt to access SOUL_D: "+get_stack()+" "+identify(previous_object(-1)));
}

varargs int AddAdverbs(string array advs...) {
    validate();
    Adverbs = distinct_array(Adverbs + advs);
    eventSave();
    return 1;
}

varargs int AddRule(string verb, string rle, mixed array msg,
        string array advs) {
    class emote e = Emotes[verb];
    class rule r;
    validate();

    if( !e ) {
        return 0;
    }
    r = new(class rule);
    r->Message = msg;
    r->Adverbs = (advs || ({}));
    e->Rules[rle] = r;
    if( catch(parse_add_rule(verb, rle)) ) {
        return 0;
    }
    eventSave();
    return 1;
}

int AddVerb(string verb, string err) {
    class emote e;
    validate();

    if( Emotes[verb] ) {
        return 0;
    }
    else {
        e = new(class emote);
    }
    e->ErrorMessage = err;
    e->Rules = ([]);
    Emotes[verb] = e;
    eventSave();
    return 1;
}

int RemoveRule(string emt, string rle) {
    class emote e = Emotes[emt];
    validate();

    if( !e ) {
        return 0;
    }
    map_delete(e->Rules, rle);
    eventSave();
    return 1;
}

int RemoveVerb(string verb) {
    validate();
    map_delete(Emotes, verb);
    eventSave();
    return 1;
}

string array GetAdverbs() {
    return copy(Adverbs);
}

varargs mixed array GetChannelEmote(string emote, string parse, string args) {
    class emote e = Emotes[emote];
    mapping adverb = ([]);
    class rule r;

    if( !e ) {
        return 0;
    }
    r = e->Rules[parse];
    if( !r ) {
        parse = replace_string(parse, "LIV", "LVS");
        r = e->Rules[parse];
        if( !r ) {
            return 0;
        }
    }
    if( args ) {
        string array pool = r->Adverbs;
        int any_adv = (member_array("*", pool) != -1);

        if( member_array("-", pool) != -1 ) {
            pool += Adverbs;
        }
        if( !any_adv && member_array(args, pool) == -1 ) {
            string array matches = regexp(pool, "^" + args);

            if( !sizeof(matches) ) {
                return 0;
            }
            adverb = ([ "$adverb" : matches[0] ]);
        }
        else {
            adverb = ([ "$adverb" : args ]);
        }
    }
    return ({ r->Message, adverb });
}

string array GetEmotes() {
    return keys(Emotes);
}

string GetErrorMessage(string verb) {
    class emote e = Emotes[verb];

    if( !e ) {
        return 0;
    }
    return e->ErrorMessage;
}

string GetHelp(string arg) {
    class emote e;
    string array rls;
    string str;

    if( arg == "soul" ) {
        str = "Your \"soul\" is a system of expressions you can use "
            "to express how you are feeling.  Though it does not really "
            "cause anything to happen, other people, including NPC's, may "
            "react to your emotions, especially when they are violent or "
            "negative.\n\n"
            "Some soul commands allow you to express an adverb to give some "
            "sort of emphasis to the expression.  Some commands are limited "
            "to a certain set of adverbs, while most commands will allow you "
            "to choose from the list of system wide adverbs given below.  "
            "You may always use your racial adverb in any expression "
            "allowing an adverb.  A racial adverb is simply a way of "
            "emoting unique to your race, like \"smile gnomishly\".\n\n"
            "For a list of soul commands, try <help feelings>.\n\n"
            "The list of system adverbs are:\n";
        str += format_page(sort_array(Adverbs, 1), 5);
        return str;
    }
    e  = Emotes[arg];
    if( !e ) {
        return 0;
    }
    rls = keys(e->Rules);
    if( !sizeof(rls) ) {
        return "This emote is not yet valid.";
    }
    else {
        int w = member_array("", rls);

        if( w > 0 ) {
            if( w < sizeof(rls) -1 ) {
                rls = ({ rls[w], rls[0..(w-1)]..., rls[(w+1)..]... });
            }
            else {
                rls = ({ rls[w], rls[0..(w-1)]... });
            }
        }
    }
    if( rls[0] == "" ) {
        str = "Syntax: <" + arg + ">\n";
    }
    else {
        class rule r = e->Rules[rls[0]];
        string array pool = r->Adverbs;
        int all_advs = (member_array("-", r->Adverbs) != -1);
        int anything = (member_array("*", r->Adverbs) != -1);
        string rule = rls[0];

        rule = replace_string(rule, "LIV", "SINGLE_LIVING");
        rule = replace_string(rule, "LVS", "ONE_OR_MORE_LIVINGS");
        if( anything ) {
            rule = replace_string(rule, "STR", "PHRASE");
        }
        else if( all_advs ) {
            pool = (pool - ({ "-", "*" })) + ({ "system adverbs" });
            rule = replace_string(rule, "STR", "ADVERB");
        }
        str = "Syntax: <" + arg + " " + rule + ">\n";
        if( !anything ) {
            if( sizeof(pool) ) {
                str += " Adverbs: " + item_list(pool);
            }
        }
        str += "\n";
    }
    if( sizeof(rls) > 1 ) {
        rls = rls[1..];
        foreach(string rule in rls) {
            class rule r = e->Rules[rule];

            if( rule == "" ) {
                str += "        <" + arg + ">\n";
            }
            else {
                string array pool = r->Adverbs;
                int all_advs = (member_array("-", r->Adverbs) != -1);
                int anything = (member_array("*", r->Adverbs) != -1);

                rule = replace_string(rule, "LIV", "SINGLE_LIVING");
                rule = replace_string(rule, "LVS", "ONE_OR_MORE_LIVINGS");
                if( anything ) {
                    rule = replace_string(rule, "STR", "PHRASE");
                }
                else if( all_advs ) {
                    pool = (pool - ({ "-", "*" })) + ({ "system adverbs" });
                    rule = replace_string(rule, "STR", "ADVERB");
                }
                str += "        <" + arg + " " + rule + ">";
                if( !anything ) {
                    if( sizeof(pool) ) {
                        str += " Adverbs: " + item_list(pool);
                    }
                }
                str += "\n";
            }
        }
    }
    str += "\n";
    str += capitalize(arg) + " is a soul command and affects nothing.\n";
    str += "System adverbs are listed in <help soul>.\n";
    str += "SINGLE_LIVING: You can target a single living thing\n"
        "ONE_OR_MORE_LIVINGS: You can target multiple people using \"all\"\n"
        "PHRASE: Any random phrase\n"
        "ADVERB: Any adverb from the list of supported adverbs, or your "
        "racial adverb.";
    return str;
}

void SetErrorMessage(string verb, string msg) {
    class emote e = Emotes[verb];
    validate();

    if( !e ) {
        AddVerb(verb, msg);
    }
    else {
        e->ErrorMessage = msg;
        eventSave();
    }
}

string GetRaceAdverb(mixed who) {
    string res;

    if( objectp(who) ) {
        res = who->GetRace();
    }

    if( !res ) {
        return "godly";
    }
    switch(res) {
        case "gnome": return "gnomishly";
        case "elf": return "elvenly";
        case "half-elf": return "half-elvenly";
        case "orc": return "orcishly";
        case "half-orc": return "half-orcishly";
        default: return res + "ly";
    }
}

mapping GetRules(string emote) {
    class emote e = Emotes[emote];
    mapping m = ([]);

    if( !e ) {
        return 0;
    }
    foreach(string rle, class rule r in e->Rules) {
        m[rle] = ({ r->Adverbs, r->Message });
    }
    if( !master()->valid_apply(({ PRIV_ASSIST }))  
            || (this_player() && !member_group(this_player(), "EMOTES"))){
        return copy(m);
    }
    return m;
}

int CanTarget(object who, string verb, object target, string rule) {
    if( Emotes[verb] ) {
        return 1;
    }
    else {
        return 0;
    }
}

mixed can_verb_rule(string verb, string rle) {
    class emote e = Emotes[verb];
    class rule r;

    if( !e ) {
        return 0;
    }
    r = e->Rules[rle];
    if( !r ) {
        return 0;
    }
    return 1;
}

varargs mixed do_verb_rule(string verb, string rle, mixed args...) {
    object env = environment(this_player());
    class emote e = Emotes[verb];
    class rule r = e->Rules[rle];
    string adv = 0;

    switch( rle ) {
        case "":
            args = 0;
            adv = 0;
            break;

        case "LIV": case "at LIV": case "with LIV": case "to LIV":
        case "around LIV": case "on LIV":
        case "OBJ": case "at OBJ": case "with OBJ": case "to OBJ":
        case "LVS": case "at LVS": case "with LVS": case "to LVS":
        case "around LVS": case "on LVS":
            args = args[0];
            adv = 0;
            break;

        case "STR": case "for STR": case "about STR":
            adv = args[0];
            args = 0;
            break;

        case "STR LIV": case "STR at LIV": case "STR with LIV":
        case "STR around LIV": case "STR on LIV":
        case "for STR LIV": case "for STR to LIV":
        case "STR OBJ": case "STR at OBJ": case "STR with OBJ":
        case "STR LVS": case "STR at LVS": case "STR with LVS":
        case "STR around LVS": case "STR on LVS":
        case "for STR LVS": case "for STR to LVS":
            adv = args[0];
            args = args[1];
            break;

        case "LIV STR": case "at LIV STR": case "LIV of STR": case "with LIV STR":
        case "around LIV STR": case "on LIV STR":
        case "LIV for STR": case "to LIV for STR":
        case "OBJ STR": case "at OBJ STR": case "OBJ of STR": case "with OBJ STR":
        case "LVS STR": case "at LVS STR": case "LVS of STR": case "with LVS STR":
        case "around LVS STR": case "on LVS STR":
        case "LVS for STR": case "to LVS for STR":
            adv = args[1];
            args = args[0];
            break;

        default:
            this_player()->eventPrint("Unknown soul syntax.");
            return 1;
    }
    if( arrayp(args) ) {
        args = filter(args, (: objectp :));
    }
    if( adv ) {
        string array pool = r->Adverbs + ({ GetRaceAdverb(this_player()) });
        int any_adv = (member_array("*", pool) != -1);

        if( member_array("-", pool) != -1 ) {
            pool += Adverbs;
        }
        if( !any_adv && member_array(adv, pool) == -1 ) {
            string array matches = regexp(pool, "^" + adv);

            if( !sizeof(matches) ) {
                this_player()->eventPrint("You cannot " + verb + " " + adv +
                        "!");
                return 1;
            }
            adv = matches[0];
        }
        send_messages(r->Message[0], r->Message[1], this_player(), args, env,
                ([ "$adverb" : adv ]));
        if( args ) {
            args->eventReceiveEmote(this_player(), verb, adv);
        }
    }
    else {
        send_messages(r->Message[0], r->Message[1], this_player(), args, env);
        if( args ) {
            args->eventReceiveEmote(this_player(), verb);
        }
    }
    return 1;
}

static void create() {
    SetSaveFile(SAVE_SOUL);
    daemon::create();
    if(!file_exists(GetSaveFile()) && 
            file_exists(old_savename(GetSaveFile()))){
        cp(old_savename(GetSaveFile()), GetSaveFile());
    }
    SetNoClean(1);
    parse_init();
    foreach(string verb, class emote data in Emotes) {
        foreach(string rle in keys(data->Rules)) {
            parse_add_rule(verb, rle);
        }
    }
    eventSave();
}
