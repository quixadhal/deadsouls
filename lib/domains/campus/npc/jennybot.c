#include <lib.h>
inherit LIB_BOT;
inherit LIB_ACTIVATE;

object player, bot, ob, noobster;
static string name, watchline;
static int count, active, tip, tipnumber, current_tip, hb, mooch, greeting, greetwait;
string *watchlist = ({});
static string save_file = save_file("/domains/campus/save/jennybot");

mixed GreetingResponse(object who, mixed foo, string message, mixed bar){
    int greet;
    message = lower_case(message);
    if(!strsrch(message, "hi") || !strsrch(message, "hello") ||
            !strsrch(message, "hey") || !strsrch(message, "sup")){
        tell_player(who,"The woman does not respond to your greeting, "+
                "but you sense that you can: \n%^BOLD%^look at woman%^RESET%^");
    }
    return 1;
}

string LongDesc(){
    string ret;
    if(!active){
        ret = "On closer inspection, this attractive "+
            "young lady is no lady at all...she's an android! "+
            "She appears to be totally motionless and frozen "+
            "in place, with a friendly smile. Perhaps you "+
            "can make her do something by typing: %^RED%^activate bot%^RESET%^ ";
    }
    else {
        ret = "On closer inspection, this attractive "+
            "young lady is no lady at all...she's an android! "+
            "She appears to be in the middle of giving an orientation "+
            "on this mud, with bizarrely friendly mannerisms. Perhaps you "+
            "can make her be quiet by typing: %^RED%^deactivate bot%^RESET%^ ";
    }
    return ret;
}


static void create(){
    watchlist = ({});
    ::create();
    RestoreObject(save_file);
    SetKeyName("jennybot");
    SetId(({"guide","guidebot","fembot","bot","jennifer","niffy","android","jenny","robot","woman","lady"}));
    SetAdjectives(({"orientation","young","female","polite","pretty","guide","newbie","simple","extremely"}));
    SetGender("female");
    SetShort("a polite young woman");
    SetLong( (: LongDesc :) );
    SetInventory(([
                "/domains/campus/armor/pillbox_hat" : "wear hat",
                "/domains/campus/armor/wglove_r" : "wear white right glove",
                "/domains/campus/armor/wglove_l" : "wear white left glove",
                "/domains/campus/armor/necklace" : "wear necklace on neck",
                "/domains/campus/armor/bluedress" : "wear dress",
                ]));
    SetMelee(1);
    SetPolyglot(1);
    SetLevel(99);
    SetRace("android");
    SetAction(1, ({
                "Jenny straightens her hair.",
                "Jenny the guide bot touches up her rouge a bit.", 
                "Jenny smiles."}));
    AddCommandResponse("shutdown", (: eventTurnOff :));
    AddCommandResponse("shut down", (: eventTurnOff :) );
    AddCommandResponse("shut up", (: eventTurnOff :));
    AddCommandResponse("go away", (: eventTurnOff :) );
    SetTalkResponses( ([
                "hi" : (: GreetingResponse :),
                "hey" : (: GreetingResponse :),
                "sup" : (: GreetingResponse :),
                "hello" : (: GreetingResponse :),
                ]) );
    set_heart_beat(1);
    ob=this_object();
    count=210;
    tip=0;
    tipnumber = 16;
    greeting = 0;
    greetwait = 0;
    SetLanguage("common",100);
    SetDefaultLanguage("common");
}

varargs int eventGreet(string newbie){
    object noob;
    string guy,prespiel,spiel;
    if((!newbie || newbie == "") || !noobster){
        return 0;
    }
    if(!newbie || newbie == "") newbie = noobster->GetKeyName();
    if(newbie && newbie != "there") noob = find_player(newbie);
    if(noob && newbie != "there") guy = noob->GetName();
    else guy = "there";
    tell_room(environment(this_object()),"The polite young "+
            "lady springs to life!\n");
    prespiel = "Jennybot says, \"%^BOLD%^CYAN%^ Hello, "+guy;
    spiel = read_file("/domains/campus/txt/jenny/spiel.txt");
    tell_room(environment(this_object()),prespiel+spiel);
    tell_room(environment(this_object()),"\n\t%^RED%^activate bot%^RESET%^\n");
    eventForce("yell DEDDA SORUZE: GETTO DA ZE!");
    tell_room(environment(this_object()),"The polite young "+
            "woman becomes totally motionless again.");
    noob->SetProperty("greeted",1);
    return 1;
}

int eventCheckNoob(){
    object array people;
    people=(get_livings(environment(this_object()),1));
    if(sizeof(people)){
        foreach(object dude in people){
            int greeted;
            greeted = dude->GetProperty("greeted");
            if(!greeted && dude->GetLevel() < 2 ) {
                greeting = 1;
                noobster = dude;
            }
        }
    }
    return 1;
}

void init(){
    ::init();
    add_action("next_tip","next");
    eventCheckNoob();
}

int next_tip(string str){
    if(!str) return 0;
    if(str=="") return 0;
    if(str="tip"){
        if(active != 1) { write("Jennybot is not active."); return 1; }
        if(tip == tipnumber) ob->eventForce("say Sorry. No more tips.");
        else {
            this_object()->eventDoTip(tip);
        }
        return 1;
    }
}

