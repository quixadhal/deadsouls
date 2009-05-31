/*    /secure/sefun/messaging.c
 *    From the Dead Souls Mud Library
 *    Messaging efuns, based on ideas shamelessly borrowed from Lima
 *    Created by Descartes of Borg 961029
 *    Version: @(#) messaging.c 1.10@(#)
 *    Last modified: 96/12/16
 */

#include <pov.h>

string create_message(int pov, string array verb, string message,
        string subject, string subject_gen, string targets,
        string target_gen, mapping special) {
    string array words = explode(message, " ");
    string array control = copy(words);
    int i, maxi = sizeof(words);
    int verb_count = 0;
    int c;

    switch(pov) {
        case POV_SUBJECT:
            for(i=0; i<maxi; i++) {
                string punctuation = "";

                if( words[i] == "" ) {
                    continue;
                }
                else if( words[i][0] == '\\' ) {
                    if( strlen(words[i]) < 2 ) {
                        continue;
                    }
                    if( words[i][1] != '$' ) {
                        continue;
                    }
                    words[i] = words[i][1..];
                    continue;
                }
                else if( words[i][0] != '$' ) {
                    continue;
                }
                while( words[i] != "" ) { // break this or hang
                    c = words[i][<1];
                    if( c < 'a' || c > 'z' ) {
                        punctuation = words[i][<1..<1] + punctuation;
                        words[i] = words[i][0..<2];
                    }
                    else {
                        break;
                    }
                }
                switch(words[i]) {
                    case "$agent_verb":
                        words[i] = verb[verb_count++];
                    break;

                    case "$agent_name": case "$agent_nominative":
                        case "$agent_objective":
                        words[i] = "you";
                    break;

                    case "$agent_possessive_noun":
                        words[i] = "your";
                    break;

                    case "$agent_possessive":
                        words[i] = "your";
                    break;

                    case "$agent_reflexive":
                        words[i] = "yourself";
                    break;

                    case "$target_verb":
                        if( targets == subject ) {
                            words[i] = verb[verb_count];
                        }
                        else {
                            if( target_gen == "plural" ) {
                                words[i] = verb[verb_count];
                            }
                            else {
                                words[i] = pluralize(verb[verb_count]);
                            }
                        }
                    verb_count++;
                    break;

                    case "$target_name":
                        if( subject == targets ) { // subject? object?
                            if( i == 0 ) { // certainly subject
                                words[i] = "you";
                            }
                            else if( i == sizeof(words)-1 ) { // probably object
                                words[i] = "yourself";
                            }
                            else if( words[i+1] == "$target_verb" ) { // subject?
                                words[i] = "you";
                            }
                            else if(member_array("$agent_verb",control[0..i]) != -1 ) {
                                words[i] = "yourself"; // getting desperate here
                            }
                            else { // Oh hell
                                words[i] = "you";
                            }
                        }
                        else {
                            words[i] = targets;
                        }
                    break;

                    case "$target_nominative":
                        if( subject == targets ) {
                            words[i] = "you";
                        }
                        else {
                            if( !target_gen ) {
                                words[i] = targets;
                            }
                            else if( target_gen == "plural" ) {
                                words[i] = "they";
                            }
                            else {
                                words[i] = nominative(target_gen);
                            }
                        }
                    break;

                    case "$target_objective":
                        if( subject == targets ) {
                            words[i] = "yourself";
                        }
                        else {
                            if( !target_gen ) {
                                words[i] = targets;
                            }		    
                            else if( target_gen == "plural" ) {
                                words[i] = "them";
                            }
                            else {
                                words[i] = objective(target_gen);
                            }
                        }
                    break;

                    case "$target_possessive_noun":
                        if( subject == targets ) {
                            words[i] = "your";
                        }
                        else {
                            if( !target_gen ) {
                                words[i] = possessive_noun(targets);
                            }
                            else if( target_gen == "plural" ) {
                                words[i] = "their";
                            }
                            else {
                                words[i] = possessive_noun(targets);
                            }
                        }
                    break;

                    case "$target_possessive":
                        if( subject == targets ) {
                            words[i] = "your";
                        }
                        else {
                            if( !target_gen ) {
                                words[i] = possessive_noun(targets);
                            }
                            else if( target_gen == "plural" ) {
                                words[i] = "their";
                            }
                            else {
                                words[i] = possessive(target_gen);
                            }
                        }
                    break;

                    case "$target_reflexive":
                        if( subject == targets ) {
                            words[i] = "yourself";
                        }
                        else {
                            if( !target_gen ) {
                                words[i] = targets;
                            }
                            else if( target_gen == "plural" ) {
                                words[i] = "themselves";
                            }
                            else {
                                words[i] = reflexive(target_gen);
                            }
                        }
                    break;

                    default:
                    if( special[words[i]] ) {
                        words[i] = special[words[i]];
                    }
                }
                words[i] = words[i] + punctuation;
            }
            return capitalize(implode(words, " "));

        case POV_TARGET:
            for(i=0; i<maxi; i++) {
                string punctuation = "";

                if( words[i] == "" ) {
                    continue;
                }
                else if( words[i][0] == '\\' ) {
                    if( strlen(words[i]) < 2 ) {
                        continue;
                    }
                    if( words[i][1] != '$' ) {
                        continue;
                    }
                    words[i] = words[i][1..];
                    continue;
                }
                else if( words[i][0] != '$' ) {
                    continue;
                }
                while( words[i] != "" ) { // break this or hang
                    c = words[i][<1];
                    if( c < 'a' || c > 'z' ) {
                        punctuation = words[i][<1..<1] + punctuation;
                        words[i] = words[i][0..<2];
                    }
                    else {
                        break;
                    }
                }
                switch(words[i]) {
                    case "$agent_verb":
                        words[i] = pluralize(verb[verb_count++]);
                    break;

                    case "$agent_name":
                        words[i] = subject;
                    break;

                    case "$agent_nominative":
                        if( !subject_gen ) {
                            words[i] = subject;
                        }
                        else {
                            words[i] = nominative(subject_gen);
                        }
                    break;

                    case "$agent_objective":
                        if( !subject_gen ) {
                            words[i] = subject;
                        }
                        else {
                            words[i] = objective(subject_gen);
                        }
                    break;

                    case "$agent_possessive_noun":
                        words[i] = possessive_noun(subject);
                    break;

                    case "$agent_possessive":
                        if( !subject_gen ) {
                            words[i] = possessive_noun(subject);
                        }
                        else {
                            words[i] = possessive(subject_gen);
                        }
                    break;

                    case "$agent_reflexive":
                        if( !subject_gen ) {
                            words[i] = subject;
                        }
                        else {
                            words[i] = reflexive(subject_gen);
                        }
                    break;

                    case "$target_verb":
                        words[i] = verb[verb_count++];
                    break;

                    case "$target_name": case "$target_nominative":
                        case "$target_objective":
                        words[i] = "you";
                    break;

                    case "$target_possessive_noun":
                        words[i] = "your";
                    break;

                    case "$target_possessive":
                        words[i] = "your";
                    break;

                    case "$target_reflexive":
                        words[i] = "yourself";
                    break;

                    default:
                    if( special[words[i]] ) {
                        words[i] = special[words[i]];
                    }
                }
                words[i] = words[i] + punctuation;
            }
            return capitalize(implode(words, " "));

        case POV_OBSERVER:
            for(i=0; i<maxi; i++) {
                string punctuation = "";

                if( words[i] == "" ) {
                    continue;
                }
                else if( words[i][0] == '\\' ) {
                    if( strlen(words[i]) < 2 ) {
                        continue;
                    }
                    if( words[i][1] != '$' ) {
                        continue;
                    }
                    words[i] = words[i][1..];
                    continue;
                }
                else if( words[i][0] != '$' ) {
                    continue;
                }
                while( words[i] != "" ) { // break this or hang
                    c = words[i][<1];
                    if( c < 'a' || c > 'z' ) {
                        punctuation = words[i][<1..<1] + punctuation;
                        words[i] = words[i][0..<2];
                    }
                    else {
                        break;
                    }
                }
                switch(words[i]) {
                    case "$agent_verb":
                        //Following fixed line provided by Aransus @ Pyloros
                        words[i] = ""+pluralize(verb[verb_count++]);
                    break;

                    case "$agent_name":
                        words[i] = subject;
                    break;

                    case "$agent_nominative":
                        if( !subject_gen ) {
                            words[i] = subject;
                        }
                        else {
                            words[i] = nominative(subject_gen);
                        }
                    break;

                    case "$agent_objective":
                        if( !subject_gen ) {
                            words[i] = subject;
                        }
                        else {
                            words[i] = objective(subject_gen);
                        }
                    break;

                    case "$agent_possessive_noun":
                        words[i] = possessive_noun(subject);
                    break;

                    case "$agent_possessive":
                        if( !subject_gen ) {
                            words[i] = possessive_noun(subject);
                        }
                        else {
                            words[i] = possessive(subject_gen);
                        }
                    break;

                    case "$agent_reflexive":
                        if( !subject_gen ) {
                            words[i] = subject;
                        }
                        else {
                            words[i] = reflexive(subject_gen);
                        }
                    break;

                    case "$target_verb":
                        if( target_gen != "plural" ) {
                            words[i] = pluralize(verb[verb_count]);
                        }
                        else {
                            words[i] = verb[verb_count];
                        }
                    verb_count++;
                    break;

                    case "$target_name": 
                        words[i] = targets;
                    break;

                    case "$target_nominative":
                        if( !target_gen ) {
                            words[i] = targets;
                        }
                        else if( target_gen == "plural" ) {
                            words[i] = "they";
                        }
                        else {
                            words[i] = nominative(target_gen);
                        }
                    break;

                    case "$target_objective":
                        if( !target_gen ) {
                            words[i] = targets;
                        }
                        else if( target_gen == "plural" ) {
                            words[i] = "them";
                        }
                        else {
                            words[i] = objective(target_gen);
                        }
                    break;

                    case "$target_possessive_noun":
                        if( !target_gen ) {
                            words[i] = possessive_noun(targets);
                        }
                        else if( target_gen == "plural" ) {
                            words[i] = "their";
                        }
                        else {
                            words[i] = possessive_noun(targets);
                        }
                    break;

                    case "$target_possessive":
                        if( !target_gen ) {
                            words[i] = possessive_noun(targets);
                        }
                        else if( target_gen == "plural" ) {
                            words[i] = "their";
                        }
                        else {
                            words[i] = possessive(target_gen);
                        }
                    break;

                    case "$target_reflexive":
                        if( !target_gen ) {
                            words[i] = targets;
                        }
                        else if( target_gen == "plural" ) {
                            words[i] = "themselves";
                        }
                        else {
                            words[i] = reflexive(target_gen);
                        }
                    break;

                    default:
                    if( special[words[i]] ) {
                        words[i] = special[words[i]];
                    }
                }
                words[i] = words[i] + punctuation;
            }
            return capitalize(implode(words, " "));

        default:
            error("Unknown POV type: " + pov + "\n");
    }
}

