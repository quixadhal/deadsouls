#include "include/pistol.h"
#include <lib.h>
#include <damage_types.h>
#include <dirs.h>
inherit LIB_STORAGE;

private int MaxAmmo, Millimeter, AmmoSize;
private string PistolType, AmmoType;
private string AmmoFile;
private float Caliber;
private string *namen;
private string *ammonamen;
private int loaded, rounds;
private int shells;
private int mag,cloned;
private int autohit;
private int dam;

static void create(){
    string *s_save, *a_save;

    ::create();

    s_save = storage::GetSave();
    a_save = s_save;

    AddSave( a_save );
    AddSave( ({ "loaded", "rounds", "shells" , "mag" , "cloned" }) );
    MaxAmmo=6;
    AmmoFile="/domains/town/obj/round";
    PistolType="revolver";
    AmmoType="round";
    loaded=1;
    rounds=0;
}
void init(){
    object ob;
    object *inv;
    add_action("startLoad","load");
    add_action("startUnload","unload");
    this_object()->CalculateAmmoSize();
    namen=this_object()->GetId();
    if(PistolType=="revolver"
      && !present("cylinder",this_object())){
        new("/lib/cylinder")->eventMove(this_object());
    }
    if(PistolType=="revolver" && !cloned){
        cloned=1;
        ob=present("cylinder", this_object());
        inv=all_inventory(ob);
        filter(inv, (: this_object()->InitRevolver($1->GetId()) :) );
    }
    if(PistolType=="auto" && !present("clip",this_object()) ){
        mag=0;
        loaded=0;
        rounds=0;
    }
}
int InitRevolver(string *arr){
    if(member_array("round",arr) != -1){
        rounds++;
        return 1;
    }
    if(member_array("shell",arr) != -1){
        shells++;
        return 1;
    }
}
mixed CanGetFrom(object who, object item){
    return "It doesn't work that way. Try unloading it.";
}

mixed CanPutInto(object who, object what){
    return "It doesn't work that way. Try loading the pistol with something.";
}

int CanReceive(object ob){
    string *idarray;
    if(PistolType=="revolver"){
        idarray=ob->GetId();
        if(member_array("round",idarray) != -1){
            write("Try: load pistol with round.");
            return 0;
        }
    }
    if(PistolType != "auto" && ob->GetKeyName() != "revolver cylinder"){
        write("This pistol only receives bullets.");
        return 0;
    }
    idarray=ob->GetId();
    if(PistolType=="auto" && member_array("magazine",idarray) == -1){
        write("This pistol only receives ammunition clips.");
        return 0;
    }
    if(PistolType=="auto" && ob->GetMillimeter() != this_object()->GetMillimeter() ){
        write("That is not the correct ammunition clip size.");
        return 0;
    }
    if(PistolType=="auto" && ob->GetCaliber() != this_object()->GetCaliber() ){
        write("That is not the correct ammunition clip caliber.");
        return 0;
    }
    if(PistolType=="auto" && ob->GetAmmoType() != this_object()->GetAmmoType() ){
        write("That is not the correct ammunition clip type.");
        return 0;
    }
    if(PistolType=="auto" && mag){
        write("The pistol is already loaded.");
        return 0;
    }
    if(PistolType=="auto"){
        mag=1;
        rounds=sizeof(all_inventory(ob));
    }
    return 1;
}
int CanRelease(object ob){
    if(ob->GetKeyName()=="revolver cylinder"){
        return 0;

    }
    return 1;
}

mixed eventShoot(object ob, mixed target){
    object cible;
    if(stringp(target)) cible = present(target,environment(environment(this_object())));
    if(objectp(target)){
        cible = target;
        target = cible->GetName(); 
    }
    if(!rounds || rounds == 0){
        write("Your weapon is not loaded.\n");
        say(environment(this_object())->GetName()+" tries to shoot "+capitalize(target)+" with an unloaded weapon.\n");
        return 1;
    }
    write("You shoot at "+capitalize(target)+"!\n");
    say(environment(this_object())->GetName()+" shoots at "+capitalize(target)+"!\n");
    if(cible) tell_object(cible, environment(this_object())->GetName()+
          " shoots at you!\n");
    this_object()->eventFire(target);
    if(PistolType=="auto") new("/lib/shell")->eventMove(environment(environment(this_object())));
    return 1;
}

