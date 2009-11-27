/*    /secure/cmds/admins/addemote.c
 *    From the Dead Souls Mud Library
 *    A sane command for doing emote addition
 *    Created by Descartes of Borg 961208
 *    Version: @(#) addemote.c 1.2@(#)
 *    Last modified: 96/12/14
 */

#include <lib.h>
#include <daemons.h>
#include <message_class.h>

inherit LIB_DAEMON;

static void MainMenu();
static void EditErrorMessage(string emote);
static void ShowEmote(string emote);
static void AddEmote(string emote);
static void AddRule(string rule, string emote);

static private void validate() {
    if(!this_player()) return 0;
    if( !(master()->valid_apply(({ "ASSIST" }))) &&
            !member_group(this_player(), "EMOTES") )
        error("Illegal attempt to access addemote: "+get_stack()+" "+identify(previous_object(-1)));
}

static void EnterEmote(string emote, string array emotes) {
    int x = to_int(emote);
    validate();
    if( x < 1 || x > sizeof(emotes) ) {
        if( member_array(emote, emotes) == -1 ) {
            if( emote[0] == 'q' ) {
                return;
            }
            MainMenu();
            return;
        }
    }
    else {
        emote = emotes[x-1];
    }
    ShowEmote(emote);
}

static void EnterEditChoice(string str, string array rules, string emote) {
    int x = to_int(str);
    validate();
    if( x < 1 || x > sizeof(rules) ) {
        if( member_array(str, rules) == -1 ) {
            if( !str || str == "" ) {
                MainMenu();
                return;
            }
            if( str[0] == 'q' ) {
                return;
            }
            else if( str[0] == 'e' ) {
                EditErrorMessage(emote);
                return;
            }
            else if( str[0] == 'a' ) {
                this_player()->eventPrint("Enter verb rule: ", MSG_PROMPT);
                input_to((: AddRule :), emote);
                return;
            }
            else {
                ShowEmote(emote);
                return;
            }
        }
        AddRule(str, emote);
    }
    else {
        AddRule(rules[x-1], emote);
    }
}

static void AddAdverbs(string adv, string emote, string array rules,
        string array verbs, string msg) {
    string array adverbs;
    validate();
    if( !adv || adv == "" ) {
        adverbs = ({});
    }
    else {
        adverbs = map(explode(adv, ","), (: trim :));
    }
    foreach(string rule in rules) {
        SOUL_D->AddRule(emote, rule, ({ verbs, msg }), adverbs);
    }
    this_player()->eventPrint("Emote '" + emote + "' added.");
}

static void AddMessage(string msg, string emote, string array rules,
        string array verbs) {
    validate();
    if( !msg || msg == "" ) {
        this_player()->eventPrint("Which message? [q to quit] ", MSG_PROMPT);
        input_to((: AddMessage :), emote, rules, verbs);
        return;
    }
    if( lower_case(msg) == "q" ) {
        this_player()->eventPrint("Addition of rule aborted.");
        return;
    }
    this_player()->eventPrint("Enter adverbs: ", MSG_PROMPT);
    input_to((: AddAdverbs :), emote, rules, verbs, msg);
}

static void AddVerbs(string list, string emote, string array rules) {
    string array verbs = map(explode(list, ","), (: trim :));
    validate();

    this_player()->eventPrint("Enter message: ", MSG_PROMPT);
    input_to((: AddMessage :), emote, rules, verbs);
}

static void AddRule(string rule, string emote) {
    string array rules = map(explode(rule, ","), (: trim :));
    validate();

    if( !sizeof(rules) ) {
        rules = ({ "" });
    }
    this_player()->eventPrint("Enter the verbs: ", MSG_PROMPT);
    input_to((: AddVerbs :), emote, rules);
}

