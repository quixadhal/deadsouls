/* /lib/detect.c
 * from the Dead Souls LPC Library
 * inheritable object for all tangible game objects
 * created by Descartes of Borg 950207
 */

#include <dirs.h>
#include "include/detect.h"

int direct_detect_wrd_in_obj(string word){
    if( environment() != this_player() &&
            environment() != environment(this_player()) ){
        this_player()->eventPrint("You need better access to it.");
        return 0;
    }
    else return 1;
}

mixed eventDetect(object who, string str, int ability){
    mixed magic;

    magic = GetProperty("magic item");
    if( str == "magic" ){
        if( !magic ){
            if( random(ability) > 50 ){
                who->AddSkillPoints("conjuring", random(ability * 2 + 1));
                message("my_action", "You are certain there is no magic "
                        "in " + GetShort() + ".", who);
                return 1;
            }
            else {
                string *spells;
                string spell;

                if( random(ability) > 50 ){
                    who->AddSkillPoints("conjuring", random(ability));
                    message("my_action", "You do not detect any magic "
                            "in " + GetShort() + ".", who);
                    return 1;
                }
                who->AddSkillPoints("conjuring", random(5));
                spells = map(get_dir(DIR_SPELL_VERBS + "/*.c"), (: $1[0..<3] :));
                spell = spells[random(sizeof(spells))];
                message("my_action", "You vaguely sense \"" + spell + "\".",
                        who);
                return 1;
            }
        }
        else {
            string *spells;
            string spell;

            if( random(ability) > 20 ){
                who->AddSkillPoints("conjuring", random(1 + 2* ability));
                if( arrayp(magic) ) magic = magic[random(sizeof(magic))];
                if( random(ability) > 50 )
                    message("my_action", "You definitely sense \"" +
                            magic + "\".", who);
                else message("my_action", "You vaguely sense \"" + magic +
                        "\".", who);
                return 1;
            }
            who->AddSkillPoints("conjuring", random(5) + 1);
            if( random(ability) > 50 ){
                message("my_action", "You do not detect any magic "
                        "in " + GetShort() + ".", who);
                return 1;
            }
            spells = map(get_dir(DIR_SPELL_VERBS + "/*.c"), (: $1[0..<3] :));
            spell = spells[random(sizeof(spells))];
            message("my_action", "You vaguely sense \"" + spell + "\".", who);
            return 1;
        }
    }
    if( !magic ){
        if( random(ability) > 20 ){
            who->AddSkillPoints("conjuring", random(ability) + 5);
            message("my_action", "You do not sense " + str + ".", who);
        }
        else {
            who->AddSkillPoints("conjuring", random(5));
            message("my_action", "You are not at all certain.", who);
        }
        return 1;
    }
    if( (arrayp(magic) && member_array(str, magic) != -1) ||
            (stringp(magic) && str == magic) ){
        if( random(ability) > 10 ){
            who->AddSkillPoints("conjuring", random(ability));
            message("my_action", "You sense " + str + " in " +
                    GetShort() + ".", who);
        }
        else {
            who->AddSkillPoints("conjuring", random(5));
            message("my_action", "Everything seems fuzzy in your head.", who);
        }
        return 1;
    }
    if( random(ability) > 10 ){
        who->AddSkillPoints("conjuring", random(ability));
        message("my_action", "You do not sense " + str + " in " +
                GetShort() + ".", who);
        return 1;
    }
    who->AddSkillPoints("conjuring", random(5));
    message("my_action", "Everything seems fuzzy in your head.", who);
    return 1;
}
