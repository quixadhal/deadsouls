#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ACTIVATE;
inherit LIB_INSTALL;
inherit LIB_WORN_STORAGE;

static int active = 0;
int lupus;
mapping SpecialFuns = ([]);

int CheckPanel(){
    if(this_object()->GetClosed()){
        write("The wrist computer is closed. The panel is not accessible.");
    }
    else {
        write("A panel you can read.");
    }
    return 1;
}

static void create() {
    worn_storage::create();
    SetKeyName("wrist computer");
    SetId( ({ "computer", "bracer","comp","wristcomp","system" }) );
    SetAdjectives( ({ "wrist","odd","odd looking","complex","tough","rugged",
                "tactical", "tactical data" }) );
    SetShort("a Yautja wrist computer");
    SetLong("An odd looking bracer intended to be worn on the arm. It looks "
            "extremely complex yet also very tough and rugged. One may perhaps "
            "\"activate bracer\". It appears that one can access special functions "
            "by opening it.");
    SetDamagePoints(75);
    SetVendorType(VT_ARMOR);
    SetMass(10);
    SetBaseCost("silver", 1000);
    SetArmorType(A_CUSTOM);
    SetProtection(BLUNT, 10);
    SetProtection(BLADE, 10);
    SetProtection(KNIFE, 20);
    SetProtection(HEAT, 30);
    SetRestrictLimbs(({"left arm"}));
    SetMaxCarry(100);
    SetCanClose(1);
    SetClosed(1);
    SetLanguage("Yautja");
    SetItems( ([
                ({"panel","functions"}) : (: CheckPanel :),
                ]) );
    SetRead( ([
                ({ "panel", "default" }) :"Yautja tactical data system, version .09",
                //"panel": (: eventRead :),
                ]) );
    SetMaxClones(2);
}

int eventInitialize(){
    object env;
    if(!(env = environment()) || !living(env)) return 0;
    foreach(object module in all_inventory()){
        if(module) module->eventInstall(env, this_object());
    }
    return 1;
}

void init(){
    ::init();
    eventInitialize();
}

int eventMove(mixed arg){
    eventInitialize();
    return ::eventMove(arg);
}

varargs int YautLang(object whom){
    if(!whom) whom = this_player(); 
    if(!whom) return 0;
    if(whom->GetPolyglot()) return 100;
    return whom->GetLanguageLevel("Yautja");
}

varargs void yaut_write(string str, object whom){
    if(!str || !sizeof(str)) return;
    if(!whom) write("%^MAGENTA%^"+translate(str,YautLang())+"%^RESET%^");
    else tell_object(whom,"%^MAGENTA%^"+translate(str,YautLang(whom))+"%^RESET%^");
}

varargs void yaut_say(string str, mixed whom){
    object *exclude = ({});
    object *include = get_livings(environment(this_player()));
    if(whom){
        if(objectp(whom)) exclude = ({ whom });
        else exclude = whom;
    }
    include -= exclude;
    if(sizeof(include))
        foreach(object dude in include){
            yaut_write(str, dude);
        }
}

int eventTurnOn(){
    object *contents = all_inventory();
    if(!(this_object()->GetWorn()) || !this_player() ||
            environment(this_object()) != this_player()){
        write("You are not wearing the wrist computer.");
        return 1;
    }
    write("You activate the wrist computer. The computer says:");
    say(this_player()->GetName()+" operates "+possessive(this_player())+" "
            "wrist computer. You hear the computer say: "); 
    yaut_say("Computer online.");
    if(sizeof(contents)) contents->eventPowerOn();
    active = 1;
    return 1;
}

int eventTurnOff(){
    object *contents = all_inventory();
    write("You deactivate the wrist computer. The computer says:");
    say(this_player()->GetName()+" operates "+possessive(this_player())+" "
            "wrist computer. You hear the computer say: ");
    yaut_say("Computer offline.");
    if(sizeof(contents)) contents->eventPowerOff();    
    active = 0;
    return 1;
}

varargs mixed eventInstall(object what, object where){
    write("The wrist computer is not installable anywhere");
    return 1;
}

int CanReceive(object ob){
    if(!answers_to("yautja data module",ob)){
        write("That is not a proper data module for this computer.");
        return 0;
    }
    else return 1;
}

