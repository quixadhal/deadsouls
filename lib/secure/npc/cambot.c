#include <lib.h>
#include <position.h>

inherit LIB_SENTIENT;
inherit LIB_TURN;
int recording;
string baseshort, recfile, gstr, gstr2;
mixed owner;

static void create() {
    sentient::create();
    SetKeyName("cambot");
    SetId( ({"bot", "robot"}) );
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a cambot");
    SetLong("This is a metallic sphere, about one foot in diameter, with numerous lenses affixed to its surface.");
    SetPosition(POSITION_FLYING);
    SetLevel(1);
    SetPacifist(1);
    SetNoClean(1);
    SetRace("bot");
    SetClass("observer");
    SetGender("neuter");
    SetPolyglot(1);
    baseshort = GetShort();

}

void init(){
    ::init();
    add_action("SetRecordingFile","setfile");
}

static mixed SetOwner(mixed foo){
    owner = foo;
    return owner;
}

mixed GetOwner(){
    return owner;
}

int SetRecordingFile(string str){
    string pprefix, filename;
    if(!sizeof(str) && sizeof(recfile)){
        write("The recording file is currently: "+recfile);
        return 1;
    }
    if(!sizeof(str)){
        str = truncate(generate_tmp(),2)+".txt";
    }
    if(directory_exists(str)){
        write("Please specify a file, not a directory.");
        return 1;
    }
    pprefix = path_prefix(str);
    if(!directory_exists(pprefix)){
        write("That is not a valid path.");
        return 1;
    }
    filename = last_string_element(str);
    gstr2 = str;
    if(!check_privs(this_player(),str) || !unguarded( (: write_file(gstr2,"New log: "+timestamp()+"\n") :) )){
        write("That is not a valid path.");
        return 1;
    }
    if(last(str,2) == ".c" || last(str,2) == ".h" ||
            last(str,4) == ".cfg"){
        write("That file has an invalid extension for recording over.");
        return 1;
    }
    recfile = str;
    owner = previous_object();
    write("Setting recording file to: "+recfile);
    unguarded( (: write_file(recfile,"New log: "+timestamp()+"\n") :) );
    return 1;
}

void catch_tell( string message ){
    this_object()->receive_message("catch_tell",message);
}

void receive_message(string s1, string s2){
    if(s2){
        gstr = replace_string(s2,"%^CYAN%^","");
        gstr = replace_string(gstr,"%%^^CYAN%%^^","");
        if(recording) unguarded( (: write_file(recfile,timestamp()+": "+gstr+"\n") :) );
    }
}

varargs mixed eventHearTalk(object who, object target, int cls, string verb,
        string msg, string lang) {
    this_object()->receive_message("me",who->GetName()+" "+verb+"s: "+msg) ;
    return;
}

varargs int doPrint(string msg, string msg_class){
    this_object()->receive_message("me again",msg) ;
    return 1;
}

varargs int eventPrint(string msg, string msg_class){
    doPrint( msg, msg_class) ;
    return 1;
}

int eventTurnOn(object ob){
    if(this_player() != environment()  && environment(this_player()) !=environment()) {
        write("It isn't within reach.");
        return 1;
    }
    if(!archp(this_player())){
        write("This is an arch-level cambot. You may not tamper with it.");
        return 0;
    }
    if(!recording){
        write("You turn on the cambot.");
        say(this_player()->GetName()+" turns on a cambot.");
        SetShort(baseshort+" %^BOLD%^RED%^%^FLASH%^recording%^RESET%^");
        if(!sizeof(recfile)) recfile = truncate(generate_tmp(),2)+".txt";
        write("Recording file is: "+recfile);
        unguarded( (: write_file(recfile,"New log: "+timestamp()+"\n") :) );
        recording = 1;
        return 1;
    }
    if(recording){
        write("It is already on.");
        return 1;
    }
}

varargs mixed eventTurnOff(string str){
    if(this_player() != environment()  && environment(this_player()) !=environment()) { write("It isn't within reach."); return 1; }
    if(!archp(this_player())){
        write("This is an arch-level cambot. You may not tamper with it.");
        return 0;
    }
    if(recording){
        write("You turn off the cambot.");
        say(this_player()->GetName()+" turns off a cambot.");
        SetShort(baseshort);
        recording = 0;
        return 1;
    }
    if(!recording){
        write("It is already off.");
        return 1;
    }
}

int eventDestruct(){
    if(!this_player()) return 0;
    if(!archp(this_player())){
        write("This is an arch-level cambot. You may not tamper with it.");
        return 0;
    }
    else return sentient::eventDestruct();
}

int eventDie(){
    if(!this_player()) return 0;
    if(!archp(this_player())){
        write("This is an arch-level cambot. You may not tamper with it.");
        this_object()->AddHP(1000);
        return 0;
    }
    else return sentient::eventDie();
}

int eventForce(string str){
    if(!this_player()) return 0;
    if(!archp(this_player())){
        write("This is an arch-level cambot. You may not tamper with it.");
        return 0;
    }
    else return sentient::eventForce(str);
}

int eventMove(mixed dest){
    if(recording){
        write("Cambot cannot move while in recording mode.");
        return 0;
    }
    else return sentient::eventMove(dest);
}

int eventMoveLiving(mixed dest){
    if(recording){
        write("Cambot cannot travel while in recording mode.");
        return 0;
    }
    else return sentient::eventMoveLiving(dest);
}