static void AddErrorMessage(string msg, string emote) {
    validate();
    if( !msg || msg == "" ) {
        this_player()->eventPrint("Which message? [q to quit] ", MSG_PROMPT);
        input_to((: AddErrorMessage :), emote);
        return;
    }
    if( !emote || emote == "" ) {
        this_player()->eventPrint("Which emote? [q to quit] ", MSG_PROMPT);
        input_to((: AddEmote :));
        return;
    }	
    emote = lower_case(emote);
    if( lower_case(msg) == "q" ) {
        this_player()->eventPrint("Addition of emote aborted.");
        return;
    }
    SOUL_D->AddVerb(emote, msg);
    this_player()->eventPrint("Enter a parser rule for " + emote + ": ",
            MSG_PROMPT);
    input_to((: AddRule :), emote);
}

static void AddEmote(string emote) {
    validate();
    if( !emote || emote == "" ) {
        this_player()->eventPrint("Which emote? [q to quit] ", MSG_PROMPT);
        input_to((: AddEmote :));
        return;
    }
    emote = lower_case(emote);
    if( emote == "q" ) {
        this_player()->eventPrint("Addition of emote aborted.");
        return;
    }
    this_player()->eventPrint("Enter error message: ", MSG_PROMPT);
    input_to((: AddErrorMessage :), emote);
}

static void MainMenu() {
    string array emotes = SOUL_D->GetEmotes();
    string array display = allocate(sizeof(emotes));
    int array screen = this_player()->GetScreen() || ({ 80, 25 });
    string tmp;
    int i;
    validate();

    tmp = center("Dead Souls Emote Editor", screen[0]) + "\n";
    for(i=0; i<sizeof(display); i++) {
        display[i] = "[" + (i+1) + "] " + emotes[i];
    }
    tmp += format_page(display, screen[0]/17) + "\n";
    this_player()->eventPrint(tmp, MSG_SYSTEM);
    this_player()->eventPrint("Enter an emote or 'q' to quit: ", MSG_PROMPT);
    input_to((: EnterEmote :), emotes);
}

static void EditErrorMessage(string emote) {
    validate();
    this_player()->eventPrint("Enter new error message: ", MSG_PROMPT);
    input_to(function(string str, string emote) {
            if( str && str != "" ) {
            SOUL_D->SetErrorMessage(emote, str);
            }
            ShowEmote(emote);
            }, emote);
}

static void ShowEmote(string emote) {
    string err = SOUL_D->GetErrorMessage(emote);
    mapping rules = SOUL_D->GetRules(emote);
    int array screen = this_player()->GetScreen() || ({ 80, 25 });
    string tmp = center("Dead Souls Emote Editor", screen[0]) + "\n\n";
    string tmp2 = "";
    string array rule_array = allocate(sizeof(rules));
    int i = 0;
    validate();

    tmp += "%^GREEN%^Emote%^RESET%^: " + emote + "\n";
    tmp += "%^GREEN%^Error Message%^RESET%^: " + err + "\n";
    tmp += "%^GREEN%^Rules%^RESET%^:\n";
    foreach(string rule, mixed array data in rules) {
        rule_array[i] = rule;
        tmp += "[" + (i+1) + "] \"" + rule + "\": " + data[1][1] + "\n";
        tmp += "  Adverbs: " + wrap(item_list(data[0]), screen[0]) + "\n";
        i++;
    }
    //Following fix courtesy of Alecksy
    this_player()->eventPage(explode(tmp,"\n"), MSG_SYSTEM);
    tmp2 = "Enter rule to edit, 'a' to add rule, 'e' to edit error msg, or 'q' to quit: ";
    this_player()->eventPrint(tmp2,MSG_PROMPT);
    input_to( (: EnterEditChoice :), rule_array, emote );
}

mixed cmd(string args) {
    if( !(master()->valid_apply(({ "ASSIST" }))) &&
            !member_group(this_player(), "EMOTES") ){
        write("You are not admin, nor a member of the EMOTES group.");
        return 1;
    }

    if( !args || args == "" ) {
        this_player()->eventPrint("Enter emote name: ", MSG_PROMPT);
        input_to((: AddEmote :));
    }
    else if( args[0] == '-' ) {
        if( args == "-edit" ) {
            MainMenu();
        }
        else if( args == "-add") {
            this_player()->eventPrint("Enter emote name: ", MSG_PROMPT);
            input_to((: AddEmote :));
        }
        else {
            MainMenu();
        }
    }
    else {
        if( member_array(args, SOUL_D->GetEmotes()) == -1 ) {
            AddEmote(args);
        }
        else {
            ShowEmote(args);
        }
    }
    return 1;
}