int eventFire(string str){
    object ob;
    int tempclass, i, dex;
    int TorsoNum, NumLimbs;
    mixed dexmap;
    string tempshort,templong;
    string *limbarr;
    int limbhit;
    string limbname;
    string s1,s2;
    rounds--;
    if(this_object()->GetPistolType() == "auto"){
        if(first_inventory(present("magazine",this_object())) ){
            first_inventory(present("magazine",this_object()))->eventDestruct();
            present("magazine",this_object())->MinusAmmo(1);
        }
        else rounds=0;
    }
    if(this_object()->GetPistolType() == "revolver"){
        shells++;
        ob=present("cylinder",this_object());
        if(present("round",ob)) present("round",ob)->eventDestruct();
        new("/lib/shell")->eventMove(ob);
    }
    if(rounds <= 0) loaded=0;
    ob=present(str,environment(environment(this_object())));
    if(creatorp(ob)){
        write(ob->GetName()+" catches your bullet in "+possessive(ob)+" teeth.\n");
        say(ob->GetName()+" catches the bullet in "+possessive(ob)+" teeth.\n");
        autohit=0;
        return 1;
    }
    if(ob && !living(ob) && base_name(ob) != LIB_CORPSE){
        if(!sscanf(ob->GetLong(),"%sIt has been damaged by gun%s",s1,s2)){
            tempclass=ob->GetClass();
            if(tempclass) ob->SetClass(tempclass/2);
            tempshort=ob->GetShort();
            tempshort = "a damaged "+remove_article(tempshort);
            ob->SetShort(tempshort);
            templong=ob->GetLong();
            if(sscanf(templong,"%s\n\n%s",s1,s2) >=1){
                templong=s1+" It has been damaged by gunfire.\n\n";
            }
            else
                templong += " It has been damaged by gunfire.";
            ob->SetLong(templong);
            tell_room(environment(environment(this_object())),"The bullet smashes into "+lower_case(ob->GetShort())+"!\n");
            return 1;
        }
        tell_room(environment(environment(this_object())),"The bullet smashes into "+lower_case(ob->GetShort())+"!\n");
        tempclass=ob->GetClass();
        if(tempclass) ob->SetClass(tempclass/2);
        return 1;
    }
    i = random(100);
    if(!creatorp(environment())){
        dexmap=environment(this_object())->GetStat("coordination");
        dex=dexmap["level"];
    }
    else dex = 200;
    if((ob && living(ob)) && (i < dex || autohit==1)){
        NumLimbs=sizeof(ob->GetLimbs());
        TorsoNum=member_array(ob->GetTorso(),ob->GetLimbs());
        i=random(100);
        if(i < 50) limbhit=TorsoNum;
        else limbhit=random(NumLimbs);
        limbarr=ob->GetLimbs();
        limbname=limbarr[limbhit];
        tell_room(environment(environment(this_object())),"The bullet smashes into "+
          capitalize(str)+"'s "+limbname+"!\n",ob);
        tell_object(ob,"The bullet smashes into your "+limbname+"!\n");
        ob->AddLead("gunshot_wounds", 1);
        ob->SetAttack(this_agent());
        if(!present("firearms_wound",ob)){
            new("/domains/town/obj/wound")->eventMove(ob);
        }
        if(Caliber) dam = to_int(Caliber/6);
        if(Millimeter) dam = Millimeter;
        if(!dam) dam = 7;
        dam *= random(10);
        dam += random(environment(this_object())->GetStatLevel("coordination"));
        dam += environment(this_object())->GetSkillLevel("projectile attack");
        if(creatorp(this_player())) write("you do "+dam+" points of damage");

        ob->eventReceiveDamage(environment(this_object()),(PIERCE|TRAUMA), dam, 1, limbname);
        if(!ob->GetInCombat()){
            ob->eventForce("attack "+environment(this_object())->GetKeyName());
        }
        autohit=0;
        return 1;
    }
    write("Your shot misses its mark.\n");
    say(environment(this_object())->GetName()+"'s shot misses its mark.\n");
    this_object()->missed_shot();
    return 1;
}
int missed_shot(){
    object ob,maghere,magstuff;
    string str;
    int i;
    object *inv;
    string *arr;
    if(environment(environment(this_object()))->GetClimate() != "indoors"){
        return 1;
    }
    inv=all_inventory(environment(environment(this_object())));
    i=random(sizeof(inv));
    if(living(inv[i])){
        tell_room(environment(environment(this_object())), inv[i]->GetName()+" is struck "+
          "by the stray bullet!",inv[i]);
        tell_object(inv[i],"You are struck by a stray bullet from "+environment(this_object())->GetName()+
          "'s gun.\n");
    }
    if(!living(inv[i])){
        arr=explode(inv[i]->GetKeyName(),"");
        if(arr[sizeof(arr)-1] =="s"){
            tell_room(environment(environment(this_object())),capitalize(inv[i]->GetShort())+" are struck by the stray bullet.");
        }
        else
            tell_room(environment(environment(this_object())),capitalize(inv[i]->GetShort())+" is struck by the stray bullet.");
    }
    autohit=1;
    rounds++;
    this_object()->eventFire(inv[i]->GetKeyName());
    if(this_object()->GetPistolType() == "revolver"){
        shells--;
        present("shell",present("cylinder",this_object()))->eventDestruct();
        ob=new("/lib/round");
        ob->SetCaliber(this_object()->GetCaliber());
        ob->SetMillimeter(this_object()->GetMillimeter());
        ob->SetPistolType(this_object()->GetPistolType());
        ob->SetAmmoType(this_object()->GetAmmoType());
        ob->eventMove(present("cylinder",this_object()));
    }
    else
    {
        str = "";
        maghere=present("magazine",this_object());
        if(maghere) magstuff = first_inventory(maghere);
        if(maghere && magstuff) str=base_name(magstuff);
        if(str && str !="" )	new(str)->eventMove(maghere);
    }
    return 1;
}
int CalculateAmmoSize(){
    float i;
    i=this_object()->GetCaliber();
    if(i > 0){
        if(stringp(i)) sscanf(i,"%d",i);
        if(intp(i)) to_float(i);
        while(i > 1.0){
            i = i*(0.1);
        }
        i = i*(25.4);
        AmmoSize=to_int(i);
        dam=AmmoSize*(AmmoSize);
        dam *= 3;
        return 1;
    }
    AmmoSize=this_object()->GetMillimeter();
    return 1;
}
int ShowRounds(){ environment(this_object())->eventPrint("Pistol has: "+rounds+" rounds.\n");
    environment(this_object())->eventPrint("Pistol has: "+shells+" shells.\n");
    return 1;
}
int SetAmmo(int i){ rounds=i; return 1; }
int GetAmmo(){ return rounds; }
int GetMag(){ return mag; }
int SetMag(int i){ mag=i; return 1; }
int SetLoaded(int i){ loaded=i; return 1; }
int GetLoaded(){ return loaded; }
int SetAmmoFile(string str){ AmmoFile=str; return 1; }
int SetAmmoType(string str){ AmmoType=str; return 1; }
int SetPistolType(string str){ PistolType=str; return 1; }
int SetMaxAmmo(int x){ MaxAmmo=x; return 1; }
int SetCaliber(int x){ Caliber=x; return 1; }
int SetMillimeter(int x){ Millimeter=x; return 1; }
string GetAmmoType(){ return AmmoType; }
string GetPistolType(){ return PistolType; }
int GetMaxAmmo(){ return MaxAmmo; }
int GetCaliber(){ return Caliber; }
int GetMillimeter(){ return Millimeter; }

