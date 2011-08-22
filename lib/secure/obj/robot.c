#include <lib.h>
#include NETWORK_H
#include <socket_err.h>
#define ANSI(p) sprintf("%c["+(p)+"m", 27)
#define ESC(p) sprintf("%c"+(p), 27)

inherit LIB_ITEM;

static int observing = 0;
static int counter = 0, attempting, connected, socket ;
static int dud_count = 0, spawning, last_action, loop_count = 0;
static int maxbox = 32;
static int newbot = 1;
static object person, player;
static string preset, name, passwd, gender;
static string display_name, email, real_name, race;
static string *exits, previous_command;
static string travel = "go ";
static int enable = 0;
static string ip = "127.0.0.1 1111";
static string local_currency = "silver";
static string watching = "";
static int broadcast, onhand, open_account, balance, wander;
static int pocket_money = 600;
static int spent, in_combat, recursion_brake;
static mixed *socks_array = ({});

mapping AnsiMap1 =
([ "RESET":ANSI("0"), "BOLD":ANSI(1), "FLASH":ANSI(5),
 "BLACK":ANSI(30), "RED":ANSI(31), "GREEN":ANSI(32),
 "ORANGE":ANSI(33),     "YELLOW":ANSI(1)+ANSI(33), "BLUE": ANSI(34),
 "CYAN":ANSI(36), "MAGENTA":ANSI(35), "BLACK":ANSI(30),
 "WHITE": ANSI(37), "B_RED":ANSI(41), "B_GREEN":ANSI(42),
 "B_ORANGE":ANSI(43), "B_YELLOW":ANSI(1)+ANSI(43), "B_BLUE":ANSI(44),
 "B_CYAN":ANSI(46), "B_BLACK":ANSI(40), "B_WHITE": ANSI(47),
 "CLEARLINE":ESC("[L")+ESC("[G"), "B_MAGENTA":ANSI(45), "STATUS":"",
 "WINDOW":"", "INITTERM":ESC("[H")+ESC("[2J"), "ENDTERM":"" ]); 
mapping AnsiMap2 = ([]);

int parse_comm( string str );
int DoorHandler(string str);
int do_connect(string args);
int do_reset( string args );
int Setup();
string eventBolo(string str);
string eventWatch(string str, string watching);
int eventCombatPrep();

int report(string str){
    object owner;
    if(!this_object() || !(owner = environment(this_object()))){
        return 0;
    }
    if(!owner || !creatorp(owner)) return 0;
    if(observing){
        tell_player(owner, "%^RED%^OBSERVER%^BLUE%^ "+name+"%^RESET%^ "+str);
    }
    return 1;
}

varargs static void validate(int i){
    if(i){
        if(!socket_status(i) || !socket_status(i)[5]){
            error("Bad socket, fd "+i);
        }
    }
}

static void create(mixed arg)
{
    item::create();
    AnsiMap2 = ([]);
    if(arg && stringp(arg)) ip = arg;
    SetShort( "a gamebot" ) ;
    SetLong( "A gamebot control module.");
    SetMass( 5 ) ;
    attempting = 0 ;
    connected = 0 ;
    socket = 0 ;
    person = 0 ;
    parse_init();
    set_heart_beat(0);
    SetNoClean(1);
    if(!name) name = alpha_crypt(random(8)+3);
    SetId(({"bot","gamebot","module",name}));
    name = "xkcd"+name;
    SetKeyName(name);
    if(!passwd) passwd = "password";
    if(!gender) gender = "male";
    if(!display_name) display_name = "Gamebot";
    if(!email) email = "bot@delete.me";
    if(!real_name) real_name = "Chachamaru Karakuri";
    if(!race) race = "human";
    foreach(mixed key, mixed val in AnsiMap1){
        AnsiMap2[val] = key;
    }
    if(!undefinedp(arg) && intp(arg)) maxbox = arg;
}