string GetHelp(){
    return ("Syntax: <addemote>\n"
            "        <addemote EMOTE>\n"
            "        <addemote -edit>\n"
            "        <addemote -add>\n\n"
            "You must be admin or member of the EMOTES group to use this command.\n\n"
            "Used to add and edit emotes in the soul daemon using complex "
            "rules and the Dead Souls messaging system.  In order to add "
            "a new emote using this command, you need to know the following "
            "information:\n"
            "\t* Verb parse rule\n"
            "\t* Verbs to be conjugated in the message\n"
            "\t* The messaging system generic format for the message\n"
            "\t* Any adverbs supported specially for the message\n\n"
            "The verb parse rule is a set of tokens governing how the "
            "command should be entered. The only tokens supported by the "
            "soul are LIV (for one living thing), LVS (for one to many living "
        "things), and STR (for any arbitrary string).  In addition, the "
            "rule can contains prepositions like to, at, on, about, etc.  "
            "For example, to allow a player to do <smile at descartes>, you "
            "need the parse rule <at LIV> or <at LVS>.  <smile oddly at "
            "descartes> would be <STR at LIV> or <STR at LVS>.\n"
            "In general, you should allow players to enter in commands using "
            "correct English and a syntax without prepositions.  For "
            "example, since we have <at LVS>, we should also allow "
            "<LVS>.  To support identical rules like this, when the "
            "<addemote> command asks for a parser rule, you can enter in "
            "multiple identical rules on the same line, separated by "
            "commas:\n"
            "Enter parser rule: at LVS,LVS\n\n"
            "The next step is defining which verbs need conjugating in your "
            "message for the emote.  With the smile emote, the only verb is "
            "\"smile\".  However, for more complex emotes, you may have "
            "multiple verbs which need conjugating.  Just enter them as a "
            "comma separated list.  For example, if you have an emote that "
            "should print the message \"Descartes rants and raves.\", then "
            "you will need to enter the verbs rant and rave:\n"
            "Enter verbs: rant,rave\n\n"
            "The next step is specifying the message that gets shown.  Your "
            "message uses place holders, also called tokens, to stand in "
            "for bits of the message that change depending on point of "
            "view.  In the rant and rave example, Descartes would see:\n"
            "You rant and rave.\n"
            "Everyone else sees:\n"
            "Descartes rants and raves.\n"
            "The only thing that stays the same is the word \"and\".  You "
            "need tokens to stand for the rest.  A full list of tokens is "
            "specified in <help messaging> and they all begin with a $ sign.  "
            "One extremely important thing to remember is that you need "
            "the exact same number of $agent_verb or $target_verb tokens in "
            "your message as the number of verbs you specified for "
            "conjugation.  In the rant and rave example, I need two verb "
            "tokens to match up with my two verbs I am conjugating.  They "
            "will be handled in the order they appear.  In other words, the "
            "rant verb will match to the first verb token, and the rave "
            "verb will match to the second verb token.  Verb tokens count "
            "as any combination of $agent_verb and $target_verb:\n"
            "Enter message: $agent_name $agent_verb and $agent_verb.\n\n"
            "The last thing you need to enter is a list of adverbs.  "
            "As with other lists, this list is comma separated.  And as with "
            "other bits of this system, it has a little complexity to it.  "
            "If you do not enter anything, then no adverbs are used for this "
            "rule.  In fact, you must specify a STR token in the rule for "
            "adverbs to be relevant.  If you do specify an STR rule, you "
            "should specify something on the adverb line.  Any adverbs you "
            "enter are considered special adverbs just for this rule.  If "
            "you wish the general list of adverbs to be available as well, "
            "you can enter a - as one of the adverbs.  If you wish the "
            "player to be able to add anything that comes to mind, enter "
            "a *:\n"
            "Enter adverbs: -");
}
