// This is a tool for me, Cratylus, by me, Cratylus. It's
// here because as lib maintainer I reserve the privilege to add 
// things for me that are to my convenience. You must never, ever
// use this, and if you do, I do not want to hear about the
// problems it caused.

#include <lib.h>
#include <save.h>
#include <daemons.h>
#include <vendor_types.h>
inherit LIB_ITEM;

static int numstress = 1;
string savefile = save_file(SAVE_WEIRDER);
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
        "/domains/campus/npc", "/domains/cave/npc" });
string *room_dirs = ({ "/domains/campus/room", "/domains/default/room",
        "/domains/town/room", "/domains/Ylsrim/room", "/domains/Praxis",
        "/domains/cave/room",
        "/secure/room", "/domains/town/virtual/sub", "/domains/town/virtual/sky", 
        "/domains/town/virtual/surface", "/domains/town/virtual/bottom",
        "/domains/town/virtual/forest", "/domains/town/space"  });
string *obj_dirs = ({ "/domains/campus/obj", "/domains/default/obj",
        "/domains/town/obj", "/domains/Ylsrim/obj", "/obj", "/std", 
        "/secure/obj", "/shadows", "/secure/modules", "/domains/cave/obj"  });
string *weap_dirs = ({ "/domains/campus/weap", "/domains/default/weap",
        "/domains/town/weap", "/domains/Ylsrim/weap", "/domains/cave/weap"  });
string *armor_dirs = ({ "/domains/campus/armor", "/domains/default/armor",
        "/domains/town/armor", "/domains/Ylsrim/armor", "/domains/cave/armor"  });
string *meals_dirs = ({ "/domains/campus/meals", "/domains/default/meals",
        "/domains/town/meals", "/domains/Ylsrim/meals", "/domains/cave/meals"  });
string *doors_dirs = ({ "/domains/campus/doors", "/domains/default/doors",
        "/domains/town/doors", "/domains/Ylsrim/doors", "/domains/cave/doors"  });
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
string *exceptions = ({});
static private string gstr;

void validate(){
    if(!this_player() || !archp(this_player())){
        error("No.");
    }
}

int yeik(string str){
    int cout, err;
    object *blanks;
    validate();
    err=catch(blanks=objects((: base_name($1)==LIB_BLANK :))[0..63000]);
    if(str){
        cout = call_out("yeik",1,"on");
        nomore = 0;
        yeik = 1;
        if(err){
        }
        if(!environment() || !archp(this_player())){
            error("no!");
        }
        for(int i = 400;i>0;i--){
            call_out("yeik",1,"on");
            new(LIB_BLANK);
        }
    }
    else {
        if(!nomore) cout = call_out("yeik",2);
        yeik = 0;
        if(sizeof(blanks)){
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
    exceptions = ({ "monty.c","charles.c","charly.c","tree.c","lars.c",
            "wraith.c", "archwraith.c","drone2.c","beggar.c","drone.c"});
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
    add_action("vargon","vargon");
    add_action("commcheck","commcheck");
    add_action("perfcheck","perfcheck");
}

static int loadthing(string str){
    if(last(str,2) == ".c"){
        gstr = str;
        if(unguarded( (: find_object(truncate(gstr,2)) :)) ) return 1;
        reset_eval_cost();
        catch( update(str) );
    }
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
            if(!strsrch(loadee,"/obj/area_room")) continue;
            if(!strsrch(loadee,"/obj/stargate")) continue;
            obs += ({ loadee });
            call_out("loadthing", 0, loadee);
        }
    }
    return 1;
}

int loadnpcs(mixed args){
    int i;
    validate();
    if(!args) i = 1;
    if(intp(args)) i = args;
    if(stringp(args)) i = atoi(args);
    if(!npcs) npcs = ({});
    foreach(string npcsdir in npc_dirs){
        foreach(string npcfile in get_dir(npcsdir+"/")){
            string loadee;
            int x = i;
            if(member_array(npcfile, exceptions) != -1){
                tc("skipping "+npcfile,"green");
                continue;
            }
            loadee = npcsdir+"/"+npcfile;
            npcs += ({ loadee });
            while(x){
                call_out("loadthing", 0, loadee);
                x--;
            }
        }
    }
    return 1;
}

