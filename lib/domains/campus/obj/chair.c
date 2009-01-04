#include <lib.h>

inherit LIB_CHAIR;


static void create() {
    chair::create();
    SetKeyName("chair");
    SetId("chair");
    SetAdjectives( ({ "swivel", "small", "black" }) );
    SetShort("a swivel chair");
    SetLong("This is a small swivel chair, with tasteful and understated "+
            "black covers.");
    SetMass(1500);
    SetDollarCost(15);
    SetMaxSitters(1);
}
void init(){
    ::init();
    add_action("swivel","swivel");
}
int swivel(string str){
    int hit,i;
    object *dupes;
    dupes = get_dupes(this_object(),environment(this_object()) );
    if(!str || str =="" || str == "in chair" || str == "in the chair"){
        hit = 42;
        for(i=0;i<sizeof(dupes);i++){
            if( member_array(this_player(),dupes[i]->GetSitters()) != -1) hit = 7;
            if( member_array(this_player(),this_object()->GetSitters()) != -1) hit = 7;
        }
        if( hit == 7) {
            write("You swivel around in your swivel chair! Whee!");
            say(this_player()->GetName()+" swivels around in "+possessive(this_player())+" "+
                    "swivel chair, yelling \"WHEEEE!!!\"");
            return 1;
        }
        else { write("You are not sitting in a swivel chair."); }
    }
    if(hit == 42) return 1;
    else return 0;
}
mixed CanGet(object ob) { return "The chair does not budge.";}
