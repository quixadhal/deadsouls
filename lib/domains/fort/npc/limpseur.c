#include <lib.h>

inherit  LIB_TEACHER;
void create(){
    ::create();
    SetKeyName("limpseur");
    SetId( ({"teacher", "school teacher", "schoolteacher", "language teacher", "master", "language master"}) );
    SetAllLanguages(1);
    SetGender("male");
    SetRace("human");
    SetNativeLanguage("english");
    SetTeachingFee(200);
    SetLocalCurrency("silver");
    SetShort("Limpseur the Language Master");
    SetLong("Limpseur is an old man that appears so aged as to be almost mummified. Local legend has it that as a youth, Limpseur was a hero under a different name. He slew a dragon and ate its heart, giving him the mystical power of knowing all languages. It's likely true, since he offers lessons in any language you wish for the right price.");
    SetAutoStand(0);
    SetLevel(1);
    AddTeachingLanguages( ({ "English" }) );
}
void init() {
    ::init();
}