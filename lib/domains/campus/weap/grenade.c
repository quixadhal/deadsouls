#include <lib.h>
#include <vendor_types.h>
#include <damage_types.h>
inherit LIB_ITEM;
inherit LIB_PULL;
string pin_desc();
string lev_desc();
int detonated;
int count, armed;
string *limbs;
void analyze(string butt);
int HitLivings(object munch);
void create(){
    item::create();
    SetKeyName("concussion hand grenade");
    SetId( ({"grenade","hand grenade","flash-bang grenade","concussion grenade"}) );
    SetAdjectives( ({"military","pineapple"}) );
    SetShort("a hand grenade");
    SetLong("This is an olive-green military issue hand grenade. It is about the "+
      "size of an apple, and its round exterior is made of smooth metal. The "+
      "grenade's fuse consists of a short metal lever and a round pull-pin. ");
    SetMass(20);
    SetDollarCost(10);
    SetVendorType(VT_WEAPON);
    AddItem(({"pull pin","pull-pin","pin"}),"This is a round, metal pin, evidently designed for pulling.");
    AddItem(({"lever","metal lever"}),"This lever appears to be part of the fuse mechanism.");
    AddItem( ({"fuse","fuse mechanism"}) , "This fuse mechanism controls the arming of the grenade.");
    count=5;
    detonated=0;
}
void init(){
    if(armed){
	set_heart_beat(1);
	if(detonated==1) this_object()->eventDestruct();
    }
}
mixed CanPull(object who, string target) {
    if(!present(this_object()->GetKeyName(),who ) && target == "pin"){
	return "You do not have the grenade.";
    }
    if(this_object()->GetId() == target){
	return "You cannot pull that.";
    }
    return 1;
}
mixed eventPull(object who, string target) {
    if(target =="pin" || target == "pull-pin"){
	write("You pull the grenade's pin.\n");
	say(this_player()->GetName()+" pulls the pin from a hand grenade.\n");
	new("/domains/campus/obj/pin")->eventMove(this_player());
	new("/domains/campus/obj/lever")->eventMove(this_player());
	SetShort("a live hand grenade");
	SetLong("This is an olive-green military issue hand grenade. It is about the "+
	  "size of an apple, and its round exterior is made of smooth metal. The pin and "+
	  "spoon are missing from the fuse mechanism, indicating the grenade is live "+
	  "and will detonate very soon.");
	AddItem(({"lever","metal lever"}),"The grenade is armed. The fuse lever is no longer on the grenade.");
	AddItem(({"pull pin","pull-pin","pin"}),"The grenade is armed. There is no pin in the fuse.");
	armed = 1;
	set_heart_beat(1);
	return 1;
    }
    write("Nothing happens.");
    return 1;
}
void heart_beat(){
    count--;
    if(detonated==1) this_object()->eventDestruct();
    if(count < 0){
	this_object()->detonate();
	return;
    }
    if(interactive(environment())){
	tell_object(environment(), "You are holding a live hand grenade.");
	return;
    }
    tell_object(environment(),"You hear a low hissing sound.");
    tell_object(environment(environment()), environment()->GetName()+" is holding a live hand grenade.");
}
int detonate(){
    object ob;
    string *stuffs;
    int num;
    ob = environment(this_object());
    if(living(ob) && !creatorp(ob)){
	stuffs=ob->GetLimbs();
	tell_object(ob, "\nKABOOM! You are torn to pieces by your hand grenade!\n");
	tell_room(environment(ob), "\nKABOOM! "+ob->GetName()+" is torn to pieces by "+
	  possessive(ob)+" hand grenade!\n",ob);
	detonated=2;
	foreach(string limb in stuffs){
	    if(limb != "head" && limb != "torso" && limb != "neck") ob->RemoveLimb(limb,this_object());
	}
	ob = environment(ob);
    }
    if(detonated !=2) tell_room(environment(this_object()), "\nKABOOM! The grenade detonates!\n");
    foreach(object victim in get_livings(ob)){
	this_object()->HitLivings(victim);
    }
    this_object()->eventDestruct();
    return 1;
}

void analyze(string str){
    string tmp,tmp2;
    sscanf(str,"%s#%s",tmp,tmp2);
    if(tmp="/lib/corpse"){
	filter(deep_inventory(find_object(str)), (: $1->eventDestruct() :), this_object() );
    }
    return;
}

int HitLivings(object ob){
    ob->eventReceiveDamage(this_object(),BLUNT, random(1000)+300, 1);
    return 1;
}

