#include <lib.h>
#include <council.h>
#include <daemons.h>
#include ROOMS_H

inherit LIB_ROOM;

string get_new_title(object tp);
string get_male(int lev);
string get_female(int lev);
string previous_title(object tp);
int get_cost(string which, int lev);

    int CanReceive(object ob) {
        if(!VOTING_D->is_time_to_vote())
            return ::CanReceive(ob);
        if(VOTING_D->query_voted(this_player()->query_name(),this_player()->query_class()))
            return ::CanReceive(ob);
        if(creatorp(this_player()) || this_player()->query_level() < 2)
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
    SetProperties( (["no attack": 1, "no castle":1,"light":2,"indoors":1]) );
    SetShort( "the inner sanctum of the fighters");
    SetLong(
            "Welcome to the inner sanctum of the Hall of Fighters!\n"
            "Fighters come here to learn about the art of combat. "
            "You are in what appears to be a large training hall. Large mats are "
            "on the floor and weapons hang on the walls, in various shapes and forms. "
            "In this mighty hall, a fighter may <advance>, <cost>, "
            "<list (number)>, <improve stat>, <train skill amount>, and <roll stats>. "
            "Down through a stairway guarded by a shimmering %^BLUE%^blue%^RESET%^ "
            "light is the entrance to the hall.");
    SetExits( ([ 
                "council" : "/domains/Praxis/council_hall",
                "east" : "/domains/Praxis/fighter_vote",
                "down" : "/domains/Praxis/fighter_join",
                ]) );

    ob = new("/lib/bboard");
    ob->SetKeyName("board");
    ob->SetId( ({ "board", "bulletin board", "glory board" }) );
    ob->set_board_id("fighter_board");
    ob->set_edit_ok(FIGHTER_COUNCIL);
    ob->set_max_posts(50);
    ob->move("/domains/Praxis/fighter_hall");
    ob->SetShort( "Glory Board of Fighters");
    ob->SetLong(
            "The Fighters of our reality post tales of their glorious "
            "adventures here, as well as info on the dangers out there.\n");
    //new("/realms/grumpy/fighter/obj/box.c")->move(this_object());
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
    string *male_title_str;

    if(!male_title_str) male_title_str = allocate(20);
    male_title_str[19]      ="the legend";
    male_title_str[18]      ="the wise old master of combat";
    male_title_str[17]      ="the old veteran";
    male_title_str[16]      ="the battle hardened veteran";
    male_title_str[15]      ="is death incarnate";
    male_title_str[14]      ="the war-monger";
    male_title_str[13]     = "the great gladiator";
    male_title_str[12]      ="the experienced gladiator";
    male_title_str[11]      ="the gladiator";
    male_title_str[10]      ="the great champion";
    male_title_str[9]       ="the champion";
    male_title_str[8]       ="the experienced fighter";
    male_title_str[7]       ="the employed fighter";
    male_title_str[6]       ="the fighter for hire";
    male_title_str[5]       ="the rookie fighter";
    male_title_str[4]       ="the amateur fighter";
    male_title_str[3]       ="the experienced squire";
    male_title_str[2]       ="the gung-ho squire";
    male_title_str[1]       ="the timid squire";
    male_title_str[0]       ="the page";
    return male_title_str[lev-1];
}

string get_female(int lev) {
    return get_male(lev);
}

int advance() { return ADVANCE_D->advance(); }

int train(string str) {
    string which, which_tmp;
    int amount;

    if(!str) {
        notify_fail("Correct syntax: <train skill amount>\n");
        return 0;
    }
    if(sscanf(str, "%s %s %d", which, which_tmp, amount) == 3) which = which+ " "+ which_tmp;
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
    string adj;
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
    adj = (str == "strength" ? "stronger" : (str == "intelligence" ? "more intelligent" :
                (str == "wisdom" ? "wiser" : (str == "dexterity" ? "more nimble" :
                                              (str == "constitution" ? "sturdier" : "more attractive")))));
    message("my_action", sprintf("You look %s.", adj), this_player());
    message("other_action", sprintf("%s looks much %s",
                this_player()->query_cap_name(), adj), environment(this_player()),
            ({ this_player() }));
    return 1;
}

int get_cost(string stat, int lev) {
    switch(stat) {
        case "strength": return ADVANCE_D->get_stat_cost(1, lev); break;
        case "constitution": return ADVANCE_D->get_stat_cost(1, lev); break;
        case "dexterity": return ADVANCE_D->get_stat_cost(1, lev); break;
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
