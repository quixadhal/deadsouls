// This is a tool for me, Cratylus, by me, Cratylus. It's
// here because as lib maintainer I reserve the privilege to add 
// things for me that are to my convenience. You must never, ever
// use this, and if you do, I do not want to hear about the
// problems it caused.

#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

string savefile = "/secure/save/weirder.o";
string *lib_dirs = ({ "/lib/comp","/lib/daemons","/lib/events/",
  "/lib/lvs", "/lib/lvs", "/lib/props", "/lib/std", 
  "/lib/user", "/lib/virtual", "/lib","/secure/lib","/secure/lib/net" });
string *daemon_dirs = ({ "/daemon", "/secure/daemon" });
string *cmd_dirs = ({ "/cmds/admins", "/cmds/builders", "/cmds/common",
  "/cmds/creators", "/cmds/hm", "/cmds/players", "/secure/cmds/admins",
  "/secure/cmds/builders", "/secure/cmds/common", "/secure/cmds/creators",
  "/secure/cmds/hm", "/secure/cmds/players" });
string *verb_dirs = ({ "/secure/verbs/creators", "/verbs/admins",
  "/verbs/builders", "/verbs/common", "/verbs/creators", "/verbs/items",
  "/verbs/players", "/verbs/rooms", "/verbs/spells", "/verbs/undead" });
string *npc_dirs = ({ "/domains/default/npc", "/domains/town/npc",
  "/domains/campus/npc" });
string *room_dirs = ({ "/domains/campus/room", "/domains/default/room",
  "/domains/town/room", "/domains/Ylsrim/room", "/domains/Praxis",
  "/secure/room", "/domains/town/virtual/sub", "/domains/town/virtual/sky", 
  "/domains/town/virtual/surface", "/domains/town/virtual/bottom",
  "/domains/town/virtual/forest", "/domains/town/space"  });
string *obj_dirs = ({ "/domains/campus/obj", "/domains/default/obj",
  "/domains/town/obj", "/domains/Ylsrim/obj", "/obj", "/std", 
  "/secure/obj", "/shadows", "/secure/modules"  });
string *weap_dirs = ({ "/domains/campus/weap", "/domains/default/weap",
  "/domains/town/weap", "/domains/Ylsrim/weap"  });
string *armor_dirs = ({ "/domains/campus/armor", "/domains/default/armor",
  "/domains/town/armor", "/domains/Ylsrim/armor"  });
string *meals_dirs = ({ "/domains/campus/meals", "/domains/default/meals",
  "/domains/town/meals", "/domains/Ylsrim/meals"  });
string *doors_dirs = ({ "/domains/campus/doors", "/domains/default/doors",
  "/domains/town/doors", "/domains/Ylsrim/doors"  });
string *powers_dirs = ({ "/powers/spells", "/powers/psionics",
  "/powers/feats", "/powers/trades" });
string *rooms = ({});
string *obs = ({});
string *npcs = ({});
string *weapons = ({});
string *armors = ({});
string *meals = ({});
string *doors = ({});
string *libs = ({});
string *daemons = ({});
string *cmds = ({});
string *verbs = ({});
string *powers = ({});
int nomore, yeik = 0;

void validate(){
    if(!this_player() || !archp(this_player())){
        error("No.");
    }
}

int yeik(string str){
    int cout, err;
    validate();
    if(str == "on" || yeik == 1){
        object *blanks;
        cout = call_out("yeik",2);
        nomore = 0;
        yeik = 1;
        err = catch(blanks = objects( (: base_name($1) == "/lib/blank" :) ));
        if(err){
            tc("foo!");
        }
        if(sizeof(blanks)){
            foreach(object blank in blanks){
                destruct(blank);
            }
        }
        if(!environment() || !archp(this_player())){
            error("no!");
        }
        for(int i = 20000;i>0;i--){
            new("/lib/blank");
        }
    }
    else {
        object *blanks;
        if(!nomore) cout = call_out("yeik",2);
        err = catch(blanks = objects( (: base_name($1) == "/lib/blank" :)));
        yeik = 0;
        if(sizeof(blanks)){
            //cout = call_out("yeik",2);
            foreach(object blank in blanks){
                destruct(blank);
            }

        }
        else nomore = 1;
    }
    return cout;
}

