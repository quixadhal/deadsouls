#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_PRESS;

int PushTheButton();
string color,gagnant,my_door,other_door,removed_door;
void create(){
    ::create();
    SetKeyName("button pedestal");
    SetId( ({"pedestal","altar"}) );
    SetAdjectives( ({"cylindrical","round","waist-high"}) );
    SetShort("a pedestal with a red button on it");
    SetLong("This is a cylindrical pedestal, about waist-high, "
            "of mysterious composition. A button is "
            "on it.");
    SetItems( ([
                ({"button","red button"})  : "A red button."
                ]) );
    SetMass(20);
    SetDollarCost(10);
    SetVendorType(VT_TREASURE);
    SetPress( ([
                ({"button","red button"}) : (: PushTheButton :)
                ]) );
}
mixed CanGet(object ob) { return "The pedestal does not budge.";}
int ResetGame(){
    object *objects;
    object *contents;
    string prize;
    prize = "/domains/campus/armor/silverring";
    objects = ({});
    objects+=({ load_object("/domains/campus/room/red_room3") });
    objects+=({ load_object("/domains/campus/room/green_room3") });
    objects+=({ load_object("/domains/campus/room/blue_room3") });

    objects = ({ load_object("/domains/campus/doors/red_door3") });
    objects +=({ load_object("/domains/campus/doors/green_door3") });
    objects +=({ load_object("/domains/campus/doors/blue_door3") });

    foreach(object ding in objects){
        if(ding) ding->SetClosed(1);
        if(ding) ding->SetLocked(1);
    }

    if(sscanf(gagnant,"%s door",color)>0){
        string where;
        where="/domains/campus/room/"+color+"_room3";
        //new(prize)->eventMove(load_object(where));
    }
}
int PushTheButton(){
    int genrand;
    gagnant = "";
    genrand = random(3);
    send_messages("press", "$agent_name $agent_verb the button.",
            this_player(), 0, environment(this_player()));
    if(genrand == 0) gagnant = "red door";
    if(genrand == 1) gagnant = "green door";
    if(genrand == 2) gagnant = "blue door";
#if 0
    tell_room(environment(),"A voice from the pedestal says: "
            "PRECOG: genrand is: "+genrand);
    tell_room(environment(),"A voice from the pedestal says: "
            "PRECOG: gagnant is: "+gagnant+".\n"
            "PRECOG: genrand modulus is: "+genrand);
#endif
    remove_action("doStay","stay");
    remove_action("doSwitch","switch");
    add_action("choose","choose");
    tell_room(environment(),"A voice from the pedestal says: "
            "You must now choose one door from these "
            "three. Behind one is a prize. Behind the "
            "other two, nothing. To choose the "
            "red door, for example: choose red door");
    ResetGame();
    return 1;
}
void init(){
    ::init();
    if(gagnant != "" && my_door !=""){
        add_action("doStay","stay");
        add_action("doSwitch","switch");
    }
    if(gagnant != "" && my_door == ""){
        add_action("choose","choose");
    }
}
int choose(string str){
    if(!str || str == ""){
        tell_room(environment(),"A voice from the pedestal says: "
                "Please try choosing a door, ok?");
    }
    else if(str == "door"){
        tell_room(environment(),"A voice from the pedestal says: "
                "You'll need to be more specific.");
    }
    else if(str == "red door" || str == "blue door" ||str == "green door"){
        tell_room(environment(),"A voice from the pedestal says: "
                "You choose the "+str+".");
        this_object()->MontyMagic(str);
    }
    else {
        tell_room(environment(),"A voice from the pedestal says: "
                "I don't understand that.");
    }
    reap_dummies();
    reap_other("/domains/campus/armor/silverring");
    return 1;
}
int MontyMagic(string str){
    int genrand,which;
    string *choices;
    choices = ({});
    if(str != "red door") choices += ({ "red door" });
    if(str != "green door") choices += ({ "green door" });
    if(str != "blue door") choices += ({ "blue door" });
    genrand = random(2);
    if(choices[1] == gagnant) which = 0;
    else if(choices[0] == gagnant) which = 1;
    else which = genrand;
    removed_door = choices[which];
    choices -= ({ choices[which] });
    other_door = choices[0];
    my_door = str;
    choices += ({ str });
    if(sscanf(removed_door,"%s door",color)>0) color = color;
    load_object("/domains/campus/doors/"+color+"_door3")->SetLocked(0);
    load_object("/domains/campus/doors/"+color+"_door3")->SetClosed(0);
    tell_room(environment(),"A voice from the pedestal says: "
            "I have opened the "+removed_door+"! \n"
            "You may enter the "+color+" room and see that it is empty.\n"
            "Only the "+choices[0]+" and "+choices[1]+" remain.\n\n"
            "Would you like to switch to the "+other_door+"? \n"
            "Or would you rather stay with the "+str+"?\n\n"
            "To switch, type: switch\n"
            "To stay, type: stay");
    remove_action("choose","choose");
    add_action("doStay","stay");
    add_action("doSwitch","switch");
    return 1;

}
int CheckWin(string str){
    if(sscanf(str,"%s door",color)>0) color = color;
    if(str == gagnant) {
        this_object()->WinFun();
        return 1;
    }
    load_object("/domains/campus/doors/"+color+"_door3")->SetLocked(0);
    load_object("/domains/campus/doors/"+color+"_door3")->SetClosed(0);
    tell_room(environment(),"A voice from the pedestal says: "
            "YOU LOOOOOSE!\n"
            "You may enter the "+color+" room to get your big "
            "load of NOTHING. Haaa haa!");
    return 1;
}
int doStay(){
    tell_room(environment(),"A voice from the pedestal says: "
            "Oh, how loyal!");
    remove_action("doStay","stay");
    remove_action("doSwitch","switch");
    CheckWin(my_door);
    return 1;
}
int doSwitch(){
    tell_room(environment(),"A voice from the pedestal says: "
            "Why you're inconstant as a feather!");
    remove_action("doStay","stay");
    remove_action("doSwitch","switch");
    my_door = other_door;
    CheckWin(my_door);
    return 1;
}
int WinFun(){
    load_object("/domains/campus/doors/"+color+"_door3")->SetLocked(0);
    load_object("/domains/campus/doors/"+color+"_door3")->SetClosed(0);
    tell_room(environment(),"A voice from the pedestal says: "
            "You win, kid. Congrats!\n"
            "You may enter the "+color+" room and claim your prize.\n\n"
            "Push the button on the pedestal to reset the game.");
    return 1;
}