int eventBroadcastGreen(){
    filter(all_inventory(environment()), (: $1 != this_object() :) )->eventStartBot(ip);
    return 1;
}

int eventStartBot(string str){
    int tmp;
    if(attempting || connected) return 0;
    if(str) ip = str;
    if(str && sscanf(str,"local %d",tmp)){
        ip = "127.0.0.1 "+query_host_port();
        maxbox = tmp;
        write("maxbox: "+maxbox);
        write("ip: "+ip);
    } 
    set_heart_beat(1);
    if(clonep(this_object())) call_out( (: do_connect, ip :), 1);
    return 1;
} 

int eventRespawn(){
    object newbox;
    string *file_names = ({});
    int boxnum = 0;

    if(!spent){
        foreach(object thing in all_inventory(environment())){
            if(base_name(thing) == base_name(this_object())){
                boxnum++;
                file_names += ({ file_name(thing) });
            }
        }
        if(boxnum < maxbox){
            newbox = new(base_name(this_object()), maxbox);
            newbox->eventMove(environment());
            tell_object(environment(newbox), "Respawned box "+boxnum+": "+file_name(newbox));
        }
    }
    spent = 1;
    return 1;
}

int Setup(){
    if(!enable){
        dud_count++;
        if(dud_count > 5){
            if( socket )
            {
                socket_close( socket ) ;
            }
            tell_object(environment(),"I am "+name+" and I have failed. I die.");
            parse_comm("dcon");
            if(sizeof(filter(all_inventory(environment()), (: base_name($1) == base_name(this_object()) :) )) > 1) eventDestruct();
            return 1;
        }
        do_reset("client");
        do_connect(ip);
        call_out( (: Setup :), 60);
    }
    return 1;
}

string random_act(){
    string ret;
    int randy = random(31);
    if(!newbot){
        switch(randy){
            case 0 : ret = "inventory";break;
            case 1 : ret = "score";break;
            case 2 : ret = "who";break;
            case 3 : ret = "stat";break;
            case 4 : ret = "uptime";break;
            case 5 : ret = "hist";break;
            case 6 : ret = "env";break;
            case 7 : ret = "money";break;
            case 8 : ret = "hist newbie";break;
            case 9 : ret = "hist gossip";break;
            case 10 : ret = "mudlist";break;
            case 11 : ret = "version";break;
            case 12 : ret = "open door";break;
            case 13 : ret = "cast buffer";break;
            case 14 : ret = "cast meditate";break;
            case 15 : ret = "search";break;
            case 16 : ret = "put all in my bag";break;
            case 17 : ret = "put all in my box";break;
            case 18 : ret = "put all in my chest";break;
            case 19 : ret = "wear all";break;
            case 20 : ret = "wield a sword in right hand";break;
            case 21 : ret = "wield a knife in right hand";break;
            case 22 : ret = "wield a club in right hand";break;
            case 23 : ret = "kill all";break;
            case 24 : ret = "wimpy 30";break;
            case 25 : in_combat = 0;ret = "look";break;
            case 26 : ret = "position";break;
            case 27 : ret = "click heels";break;
            case 28 : ret = "push button on omni";break;
            case 29 : ret = "pull pin on grenade";break;
            case 30 : ret = "env";break;
            default : ret = "reply :)";break;
        }
    }
    else {
        switch(newbot){
            case 1 : ret = "terminal unknown";break;
            default : ret = "customize strength 15";break;
        }
        if(newbot > 2) newbot = 0;
        else newbot++;
    }
    return ret;
}

string eventStargate(string str){
    report("stargate: "+str);
    if(grepp(str,"idle stargate")){
        int which = random(4);
        switch(which){
            case 0 : parse_comm("dial stargate lab");break;
            case 1 : parse_comm("dial tower");break;
            case 2 : parse_comm("dial campus lab");break;
            case 3 : parse_comm("dial praxis");break;
        }
    }
    if(grepp(str,"outbound stargate")){
        parse_comm("enter stargate");
    }
    return "wave";
}


