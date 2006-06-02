#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("lemur");
    SetAdjectives( ({"small", "startling", "grey", "gray"}) );
    SetId( ({"primate", "simian"}) );
    SetShort("a lemur");
    SetLong("This small animal is startling in its appearance. It looks much like a cross between a small monkey and a raccoon. It has an elongated snout and the fur around its eyes is dark, like the mask of a bandit. Yet its body is clearly simian, with the prehensile front limbs of a monkey. Its grey, furry body ends in a ridiculously long tail that is striped white and black.");
    SetLevel(1);
    SetMelee(1);
    SetRace("primate");
    SetMaxHealthPoints(40);
    SetGender("male");
}
void init(){
    ::init();
}