int startLoad(string str){
    string s1,s2;
    if(!str || str== "") return 0;
    if(member_array(str, namen) != -1){
        write("Load pistol with what?");
        return 1;
    }
    if(sscanf(str,"%s with %s",s1,s2) >= 1){
        if(!present(s1,environment(this_object()))){
            write("You have no such thing to load.\n");
            return 1;
        }
        if(member_array(s1,namen) == -1){
            present(s1, environment(this_object()))->startLoad(str);
            return 1;
        }
        if(PistolType=="revolver"){
            this_object()->LoadRevolver(s2);
            return 1;
        }
        if(PistolType=="auto"){
            this_object()->LoadAuto(s2);
            return 1;
        }
        write("Your pistol seems not to be loadable.\n");
        return 1;
    }
    if(member_array(s1,namen) != -1){
        write("Load it with what?\n");
        return 1;
    }
}
int LoadAuto(string str){
    object ob;
    string junk;
    ob=present(str,environment(this_object()));
    if(!ob){
        write("You have no such thing to load your pistol with.");
        return 1;
    }
    write("You load your pistol.");
    say(environment(this_object())->GetName()+" loads an ammunition clip into "+
      possessive(environment(this_object()))+" semiautomatic pistol.");
    ob->eventMove(this_object());
    if(!sizeof(deep_inventory(present("magazine",this_object())))) return 1;
    if(!AmmoFile=file_name(first_inventory(present("magazine",this_object()))) ){
        AmmoFile="/lib/round";
    }
    else sscanf(AmmoFile,"%s#%s",AmmoFile,junk);
    return 1;
}
int LoadRevolver(string str){
    object ob;
    ob=present(str,environment(this_object()));
    if(!ob){
        write("You do not have that to load with.\n");
        return 1;
    }
    ammonamen=ob->GetId();
    if(member_array("bullet",ammonamen) == -1){
        write("That is not something you can load your pistol with.\n");
        return 1;
    }
    if(ob->GetPistolType() != "revolver"){
        write("That's not revolver ammunition.\n");
        return 1;
    }
    if(ob->GetAmmoType() != this_object()->GetAmmoType()){
        write("That's the incorrect ammunition for this pistol.\n");
        return 1;
    }
    if(rounds + shells == MaxAmmo){
        write("Your pistol cannot hold any more rounds.\n");
        return 1;
    }
    write("You load a round into your pistol.\n");
    say(environment(this_object())->GetName()+" loads a bullet into "+
      possessive(environment(this_object()))+" revolver.");
    rounds++;
    ob->eventMove(present("revolver cylinder",this_object()));
    return 1;
}
int startUnload(string str){
    string n1,s1,s2;
    int n2;
    if(!str || str== "") return 0;
    if(mag==1 && member_array(str,namen) != -1){
        this_object()->doMagUnload();
        return 1;
    }
    if(PistolType=="revolver" && member_array(str,namen) != -1){
        if(rounds == 0 && shells == 0){
            write("Your pistol is already empty.");
            return 1;
        }
        write("You unload your pistol.");
        this_object()->doRevolverUnload("all","all");
        return 1;
    }
    if(sscanf(str,"%s %s from %s",n1,s1,s2) >=1){
        if(!sscanf(n1,"%d",n2)){
            write("You must specify a number of items to unload.\n");
            return 1;
        }
        if(member_array(s2,namen) == -1){
            if(!present(s2, environment(this_object()))){
                write("You have no such thing to unload.\n");
                return 1;
            }
            present(s2,environment(this_object()))->UnloadRevolver(str);
            return 1;
        }
        if(s1=="shell" || s1=="shells"){
            if(!shells){
                write("There are no shells in the pistol.\n");
                return 1;
            }
            if(shells < n2){
                write("There aren't that many shells in the pistol.\n");
                return 1;
            }
            this_object()->doRevolverUnload("shells",n1);
            return 1;
        }
        if(s1=="bullet" || s1=="bullets" || s1=="round"|| s1=="rounds"){
            if(!rounds){
                write("There are no rounds in the pistol.\n");
                return 1;
            }
            if(rounds < n2){
                write("There aren't that many rounds in the pistol.\n");
                return 1;
            }
            this_object()->doRevolverUnload("rounds",n1);
            return 1;
        }
    }
}

