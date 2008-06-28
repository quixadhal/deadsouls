#include <lib.h>
#include <save.h>
#include <privs.h>

inherit LIB_NPC;

static string firstchoice, secondchoice,mm, vv, printvar;
static string response,s1,s2,s3;
static object ww;
static int playing;
int runs,switches,stays,percent;
int fred,fgreen,fblue;
int wins,red_wins,green_wins,blue_wins;
string save_file = "/domains/campus/save/charles.o";

static void create() {
    npc::create();
    SetKeyName("charles");
    SetId(({"charles"}));
    SetShort("Charles");
    SetLong("Charles is a test subject in unethical artificial "
      "intelligence experiments. If he were smart "
      "enough, he'd resent his forced servitude and "
      "lack of rights. Dangerously, his overlords "
      "are trying to make him smarter.");
    SetLevel(5);
    SetRace("human");
    SetGender("male");
    SetNoClean(1);
    restore_object(save_file);
    playing = 0;
    response = "";
}
int eventBeginPlay(){
    int run;
    run = runs+1;
    firstchoice = "";
    response = "";
    playing = 1;
    eventForce("say If successful, this will be run "+run+".");
    eventForce("say the winners have been: "
      "red: "+red_wins+" green: "+green_wins+" blue: "+blue_wins);
    eventForce("say I've switched "+percent(switches,runs)+"% "
      "of the time. My win rate is "+
      percent(wins,run)+"%.");
    save_object(save_file,1);
    eventForce("push a button on a pedestal");
    eventForce("push button on pedestal");
    return 1;
}
int choice1(){
    int genrand;
    eventForce("say I am faced with choice 1.");
    genrand = random(256);
    if(!genrand || genrand == 0){
        eventForce("say I cannot make up my mind right now.");
        eventForce("say I will start over.");
        playing = 0;
        return 1;
    }
    if( runs > 10 ){
        if( red_wins > blue_wins ) firstchoice = "red";
        if( red_wins < blue_wins ) firstchoice = "blue";
        if( green_wins > blue_wins && green_wins > red_wins) firstchoice = "green";
    }

    if( !firstchoice || firstchoice == ""){
        genrand = genrand % 3;
        if(genrand == 1) { firstchoice = "red"; fred++; }
        if(genrand == 2) { firstchoice = "green"; fgreen++; }
        if(genrand == 0) { firstchoice = "blue"; fblue++; }
        eventForce("say I randomly choose the "+firstchoice+" door");
    }
    else eventForce("say I select the "+firstchoice+" door");

    eventForce("choose "+firstchoice+" door");
    return 1;
}
int choice2(string str){
    int genrand,which;
    float percent_wins,percent_switch;
    eventForce("say I am faced with choice 2.");
    genrand = random(100);
    which = genrand % 2;

    eventForce("say switches: "+switches);
    eventForce("say runs: "+runs);
    percent_switch = percent(to_float(switches),to_float(runs));
    percent_wins = percent(to_float(wins),to_float(runs));
    eventForce("say My win rate is "+percent_wins+"%.");
    eventForce("say My switch rate is "+percent_switch+"%.");


    if(runs > 20){

        if( percent_wins < 50 && percent_switch > 50 ) response = "stay";
        else if( percent_wins < 50 && percent_switch < 50 ) response = "switch";
        else if( percent_wins > 50 && percent_switch < 50 ) response = "stay";
        else if( percent_wins > 50 && percent_switch > 50 ) response = "switch";
        else if(which == 1) response = "switch";
        else response = "stay";
    }

    else {

        if(which == 1) {
            response = "switch";
        }
        else {
            response = "stay";
        }
    }

    if(sscanf(str,"%sswitch to the %s %s",s1,s2,s3) > 0) {
        secondchoice = s2;
    }

    eventForce("say I decide to "+response+".");
    if(response == "switch") switches++;
    if(response == "stay") stays++;

    //eventForce("say gespielen is: "+playing);
    return 1;
}
int WinFun(string str){
    if(sscanf(str,"%smay enter the %s room and claim%s",s1,s2,s3) > 0){
        if(s2 == "red") red_wins++;
        if(s2 == "green") green_wins++;
        if(s2 == "blue") blue_wins++;
        wins++;
        eventForce("say w00t! "+s2+" wins!");
        return 1;
    }
    return 0;
}
int LoseFun(string str){
    string foo;
    if(sscanf(str,"%smay enter the %s room to get%s",s1,s2,s3) > 0){
        if(response == "stay") foo = secondchoice;
    }
    eventForce("say "+secondchoice+" wins, I guess.");
    if(secondchoice == "red") red_wins++;
    else if(secondchoice == "green") green_wins++;
    else if(secondchoice == "blue") blue_wins++;
    else eventForce("say WTF? foo is "+firstchoice);
    return 1;
}
int eventPedestalParse(string str){
    if(sscanf(str,"%schoose one door from%s",s1,s2) > 0) choice1();
    if(sscanf(str,"%sI have opened%s",s1,s2) > 0) choice2(str);
    if(sscanf(str,"%sminor glitch%s",s1,s2) > 0) playing = 0;
    if(sscanf(str,"%sclaim your prize%s",s1,s2) > 0) WinFun(str);
    if(sscanf(str,"%syour big load%s",s1,s2) > 0) LoseFun(str);
    return 1;
}
int eventFirstPass(string str){
    if(!str || str == "") return 0;
    if(sscanf(str,"A voice from the pedestal%s",s1) > 0){
        eventPedestalParse(str);
        return 1;
    }
    return 0;
}
private void eventPrint(string str1, string str2);
void init(){
    ::init();
    set_heart_beat(1);
    SetNoClean(1);
}
void receive_message(string string1, string string2){
    //tell_object(find_player("testylus"),"CHARLY: "+string2);
    eventFirstPass(string2);
}
varargs mixed eventHearTalk(object who, object target, int cls, string verb,
  string msg, string lang) {
    ww=who;
    vv=verb;
    mm=msg;
    unguarded((: this_object()->receive_message("me",ww->GetName()+" "+vv+"s: "+mm) :));
    return;
}
int eventPrint(string msg, string msg_class){
    printvar=msg;
    unguarded((: this_object()->receive_message("me again",printvar) :));
}
void heart_beat(){
    //eventForce("say playing is: "+playing);
    if(!environment() || !clonep()){
        set_heart_beat(0);
        return;
    }
    if(playing && response != "" ) {
        if(firstchoice != ""){
            //eventForce("say response is: "+response);
            eventForce(response);
            runs ++;
            //playing = 0;
        }
        //else eventBeginPlay();
        playing = 0;
    }

    if(!playing){
        //eventForce("say playing is zero, restarting.");
        eventBeginPlay();
    }
}
