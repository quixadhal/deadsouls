#include <lib.h>

inherit LIB_SENTIENT;

int TalkFunc(){
    int rand1;
    string thing1, thing2, thing3, thing4, thing5;
    if(!this_object() || !clonep() || !environment()) return 0;

    rand1 = random(5);

    thing1 = "Sprechen Sie Deutsch?";
    thing2 = "Vielleicht soll ich hilfe suchen.";
    thing3 = "Sind Sie sicher?";
    thing4 = "Bitte sprechen sie doch ein bisschen langsamer.";
    thing5 = "Was bedeutet das?";

    switch(rand1){
        case 1 : eventForce("say "+thing1);break;
        case 2 : eventForce("say "+thing2);break;
        case 3 : eventForce("say "+thing3);break;
        case 4 : eventForce("say "+thing4);break;
        case 5 : eventForce("say "+thing5);break;
        default :  eventForce("shrug");
    }
}
int AnswerFunc(){
    int rand2;
    string answer1, answer2, answer3, answer4, answer5;

    rand2 = random(5);

    answer1 = "Das weiss ich nicht.";
    answer2 = "Jein?";
    answer3 = "Ich spreche Englisch wirklich nicht so gut.";
    answer4 = "Es ist mir egal.";
    answer5 = "Ach.";

    switch(rand2){
        case 1 : eventForce("say "+answer1);break;
        case 2 : eventForce("say "+answer2);break;
        case 3 : eventForce("say "+answer3);break;
        case 4 : eventForce("say "+answer4);break;
        case 5 : eventForce("say "+answer5);break;
        default : eventForce("shake");
    }
}
static void create() {
    sentient::create();
    SetKeyName("hans");
    SetId(({"hans"}));
    SetShort("Hans");
    SetLong("Hans is a foreign exchange student from Germany.");
    SetLevel(5);
    SetRace("human");
    SetGender("male");
    AddTalkResponse(" ", (: TalkFunc :));
    AddTalkResponse("?", (: AnswerFunc :));
}
void init(){
    ::init();
}