int doMagUnload(){
    if(!present("clip",this_object())){
        write("The pistol is already empty.");
        return 0;
    }
    write("You unload an ammo clip from your pistol.");
    say(this_player()->GetName()+" unloads an ammo clip from "+possessive(this_player())+" pistol.");
    present("clip",this_object())->eventMove(environment(this_object()));
    mag=0;
    loaded=0;
    rounds=0;
    return 1;
}

int doRevolverUnload(string what, string num){
    int i,n1,n2;
    if(sscanf(num,"%d",n1)){
    }
    if(what=="all"){
        n1=shells;
        n2=rounds;
    }
    if(what=="shells" || what=="all"){
        i=shells-n1;
        while(shells > i){
            present("shell",present("revolver cylinder",this_object()))->eventMove(environment(this_object()));
            shells--;
        }
    }
    if(what=="rounds" || what=="all"){
        if(what != "all") n2=n1;
        i=rounds-n2;
        while(rounds > i){
            present("round",present("revolver cylinder",this_object()))->eventMove(environment(this_object()));
            rounds--;
        }
    }
    if(n2 > 0){
        string things = "rounds";
        if(n2 == 1) things = "round";
        write("You unload "+cardinal(n2)+" "+things+" from your pistol.");
    }
    if(n1 > 0){
        string things = "shells";
        if(n1 == 1) things = "shell";
        write("You unload "+cardinal(n1)+" "+things+" from your pistol.");
    }
    say(environment(this_object())->GetName()+" unloads some cartridges from "+
      possessive(environment(this_object()))+" revolver.");
    return 1;
}
