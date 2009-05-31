#include <lib.h>

inherit  LIB_TRAINER;
void create(){
    trainer::create();
    SetKeyName("trainer");
    SetId("generic trainer");
    SetGender("male");
    SetRace("human");
    SetShort("a generic trainer");
    SetLong("This is a person whose job it is to teach you things. "+
            "For example, 'ask trainer "+
            "to teach wibbling' would prompt him to begin a wibble learning "+
            "lesson with you, if wibbling is in his skill set and you have "+
            "earned sufficient training points. If you lack training points, then "+
            "do some adventuring and earn a level promotion. You will "+
            "then be awarded training points."); 
    SetLevel(1);
    AddTrainingSkills( ({"wibbling"}));
}
void init() {
    ::init();
}
