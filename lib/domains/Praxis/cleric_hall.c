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
            message("my_action", "You have not yet voted for you class leader.  Please do so now.", this_player());
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
    SetProperties( ([ "no attack":1, "light" : 2, "indoors" : 1]) );
    SetShort( "the inner sanctum of the clerics");
    SetLong(
            "Welcome to the inner sanctum of the clerics!\n"
            "Clerics come to this room to contemplate their deities. "
            "The room is very sparse. The floor is made from oak, and chairs are set "
            "up all around. Very simple, and unpretensious. A large breeze fills "
            "the room, making it very light and airy. The available options for "
            "a cleric  are <cost>, <advance>, <list (number)>, <improve stat>, "
            "<train skill amount>, and <roll stats>. Through the passage guarded "
            "by a shimmering %^BLUE%^blue%^RESET%^ light is the church.");
    SetExits( 
            (["south" : "/domains/Praxis/cleric_join",
             "down" : "/domains/Praxis/crypt.c",
             "council" : "/domains/Praxis/council_hall",
             "east" : "/domains/Praxis/cleric_vote"]) );

    ob = new(LIB_BOARD);
    ob->SetKeyName("board");
    ob->SetId( ({ "board", "reality board" }) );
    ob->set_board_id("cleric_board");
    ob->set_max_posts(25);
    ob->set_edit_ok(CLERIC_COUNCIL);
    ob->move("/domains/Praxis/cleric_hall");
    ob->SetShort( "the Cleric's Board of Healing");
    ob->SetLong( "Clerics post tales of their experiences of "
            "this reality here.\n");
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
    if(str != "stats") return 0;
    //ROOM_SETTER->do_rolls();
    return 1;
}

string get_new_title(object tp)
{
    int lev;
    string gen, title;

    lev = this_player()->query_level();
    gen = this_player()->query_gender();
    if(this_player()->query_guild()) {
        if(present(tp->query_guild()+"_ob", tp)) {
            if(lev > 19) title = "High mortal";
            else title = present(tp->query_guild()+"_ob", tp)->query_title(tp);
            title += " $N";
        }
        else {
            if(lev > 19) title = "High mortal $N";
            else title = "$N";
        }
    }
    else {
        if(lev > 20) title = "High mortal $N";
        else title = "$N";
    }
    if(lev > 20) title += previous_title(tp);
    else if(gen == "male") title += " "+get_male(lev);
    else title += " "+get_female(lev);
    return title;
}

string get_male(int lev) {
    switch(lev) {
        case 1: return "the novice cleric"; break;
        case 2: return "the new student of nature"; break;
        case 3: return "the student of nature"; break;
        case 4: return "the advanced student of nature"; break;
        case 5: return "the minor believer in natural unity"; break;
        case 6: return "the believer in natural unity"; break;
        case 7: return "the unity of nature"; break;
        case 8: return "the minor low priest"; break;
        case 9: return "the minor priest"; break;
        case 10: return "the high minor priest"; break;
        case 11: return "the low cleric"; break;
        case 12: return "the junior cleric"; break;
        case 13: return "the cleric"; break;
        case 14: return "the cleric"; break;
        case 15: return "the high cleric"; break;
        case 16: return "the low master priest"; break;
        case 17: return "the master priest"; break;
        case 18: return "the high master priest"; break;
        case 19: return "the grand high priest"; break;
        case 20: return "the new high mortal cleric"; break;
        default: return ""; break;
    }
}

string get_female(int lev) {
    switch(lev) {
        case 8: return "the minor low priestess"; break;
        case 9: return "the minor priestess"; break;
        case 10: return "the high minor priestess"; break;
        case 16: return "the low master priestess"; break;
        case 17: return "the master priestess"; break;
        case 18: return "the high master priestess"; break;
        case 19: return "the grand high priestess"; break;
        default: return get_male(lev); break;
    }
}

int advance() { return ADVANCE_D->advance(); }

int train(string str) {
    string which, which_tmp;
    int amount;

    if(!str) {
        notify_fail("Train what?\n");
        return 0;
    }
    if(sscanf(str, "%s %s %d", which, which_tmp, amount) == 3) which = which + " "+which_tmp;
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
    message("Nmy_action", "You feel much ", this_player());
    message("Nother_action", this_player()->query_cap_name()+
            " looks much ", this_object(), ({ this_player() }) );
    if(str == "strength") tell_room(this_object(), "stronger.");
    else if(str == "intelligence") tell_room(this_object(), "more intelligent.");
    else if(str == "wisdom") tell_room(this_object(), "wiser.");
    else if(str == "dexterity") tell_room(this_object(), "more nimble.");
    else if(str == "constitution") tell_room(this_object(), "sturdier.");
    else tell_room(this_object(), "more attractive.");
    return 1;
}

int get_cost(string stat, int lev) {
    switch(stat) {
        case "intelligence": return ADVANCE_D->get_stat_cost(1, lev); break;
        case "wisdom": return ADVANCE_D->get_stat_cost(1, lev); break;
        case "strength": return ADVANCE_D->get_stat_cost(1, lev); break;
        default: return ADVANCE_D->get_stat_cost(2, lev); break;
    }
}

int cost(string str) {
    int bing;

    write("Costs for advancement, training, and improvement:\n");
    bing = ADVANCE_D->get_exp( this_player()->query_level() + 1 );
    if(bing < 1) write("level:\t\tIt will cost you nothing to advance.");
    else write("level:\t\t"+bing+"\n");
    write("skills: You train by spending the amount of experience you
            desire.");
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

string previous_title(object tp) {
    string pre, post, str;

    str = tp->getenv("TITLE");
    sscanf(str, "%s $N %s", pre, post);
    return post;
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