int loadweaps(){
    validate();
    if(!weapons) weapons = ({});
    foreach(string weapsdir in weap_dirs){
        mixed wdir = get_dir(weapsdir+"/");
        if(!wdir || !sizeof(wdir)) continue;
        foreach(string weapfile in wdir){
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
        mixed adir = get_dir(armsdir+"/");
        if(!adir || !sizeof(adir)) continue;
        foreach(string armorfile in adir){
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
        mixed mdir = get_dir(mealsdir+"/");
        if(!mdir || !sizeof(mdir)) continue;
        foreach(string mealfile in mdir){
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
        mixed ddir = get_dir(doorsdir+"/");
        if(!ddir || !sizeof(ddir)) continue;
        foreach(string doorfile in ddir){
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
        mixed vdir = get_dir(verbsdir+"/");
        if(!vdir || !sizeof(vdir)) continue;
        foreach(string verbfile in vdir){
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
        mixed cdir = get_dir(cmdsdir+"/");
        if(!cdir || !sizeof(cdir)) continue;
        foreach(string cmdfile in cdir){
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

int stressload(string arg){
    int i;
    if(arg) i = atoi(arg);
    numstress = (i || 1);
    tc("numnstress: "+numstress);
    if(!sizeof(rooms)) call_out("loadrooms", 0);
    if(!sizeof(npcs)) call_out("loadnpcs", 0);
    if(sizeof(rooms) && sizeof(npcs)) this_object()->startstress();
    else call_out("startstress", 9);
    return 1;
}

int startstress(){
    object *newbatch = ({});
    object *targetrooms = filter(rooms, 
            (: last($1,2) == ".c" && last($1,9) != "furnace.c" &&
             $1->GetMedium() < 2 && !$1->GetProperty("no attack") :) );
    int victims;
    tc("Starting stresstest, multiplier: "+numstress);
    foreach(string npcfile in npcs){
        object npc;
        int err, x = numstress;
        if(last(npcfile,2) != ".c") continue;
        while(x){
            err = 0;
            npc = new(npcfile);
            newbatch += ({ npc });
            while(!err){
                object where;
                if(numstress < 11) reset_eval_cost();
                where = targetrooms[abs(random(sizeof(targetrooms))-1)];
                catch(err = npc->eventMove(where));
            }
            x--;
        }
    }
    newbatch = filter(newbatch, (: objectp($1) :) );
    victims = to_int(to_float(sizeof(newbatch)) * 0.005)+1;
    tc("newbatch size: "+sizeof(newbatch));
    while(victims){
        object germ, who;
        int err, batch = sizeof(newbatch);
        if(batch){
            err = catch(germ = new("/domains/town/obj/rage"));
            who = newbatch[abs(random(batch))];
            if(who && germ) germ->eventInfect(who);
            victims--;
        }
    }
    npcs->SetWanderSpeed(1);
    npcs->SetPermitLoad(1);
    return 1;
}

int eventDestruct(){
    return ::eventDestruct();
} 

int yeik2(string str){
    int dirty;
    validate();
    if(str == "dirty") dirty = 1;
    if(str){
        string file, what = "/lib/blank";
        int on=1, clone, i=2100000000;
        object ob=new(what);
        write("Starting the bullshit. ob: "+identify(ob));
        sscanf(file_name(ob), "%s#%d", file, clone);
        if(!dirty) destruct(ob);
        if(on){
            call_out("yeik2", 2);
        }
        if(!on){
            return;
        }
        while(i) {
            i--;
            ob =new(what);
            sscanf(file_name(ob), "%s#%d", file, clone);
            if(!dirty) destruct(ob);
        }
    }
    else {
        write("Stopping the bullshit.");
    }
    return 1;
}

int vargon(string str){
    int waves, win;
    validate();
    win = sscanf(str,"%d",waves);
    if(!win || !waves) waves = 5;
    for(win=waves;win > 0;win--){
        INTERMUD_D->RawSend(({ "channel-m", 5, mud_name(),
                    this_player()->GetKeyName(), 0, 0, "coffee",
                    this_player()->GetKeyName(), win+" Yes have some." }));
    }
    return waves;
}

int commcheck(){
    int i, count = 20;
    validate();
    write("Testing comms...");
    write2("\n");
    i = time_expression {
        while(count){
            count--;
            reset_eval_cost();
            write2(".");
            flush_messages();
        }
    };
    write2("\n\n");
    write("Microseconds: "+i);
    return 1;
}

int perfcheck(string foo){
    int sauber = 1, i, count = 1000000;
    validate();
    write("Testing performance...");
    write("Recorded perfscore: "+MASTER_D->GetPerformanceScore());
    if(sizeof(foo)){
        int tmp = atoi(foo);
        sauber = 0;
        if(tmp) count = tmp;
    }
    if(sauber){
        i = time_expression {
            while(count){
                count--;
            }
        };
    }
    else {
        i = time_expression {
            while(count){
                count--;
            }
        };
    }
    write("Microseconds: "+i);
    return 1;
}