void create(){
    ::create();
    SetKeyName("weirding module");
    SetId( ({"module", "box", "weirder"}) );
    SetAdjectives( ({"small","featureless","black"}) );
    SetShort("a small black box");
    SetLong("A small, featureless black box. Whatever it is, "
      "you are somehow deeply certain it is not your business, "
      "and you must leave it alone.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    //YES = 1000;
    //NO = -1000;
    if(file_exists(savefile)){
        //unguarded( (: restore_object(savefile) :) );
    }
}
void init(){
    ::init();
    add_action("stressload","stressload");
    add_action("loadrooms","loadrooms");
    add_action("loadobs","loadobs");
    add_action("loadnpcs","loadnpcs");
    add_action("loadweaps","loadweaps");
    add_action("loadarmors","loadarmors");
    add_action("loadmeals","loadmeals");
    add_action("loaddoors","loaddoors");
    add_action("loadthings","loadthings");
    add_action("loadcmds","loadcmds");
    add_action("loaddaemons","loaddaemons");
    add_action("loadverbs","loadverbs");
    add_action("loadlibs","loadlibs");
    add_action("loadsys","loadsys");
    add_action("loadall","loadall");
    add_action("yeik","yeik");
    add_action("yeik2","yeik2");
}
int loadthing(string str){
    tc("lpc: "+str);
    if(last(str,2) == ".c"){
        update(str);
    }
    else tc("non lpc: "+str,"red");
    return 1;
}
int loadrooms(){
    validate();
    if(!rooms) rooms = ({});
    foreach(string roomdir in room_dirs){
        string *dir_array = ( get_dir(roomdir+"/") || ({}) );
        foreach(string roomfile in dir_array){
            string loadee = roomdir+"/"+roomfile;
            rooms += ({ loadee });
            call_out("loadthing", 0, loadee); 
        }
    }
    return 1;
}
int loadobs(){
    validate();
    if(!obs) obs = ({});
    foreach(string obsdir in obj_dirs){
        foreach(string obfile in get_dir(obsdir+"/")){
            string loadee = obsdir+"/"+obfile;
            obs += ({ loadee });
            call_out("loadthing", 0, loadee);
        }
    }
    return 1;
}
int loadnpcs(){
    validate();
    if(!npcs) npcs = ({});
    foreach(string npcsdir in npc_dirs){
        foreach(string npcfile in get_dir(npcsdir+"/")){
            string loadee = npcsdir+"/"+npcfile;
            npcs += ({ loadee });
            call_out("loadthing", 0, loadee);
        }
    }
    return 1;
}
int loadweaps(){
    validate();
    if(!weapons) weapons = ({});
    foreach(string weapsdir in weap_dirs){
        foreach(string weapfile in get_dir(weapsdir+"/")){
            string loadee = weapsdir+"/"+weapfile;
            weapons += ({ loadee });
            call_out("loadthing", 0, loadee);
        }
    }
    return 1;
}
int loadarmors(){
    validate();
    if(!armors) armors = ({});
    foreach(string armsdir in armor_dirs){
        foreach(string armorfile in get_dir(armsdir+"/")){
            string loadee = armsdir+"/"+armorfile;
            armors += ({ loadee });
            call_out("loadthing", 0, loadee);
        }
    }
    return 1;
}
int loadmeals(){
    validate();
    if(!meals) meals = ({});
    foreach(string mealsdir in meals_dirs){
        foreach(string mealfile in get_dir(mealsdir+"/")){
            string loadee = mealsdir+"/"+mealfile;
            meals += ({ loadee });
            call_out("loadthing", 0, loadee);
        }
    }
    return 1;
}
int loaddoors(){
    validate();
    if(!doors) doors = ({});
    foreach(string doorsdir in doors_dirs){
        foreach(string doorfile in get_dir(doorsdir+"/")){
            string loadee = doorsdir+"/"+doorfile;
            doors += ({ loadee });
            call_out("loadthing", 0, loadee);
        }
    }
    return 1;
}
int loadverbs(){
    validate();
    if(!verbs) verbs = ({});
    foreach(string verbsdir in verb_dirs){
        foreach(string verbfile in get_dir(verbsdir+"/")){
            string loadee = verbsdir+"/"+verbfile;
            verbs += ({ loadee });
            call_out("loadthing", 0, loadee);
        }
    }
    return 1;
}
int loadcmds(){
    validate();
    if(!cmds) cmds = ({});
    foreach(string cmdsdir in cmd_dirs){
        foreach(string cmdfile in get_dir(cmdsdir+"/")){
            string loadee = cmdsdir+"/"+cmdfile;
            cmds += ({ loadee });
            call_out("loadthing", 0, loadee);
        }
    }
    return 1;
}
int loaddaemons(){
    validate();
    if(!daemons) daemons = ({});
    foreach(string daemonsdir in daemon_dirs){
        foreach(string daemonfile in get_dir(daemonsdir+"/")){
            string loadee = daemonsdir+"/"+daemonfile;
            daemons += ({ loadee });
            call_out("loadthing", 0, loadee);
        }
    }
    return 1;
}
int loadlibs(){
    validate();
    if(!libs) libs = ({});
    foreach(string libsdir in lib_dirs){
        foreach(string libfile in get_dir(libsdir+"/")){
            string loadee = libsdir+"/"+libfile;
            libs += ({ loadee });
            call_out("loadthing", 0, loadee);
        }
    }
    return 1;
}
int loadpowers(){
    validate();
    if(!powers) powers = ({});
    foreach(string powersdir in powers_dirs){
        foreach(string powerfile in get_dir(powersdir+"/")){
            string loadee = powersdir+"/"+powerfile;
            powers += ({ loadee });
            call_out("loadthing", 0, loadee);
        }
    }
    return 1;
}
int loadthings(){
    validate();
    call_out("loaddoors",0);
    call_out("loadmeals",1);
    call_out("loadarmors",2);
    call_out("loadweaps",3);
    call_out("loadobs",4);
    call_out("loadnpcs",5);
    call_out("loadrooms",6);
    return 1;
}
int loadsys(){
    call_out("loadlibs",0);
    call_out("loaddaemons",1);
    call_out("loadcmds",2);
    call_out("loadverbs",3);
    call_out("loadpowers",4);
    return 1;
}
int loadall(){
    call_out("loadsys",0);
    call_out("loadthings",10);
    return 1;
}
int stressload(){
    if(!sizeof(rooms)) call_out("loadrooms", 0);
    if(!sizeof(npcs)) call_out("loadnpcs", 0);
    if(sizeof(rooms) && sizeof(npcs)) this_object()->startstress();
    else call_out("startstress", 9);
    return 1;
}
int startstress(){
    object *newbatch = ({});
    object *targetrooms = filter(rooms, 
      (: last($1,2) == ".c" && last($1,9) != "furnace.c" :) );
    int victims;
    tc("Starting stresstest");
    tc("npcs: "+identify(npcs),"green");
    tc("rooms: "+identify(rooms),"blue");
    foreach(string npcfile in npcs){
        object npc;
        int err;
        tc("processing: "+npcfile);
        if(last(npcfile,2) != ".c") continue;
        err = catch(npc = new(npcfile));
        if(err) continue;
        else tc("Successfully cloned "+identify(npc));
        newbatch += ({ npc });
        while(!err){
            object where = targetrooms[abs(random(sizeof(targetrooms))-1)];
            tc("Moving "+identify(npc)+" to "+identify(where));
            catch(err = npc->eventMove(where));
        }
    }
    newbatch = filter(newbatch, (: objectp($1) :) );
    victims = to_int(to_float(sizeof(newbatch)) * 0.1)+1;
    tc("newbatch size: "+sizeof(newbatch));
    while(victims){
        object germ, who;
        int err;
        err = catch(germ = new("/domains/town/obj/rage"));
        who = newbatch[abs(random(sizeof(newbatch))-1)];
        tc("Infecting "+identify(who)+" with "+identify(germ),"blue");
        if(germ) germ->eventInfect(who);
        victims--;
    }
    npcs->SetWanderSpeed(1);
    npcs->SetPermitLoad(1);
    return 1;
}

int eventDestruct(){
    unguarded( (: save_object(savefile) :) );
    return ::eventDestruct();
} 

int yeik2(string str){
    validate();
    if(str){
        string file, what = "/lib/pile";
        int on=1, clone, i=2100000000;
        object ob=new(what);
        write("Starting the bullshit. ob: "+identify(ob));
        sscanf(file_name(ob), "%s#%d", file, clone);
        destruct(ob);
        //if(clone > 5000){
        //    tc("too many clones");
        //    on = 0;
        //}
        if(on){
            //tc("on");
            call_out("yeik2", 2);
        }
        if(!on){
            //tc("weird");
            return;
        }
        while(i) {
            i--;
            ob =new(what);
            sscanf(file_name(ob), "%s#%d", file, clone);
            //tell_object(environment(this_object()), clone + " ");
            destruct(ob);
        }
        //tc("hmm");
    }
    else {
        write("Stopping the bullshit.");
    }
    return 1;
}
