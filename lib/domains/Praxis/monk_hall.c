#include <lib.h>
#include <council.h>
#include <daemons.h>

inherit LIB_ROOM;

string get_new_title(object tp);
string get_male(int lev);
string get_female(int lev);
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
        message("my_action", "You have not yet voted for class leader.  Please do so now.", this_player());
        call_out("move_me", 5, this_player());
    }
    return ::CanReceive(ob);
}

void move_me(object ob) {
    ob->eventMoveLiving("/domains/Praxis/"+ob->query_class()+"_vote");
    return;
}


void create() {
    object ob, box;

    ::create();
    SetProperty("no attack",1);
    SetProperty("light", 2);
    SetProperty("indoors", 1);
    SetProperty("no castle", 1);
    SetShort( "inside the hall of monks");
    SetLong(
            "Welcome to the Monastary of Eternal Holiness.\n"
            "All monks come here to advance in life and study.  The "
            "available commands are <cost>, <advance>, <list (number)>, "
            "<improve stat>, <train skill amount>, and <roll stats>.  West "
            "through a passage guarded by a shimmering %^BLUE%^blue%^RESET%^ "
            "light is the entrance to the monastery. Type <help skills> to "
            "get a list of the full names of skills.");
    SetExits( 
            (["west" : "/domains/Praxis/monk_join",
             "council" : "/domains/Praxis/council_hall",
             "east" : "/domains/Praxis/monk_vote"]) );

    ob = new(LIB_BOARD);
    ob->SetKeyName("tableau");
    ob->SetId( ({ "holy tableau", "tableau", "monk tableau", "board" }) );
    ob->set_board_id("monk_board");
    ob->set_max_posts(25);
    ob->set_edit_ok(MONK_COUNCIL);
    ob->move("/domains/Praxis/monk_hall");
    ob->SetShort( "the Holy Tableau of Monks");
    ob->SetLong( "A holy tableau into which monks mark their thoughts.\n");
    //box = new("/realms/nialson/monks/donation");
    //box->move(this_object());
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
    call_other("/domains/Praxis/setter", "do_rolls");
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
            title = present(tp->query_guild()+"_ob", tp)->query_title(tp);
            title += " $N";
        }
        else title = "$N";
    }
    else title = "$N";
    if(gen == "male") title += " "+get_male(lev);
    else title += " "+get_female(lev);
    return title;
}

string get_male(int lev) {
    string *male_title_str;

    if(!male_title_str) male_title_str = allocate(20);
    male_title_str[19]      ="the angel";
    male_title_str[18]      ="the prophet";
    male_title_str[17]      ="the saint";
    male_title_str[16]      ="the doctor of divinity";
    male_title_str[15]      ="the devil-dodger";
    male_title_str[14]      ="the pope";
    male_title_str[13]      ="the holy father";
    male_title_str[12]      ="the cardinal priest";
    male_title_str[11]      ="the spiritual father";
    male_title_str[10]      ="the great missionary";
    male_title_str[9]       ="the missionary";
    male_title_str[8]       ="the promoter of the faith";
    male_title_str[7]       ="the high priest";
    male_title_str[6]       ="the priest";
    male_title_str[5]       ="the born again priest";
    male_title_str[4]       ="the evangelist";
    male_title_str[3]       ="the preacher";
    male_title_str[2]       ="the servant of God";
    male_title_str[1]       ="the bible reader";
    male_title_str[0]       ="the choir boy";
    return male_title_str[lev-1];
}

string get_female(int lev) {
    string *fem_title_str;

    if(!fem_title_str) fem_title_str = allocate(20);
    fem_title_str[19]       ="the angel";
    fem_title_str[18]       ="the prophet";
    fem_title_str[17]       ="the saint";
    fem_title_str[16]       ="the doctor of divinity";
    fem_title_str[15]       ="the devil-dodger";
    fem_title_str[14]       ="the pope";
    fem_title_str[13]       ="the mother superior";
    fem_title_str[12]       ="her reverance";
    fem_title_str[11]       ="the cardinal priestess";
    fem_title_str[10]       ="the spiritual mother";
    fem_title_str[9]        ="the great missionary";
    fem_title_str[8]        ="the missionary";
    fem_title_str[7]        ="the promoter of the faith";
    fem_title_str[6]        ="the high priestess";
    fem_title_str[5]        ="the priestess";
    fem_title_str[4]        ="the born again priestess";
    fem_title_str[3]        ="the evangelist";
    fem_title_str[2]        ="the preacheress";
    fem_title_str[1]        ="the sevant of God";
    fem_title_str[0]        ="the choir girl";
    return fem_title_str[lev-1];
}

int advance() { return ADVANCE_D->advance(); }

int train(string str) {
    string which, which_tmp;
    int amount;

    if(!str) {
        notify_fail("Train what?\n");
        return 0;
    }
    if(sscanf(str, "%s %s %d", which, which_tmp, amount) == 3) which = which + " " + which_tmp;
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
        case "dexterity": return ADVANCE_D->get_stat_cost(1, lev); break;
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
    write("skills: You train by spending the amount of experience you
            desire.\n");
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
        if(x <1) {
            notify_fail("No such quest.\n");
            return 0;
        }
        "/domains/Praxis/quest_room"->list_quests(this_player(), x);
    }
    return 1;
}

