#include "include/firearm.h"
#include <lib.h>
#include <damage_types.h>
#include <dirs.h>
inherit LIB_STORAGE;
inherit LIB_LOAD;

private int MaxAmmo, Millimeter, AmmoSize;
private string FirearmType, AmmoType;
private string AmmoFile, firearm_name;
private float Caliber;
private string *namen;
private string *ammonamen;
private int loaded, rounds;
private int shells, magnum;
private int mag,cloned;
private int autohit;
private int dam, last_shot;

static void create(){
    string *s_save, *a_save;

    ::create();

    s_save = storage::GetSave();
    a_save = s_save;

    AddSave( a_save );
    AddSave( ({ "loaded", "rounds", "shells" , "mag" , "cloned" }) );
    MaxAmmo=6;
    AmmoFile="/domains/default/obj/round";
    FirearmType="revolver";
    AmmoType="round";
    loaded=1;
    rounds=0;
}

void init(){
    object ob;
    object *inv;
    ::init();
    this_object()->CalculateAmmoSize();
    namen=this_object()->GetId();
    if(FirearmType=="revolver"
            && !present("cylinder",this_object())){
        new("/lib/cylinder")->eventMove(this_object());
    }
    if(FirearmType=="revolver" && !cloned){
        cloned=1;
        ob=present("cylinder", this_object());
        inv=all_inventory(ob);
        filter(inv, (: this_object()->InitRevolver($1->GetId()) :) );
    }
    if(FirearmType=="auto" && !present("clip",this_object()) ){
        mag=0;
        loaded=0;
        rounds=0;
    }
}

string SetFirearmName(string str){
    if(str) firearm_name = str;
    return firearm_name;
}

string GetFirearmName(){
    if(firearm_name) return firearm_name;
    return GetKeyName();
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
    return "It doesn't work that way. Try loading the "+GetFirearmName()+" with something.";
}

