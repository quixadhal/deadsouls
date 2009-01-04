#include <lib.h>

inherit LIB_SENTIENT;


int left_hand;
void complete_quest(object ob);

create() {
    ::create();
    SetKeyName("beggar");
    SetId( ({ "beggar" }) );
    SetShort("A poor beggar");
    SetAggressive( 0);
    SetWanderSpeed(60);
    SetGender("male");
    SetLevel(4);
    SetLong("He is a poor beggar looking only for a few coins.\n");
    SetEncounter(0);
    SetMorality(30);
    SetRace( "human");
    SetMaxHealthPoints(50);
    SetHealthPoints(50);
    AddLimb("head", "FATAL", 25, 0, 4);
    AddLimb("torso", "FATAL", 50, 0, 4);
    AddLimb("right arm", "right hand", 20, 0, 4);
    AddLimb("right hand", "", 15, 0, 4);
    AddLimb("left arm", "left hand", 20, 0, 4);
    AddLimb("left hand", "", 15, 0, 4);
    AddLimb("right leg", "right foot", 25, 0, 4);
    AddLimb("right foot", "", 20, 0, 4);
    AddLimb("left leg", "left foot", 25, 0, 4);
    AddLimb("left foot", "", 20, 0, 4);
    RemoveLimb("left hand",this_object());
    left_hand = 0;
    new("/domains/Praxis/obj/misc/cold")->infect(this_object());
}
void catch_tell(string str) {
    object ob;
    string a, b, c;

    if(sscanf(str, "%s quest%*s", a)) {
        call_out("say_line", 2, "The beggar says: What quest?  I know nothing of any quests.\n");
        return;
    }
    if(sscanf(str, "%s regenerates your %s", a, b) == 2) {
        if(left_hand) return;
        if(!(ob=present((a=lower_case(a)), environment(this_object())))) return;
        //if(!query_is_limb("left hand")) return;
        left_hand = 1;
        call_out("complete_quest", 1, ob);
        return;
    }
    if(sscanf(str, "%s says: %s hand%*s", a, b) == 2) {
        //if(query_is_limb("left hand")) 
        //call_out("say_line", 2, "The beggar says: My left hand is doing fine now!\n");
        //        else 
        call_out("say_line", 2, "The beggar says: I wish it could be replaced.\n");
        return;
    }
    if(sscanf(str, "%s gives %s you", a, b) == 2) {
        ob = present( lower_case(a), environment(this_object()));
        if(ob) {
            tell_object(ob, "The beggar thanks you for your generousity.\n");
            tell_room(environment(this_object()), "The beggar thanks "+a+" for "+ob->query_possessive()+" generousity.\n", ({this_object(), ob}));
        }
    }
    if(sscanf(str, "%s?", a)) {
        call_out("say_line", 2, "The beggar says: I do not understand the question.\n");
        return;
    }
    if(sscanf(str, "%ssmiles%s", a, b) == 2)
        call_out("say_line", 2, "The beggar smiles happily.\n");
    AddAlcohol(200);
}
void complete_quest(object ob) {
    if(!ob->set_mini_quest("beggar", 5000,
                "You replaced the missing left hand of a poor beggar.\n")) {
        tell_object(ob, "The beggar thanks you.\n");
        tell_room(environment(this_object()), "The beggar thanks "+ob->query_cap_name()+".\n", ({ ob, this_object() }) );
        SetAction(5, ob->query_cap_name()+" replaced my missing left hand!");
        return;
    }
    tell_object(ob, "The beggar glows in satisfaction!\n");
    tell_object(ob, "The beggar says: Thank you so much!\n");
    tell_object(ob, "You feel more experienced.\n");
    tell_room(environment(this_object()), ob->query_cap_name()+" looks more experienced.\n", ({ ob, this_object() }) );
    SetAction(5, ob->query_cap_name()+" replaced my missing left hand!" );
}
void say_line(string str) {
    tell_room(environment(this_object()), str, ({ this_object() }) );
}
void init(){
    ::init();
}
