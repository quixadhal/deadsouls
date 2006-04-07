/* remote control by Cratylus @ Frontiers
 * Sep 21 2005
 */

#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
int controlling;
string remote;
void create(){
    ::create();
    SetKeyName("remote control");
    SetId(({"device","controller","remote","control"}));
    SetAdjectives(({"small","electronic"}));
    SetShort("a remote control");
    SetLong("This is a small electronic "+
      "device with various labeled buttons on it. "+
      "It seems you can \"control something\" with "+
      "it, and also \"release\" it. To command your "+
      "remote servant, you evidently have to preface the command "+
      "with the ] character (example: ] look at menu).");
    SetProperties(([
	"no steal" : 1,
      ]));
    SetMass(20);
    SetValue(10);
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
    add_action("control","control");
    add_action("release","release");
    add_action("do_control","]");
}
int control(string str){
    object ob;
    string *eyedees;
    if(!str){
	return 0;
    }
    ob=present(str, environment(this_player()));
    if(!ob){
	write("There is no such thing to be controlled here.");
	return 1;
    }

    if(!creatorp(this_player()) && !present("visitor pass",this_player())){
	write("Your puny mortal mind can't wrap itself around the use "
	  "of this powerful instrument.");
	log_file("adm/control",capitalize(this_player()->GetKeyName())+
	  " attempted to use the remote control on "+str+": "+timestamp()+"\n");
	tell_creators("SECURITY: "+capitalize(this_player()->GetKeyName())+
	  " attempted to use the remote control on "+str+".");
	return 1;
    }

    if(!living(ob)){
	write(capitalize(ob->GetKeyName())+" is not a living thing.");
	return 1;
    }
    if(strsrch(base_name(ob),"secure") > -1 ){
	write(ob->GetName()+" is not controllable with this device.");
	say(this_player()->GetName()+" tries to establish control over "+ob->GetName()+" and fails.\n");
	return 1;
    }
    if(controlling){
	write("Your remote control is busy controlling some other creature.");
	return 1;
    }
    if(ob->QueryDrone()) {
	string schmowner;
	schmowner = ob->GetOwner();
	if(schmowner != "NOBODY" && schmowner != this_player()->GetKeyName()){
	    write("This drone is not yours to control.");
	    return 1;
	}
	remote=file_name(ob);
	ob->SetOwner(this_player()->GetKeyName());
	ob->SetListen(1);
    }
    else {

	new("/shadows/drone")->eventShadow(ob);
	remote=file_name(ob);
	ob->SetOwner(this_player()->GetKeyName());
	ob->SetListen(1);
	eyedees = ob->GetId();
	eyedees += ({"servant","drone","thrall"});
	ob->SetId(eyedees);
    }

    write("You establish a remote control connection with "+capitalize(str)+".");
    say(this_player()->GetName()+" establishes a control link with "+capitalize(str)+".");
    controlling=1;
    return 1;
}
int do_control(string str){
    object obj;
    if(!controlling){
	write("You are not currently linked to any living thing.");
	return 1;
    }
    obj=find_object(remote);
    if(obj) obj->eventReceiveCommand(str);
    else write("There seems to be a problem.");
    return 1;
}

int release(){
    object dingus;
    if(remote && dingus=find_object(remote) ){
	dingus->SetOwner("NOBODY");
	dingus->SetListen(0);
    }
    controlling=0;
    write("You release your remote link.");
    return 1;
}
