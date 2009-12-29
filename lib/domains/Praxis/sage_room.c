//  /d/standard/sage_room
//  by Valodin
//  This is where you can learn new languages and fix old ones

#include <lib.h>
#include <daemons.h>
#include <clock.h>

//  This is how many experience points == one second
#define SEC_TO_EXP  3

inherit "/lib/std/room";

#define OLD_LANGUAGES ({ "elvish", "giantish", "orcish", "artrellian", "mischief", "sensuous", "hobbitish", "pseudo-speak", "gnomish" })

mapping queue;

void create() {
    ::create();
    queue = ([]);
    SetProperty("light", 3);
    SetProperty("indoors", 1);
    SetProperty("no steal", 1);
    SetProperty("no attack", 1);
    SetProperty("no magic", 1);
    SetShort( "Sage room");
    SetLong( "The walls of this smoke-filled room are covered with trappings "
            "from far off lands.  In the center of a huge smoke cloud sits a small "
            "white-haired man.  He looks like he is very wise.  There are books on "
            "how to speak different languages.");
    SetItems( 
            ([({"sage", "man", "old man"}) : "The sage is a tiny little "
             "wrinkled man. But he looks very wise.",
             ({"walls", "trappings"}) : "The walls are covered in strange "
             "writings in many languages, but you can hardly see them "
             "through all the smoke from the sage's pipe.", 
             "pipe" : "The sage puffs away on this small water pipe. "
             "The wisps of smoke remind you of the man's beard.",
             ({"book", "books"}) : "As you flip through the books you "
             "decide that you should study a new language."]) );
    AddExit("south", "/domains/Praxis/w_boc_la2", (: "leave_the_room" :) );
}

void init() {
    ::init();
    add_action("ask_sage", "ask");
    add_action("study_lang", "study");
}

int remove_old_lang(object ob) {
    string *langs;
    int i, szl;

    if(!function_exists("query_all_languages", ob))
        return 0;
    langs = ob->query_all_languages();
    for(i = 0, szl = sizeof(langs); i < szl; i++)
        if(member_array(langs[i], OLD_LANGUAGES) != -1)
            ob->remove_language(langs[i]);
    return 1;
}

int fix_languages() {
    write("The sage peers at you intently.");
    if(!remove_old_lang(this_player()))
    {
        write("The sage says: You need new flesh before I can help your mind.");
        return 1;
    }
    //LANG_D->init_languages(this_player());
    write("The sage says: You have been healed, my " + 
            ((this_player()->query_gender() == "male") ? "son" :
             "daughter") + ".");
    return 1;
}

int ask_sage(string str)
{
    string trash;

    notify_fail("Ask who, what?\n");
    if(!str)
        return 0;
    if(sscanf(str, "sage%s", trash, trash) == 1)
        notify_fail("What do you want to ask the sage?\n");
    if(sscanf(str, "sage%sfix%s", trash, trash) == 2)
    {
        return fix_languages();
    }
    return 0;
}

int able_to_study(int amount)
{
    int exp;

    exp = this_player()->query_exp();
    if((exp - amount) < 
            ADVANCE_D->get_exp(this_player()->query_level()))
        return 0;
    else
        return 1;
}

int coclean(mixed *element, object ob)
{
    if(element[0] != ob)
        return 0;
    else
        return 1;
}

void clean_call_outs(object ob)
{
    mixed *all_callouts;
    int i, acsz;

    all_callouts = call_out_info();
    all_callouts = filter_array(all_callouts, "coclean", this_object(), 
            this_object());
    for(i = 0, acsz = sizeof(all_callouts); i < acsz; i++)
        remove_call_out(all_callouts[i][1]);
    for(i = 0, acsz = sizeof(all_callouts); i < acsz; i++)
        if(all_callouts[i][3] != ob)
            call_out(all_callouts[i][1], all_callouts[i][2], all_callouts[i][3]);
}

int convert_time(string str)
{
    string *times;
    int ret, i, j, tln;

    times = explode(str, " ");
    ret = 0;
    tln = sizeof(times) / 2;
    for(i = 0; i < tln; i++)
    {
        j = to_int(times[2 * i]);
        switch(times[2 * i + 1])
        {
            case "seconds":
                case "second":
                j = j * SECOND;
            break;
            case "minutes":
                case "minute":
                j = j * MINUTE;
            break;
            case "hours":
                case "hour":
                j = j * HOUR;
            break;
            case "days":
                case "day":
                j = j * DAY;
            break;
            case "week":
                case "weeks":
                j = j * WEEK;
            break;
            case "month":
                case "months":
                j = j * MONTH;
            break;
            case "year":
                case "years":
                j = j * YEAR;
            break;
            default:
            return 0;
        }
        ret += j;
    }
    return ret;
}

int study_lang(string str)
{
    string lang, timestr;
    int nmtimespan;

    if(!str)
    {
        write("Study which language for how long?");
        return 1;
    }

    if(queue[this_player()])
    {
        write("The sage says: I cannot teach you two languages at once, my " +
                ((this_player()->query_gender() == "male") ? "son." : 
                 "daughter."));
        return 1;
    }

    if(!function_exists("learn_language", this_player()))
    {
        write("The sage says: I am sorry, my " + 
                ((this_player()->query_gender() == "male") ? "son" : 
                 "daughter") + ", but I cannot teach you in your current body.");
        return 1;
    }
    if((sscanf(str, "%s for %s", lang, timestr) != 2) || (lang == "") || 
            (timestr == "") || !(nmtimespan = convert_time(timestr)))
    {
        write("Study which language for how long?");
        return 1;
    }

    lang = lower_case(lang);

    if(nmtimespan <= 0)
    {
        write("The sage shakes his head at you in disgust.");
        return 1;
    }
    if(nmtimespan > DAY)
    {
        write("You decide that you will get hungry and bored long before then.");
        return 1;
    }

    if(!able_to_study(SEC_TO_EXP * nmtimespan))
    {
        write("The sage says: I am sorry, my " + 
                ((this_player()->query_gender() == "male") ? "son" : 
                 "daughter") + ", but you are not experienced enough to study "
                "for that long.");
        return 1;
    }

    write("You begin studying " + capitalize(lang) + ".  You must stay here and"
            " study until your time has ended in order to learn the langauge.");
    queue[this_player()] = ({ lang, (SEC_TO_EXP * nmtimespan) });
    call_out("advance_em", nmtimespan, this_player());

    //if(member_array(lang, (LANG_D->all_languages())) == -1)
    //{
    // write("The sage says: " + capitalize(lang) + " is an uncommon language.  "
    //	  "If you did not mean to study it, then you should leave now, my " +
    //	  ((this_player()->query_gender() == "male") ? "son." :
    //	   "daughter."));
    //   return 1;
    //}
    return 1;
}

int leave_the_room()
{
    if(queue[this_player()])
    {
        write("You decide to quit studying and go do other things.");
        map_delete(queue, this_player());
        clean_call_outs(this_player());
    }
    return 1;
}

void advance_em(mixed arg)
{
    object ob;
    string lang;
    int exp;

    ob = (object)arg;
    if(!ob || (environment(ob) != this_object()))
    {
        if(ob)
            if(queue[ob])
                map_delete(queue,ob);
        return;
    }

    if(!queue[ob])
        return;

    lang = queue[ob][0];
    exp = queue[ob][1];
    ob->add_exp(-exp);
    ob->learn_language(lang, exp);
    write("You are done studying " + capitalize(lang) + ".");
    map_delete(queue, ob);
}
