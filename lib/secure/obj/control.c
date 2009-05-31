/* remote control by Cratylus @ Frontiers
 * Sep 21 2005
 */

#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
int controlling;
string remote, control_code;
object owner;

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
    SetVendorType(VT_TREASURE);
    control_code = alpha_crypt(16);
}

void init(){
    ::init();
    if(living(environment())) owner = environment();
    else owner = 0;
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
    if(!builderp(this_player())){
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

    if(!creatorp(this_player()) && strsrch(base_name(ob), homedir(this_player()))){
        write("Only creators can control NPC's that don't belong to them.");
        say(this_player()->GetName()+" tries to establish control over "+ob->GetName()+" and fails.\n");
        return 1;
    }
    if(!strsrch(base_name(ob),"/secure") ){
        write(ob->GetName()+" is not controllable with this device.");
        say(this_player()->GetName()+" tries to establish control over "+ob->GetName()+" and fails.\n");
        return 1;
    }
    if(controlling){
        write("Your remote control is busy controlling some other creature.");
        return 1;
    }
    if(ob->GetOwner() && ob->GetOwner() != "NONE"){
        write("That creature is already in someone's thrall.");
        return 1;
    }
    if(!(ob->GetOwner())) new("/shadows/drone")->eventShadow(ob);
    remote=file_name(ob);
    ob->SetOwner(this_player()->GetKeyName(), this_player());
    ob->SetListen(1);
    ob->SetControlCode(control_code);
    eyedees = ob->GetId();
    eyedees += ({"servant","drone","thrall"});
    ob->SetId(eyedees);
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
    if(!str || str == ""){
        write("Nothing happens.");
        return 1;
    }
    if(environment() != owner){
        write("You don't seem to be in possession of the remote control.");
        tell_object(environment(),"Possible security violation on remote control.");
        error("Illegal access of remote control: "+get_stack()+" "+identify(previous_object(-1)));
        return 1;
    }
    obj=find_object(remote);
    if(obj) obj->eventReceiveCommand(str);
    else { 
        write("There seems to be a problem.");
        this_object()->release();
    }
    return 1;
}

int release(){
    object dingus;
    if(remote && dingus=find_object(remote) ){
        dingus->SetOwner("NONE");
        remove_shadow(dingus);
    }
    controlling=0;
    write("You release your remote link.");
    return 1;
}

string GetControlCode(){
    if(base_name(previous_object()) != "/shadows/drone") return alpha_crypt(16);
    else return control_code;
}
