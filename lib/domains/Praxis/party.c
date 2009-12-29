#include <lib.h>

inherit LIB_ROOM;

static private int __EntryAllowed, __PartyTime;

int query_party_time();
int x;

void create() {
    room::create();
    SetProperties( ([ "light":2, "indoors":1, "no attack":1, 
                "no teleport":1, "no steal":1, "no magic":1 ]) );
    SetShort("%^MAGENTA%^%^BOLD%^In the volcano%^RESET%^");
    SetLong(
            "You are standing on a wide, circular platform, surrounded on "
            "all sides by %^RED%^bubbling lava%^RESET%^.  The platform floats "
            "above the pool of lava at a safe distance, yet close enough for "
            "you to feel the heat as the lava sometimes flares upward. " 
            "The platform is decorated with %^MAGENTA%^floating lights "
            "%^RESET%^of %^BLUE%^varying %^ORANGE%^hues and %^RESET%^size. " 
            "Tables with refreshments have also been placed near the edges of "
            "the platform. ");
    SetItems( 
            ([
             ({"platform", "wide platform", "circular platform"}) :
             "The platform floats "
             "above the pool of lava at a safe distance, yet close enough for "
             "you to feel the heat as the lava sometimes flares upward. " 
             "The platform is decorated with %^MAGENTA%^floating lights "
             "%^RESET%^of %^BLUE%^varying %^ORANGE%^hues and %^RESET%^size. " 
             "Tables with refreshments have also been placed near the " 
             "edges of the platform. ",
             ({"sides", "side", "edges", "edge"}) : 
             "The platform is surrounded on all sides by "    
             "%^RED%^bubbling lava.%^RESET%^",

             ({"pool", "pool of lava", "lava", "bubbling lava"}) : 
             (:this_object(), "look_lava":),       

             ({"lights", "light", "floating lights", "floating light"}) :
             (:this_object(), "look_lights":),
             ({"tables", "table"}) :
             "The tables are covered with many delicious treats. "
             "Bottles containing every conceivable combination "
             "of drink possible are set up along the tables. As a matter-of-fact, you feel like pouring yourself a glass of frosty beverage right now. "
             "You intuit that pour <drinkname> will provide you with a glass of your favorite "
             "frosty beverage. ",
                 ({"treats", "treat"}) : (:this_object(), "look_treat":)
                                           ]) );
    SetExits( ([ "square" : "/domains/Praxis/square" ]) );
}

void init() {
    room::init();
    add_action("cmd_pour", "pour");
}

void start_party(string msg) {
    if(base_name(previous_object()) != "/cmds/mortal/_mudparty") return;
    if(query_party_time()) return;
    message("shout", sprintf("Party announcement from %s: A %s party is now "
                "being held to celebrate %s!", this_player()->query_cap_name(),
                mud_name(), msg), users());
    message("shout", "You have 2 minutes to type \"mudparty join\" in "
            "order to join the party!", users());
    __EntryAllowed = 1;
    __PartyTime = 1;
    call_out("deny_entry", 120);
}

static void deny_entry() { __EntryAllowed = 0; }

int query_party_time() { return __PartyTime; }

int query_entry_allowed() { return __EntryAllowed; }

string look_lava(string unused)
{
    x=random(3)+1;   

    if(x==1)
    {
        message("info",
                "%^RED%^A magnificent geyser of lava explodes into the air and "
                "parts before striking the platform, sparing you certain death. "
                "%^RESET%^",
                environment(this_player()));
    }

    if(x==2)
    {
        message("info",
                "%^RED%^A jet of flame falres from the lava and casts the room "
                "in red-hued light.%^RESET%^", environment(this_player()));
    }

    if(x==3)
    {
        message("info",
                "%^RED%^%^BOLD%^The platform shifts about as a large pillar of lava "
                "gushes up from beneath it.%^RESET%^",
                environment(this_player()));
    }

    if(x==4)
    {
        message("info",
                "%^MAGENTA%^The lava changes hues and from %^RED%^red %^MAGENTA%^to "
                "purple.%^RESET%^",
                environment(this_player()));
    }

    return("The lava is constantly shifting and bubbling. You can see large "
            "chunks of rock floating within it. ");
}


string look_lights(string unused)
{
    x=random(3)+1;

    if(x==1)
    {
        return(
                "A pretty %^YELLOW%^yellow%^RESET%^ orb drifts near to you and casts " 
                "a cheery glow on you. ");
    }

    if(x==2)
    {
        return(
                "A pretty %^BLUE%^blue%^RESET%^ orb drifts near to you and casts " 
                "a mellow glow on you. ");
    }

    if(x==3)
    {
        return(
                "A pretty %^MAGENTA%^purple%^RESET%^ orb drifts near to you and casts " 
                "a purple glow over your body. "); 
    }

    if(x==4)
    {
        return(
                "A pretty %^RED%^red%^RESET%^ orb drifts near to you and casts " 
                "a rosy glow over your body. "); 
    }
}


int look_treat()
{
    message("other_action",
            this_player()->query_cap_name()+" eats and drinks "
            "refreshments from the table after looking them over. ",
            environment(this_player()), this_player());
    message("my_action",
            "After looking at the vast array of treats before you, you "
            "decide to try and sample all of them! Punch, cookies, meats.."
            "The sky is the limit!", this_player());
    return 1;
}

int cmd_pour(string str) {
    object ob;
    string *words;

    //    if(!str) return notify_fail("Pour what?\n");
    //    if(this_player()->query_mp() < 10)
    //      return notify_fail("You are too weak for that!\n");
    //    words = explode(str, " ");
    //    if(words[0] == "A" || words[0] == "An") words[0] = lower_case(words[0]);
    //    ob = new(DRINK);
    //    ob->SetKeyName(words[sizeof(words)-1]);
    //    ob->SetMass(75);
    //    ob->SetId(explode(strip_colours(str), " "));
    //   if(sizeof(words) > 2)
    //      SetAdjectives(words[0..sizeof(words)-2]);
    //    ob->SetShort(implode(words, " "));
    //    ob->SetLong("A nice drink created by "+
    //      this_player()->query_cap_name()+".");
    //    ob->SetStrength(random(30)+2);
    //    ob->set_drink("$N drink $O.", 
    //      "$N drinks $O poured by "+this_player()->query_cap_name()+".");
    //    this_player()->add_hp(-2);
    //    if(ob->move(this_player())) {
    //        message("my_action", "You cannot carry that!", this_player());
    //        ob->destruct();
    //        return 1;
    //    }
    //    message("my_action", "You pour "+implode(words, " ")+".", this_player());
    //    message("other_action", sprintf("%s pours %s.",
    //      this_player()->query_cap_name(), implode(words, " ")),
    //      this_object(), ({ this_player() }));
    return 1;
}
