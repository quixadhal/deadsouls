#include <lib.h>
#include ROOMS_H
#include <council.h>
#include <daemons.h>

inherit LIB_ROOM;

string get_new_title(object tp);
string get_male(int lev);
string get_female(int lev);
string previous_title(object tp);
int get_cost(string which, int lev);

    int CanReceive(object ob) {
        if(!VOTING_D->is_time_to_vote())
            return ::CanReceive(ob);
        if(creatorp(this_player()) || this_player()->query_level() < 2)
            return ::CanReceive(ob);
        if(VOTING_D->query_voted(this_player()->query_name(),
                    this_player()->query_class()))
            return ::CanReceive(ob);
        else {
            message("my_action", "You have not yet voted for your class leader.  Please do so now.", this_player());
            call_out("move_me", 5, this_player());
        }
        return ::CanReceive(ob);
    }

void move_me(object who) {
    who->eventMoveLiving("/domains/Praxis/"+who->query_class()+"_vote");
    return;
}

void create() {
    object ob;

    ::create();
    SetProperty("no attack",1);
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetShort( "The inner sanctum of the tree of the mages");
    SetLong(
            "Welcome into the heart of the mage tree!\n"
            "Mages come here to advance their knowledge of the magical arts.  "
            "The available commands are <cost>, <advance>, <list (number)>, "
            "<improve stat>, <train skill amount>, and <roll stats>.  Up "
            "through a stairway "
            "guarded by a shimmering %^BLUE%^blue%^RESET%^ light is the "
            "entrance to the tree.  "
            "<help skills> will list all skills with their full, proper names.");
    SetExits( 
            (["up" : "/domains/Praxis/mage_join",
             "council" : "/domains/Praxis/council_hall",
             "east" : "/domains/Praxis/mage_vote",
             "down" : "/domains/Praxis/roots",
             "stairs" : "/domains/Praxis/trunk"]) );

    ob = new("/lib/bboard");
    ob->SetKeyName("board");
    ob->SetNoClean(1);
    ob->SetId( ({ "board", "crystal board", "bulletin board"}) );
    ob->set_board_id("mage_board");
    ob->set_max_posts(25);
    ob->set_edit_ok(MAGE_COUNCIL);
    ob->move("/domains/Praxis/mage_hall");
    ob->SetShort( "the Crystal Board of Mages");
    ob->SetLong(
            "It is a huge slab of crystal into which mages have the power "
            "to cast their thoughts about the class of mages.\n");
}

void init() {
    ::init();
    add_action("cost","cost");
    add_action("roll","roll");
    add_action("train", "train");
    add_action("improve", "improve");
    add_action("advance","advance");
    add_action("list","list");
}

int roll(string str) {
    if(str != "stats") {
        notify_fail("Correct syntax: <roll stats>\n");
        return 0;
    }
    //ROOM_SETTER->do_rolls();
    return 1;
}

string get_male(int lev) {
    string *male_title_str;

    if(!male_title_str) male_title_str = allocate(20);
    male_title_str[19]      ="the new high mortal mage";
    male_title_str[18]      ="the high mage warlock";
    male_title_str[17]      ="the master warlock";
    male_title_str[16]      ="the elder warlock";
    male_title_str[15]      ="the warlock";
    male_title_str[14]      ="the lesser warlock";
    male_title_str[13]      ="the master mage";
    male_title_str[12]      ="the elder mage";
    male_title_str[11]      ="the mage";
    male_title_str[10]      ="the low mage";
    male_title_str[9]	="the grand high enchanter";
    male_title_str[8]	="the high enchanter";
    male_title_str[7]	="the enchanter";
    male_title_str[6]	="the low enchanter";
    male_title_str[5]	="the grand master conjurer";
    male_title_str[4]	="the master conjurer";
    male_title_str[3]	="the conjurer";
    male_title_str[2]	="the apprentice conjurer";
    male_title_str[1]	="the inexperienced magic user";
    male_title_str[0]	="the novice mage";
    return male_title_str[lev-1];
}

string get_female(int lev) {
    string *female_title;

    female_title = ({
            "the new high mortal mage",
            "the high mage witch",
            "the master witch",
            "the elder witch",
            "the witch",
            "the lesser witch",
            "the master mage",
            "the elder mage",
            "the mage",
            "the low mage",
            "the grand high enchantress",
            "the high enchantress",
            "the enchantress",
            "the low enchantress",
            "the grand master conjuress",
            "the master conjuress",
            "the conjuress",
            "the apprentice conjuress",
            "the inexperienced magic user",
            "the novice mage"
    });
    if(lev>20) lev = 20;
    return female_title[20-lev];
}