int think(string str){
    string tmp, ret = "";
    int this_action = time();
    foreach(mixed element in values(AnsiMap1)){
        str = replace_string(str,element,"");
    }
    str = strip_colours(str);
    report(name+" think(\""+str+"\")");
    if(!sizeof(str)) return 0;
    this_object()->eventScanExits(str);
    if(this_action - last_action > 10 && enable){
        parse_comm("stand up");
        in_combat = 0;
    }
    last_action = this_action;

    if(enable) tmp = eventBolo(str);
    if(tmp && tmp == "break") return 1;
    if(sizeof(watching) < 1){
        if(grepp(str, "You bump into ") && grepp(lower_case(str), "door")){
            DoorHandler(str);
        }
        if(grepp(str, "A great sea")){
            travel = "swim ";
            ret = "swim west";
        }
        if(grepp(str, "You stand up.")) travel = "go ";
        if(grepp(str, "a portal forms")) ret = "enter stargate";
        if(grepp(str, "Perhaps you should try crawling.")) travel = "crawl ";
        if(grepp(str, "You are swimming.")) travel = "swim ";
        if(grepp(str, "What name do you wish")) ret = name;
        if(grepp(str, "Please enter a new name:")) ret = name;
        if(grepp(str, "Do you really wish to be known as ")) ret = "y";
        if(grepp(str, "Are you 13 years of age or older")) ret = "y";
        if(grepp(str, "Create a password of at least 5 letters")) ret = passwd;
        if(grepp(str, "Do you use a screen reader for the visuall")) ret = "n";
        if(grepp(str, "Password:")) ret = passwd;
        if(grepp(str, "Invalid password.")) ret = "12345";
        if(grepp(str, "Please confirm your password")) ret = passwd;
        if(grepp(str, "Please choose an interesting gender")) ret = gender;
        if(grepp(str, "Please choose a gender")) ret = gender;
        if(grepp(str, "to appear however you wish using spaces")) ret = name;
        if(grepp(str, "requires a valid email")) ret = email;
        if(grepp(str, "Type 'dcon' to finalize exit.")) ret = "dcon";
        if(grepp(str, "If you do not mind, please enter your real name")) ret = real_name;
        if(grepp(str, "You must now pick a race.")) ret = "pick "+race;
        if(grepp(str, "No such race.")) ret = "pick roman";
        if(grepp(str, "This mud has enabled AUTO_WIZ.")) ret = "player";
        if(grepp(str, "Autosaving...")) ret = random_act();
        if(grepp(str, "You must now pick a class.")) ret = "pick fighter";
        if(grepp(str, "An interactive copy of you currently exists.")){
            ret = "n";
            name = "xkcd"+alpha_crypt(random(8)+3);
        }
        if(grepp(str, "No more attempts allowed")) ret = "foo"; 
        if(grepp(str, "Reconnected.") || grepp(str,"You wear") ) {
            ret = random_act();
            enable = 1;
            wander = 1;
        }
        if(grepp(str, "You may choose to regenerate into a new body here.")) ret = "regenerate";
        if(grepp(str, "press enter:")) ret = "";
        if(grepp(str, "press <spacebar>:")) ret = "q";
        if(grepp(str, "%:")) ret = "";
        if(grepp(str, "Press <return> to continue:")) ret = "";
        if(grepp(str, "stargate")){
            ret = eventStargate(str);
        }
        if(grepp(str, "a portal forms")) ret = "enter stargate";
        previous_command = ret;
    }

    else {
        ret = eventWatch(str, watching);
    }
    ret = trim(ret,"green");
    if(ret && ret != "" && sizeof(ret)) {
        report(" command: "+ret);
        this_object()->parse_comm(ret);
    }
    return 1;
}