varargs mixed eventUninstallModule(object which, int auto){
    object *contents = all_inventory();
    object module = previous_object();
    if(which) module = which;
    if(!auto) say(this_player()->GetName()+" operates "+possessive(this_player())+" "
            "wrist computer.");
    if(!active){
        if(!auto) write("The computer is not active.");
        return 1;
    }
    if(!auto){
        write("You attempt to uninstall a module from the wrist computer. The computer says:");
        say(this_player()->GetName()+"'s wrist computer says: ");
        yaut_say("Uninstalling...");
    }
    if(SpecialFuns[module])
        foreach(mixed key, mixed val in SpecialFuns[module]){
            remove_action("foo",SpecialFuns[module][key]["hook"]);
            if(!auto){
                yaut_say(key+" successfully uninstalled.");
            }
        }
    map_delete(SpecialFuns, module);
    return 1;
}

varargs mixed eventInstallModule(mapping ModuleData, int auto){
    string *contents = ({});
    object module = previous_object();
    if(member_array("eventInitialize",call_stack(2)) != -1) auto = 1;
    if(!auto) say(this_player()->GetName()+" operates "+possessive(this_player())+" "
            "wrist computer.");
    if(!answers_to("Yautja data module",module)){
        if(!auto) write("That is not a proper data module for this computer.");
        return 0;
    }
    if(!active){
        if(!auto) write("The computer is not active.");
        return 1;
    }
    if(this_object()->GetClosed()){
        write("The computer is closed.");
        return 1;
    }
    if(sizeof(all_inventory()))
        foreach(object element in all_inventory()){
            contents += ({ base_name(element) });
        }
    if(member_array(base_name(module),contents) != -1){
        if(!auto){
            write("The wrist computer already contains that type of module.");
            return 0;
        }
    }
    SpecialFuns[module] = ModuleData;
    if(!auto){ 
        write("You install a module into the wrist computer. The computer says:");
        say(this_player()->GetName()+"'s wrist computer says: ");
        yaut_say("Installing...");
    }
    foreach(mixed key, mixed val in SpecialFuns[module]){
        add_action(SpecialFuns[module][key]["function"],SpecialFuns[module][key]["hook"]);
        if(!auto){ 
            yaut_say(key+" successfully installed.");
            module->eventMove(this_object());
        }
    }
    return 1;
}

mixed CanPutInto(object who, object item){
    return "This is a wrist computer. One can install modules on it.";
}

mixed CanGetFrom(object who, object item){
    return "This is a wrist computer. One can uninstall modules from it.";
}

varargs mixed eventRead(mixed who, mixed str){
    object dude;
    string what;
    string ret = "Yautja tactical data system display. Installed modules:\n";
    if(stringp(who)){
        what = who;
        dude = this_player();
    }
    else {
        dude = who;
        what = str;
    }
    if(this_object()->GetClosed()){
        write("The wrist computer is closed. There is nothing to read.");
        return 1;
    }
    if(!active){
        write("The wrist computer is not activated.");
        return 1;
    }
    foreach(mixed ob in all_inventory()){
        if(ob->Report()) ret += ob->Report();
    }
    if(!lupus && random(99) > 95){
        lupus = 1;
        ret = "BAD WOLF";
    }
    SetRead( ([
                ({ "panel", "default" }) :ret,
                ]) );
    return ::eventRead(dude, what);
}

varargs mixed eventOpen(object who, object tool){
    SetProtection(BLUNT, 1);
    SetProtection(BLADE, 1);
    SetProtection(KNIFE, 2);
    SetProtection(HEAT, 3);
    ::eventOpen(who || 0, tool || 0);
    if(!(this_object()->GetClosed())){
        write("Opening the wrist computer yields a panel you can read.");
        return 1;
    }
    return 0;
}

mixed eventClose(object who){
    SetProtection(BLUNT, 10);
    SetProtection(BLADE, 10);
    SetProtection(KNIFE, 20);
    SetProtection(HEAT, 30);
    return ::eventClose(who || 0);
}

string GetInternalDesc(){
    return "This is the interface for the wrist computer. You may try to \"read computer\"";
}

varargs mixed eventUnequip(object who){
    object *contents = all_inventory();
    if(sizeof(contents)) contents->eventPowerOff();
    ::eventUnequip(who||0);
}