int advance() { return ADVANCE_D->advance(); }

int train(string str) {
    string which, which_tmp;
    int amount;

    if(!str) {
        notify_fail("Corract syntax: <train skill amount.\n");
        return 0;
    }
    if(sscanf(str, "%s %s %d", which, which_tmp, amount) == 3) which = which +" "+ which_tmp;
    else if(sscanf(str, "%s %d", which, amount) !=2) {
        notify_fail("Correct syntax: <train skill amount>\n");
        return 0;
    }
    which = lower_case(which);
    if(!this_player()->skill_exists(which)) {
        notify_fail("No such skill.\n");
        return 0;
    }
    return ADVANCE_D->train_player(this_player(), which, amount);
}

int improve(string str) {
    string *stats;
    int stat_cost;

    stats = ({ "strength", "intelligence", "wisdom", "dexterity", "constitution", "charisma" });
    if(!str) {
        notify_fail("Improve what?\n");
        return 0;
    }
    str = lower_case(str);
    if(member_array(str, stats) == -1) {
        notify_fail("You have no such stat.\n");
        return 0;
    }
    stat_cost = get_cost(str, this_player()->query_base_stats(str));
    if( this_player()->query_exp()-stat_cost < ADVANCE_D->get_exp( this_player()->query_level() ) ) {
        notify_fail("You are not experienced enough to improve yourself in that way.\n");
        return 0;
    }
    this_player()->SetStat(str, this_player()->query_base_stats(str) + 1);
    this_player()->add_exp(-stat_cost);
    write("You feel much ");
    say(this_player()->query_cap_name()+" looks much ");
    if(str == "strength") tell_room(this_object(), "stronger.\n");
    else if(str == "intelligence") tell_room(this_object(), "more intelligent.\n");
    else if(str == "wisdom") tell_room(this_object(), "wiser.\n");
    else if(str == "dexterity") tell_room(this_object(), "more nimble.\n");
    else if(str == "constitution") tell_room(this_object(), "sturdier.\n");
    else tell_room(this_object(), "more attractive.\n");
    return 1;
}

int get_cost(string stat, int lev) {
    switch(stat) {
        case "intelligence": return ADVANCE_D->get_stat_cost(1, lev); break;
        case "constitution": return ADVANCE_D->get_stat_cost(1, lev); break;
        case "wisdom": return ADVANCE_D->get_stat_cost(1, lev); break;
        default: return ADVANCE_D->get_stat_cost(2, lev); break;
    }
}

int cost(string str) {
    int bing;

    write("Costs for advancement, training, and improvement:\n");
    bing = ADVANCE_D->get_exp( this_player()->query_level() + 1 );
    if(bing < 1) write("level:\t\tIt will cost you nothing to advance.");
    else write("level:\t\t"+bing+"\n");
    write("\nskills: You train by spending the amount of experience you
            desire.\n");
    write("\n");
    write("strength:\t\t" + get_cost("strength",
                this_player()->query_base_stats("strength")) +
            "\t\tconstitution:\t\t" + get_cost("constitution",
                this_player()->query_base_stats("constitution")) );
    write("intelligence:\t\t" + get_cost("intelligence",
                this_player()->query_base_stats("intelligence")) +
            "\t\tdexterity:\t\t" + get_cost("dexterity",
                this_player()->query_base_stats("dexterity")) );
    write("wisdom:\t\t" + get_cost("wisdom",
                this_player()->query_base_stats("wisdom")) +
            "\t\tcharisma:\t\t" + get_cost("charisma",
                this_player()->query_base_stats("charisma")) );
    return 1;
}

int list(string str) {
    int x;

    if(!str) "/domains/Praxis/quest_room"->list_quests(this_player(), 0);
    else {
        if(sscanf(str, "%d", x) != 1) {
            notify_fail("You must give the number of the quest you want listed.\n");
            return 0;
        }
        if(x<1) {
            notify_fail("No such quest.\n");
            return 0;
        }
        "/domains/Praxis/quest_room"->list_quests(this_player(), x);
    }
    return 1;
}