string eventBolo(string str){
    string ret = "";
    wander = 0;

    if(grepp(str, "Please enter a new name:")){
        name = alpha_crypt(random(8)+3);
        SetId(({"bot","gamebot","module",name}));
        name = "xkcd"+name;
        SetKeyName(name);
        parse_comm(name);
    }

    if(grepp(str, "To enter a sample set of rooms")){
        parse_comm("customize strength 15");
        parse_comm("pk on");
        parse_comm("pk on");
        parse_comm("wimpy 30");
        parse_comm("chfn");
        return "break";
    }

    if(grepp(str, "Real name ")){
        parse_comm(real_name);
        return "break";
    }

    if(grepp(str, "Wimpy mode:") && grepp(str, "off")){
        parse_comm("wimpy 30");
    }

    if(grepp(str, "Reprompt mode:") && grepp(str, "off")){
        parse_comm("reprompt on");
    }
    if(grepp(str, "Charmode:") && grepp(str, "off")){
        //parse_comm("charmode on");
    }
    if(grepp(str, "Keepalive mode:") && !grepp(str, "off")){
        parse_comm("keepalive off");
    }

    if(grepp(str, "You can't crawl in your current position")){
        parse_comm("position");
        parse_comm("look");
    }

    if(grepp(str, "fight unless you are up")){
        parse_comm("position");
        parse_comm("look");
    }

    if(grepp(str, "Go in which direction?")){
        parse_comm("position");
        parse_comm("look");
    }

    if(grepp(str, "Your injuries prevent easy movement")){
        parse_comm("position");
        parse_comm("look");
    }

    if(grepp(str, "You are prone")){
        travel = "crawl ";
        parse_comm("look");
        report("travel: ("+travel+")");
    }
    if(grepp(str, "You are sitting")){
        travel = "crawl ";
        parse_comm("look");
    }

    if(grepp(str, "You are flying")){
        travel = "fly ";
        parse_comm("look");
    }

    if(grepp(str, "You are standing.")){
        travel = "go ";
        parse_comm("look");
    }

    if(grepp(str, "You are not ")){
        parse_comm("position");
    }

    if(grepp(str, "press enter:") || grepp(str, "%:") ||
            grepp(str,"Press <return> to continue:" )){
        parse_comm("");
    }

    if(grepp(str, "a portal forms")){
        parse_comm("enter stargate");
    }

    if(grepp(str, "stargate")){
        eventStargate(str);
    }

    if(!recursion_brake){
        if(grepp(str, "table")){
            parse_comm("get all from table");
        }

        if(grepp(str, "wardrobe")&& !grepp(str, "There is no ")) {
            parse_comm("open a wardrobe");
            parse_comm("get all from wardrobe");
            parse_comm("wear all");
        }

        if(grepp(str, "chest")&& !grepp(str, "There is no ")) {
            parse_comm("open a chest");
            parse_comm("get all from a chest");
            parse_comm("wear all");
        }

        if(grepp(str, "bag") && !grepp(str, "There is no ")){ 
            parse_comm("open a bag");
            parse_comm("get all from a bag");
            parse_comm("wear all");
        }

        if(grepp(str, "box")&& !grepp(str, "There is no ")) {
            parse_comm("open a box");
            parse_comm("get all from a box");
            parse_comm("wear all");
        }

        if(grepp(str, "a knife rack")){
            parse_comm("get all from rack");
            parse_comm("unwield all");
            parse_comm("wield a carving knife in right hand");
        }

        if(grepp(str, "large stove")&& !grepp(str, "There is no ")){
            parse_comm("open stove");
        }

        if(grepp(str, "You get") && (grepp(str, "flesh") || grepp(str, "corpse"))){
            parse_comm("get all from a corpse");
            parse_comm("get all from my  a pile");
        }

        if(grepp(str, "You swing at")){
            if(!in_combat){
                eventCombatPrep();
            }
        }

        if(grepp(str, " rat ") ){
            parse_comm("kill a rat");
        }

        if(grepp(str, "newt")){
            parse_comm("kill a newt");
        }

        if(grepp(str, "gecko")){
            parse_comm("kill a gecko");
        }

        if(grepp(str, " orc") && ( grepp(str, "is standing here") || grepp(str, "are standing here")) ){
            eventCombatPrep();
            parse_comm("target first orc");
        }

        if(grepp(str, "Mansion Garden")) {
            parse_comm("drop ladder");
            parse_comm("climb ladder");
        }

        if(grepp(str, "Otik, the keeper of the shop")){
            parse_comm("wear all");
            parse_comm("unwield all");
            parse_comm("sell all to otik");
            parse_comm("buy sword from otik");
            parse_comm("wield sword in right hand");
            ret = "say Thank you, Otesanek.";
        }

        if(grepp(str, "Herkimer the kind wizard")){
            parse_comm("ask herkimer to teach buffer");
            parse_comm("ask herkimer to teach meditate");
        }
        recursion_brake = 10;
    }

    if(grepp(str,"You must create an account") ){ 
        parse_comm("say ok, dude");
        parse_comm("request account from zoe");
    }
    if(grepp(str, "Dirk the Tired")){
        parse_comm("ask dirk to advance");
    }

    if(grepp(str, "Zoe the bank teller")){
        parse_comm("say hello, Zoe");
        parse_comm("money");
        parse_comm("ask zoe for balance");
        if((onhand - pocket_money) > 0) 
            parse_comm("ask zoe to deposit "+(onhand - pocket_money)+" "+local_currency);
        if(balance - pocket_money + onhand > 0 && (onhand + balance - 5) < pocket_money) 
            parse_comm("ask zoe to withdraw "+(pocket_money - onhand)+" "+local_currency);
    }

    if(grepp(str,"In your pockets you find only")){
        string s1, s2;
        int i1;
        if(sscanf(str,"%s %d "+local_currency+"%s",s1,i1,s2) < 3)
            onhand = 0;
        else onhand = i1;
    }

    if(grepp(str, "and open an account with") ||
            grepp(str,"You already have an account with") )
        open_account = 1;

    if(grepp(str, "Your last transaction:")){
        string s1, s2, s3, s4;
        int i1;
        if(sscanf(str,"%s"+local_currency+"%s:%s\n%s",s1,s2,s3,s4) == 4){
            balance = atoi(trim(s3));
        }
    }
    wander = 1;
    return ret;
}

