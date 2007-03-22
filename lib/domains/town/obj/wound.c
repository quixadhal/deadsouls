#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>
#include <daemons.h>
inherit LIB_ITEM;
void create(){
    ::create();
    SetKeyName("wound");
    SetId(({"wound","firearms_wound"}));
    SetInvis(1);
    SetMass(0);
    SetBaseCost("silver",0);
    SetVendorType(VT_TREASURE);
}
void init(){
    add_action("lookit","look");
    set_heart_beat(20);
}
void lookit(string str){
    if(str == "at wound" || str == "at wounds" || str == "at my wounds" ||
      str == "at my wound" || str == "at gunshot wound" || str == "at gunshot wounds"){
	if(base_name(this_player()) == base_name(environment()) ){
	    write("You have been wounded by gunfire. There "+
	      "are "+cardinal(this_player()->GetLead())+" bullet holes "+
	      "in your body.");
	    return;
	}
    }
}
string GetAffectLong(object ob) {
    if(!ob || !living(ob)) return 0;
    return ob->GetName() + " is suffering from gunshot wounds.";
}
void heart_beat(){
    int dam,num,tmp;
    object ob;
    ob=environment();
    if(!living(ob)){ this_object()->eventDestruct(); return; }
    num = ob->GetLead();

    if(!num){ 
	tmp = ob->GetLead("gunshot_wounds");
	ob->AddLead("gunshot_wounds", -tmp);
	tmp = ob->GetLead("rifleshot_wounds");
	ob->AddLead("rifleshot_wounds", -tmp);
	this_object()->eventDestruct(); 
	return; 
    }

    dam = num*10;
    if(!RACES_D->GetNonMeatRace(ob->GetRace())){
	tell_object(ob,"You bleed heavily from your gunshot wounds.");
	tell_room(environment(ob), ob->GetName()+
	  " bleeds heavily from "+possessive(environment())+" gunshot wounds.",environment());
	ob->eventReceiveDamage(this_object(),TRAUMA,dam,0,"torso");
    }
    return;
}
mixed CanGet(object ob) { return "Your hands slip on the gunshot wounds.";}
mixed CanDrop(object ob) { return "Your hands slip on the gunshot wounds.";}
