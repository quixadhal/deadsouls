#include <lib.h>
#include <vendor_types.h>
#include <vision.h>
inherit LIB_ITEM;

varargs string translate_vision(object ob){
    int vision;
    string ret = " ";
    if(!ob) vision = this_player()->GetEffectiveVision();
    else vision = ob->GetEffectiveVision();
    switch(vision){
        case 0 : ret = "level 0: blindness.";break; 
        case 1 : ret = "level 1: too dark.";break; 
        case 2 : ret = "level 2: dark.";break; 
        case 3 : ret = "level 3: dim.";break; 
        case 4 : ret = "level 4: clear.";break; 
        case 5 : ret = "level 5: light.";break; 
        case 6 : ret = "level 6: bright.";break; 
        case 7 : ret = "level 7: too bright.";break; 
        default : ret = "a mystery.";
    }
    return ret;
}

varargs mixed EvaluateRadiantLight(object ob, int report){
    int x = 0;
    string rep = "";
    string subrep = "";
    if(!ob) ob = this_player();
    foreach(object guy in get_livings(environment(ob))){
        if(guy->GetRadiantLight()) 
            rep += guy->GetName()+"'s radiant light is: "+guy->GetRadiantLight()+".\n";
        foreach(object item in all_inventory(guy)){
            x += item->GetRadiantLight();
            if(item->GetRadiantLight()){
                subrep += guy->GetName()+"'s "+item->GetName()+"'s radiant light is: "+
                    item->GetRadiantLight()+".\n";
            }
        }
    }
    if(!report) return x;
    else return "Livings: \n"+rep+"\nObjects:\n"+subrep;
}

string DefaultReading(){
    write("This small device is a portable light meter. It tells "+
            "you what the available light is.");
    write("The ambient light here is: "+environment(this_player())->GetAmbientLight());
    write("Total radiant light here is: "+EvaluateRadiantLight());
    write("Your vision range is: "+this_player()->GetEffectiveVision(environment(this_player()),1));
    write("\nYour effective vision is "+translate_vision());
    write("\nFull radiance details:\n"+EvaluateRadiantLight(this_player(),1));
    return "";
}

varargs mixed eventUse(mixed arg){
    return DefaultReading();
}

void create(){
    ::create();
    SetKeyName("light meter");
    SetAdjectives( ({"light"}) );
    SetId( ({"meter", "lightmeter"}) );
    SetShort("a light meter");
    SetLong( (: DefaultReading :) );
    SetProperties(([
                "no steal" : 1,
                ]));
    SetNoCondition(1);
    SetMass(20);
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
}