void heart_beat(){
    int bots;
    counter++;
    if(recursion_brake) recursion_brake--;

    if(!environment(this_object())) return;
    if(!adminp(environment(this_object()))) return;
    if(!clonep(this_object())) return;
    bots = sizeof(filter(all_inventory(environment()),
                (: base_name($1) == base_name(this_object()) :)));

    if((!spent || bots < 2) && enable && bots < maxbox) 
    {
        call_out( (: eventRespawn :), 2 );
    }

    if(!enable) return;
    if(!(counter % 5)) this_object()->eventWalkabout();
    if(!broadcast) eventBroadcastGreen();
    if(!(counter % 20) && travel == "crawl ") parse_comm("stand up");
    if(counter > 1000) counter = 0;
}

void init(){
    ::init();
    add_action( "do_connect", ({ "connect", "telnet" }) ) ;
    add_action( "do_reset", "reset" ) ;
    add_action( "do_reconnect", "reconnect" ) ;
    add_action( "eventStartBot", "bot");
    add_action( "do_observe","observe");
}

int do_observe(string str){
    if(str == name ){
        if(!observing) observing = 1;
        else observing = 0;
        return 1;
    }
    return 0;
}

int DoorHandler(string str){
    string s1;
    if(!sscanf(str,"You bump into %s.",s1)){
        parse_comm("open first door");
        return 0;
    }
    parse_comm("unlock first door with first key");
    parse_comm("unlock first door with second key");
    parse_comm("open "+s1);
    parse_comm("open first door");
    return 1;
}