void send_messages(mixed verb, string message, object subject, mixed targets,
        mixed observers, mapping special) {
    string sname = subject->GetName();
    string sgen = (subject->GetGender() || "neuter");
    string tname, tgen;

    if( arrayp(targets) ) {
        if( !sizeof(targets) ) {
            tname = tgen = 0;
        }
        else if( sizeof(targets) == 1 ) {
            if( living(targets[0]) ) {
                tname = targets[0]->GetName();
                tgen = (targets[0]->GetGender() || "neuter");
            }
            else {
                tname = targets[0]->GetShort();
                tgen = "neuter";
            }
            targets = targets[0];
        }
        else {
            if( member_array(subject, targets) != -1 ) {
                targets -= ({ subject });
                if( sizeof(targets) == 1 ) {
                    if( living(targets[0]) ) {
                        tname = targets[0]->GetName();
                        tgen = (targets[0]->GetGender() || "neuter");
                    }
                    else {
                        tname = targets[0]->GetShort();
                        tgen = "neuter";
                    }
                    targets = targets[0];
                }
                else {
                    tname = item_list(targets);
                    tgen = "plural";
                }
            }
        }
    }
    else if( objectp(targets) ) {
        if( living(targets) ) {
            tname = targets->GetName();
            tgen = (targets->GetGender() || "neuter");
        }
        else {
            tname = targets->GetShort();
            tgen = "neuter";
        }
    }
    else {
        tname = 0;
        tgen = 0;
    }
    if( stringp(verb) ) {
        verb = ({ verb });
    }
    if( subject ) {
        string str = create_message(POV_SUBJECT, verb, message, sname, sgen,
                tname, tgen, special);

        subject->eventPrint(str);
    }
    if( targets && targets != subject ) {
        string str = create_message(POV_TARGET, verb, message, sname, sgen,
                tname, tgen, special);

        targets->eventPrint(str);
    }
    if( observers ) {
        string str = create_message(POV_OBSERVER, verb, message, sname, sgen,
                tname, tgen, special);
        object array exclude = ({});

        if( subject ) {
            exclude = ({ subject });
        }
        if( objectp(targets) ) {
            exclude = ({ exclude..., targets });
        }
        else if( arrayp(targets) ) {
            exclude = ({ exclude..., targets... });
        }
        observers->eventPrint(str, exclude);
    }
}    