int CanReceive(object ob){
    string *idarray;
    if(FirearmType=="revolver"){
        if(rounds == MaxAmmo){
            write("It is already fully loaded.");
            return 1;
        }
    }
    if(FirearmType != "auto" && ob->GetKeyName() != "revolver cylinder"){
        write("This "+GetFirearmName()+" only receives bullets.");
        return 0;
    }
    idarray=ob->GetId();
    if(FirearmType=="auto" && member_array("magazine",idarray) == -1){
        write("This "+GetFirearmName()+" only receives ammunition clips.");
        return 0;
    }
    if(FirearmType=="auto" && ob->GetMillimeter() != this_object()->GetMillimeter() ){
        write("That is not the correct ammunition clip size.");
        return 0;
    }
    if(FirearmType=="auto" && ob->GetCaliber() != this_object()->GetCaliber() ){
        write("That is not the correct ammunition clip caliber.");
        return 0;
    }
    if(FirearmType=="auto" && ob->GetAmmoType() != this_object()->GetAmmoType() ){
        write("That is not the correct ammunition clip type.");
        return 0;
    }
    if(FirearmType=="auto" && mag){
        write("The "+GetFirearmName()+" is already loaded.");
        return 0;
    }
    if(FirearmType=="auto"){
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

varargs mixed eventShoot(object ob, mixed target, string dir, string whom){
    object cible;
    object shell;
    object *obs;

    if(!target || dir){
        write("You can't shoot that way with this weapon.");
        return 1;
    }

    if(objectp(target)){
        cible = target;
    }
    else {
        obs = filter(get_livings(environment(this_player())),
                (: answers_to($(target), $1) :));
        if(!sizeof(obs)) cible = present(target,environment(this_player()));
        else cible = obs[0];
    }
    if(cible) target = cible->GetName();

    else {
        write("It seems there's a problem targeting "+target+".");
        return 1;
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
    if(FirearmType=="auto"){
        shell = new(LIB_SHELL);
        if(!shell) return 1;
        shell->SetFirearmType(GetFirearmType());
        shell->SetCaliber(GetCaliber());
        shell->eventMove(environment(environment(this_object())));
    }
    return 1;
}

int eventFire(mixed str){
    object ob;
    object *obs;
    int tempclass, i, dex;
    int TorsoNum, NumLimbs;
    mixed dexmap;
    string tempshort,templong;
    string *limbarr;
    int limbhit;
    string limbname;
    string s1,s2;
    rounds--;
    if(this_object()->GetFirearmType() == "auto"){
        if(first_inventory(present("magazine",this_object())) ){
            first_inventory(present("magazine",this_object()))->eventDestruct();
            present("magazine",this_object())->MinusAmmo(1);
        }
        else rounds=0;
    }
    if(this_object()->GetFirearmType() == "revolver"){
        shells++;
        ob=present("cylinder",this_object());
        if(present("round",ob)) present("round",ob)->eventDestruct();
        new("/lib/shell")->eventMove(ob);
    }
    if(rounds <= 0) loaded=0;
    ob = 0;
    if(objectp(str)) ob = str;
    else obs = filter(get_livings(environment(this_player())),
            (: answers_to($(str), $1) :));
    if(!ob && !sizeof(obs)) ob = present(str,environment(this_player()));
    else if(!ob) ob = obs[0];
    if(creatorp(ob)){
        write(ob->GetName()+" catches your bullet in "+possessive(ob)+" teeth.\n");
        say(ob->GetName()+" catches the bullet in "+possessive(ob)+" teeth.\n");
        autohit=0;
        return 1;
    }
    if(ob && !living(ob) && base_name(ob) != LIB_CORPSE){
        tell_room(environment(environment(this_object())),
                "The bullet smashes into "+lower_case(ob->GetShort())+"!\n");
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
            return 1;
        }
        tempclass=ob->GetClass();
        if(tempclass) ob->SetClass(tempclass/2);
        return 1;
    }
    i = random(100);
    if(!creatorp(environment())){
        dex = environment(this_object())->GetStatLevel("coordination");
        dex += (environment(this_object())->GetStatLevel("luck") / 3);
        if(environment(this_object())->GetSkillLevel("firearms") < 10){
            int now = time();
            if(last_shot == now) dex -= 100;
            else if((now - last_shot) < 2)  dex -= 50;
        }
    }
    else dex = 200;
    last_shot = time();
    if((ob && living(ob)) && (i < dex || autohit==1)){
        if(random(2)) limbname = ob->GetTorso();
        else limbname = scramble_array(ob->GetLimbs())[0];
        tell_room(environment(environment(this_object())),"The bullet smashes into "+
                capitalize(str)+"'s "+limbname+"!\n",ob);
        tell_object(ob,"The bullet smashes into your "+limbname+"!\n");
        ob->AddLead("gunshot_wounds", 1);
        ob->SetAttack(this_agent());
        if(!present("firearms_wound",ob)){
            new(LIB_WOUND)->eventMove(ob);
        }
        if(Caliber){
            float tmp;
            int extradam = this_object()->GetMagnum();
            if(to_float(Caliber) < 1.00) Caliber = to_float(Caliber) * 100.00;
            if(Caliber > 99) tmp = to_float(Caliber) * 0.10;
            dam = to_int(tmp);
            if(extradam) dam += (random(extradam/2) + extradam/2);
        }
        if(Millimeter){
            dam = Millimeter;
            dam *= random(10);
        }
        if(!dam) dam = 7;

        dam += random(environment(this_object())->GetStatLevel("coordination"));
        dam += environment(this_object())->GetSkillLevel("projectile attack");
        dam -= random(ob->GetStatLevel("luck"));
        dam -= random(ob->GetSkillLevel("projectile defense"));

        if(creatorp(this_player())) write("you do "+dam+" points of damage");

        ob->eventReceiveDamage(environment(this_object()),(PIERCE), dam, 0, limbname);
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
        string shortd = (inv[i]->GetShort() || "");
        arr=explode(inv[i]->GetKeyName(),"");
        if(sizeof(arr) && arr[sizeof(arr)-1] =="s"){
            tell_room(environment(environment(this_object())), 
                    capitalize(shortd)+" are struck by the stray bullet.");
        }
        else
            tell_room(environment(environment(this_object())),
                    capitalize(shortd)+" is struck by the stray bullet.");
    }
    autohit=1;
    rounds++;
    this_object()->eventFire(inv[i]->GetKeyName());
    if(this_object()->GetFirearmType() == "revolver"){
        shells--;
        present("shell",present("cylinder",this_object()))->eventDestruct();
        ob=new("/lib/round");
        ob->SetCaliber(this_object()->GetCaliber());
        ob->SetMillimeter(this_object()->GetMillimeter());
        ob->SetFirearmType(this_object()->GetFirearmType());
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
int ShowRounds(){ environment(this_object())->eventPrint("Firearm has: "+rounds+" rounds.\n");
    environment(this_object())->eventPrint("Firearm has: "+shells+" shells.\n");
    return 1;
}
int SetAmmo(int i){ rounds=i; return 1; }
int GetAmmo(){ return rounds; }
int GetMag(){ return mag; }
int SetMag(int i){ mag=i; return 1; }
int SetLoaded(int i){ loaded=i; return 1; }
int GetLive(){ return loaded; }
int SetAmmoFile(string str){ AmmoFile=str; return 1; }
int SetAmmoType(string str){ AmmoType=str; return 1; }
int SetFirearmType(string str){ FirearmType=str; return 1; }
int SetMaxAmmo(int x){ MaxAmmo=x; return 1; }
int SetCaliber(int x){ Caliber=x; return 1; }
int SetMillimeter(int x){ Millimeter=x; return 1; }
string GetAmmoType(){ return AmmoType; }
string GetFirearmType(){ return FirearmType; }
int GetMaxAmmo(){ return MaxAmmo; }
int GetCaliber(){ return Caliber; }
int GetMillimeter(){ return Millimeter; }

mixed eventLoad(object ob){
    mixed can = CanLoad(this_player());
    if(!can || !intp(can)) return 0;
    if(!ob || !objectp(ob) || (objectp(ob) && living(ob))){
        write("Load "+GetKeyName()+" with what?");
        return 1;
    }
    if(GetFirearmType() == "revolver" && ob && objectp(ob) &&
            ob->GetFirearmType() == "revolver" && (base_name(ob) == LIB_ROUND ||
                inherits(LIB_ROUND,ob)) && ob->GetCaliber() == GetCaliber() &&
            rounds != MaxAmmo){
        rounds++;
    }
    return 1;
}

mixed eventUnload(mixed what){
    string n1,s1,s2;
    int n2;
    mixed can = CanUnload(this_player());
    if(!can || !intp(can)) return 0;

    if(FirearmType=="auto" && !mag){
        write("It's already unloaded.");
        return 1;
    }
    if(mag==1 && environment(this_object()) == this_player()){
        this_object()->doMagUnload();
        return 1;
    }
    if(FirearmType=="revolver" && 
            environment(this_object()) == this_player()){
        if(rounds == 0 && shells == 0){
            write("Your "+GetFirearmName()+" is already empty.");
            return 1;
        }
        if(intp(what) && what < 7 && what > 0) true();
        else what = "all";
        write("You unload your "+GetFirearmName()+".");
        this_object()->doRevolverUnload("all",what);
        return 1;
    }
}

int doMagUnload(){
    if(!present("clip",this_object())){
        write("The "+GetFirearmName()+" is already empty.");
        return 0;
    }
    write("You unload an ammo clip from your "+GetFirearmName()+".");
    say(this_player()->GetName()+" unloads an ammo clip from "+possessive(this_player())+" "+GetFirearmName()+".");
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
        write("You unload "+cardinal(n2)+" "+things+" from your "+GetFirearmName()+".");
    }
    if(n1 > 0){
        string things = "shells";
        if(n1 == 1) things = "shell";
        write("You unload "+cardinal(n1)+" "+things+" from your "+GetFirearmName()+".");
    }
    say(environment(this_object())->GetName()+" unloads some cartridges from "+
            possessive(environment(this_object()))+" revolver.");
    return 1;
}

int GetMaxLoaded(){
    if(rounds + shells >= MaxAmmo) return 1;
    return 0;
}

int GetMagnum(){
    return magnum;
}

int SetMagnum(int i){
    if(i > 0) magnum = i;
    else magnum = 0;
    return magnum;
}