int eventScanExits(string str){
    string s1, s2, s3;
    string *oldexits = ({travel+"north",travel+"south",travel+"east",travel+"west", travel+"up", travel+"down",travel+"out"});
    string *newexits = ({});
    string *dirs = ({});
    str = strip_colours(str);
    if(grepp(str,"too dark to see") || grepp(str,"too bright to see") ||
            grepp(str,"go nowhere at all")){
        exits = ({});
    }
    if(!sizeof(exits) && grepp(str," leaves ")){
        if(sscanf(str,"%s leaves %s.",s1,s2) == 2){
            exits = ({travel+s2});
        }
    }
    if(sscanf(str,"%sObvious exit: %s\n%s",s1,s2,s3) == 3 ||
            sscanf(str,"%sObvious exits: %s\n%s",s1,s2,s3) == 3 ||    
            sscanf(str,"%sObvious exit: %s",s1,s2) == 2 ||
            sscanf(str,"%sObvious exits: %s",s1,s2) == 2){
        dirs = explode(s2,", ");
    }
    else if(sscanf(str,"%s [%s]%s", s1, s2, s3) ){
        if(s2) dirs = explode(s2,",");
    }
    if(sizeof(dirs)){
        string Esc = sprintf("%c",27);
        oldexits = ({});
        foreach(string dir in dirs){
            string junk1, junk2;
            if(grepp(dir, Esc)){
                if(sscanf(dir,"%s"+Esc+"%s",junk1,junk2)) dir = junk1;
            }
            if(!dir || !sizeof(dir)) continue;
            dir = trim(dir);
            switch(dir){
                case "u" : dir = "up";break;
                case "d" : dir = "down";break;
                case "o" : dir = "out";break;
                case "n" : dir = "north";break;
                case "s" : dir = "south";break;
                case "e" : dir = "east";break;
                case "w" : dir = "west";break;
                case "ne" : dir = "northeast";break;
                case "nw" : dir = "northwest";break;
                case "se" : dir = "southeast";break;
                case "sw" : dir = "southwest";break;
            }
            if(grepp(dir,"enter ")) newexits += ({ dir });
            else newexits += ({ travel+dir });
        }
        if(sizeof(newexits)) {
            exits = newexits;
            enable = 1;
        }
    }
    if(!sizeof(exits)) exits = oldexits;
    report("exits: "+identify(exits));
    return 1;
}

int eventWalkabout(){
    int randy = random(sizeof(exits));
    if(in_combat) return 0;
    parse_comm(exits[randy]);
    parse_comm("get all");
    return 1;
}

int eventCombatPrep(){
    parse_comm("unwield all");
    parse_comm("wear all");
    parse_comm("wield first sword in right hand");
    parse_comm("wield first knife in right hand");
    parse_comm("wield first club in right hand");
    parse_comm("wield first axe in right hand");
    parse_comm("wield first dagger in right hand");
    parse_comm("wield first staff in right hand and left hand");
    parse_comm("wield first axe in right hand and left hand");
    parse_comm("wear pack");
    parse_comm("put all in bag");
    parse_comm("put all in pack");
    parse_comm("drop all");
    in_combat = 1;
    return 1;
}

void SetConnection(string str){
    if(str) preset = str;
}

int do_reconnect()
{
    if( !connected )
    {
        notify_fail( "The telnet client is not connected!\n" ) ;
        return 0 ;
    }
    person = this_object() ;
    return 1 ;
}

int do_reset( string args )
{
    notify_fail( "Usage: reset client\n" ) ;
    if( !args || args == "" )
    {
        return 0 ;
    }
    if( args != "client" )
    {
        return 0 ;
    }
    if( connected )
    {
        if( socket )
        {
            socket_close( socket ) ;
        }
    }
    attempting = 0 ;
    connected = 0 ;
    socket = 0 ;
    person = 0 ;
    return 1 ;
}