int refreshlist(){
    string playername;
    playername = this_player()->GetKeyName();
    if(watchlist && member_array(playername, watchlist) != -1) mooch = 1;
    else {
        if(!watchlist) watchlist = ({});
        watchlist += ({ playername });
        mooch = 0;
    }
    watchlist = distinct_array(watchlist);
    SaveObject(save_file,1);
    return 1;
}

int eventTurnOff(){
    if( active == 0 ){
        write("Jennybot is already inactive.");
    }
    tip = 0;
    if( active != 0) {
        eventForce("yell DEDDA SORUZE: GETTO DA ZE!");
        tell_room(environment(this_object()),"Jenny nods and becomes motionless again, "+
                "her expression now fixed and staring out into "+
                "space.");
    }
    active=0;
    return 1;
}

int eventTurnOn(){
    if(!ob) ob = this_object();
    player=this_player();
    name=this_player()->GetName();
    if(!name || !sizeof(name)) name = "player";
    if(active==1){
        write("Jennybot has already been activated.");
        return 1;
    }
    refreshlist();
    active=1;
    hb=0;
    tip=1;
    write("The female android comes to life! She "+
            "smiles at you and straightens her dress.");
    ob->eventForce("say Hello, "+name+"! I'm Jenny, the LPC University "+
            "newbie guide bot. I'm an extremely simple android, so "+
            "please don't expect a lot of interactivity.");
    ob->eventForce("smile "+name);
    ob->eventForce("say I'm here to give you a few "+
            "tips. To deactivate me, simply "+
            "type: %^RED%^deactivate bot%^CYAN%^.");
    ob->eventForce("say To jump to the next tip, type: next tip");
    return 1;
}

int eventAct4(){
    if(!new("/domains/campus/obj/note")->eventMove(this_object())){
        tell_room(environment(this_object()),"Oops! There's a bug, "+
                "and I don't have a note for you. Let's pretend I gave you "+
                "one and move on. Please email your admin about this, though.");
        return 1;
    }
    if(player && environment(this_object()) == environment(player)) {
        eventForce("give note to "+player->GetName());
    }
    return 1;
}

int eventAct6(){
    if(!new("/domains/campus/obj/map")->eventMove(this_object())){ 
        tell_room(environment(this_object()),"Oops! There's a bug, "+
                "and I don't have a map for you. Let's pretend I gave you "+ 
                "one and move on. Please email your admin about this, though."); 
        return 1;
    }
    if(player && environment(this_object()) == environment(player)) {
        eventForce("give map to "+player->GetName());
    }
    return 1;
}

int eventAct8(){
    if(mooch || !new("/domains/campus/armor/newbie_cap")->eventMove(this_object())){
        tell_room(environment(this_object()),
                "I don't have a hat for you. Let's pretend I gave you "+
                "one and move on.");
    }
    if(mooch || !new("/domains/campus/obj/squirtbag")->eventMove(this_object())){
        tell_room(environment(this_object()),
                "I don't have a bag for you. Let's pretend I gave you "+
                "one and move on.");
    }
    if(player && environment(this_object()) == environment(player)) {
        eventForce("give cap to "+player->GetName());
    }
    if(player && environment(this_object()) == environment(player)) {
        eventForce("give bag to "+player->GetName());
    }
    return 1;
}

int eventAct9(){
    eventForce("smile "+player->GetName());
    if(!new("/domains/campus/meals/badapple")->eventMove(this_object())){
        tell_room(environment(this_object()),"Oops! There's a bug, "+
                "and I don't have a rotten apple for you. Let's pretend I gave you "+
                "one and move on. Please email your admin about this, though.");
    }
    if(!new("/domains/campus/meals/apple")->eventMove(this_object())){
        tell_room(environment(this_object()),"Oops! There's a bug, "+
                "and I don't have an apple for you. Let's pretend I gave you "+
                "one and move on. Please email your admin about this, though.");
    }
    if(player && environment(this_object()) == environment(player)) {
        eventForce("give first apple to "+player->GetName());
        eventForce("give my apple to "+player->GetName());
        return 1;
    }
}

int eventAct11(){
    eventForce("smirk");
    return 1;
}

int eventSwitch(int arg){
    switch(arg){
        case 4:eventAct4();break;
        case 6:eventAct6();break;
        case 8:eventAct8();break;
        case 9:eventAct9();break;
        case 11:eventAct11();break;
        default:write("");break;
                return 1;
    }
}

int eventDoTip(int i){
    tip++;
    hb=0;
    eventSwitch(i);
    if(tip > tipnumber) {
        this_object()->eventTurnOff("bot");
        return 1;
    }
    tell_room(environment(this_object()),read_file("/domains/campus/txt/jenny/"+i+".txt"));

}

void heart_beat(){
    hb++;
    if(greeting) greetwait++;
    if(noobster && greetwait > 0){
        eventGreet();
        noobster = 0;
        greetwait = 0;
        greeting = 0;
    }
    if(hb > 20 && active) eventDoTip(tip);
}
