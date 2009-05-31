#include <lib.h>
#include <vendor_types.h>

inherit LIB_STORAGE;
inherit LIB_PRESS;
inherit LIB_INSTALL;
inherit LIB_ACTIVATE;

string oportal = "/secure/obj/portal_orange";
string bportal = "/secure/obj/portal_blue";

varargs mixed CheckOrange(int ob){
    object o, *tmp;
    int orange = sizeof(tmp=filter(deep_inventory(this_object()),
                (: base_name($1) == "/domains/default/obj/vial_orange" :) ) );
    if(orange) o = tmp[0];
    if(ob) return o;
    return orange;
}

varargs mixed CheckBlue(int ob){
    object b, *tmp;
    int blue = sizeof(tmp=filter(deep_inventory(this_object()),
                (: base_name($1) == "/domains/default/obj/vial_blue" :) ) );
    if(blue) b = tmp[0];
    if(ob) return b;
    return blue;
}

string ExaOrange(){
    string ret = "An orange button. ";
    if(CheckOrange()) ret += "It is glowing.";
    else ret += "It is darkened.";
    return ret;
}

string ExaBlue(){
    string ret = "A blue button. ";
    if(CheckBlue()) ret += "It is glowing.";
    else ret += "It is darkened.";
    return ret;
}  

mixed PressOrange(mixed args...){
    int orange = CheckOrange();
    if(orange){
        orange = new(oportal)->eventMove(environment(this_player()));
    }
    if(orange){
        write("You press the button and with a deep thumping "+
                "sound an orange portal appears.");
        say(this_player()->GetName()+" presses a button on "+
                possessive(this_player())+" portal generator, and with "+
                "a deep thumping sound an orange portal appears.");
    }  
    else {
        write("Click.");
        say(this_player()->GetName()+" presses a button on "+
                possessive(this_player())+" portal generator.");
    }
    return 1;
}

mixed PressBlue(mixed args...){
    int blue = CheckBlue();
    if(blue){
        blue = new(bportal)->eventMove(environment(this_player()));
    }
    if(blue){
        write("You press the button and with a deep thumping "+
                "sound a blue portal appears.");
        say(this_player()->GetName()+" presses a button on "+
                possessive(this_player())+" portal generator, and with "+
                "a deep thumping sound a blue portal appears.");
    }
    else {
        write("Click.");
        say(this_player()->GetName()+" presses a button on "+
                possessive(this_player())+" portal generator.");
    }
    return 1;
}

string ReadLabel(){
    string ret = "";
    ret += "GCM Heavy Industries Portal Generator Mk I\n\n";
    ret += "* Please read this beard to know act According to carry on!\n";
    ret += "* no portaling through between same room allowed Interdicted!\n";
    ret += "* Please the human user to be sign release form the organs!\n";
    ret += "* If you are stolen, call the police at once!\n";
    ret += "* values of "+sprintf("%c",223)+" may give rise to dom!\n";
    return ret;
}

string ReadScribbles(){
    string ret = "";
    ret += "this technology was stolen by GCM and they don't understand it. ";
    ret += "NOBODY HAS EVER RETURNED FROM USING PORTALS IN THE SAME ROOM! ";
    ret += "and whatever you do, stay away from th\n";
    return ret;
}

void create(){
    ::create();
    SetKeyName("GCM Portal Generator Mk I");
    SetId( ({"generator", "device", "generator mk i","generator mark i"}) );
    SetAdjectives( ({"sleek","mk i","futuristic","portal","gcm"}) );
    SetShort("a portal generator");
    SetLong("This is a sleek, futuristic device with orange and "+
            "blue buttons. It has a printed label you can read, and it looks "+
            "like someone has scribbled some markings on it as well.");
    SetMass(100);
    SetBaseCost("silver",5000);
    SetVendorType(VT_TREASURE);
    SetItems( ([
                ({"label","printed label"}) : "A printed label you can read.",
                ({"markings","scribbles","scribbled markings"}) : "Markings "+
                "you can read, apparently scribbled in haste.",
                ({ "button", "buttons" }) : "The portal generator has two buttons, one"+  
                " orange, one blue.",
                ({ "orange button" }) : (: ExaOrange :),
                ({ "blue button" }) : (: ExaBlue :),
                ]) );
    SetReads( ([
                "default" : "Try 'read label on generator'",
                ({"label","printed label"}) : (: ReadLabel :),
                ({"markings","scribbles","scribbled markings"}) : (: ReadScribbles :),
                ]) );
    SetPress( ([
                "default" : "Try 'push blue button on generator'",
                ({ "button", "buttons" }) : "You have to press one or the other.",
                ({"orange button"}) : (: PressOrange :),
                ({"blue button"}) : (: PressBlue :),
                ]) );
    SetMaxCarry(200);
    SetUnique(1);
}

varargs mixed eventInstall(object what, object where){
    write("The portal generator is not installable anywhere");
    return 1;
}

mixed CanPutInto(object who, object item){
    return "This is a portal generator. One can install things in it.";
}

mixed CanGetFrom(object who, object item){
    return "This is a portal generator. One can uninstall things from it.";
}

int CanReceive(object ob){
    if(base_name(ob) == "/domains/default/obj/vial_blue"){
        if(CheckBlue()){
            write("It doesn't fit.");
            return 0;
        }
        else return 1;
    }
    if(base_name(ob) == "/domains/default/obj/vial_orange"){
        if(CheckOrange()){
            write("It doesn't fit.");
            return 0;
        }
        else return 1;
    }
    write("That doesn't belong in there.");
    return 0;
}

int eventTurnOn(){
    write("Try 'push orange button on generator'");
    return 1;
}

void init(){
    ::init();
}