int do_connect(string args)
{
    int new_socket, sc_result, port ;
    string error, ip_address ;

    if( !args || args == "" )
    {
        notify_fail( "You fail." ) ;
        return 0 ;
    }
    if( sscanf( args, "%s %d", ip_address, port ) != 2 )
    {
        notify_fail( "You fail." ) ;
        return 0 ;
    }
    if( attempting )
    {
        notify_fail( "Telnet connection attempt already in progress.\n" ) ;
        return 0 ;
    }
    if( connected )
    {
        notify_fail( "Already connected...\n" ) ;
        return 0 ;
    }
    new_socket = socket_create( STREAM, "read_callback", "close_callback" ) ;

    write("new_socket: "+new_socket);
    loop_count++;
    if(loop_count > 10) this_object()->eventDestruct();

    foreach(mixed element in socket_status()){
        if(intp(element[0]) && element[0] != -1 && !grepp(element[3],"*")){ 
            if(!socks_array) socks_array = ({});
            socks_array += ({ element[0] });
        }
    }
    socks_array = ({});

    if( new_socket < 0 )
    {
        switch( new_socket )
        {
            case EEMODENOTSUPP :
                error = "Socket mode not supported.\n" ;
                break ;
            case EESOCKET :
                error = "Problem creating socket.\n" ;
                break ;
            case EESETSOCKOPT :
                error = "Problem with setsockopt.\n" ;
                break ;
            case EENONBLOCK :
                error = "Problem with setting non-blocking mode.\n" ;
                break ;
            case EENOSOCKS :
                error = "No more available efun sockets.\n" ;
                break ;
            case EESECURITY :
                error = "Security violation attempted.\n" ;
                break ;
            default :
                error = "Unknown error code: " + new_socket + ".\n" ;
                break ;
        }
        notify_fail( "Unable to connect, problem with socket_create.\n"
                "Reason: " + error ) ;
        return 0 ;
    }
    sc_result = socket_connect( new_socket, ip_address + " " + port,
            "read_callback", "write_callback" ) ;
    if( sc_result != EESUCCESS )
    {
        notify_fail( "Failed to connect.\n" ) ;
        return 0 ;
    }
    attempting = 1 ;
    socket = new_socket ;
    person = (object)previous_object() ;
    player=this_object();
    tell_object(environment(),"I am "+name+" , a.k.a "+file_name(this_object())+
            " and I am connected to "+ip+" on socket"+socket+"\n");
    spent = 0;
    return 1 ;
}

void read_callback( int fd, mixed message )
{
    validate(fd);
    player->eventPrint(message);
    player->think(message);
}

void close_callback( int fd )
{
    validate(fd);
    if( connected )
    {
    }
    if( attempting )
    {       
    }
    socket_close( fd ) ;
    attempting = 0 ;
    connected = 0 ;
    socket = 0 ;
}

void write_callback( int fd )
{
    validate(fd);
    attempting = 0 ;
    connected = 1 ;
}

int parse_comm( string str ){
    int write_stat = 0;
    string tmpstr = "";
    int i = sizeof(str);
    for(int j = 0;j < i;j++){
        if(str[j] == 32) tmpstr += " ";
        else if(str[j] > 32 && str[j] < 128) tmpstr += str[j..j];
    }
    str = replace_string(tmpstr,sprintf("%c",27)+"[0m","");
    report("parse_comm(\""+str+"\")");
    if(str=="dcon" || str=="quit")
    {
        socket_close( socket ) ;
        attempting = 0 ;
        connected = 0 ;
        socket = 0 ;
        person = 0 ;
        this_object()->eventDescribeEnvironment();
        return 1 ;
    } else {
        if( !connected )
        {
            int fco;
            enable = 0;
            fco = find_call_out("Setup");
            if(fco == -1) Setup();
            return 1 ;
        }
        if( attempting )
        {
            int fco;
            enable = 0;
            fco = find_call_out("Setup");
            if(fco == -1) Setup();
            return 1 ;
        }
        validate(socket);
        write_stat = socket_write( socket, str + "\n" ) ;
        return 1 ;
    }
}
