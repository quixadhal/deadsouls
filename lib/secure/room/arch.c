#include <lib.h>
#include <daemons.h>
inherit LIB_ROOM;

mixed eventReadPrintout(){
    mapping MudMap2;
    string *all_dead_souls = ({});
    string ret = "";
    string tmpfile = generate_tmp();
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
	ret += mud + "\t\t" +MudMap2[mud][5] + "\n";
    }
    ret += "\nTotal: "+sizeof(all_dead_souls);
    write_file(tmpfile,ret);
    this_player()->eventPage(tmpfile);
    rm(tmpfile);
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
    string tmpfile = generate_tmp();
    MudMap = INTERMUD_D->GetMudList();
    if(!sizeof(MudMap)){
	write("Intermud3 link down. Stats unavailable.");
	return 1;
    }
    if(MudMap["Dead Souls"]) canonical = MudMap["Dead Souls"][5];
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
		ret += mud + "\t\t" +MudMap[mud][5] + "\n";
	}
    }
    if(!grepp(ret,"current version") || !grepp(canonical,mudlib_version())){
	ret += "\n%^CYAN%^Muds running our version of Dead Souls:%^RESET%^\n";
	foreach(string mud in dead_keys){
	    ret += mud + "\t\t" +MudMap[mud][5] + "\n";
	}
    }
    ret += "\n%^RED%^Muds running any version of Dead Souls:%^RESET%^\n";
    foreach(string mud in all_dead_keys){
	ret += mud + "\t\t" +MudMap[mud][5] + "\n";
    }

    ret += "\nTotal: "+sizeof(all_dead_keys);

    ret+= "\n\n%^YELLOW%^Dead Souls muds online:%^RESET%^ \n";
    foreach(string mud in online_muds){
	ret += mud + "\t\t" +MudMap[mud][5] + "\n";
    }

    write_file(tmpfile,ret);
    this_player()->eventPage(tmpfile);
    rm(tmpfile);
    return "";

}

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Arch Room");
    SetLong("This is a polished, antiseptic room composed of some "
      "white, gleaming material. There is a viewscreen on a wall here, "
      "with a control panel alonside it. "
      "There is a shimmering portal "
      "on the north wall. A long printout hangs from the panel.");
    SetItems( ([ ({"wall","walls"}) : "The walls seem composed "
	"of some advanced polymer. They are extremely clean and highly "
	"polished.",
	"room" : "This looks like it might be the control room "
	"for the mud.",
	//({"screen","viewscreen"}) : "This is a display screen of some sort.",
	({"screen","viewscreen"}) : (: eventReadScreen :) ,
	({"printout"}) : (: eventReadPrintout :) ,
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
    SetRead("screen", (: eventReadScreen :) );
    SetRead("printout", (: eventReadPrintout :) );

    SetExits( ([
	"north" : "/domains/default/room/wiz_hall.c",
      ]) );
    SetListen("default", "You can faintly hear a low hum coming from the walls.");
    SetListen( ({"wall","walls"}), "You hear a low throbbing sound, as if from machinery.");
    AddEnter("portal" , "/domains/default/room/wiz_hall");
    ob = new("/lib/bboard");
    ob->SetKeyName("chalkboard");
    ob->SetId( ({ "board", "chalkboard" }));
    ob->set_board_id("admin_board");
    ob->set_max_posts(30);
    ob->SetShort("The Arch Board");
    ob->SetLong("This is the Arch board. You know how to use it.");
    ob->eventMove(this_object());
}
int CanReceive(object ob) {
    if( living(ob) && !archp(ob)  ){
	message("info","The arch room is available only to "+
	  "admins, sorry.",ob);
	return 0;
    }
    return 1;
}
void init(){
    ::init();
}
