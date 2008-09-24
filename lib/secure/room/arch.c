#include <lib.h>
#include <daemons.h>
#include <message_class.h>
inherit LIB_ROOM;

int imud_enabled = 0;
//string tmpfile;

mixed eventReadPrintout(){
    mapping MudMap2;
    string *all_dead_souls = ({});
    string ret = "";
    MudMap2 = INTERMUD_D->GetMudList();
    if(!sizeof(MudMap2)){
        write("Intermud3 link down. Stats unavailable.");
        return 1;
    }
    foreach(string key, mixed *val in MudMap2){
        if(grepp(val[5],"Dead Souls")) all_dead_souls += ({ key });
    }
    ret += "%^RED%^Muds, alive or dead, test or production, that tried Dead Souls:%^RESET%^\n";
    foreach(string mud in all_dead_souls){
        ret += mud + "\t\t" +MudMap2[mud][5] + "%^RESET%^\n";
    }
    ret += "\nTotal: "+sizeof(all_dead_souls);
    this_player()->eventPage( explode(ret,"\n") );
    return "";
}

mixed eventReadScreen(){
    mapping MudMap;
    string *dead_keys = ({});
    string *all_dead_keys = ({});
    string *good_muds = ({});
    string *online_muds = ({});
    string ret = "";
    string canonical = "UNKNOWN";
    MudMap = INTERMUD_D->GetMudList();
    if(!sizeof(MudMap)){
        write("Intermud3 link down. Stats unavailable.");
        return 1;
    }
    if(MudMap["Frontiers"]) canonical = MudMap["Frontiers"][5];
    foreach(string key, mixed *val in MudMap){
        if(!grepp(key,"Dead_Souls_")){
            if(grepp(val[5],mudlib_version()))
                dead_keys += ({ key });
            if(grepp(val[5],"Dead Souls")) all_dead_keys += ({ key });
            if(canonical != "UNKNOWN" && grepp(val[5],canonical)) good_muds += ({ key });
            if(grepp(val[5],"Dead Souls") && val[0] == -1) online_muds += ({ key });
        }
    }
    if(canonical != "UNKNOWN"){
        ret += "%^GREEN%^Muds running the current version of Dead Souls:%^RESET%^\n";
        foreach(string mud in good_muds){
            if(mud != "DeadSoulsWin")
                ret += mud + "\t\t" +MudMap[mud][5] + "%^RESET%^\n";
        }
    }
    if(!grepp(ret,"current version") || !grepp(canonical,mudlib_version())){
        ret += "\n%^CYAN%^Muds running our version of Dead Souls:%^RESET%^\n";
        foreach(string mud in dead_keys){
            ret += mud + "\t\t" +MudMap[mud][5] + "%^RESET%^\n";
        }
    }
    ret += "\n%^RED%^Muds running any version of Dead Souls:%^RESET%^\n";
    foreach(string mud in all_dead_keys){
        ret += mud + "\t\t" +MudMap[mud][5] + "%^RESET%^\n";
    }

    ret += "\nTotal: "+sizeof(all_dead_keys);

    ret+= "\n\n%^YELLOW%^Dead Souls muds online:%^RESET%^ \n";
    foreach(string mud in online_muds){
        ret += mud + "\t\t" +MudMap[mud][5] + "%^RESET%^\n";
    }
    this_player()->eventPage( explode(ret,"\n") );
    return "";

}

void SetImud(int i){
    if(!i) i = 0;
    imud_enabled = i;
}

int GetImud(){
    return imud_enabled;
}

string SignRead(){
    string ret = "";
    if(INTERMUD_D->GetConnectedStatus())
        ret += "\"I3 connection: %^BOLD%^GREEN%^ONLINE%^RESET%^, ";
    else ret +=  "\"I3 connection: %^BOLD%^RED%^OFFLINE%^RESET%^, ";

    switch (IMC2_D->getonline()) {
    case 1:
        ret += "IMC2: %^BOLD%^GREEN%^ONLINE%^RESET%^\"";
        break;
    case 2:
        ret += "IMC2: %^BOLD%^YELLOW%^WAITING FOR ACCEPTANCE%^RESET%^\"";
        break;
    case 3:
        ret += "IMC2: %^BOLD%^RED%^OFFLINE: CONNECT ERROR%^RESET%^\"";
        break;
    case 4:
        ret += "IMC2: %^BOLD%^RED%^OFFLINE: HUB DOWN%^RESET%^\"";
        break;
    case 5:
        ret += "IMC2: %^BOLD%^RED%^OFFLINE: BANNED%^RESET%^\"";
        break;
    default:
        ret += "IMC2: %^BOLD%^RED%^OFFLINE%^RESET%^\"";
        break;
    }
    return ret;
}

string LongDesc(){
    string desc = "This is a polished antiseptic room composed of some "+
    "white gleaming material. There is a viewscreen on a wall here, "+
    "with a control panel alonside it. "+
    "A long printout hangs from the panel."
    "\nThe network troubleshooting room is down from here.";
    desc += "\nA sign on the wall reads: "+SignRead();
    return desc;
}

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Arch Room");
    SetLong( (: LongDesc :) );
    SetItems( ([ ({"wall","walls"}) : "The walls seem composed "
        "of some advanced polymer. They are extremely clean and highly "
        "polished.",
        "room" : "This looks like it might be the control room "
        "for the mud.",
        //({"screen","viewscreen"}) : "This is a display screen of some sort.",
        ({"screen","viewscreen"}) : (: eventReadScreen :) ,
        ({"printout"}) : (: eventReadPrintout :) ,
        ({"sign"}) : "A sign you can read.",
        ({"panel","control panel"}): "This seems to be the main control "
        "panel for the mud. It contains a bewildering array of "
        "keypads, but the most prominent feature of the control panel "
        "is a metallic plate in its center, shaped in the form of a "
        "human hand.",
        ({"camera","hal","HAL"}) : "This is the rectangular faceplate of "
        "a camera mounted within the wall. On the upper end of the "
        "rectangle is a stamped label. On the lower end is the lens, "
        "which has at its center a glowing red light.",
        ({"label","stamped label"}) : "A stamped metal label. The label "
        "reads: 'HAL 9000'.",
        ({"plate","metallic plate","identification plate"}) : "This "
        "appears to be an identification plate of some sort, designed "
        "to accomodate a human hand.",
        "portal" : "A portal to another place." ]) );
    SetExits( ([
        "north" : "/domains/default/room/wiz_hall",
        "down" : "/secure/room/network.c",
      ]) );
    SetEnters( ([
      ]) );
    SetProperties(([
        "no peer" : 1,
      ]));
    SetRead("screen", (: eventReadScreen :) );
    SetRead("printout", (: eventReadPrintout :) );
    SetRead("sign", (: SignRead :) );

    SetListen("default", "You can faintly hear a low hum coming from the walls.");
    SetListen( ({"wall","walls"}), "You hear a low throbbing sound, as if from machinery.");
    SetInventory( ([
        "/secure/obj/arch_board" : 1,
      ]) );
}
int CanReceive(object ob) {
    if( !archp(ob)  && base_name(ob) != "/secure/obj/arch_board"){
        message("info","The arch room is available only to "+
          "admins, sorry.",ob);
        return 0;
    }
    return 1;
}
void init(){
    ::init();
}
