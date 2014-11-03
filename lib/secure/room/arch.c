#include <lib.h>
#include <daemons.h>
#include <message_class.h>

#ifndef DISABLE_IMC2
#define DISABLE_IMC2 0
#endif 

inherit LIB_ROOM;

int imud_enabled = 0;
string gname;
string *ulist = ({});
mapping umap = ([]);

void validate(){
    if(!this_player() || !archp(this_player())){
        error("No.");
    }
}

int ReceiveObs(object ob){
    string bname = base_name(ob);
    umap[bname] = (umap[bname] + 1);
    return 0;
}

string ReadScreen(){
    string *base_names = ({});
    string ret = "Top loaded objects:\n";
    int hbs, hbs2, rooms, npcs, meminf;
    mixed *foo = ({});
    object *floods = ({});
    validate();
    floods = objects( (: (base_name($1) == LIB_FLOW || inherits(LIB_FLOW, $1))
                && clonep($1) :) );
    ulist = ({});
    umap = ([]);
    if(sizeof(objects()) < 8192){
        foo = objects( (: ReceiveObs($1) :) );
        ulist = ({});
        foreach(mixed key, mixed val in umap){
            reset_eval_cost();
            ulist += ({ ({ val, key }) });
        }
        ulist = sort_array(ulist, -1)[0..9];
        if(sizeof(ulist)){
            foreach(mixed element in ulist){
                ret += element[1]+" "+element[0]+"\n";
            }
        }
    }
    else {
        ret += "Too many objects to sort.\n";
    }
    rooms = sizeof(objects( (: inherits(LIB_ROOM, $1) :) ) );
    npcs = sizeof(objects( (: inherits(LIB_NPC, $1) && clonep($1):) ) );
    hbs = sizeof(filter( heart_beats(), (: !living($1) :) ));
    hbs2 = sizeof(filter( heart_beats(), (: !living($1) &&
                    !inherits(LIB_ROOM, $1) :) ));
    ret += "\nTotal number of loaded objects: "+sizeof(objects())+"\n";
    ret += "Loaded rooms: "+rooms+"\n";
    ret += "Cloned NPC's: "+npcs+"\n\n";
    ret += "Total number of connected users: "+sizeof(users())+"\n";
    ret += "Pending callouts: "+sizeof(call_out_info())+"\n";
    ret += "File descriptors in use: "+
        (sizeof(explode(dump_file_descriptors(),"\n"))-3)+"\n";
    ret += "Nonlivings with heartbeats: "+hbs+", !rooms: "+hbs2+"\n";
    ret += "Flood objects: "+sizeof(floods)+"\n";
    if(true()){
        string tmp;
        meminf = memory_info()/1000000;
        if(!meminf) tmp = "less than 1 meg";
        else tmp = meminf+" megs";
        if(meminf){
            ret += "Memory in use (allocated memory will be higher): "+
                tmp+"\n";
        }
    }
    return ret;
} 

string eventReadScreen(){
    validate();
    return unguarded( (: ReadScreen :) );
}

mixed eventReadPrintout(){
    return read_file("/doc/old/the_beginning.txt");
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

    if(!INTERMUD_D->GetEnabled()){
        ret += "\"I3 connection: %^B_BLACK%^%^BOLD%^WHITE%^DISABLED%^RESET%^, ";
    }
    else if(INTERMUD_D->GetConnectedStatus())
        ret += "\"I3 connection: %^BOLD%^GREEN%^ONLINE%^RESET%^, ";
    else ret +=  "\"I3 connection: %^BOLD%^RED%^OFFLINE%^RESET%^, ";

    switch (IMC2_D->getonline()) {
        case 1:
            ret += "IMC2: %^BOLD%^GREEN%^ONLINE%^RESET%^\"";
            break;
        case 2:
            if(!IMC2_D->GetEnabled()){
                ret += "IMC2: %^B_BLACK%^%^BOLD%^WHITE%^DISABLED%^RESET%^\"";
            }
            else {
                ret += "IMC2: %^BOLD%^YELLOW%^WAITING FOR ACCEPTANCE%^RESET%^\"";
            }
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
        "with a control panel alongside it. "+
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
    SetCoordinates("-2,2,0");
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
