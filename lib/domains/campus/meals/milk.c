#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;

int SetDesc(){
    string picture;
    int which = random(16);
    switch(which){
        case 0 : picture = "Ashon";break;
        case 1 : picture = "Brodbane";break;
        case 2 : picture = "Jonez";break;
        case 3 : picture = "Alexander Tau";break;
        case 4 : picture = "Javelin";break;
        case 5 : picture = "Abby";break;
        case 6 : picture = "Shadyman";break;
        case 7 : picture = "Shigs";break;
        case 8 : picture = "Tigwyk";break;
        case 9 : picture = "Capo";break;
        case 10 : picture = "Keeperofkeys";break;
        case 11 : picture = "Lars";break;
        case 12 : picture = "Beek";break;
        case 13 : picture = "Rust";break;
        case 14 : picture = "Deathblade";break;
        case 15 : picture = "Marius";break;
    }
    SetLong("This is a little carton of grade-A milk. The rear has a "+
            "picture of "+picture+", and reads \"Have you seen me?\"");
    return 1;
}

static void create()  {
    meal::create();
    SetDesc();
    SetKeyName("milk");
    SetId("milk");
    SetAdjectives(({"milk1","milk2"}));
    SetShort("a carton of milk");
    SetMass(10);
    SetBaseCost("silver", 1);
    SetMealType(MEAL_DRINK);
    SetStrength(25);
    SetMealMessages("You drink your milk, and feel refreshed.", "$N drinks "+
            "$P milk.");
    SetEmptyName("carton");
    SetEmptyShort("an empty carton of milk");
    SetEmptyLong("A little carton container that once held "+
            "grade-A milk.");
}
void init(){
    ::init();
}
