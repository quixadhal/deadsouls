#include <lib.h>

inherit LIB_CHAIR;

static void create() {
    chair::create();
    SetKeyName("chair");
    SetAdjectives( ({"simple", "wood", "wooden", "rocking", "classic"}) );
    SetId("chair");
    SetShort("a wooden rocking chair");
    SetLong("A simple wooden rocking chair, fashioned in a classic, non-nonsense style common in the rural parts of town.");
    SetMass(1500);
    SetDollarCost(15);
    SetMaxSitters(1);
}
void init(){
    add_action("rock","rock");
}

int rock(string str){
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
	    write("You gently rock in your rocking chair.");
	    say(this_player()->GetName()+" gently rocks in "+possessive(this_player())+" "+
	      "rocking chair.");
	    return 1;
	}
	else { write("You are not sitting in a rocking chair."); }
    }
    if(hit == 42) return 1;
    else return 0;
}

mixed CanGet(object ob) { return "The chair does not budge.";}
