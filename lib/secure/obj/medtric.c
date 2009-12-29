#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void analyze(string butt);
object scanner,tricorder,person;
object *gstuff,*ngstuff;
mixed *statlist,*skilllist;
mapping this_stat,this_skill;
int allowed,statpoints,statlevel,statclass,tempint,i;
int skilllevel,skillclass,skillpoints;
string name;
string *stumps;
string warning = "We remind you that the tricorder cannot speak. In the event "+
"that it does speak, you are instructed to disregard its advice.";

void create(){
    item::create();
    SetKeyName("creator tricorder");
    SetId(({"tricorder","scanner","device","tool","medtric","tric"}));
    SetAdjectives(({"electronic","admin","diagnostic"}));
    SetShort("a medical tricorder");
    SetLong("This is a palm-sized electronic device designed to run various "+
            "medical tests and provide treatment of disease and injury. "+
            "Written in bold red letters across its face are the words: \"%^BOLD%^RED%^FOR AUTHORIZED TEST CHARACTER "+
            "USE ONLY%^RESET%^\".\n%^YELLOW%^NOTE: Some functions do not work "+
            "with drones. This is due to safeguards in the master object security "+
            "model, and it will not be \"fixed\".%^RESET%^\n"+
            "There is a warning on the tricorder you can read.\n"+
            "\n Labeled buttons on this device read:\n"+
            //"* medscan: scan for disease, poison, etc\t%^RED%^offline%^RESET%^\n"+
            "* fscan: readout of all files inherited\t\t%^GREEN%^ONLINE%^RESET%^\n"+
            //"* enshadow: move a shadow to an object\t\t%^RED%^offline%^RESET%^\n"+
            "* deshadow: remove all shadows from an object\t%^GREEN%^ONLINE%^RESET%^\n"+
            //"* inject: insert substance into patient\t\t%^RED%^offline%^RESET%^\n"+
            "* extract: remove a foreign body or substance\t%^GREEN%^ONLINE%^RESET%^ (slugs only)\n"+
            "* modhealth: raise or lower health level\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* modcaff: raise or lower caffeine level\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* modalc: raise or lower alcohol level\t\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* modfood: raise or lower food level\t\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* moddrink: raise or lower drink level\t\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* modmag: raise or lower magic level\t\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* modstam: raise or lower stamina\t\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* modtox: raise or lower poison\t\t\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* setskill: set skill level\t\t\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* setstat: set stat level\t\t\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* amputate: remove limb\t\t\t\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* regenerate: restore all amputated limbs\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* cure: eliminate known diseases\t\t%^GREEN%^ONLINE%^RESET%^\n"+
            "* infect: introduce disease into patient\t%^GREEN%^ONLINE%^RESET%^\n"+
            //"* inoculate: raise immunity to a disease\t%^RED%^offline%^RESET%^\n"+
            //"* posture: change your own posture.\t\t%^RED%^offline%^RESET%^\n"+
            "");
    SetRead( "default" , warning);
    SetRead( "warning", warning );
    SetProperties(([
                "no steal" : 1,
                ]));
    SetMass(12);
    SetVendorType(VT_TREASURE);
}

void init(){
    ::init();
    add_action("fscan","fscan");
    add_action("assess","assess");
    add_action("assess","diagnose");
    add_action("extract","extract");
    add_action("addhp","addhp");
    add_action("addhp","modhealth");
    add_action("modcaff","modcaff");
    add_action("modalc","modalc");
    add_action("modfood","modfood");
    add_action("moddrink","moddrink");
    add_action("modmag","modmag");
    add_action("modstam","modstam");
    add_action("modtox","modtox");
    add_action("setstat","setstat");
    add_action("setskill","setskill");
    add_action("regenerate","regenerate");
    add_action("amputate","amputate");
    add_action("infect","infect");
    add_action("germ_scan","germscan");
    add_action("germ_squash","cure");
    add_action("medscan","medscan");
    add_action("posture","posture");
    add_action("enshadow","enshadow");
    add_action("deshadow","deshadow");
    add_action("TestFun","HEINEKEN");
}

int TestFun(){
    write("WOOHOO");
    return 1;
}

varargs int preAction(int restricted){
    scanner=this_player();
    tricorder=this_object();

    if(!this_player()) return 0;

    if(!present(tricorder, scanner)){
        write("You are not holding the tricorder.");
        return 2;
    }

    if(restricted && !creatorp(this_player())){
        write("This function is not permitted to builders.");
        return 2;
    }
    if(!builderp(this_player())){
        write("Your puny mortal mind can't wrap itself around the use "
                "of this powerful instrument.");
        log_file("adm/tricorder",capitalize(this_player()->GetKeyName())+
                " attempted to use the medical tricorder: "+timestamp()+"\n");
        tell_creators("SECURITY: "+capitalize(this_player()->GetKeyName())+
                " attempted to use the medical tricorder.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 2;
    }

    say(scanner->GetName()+" operates a medical tricorder.",scanner);
    write("The tricorder makes a low, warbling sound.");
    return 1;
}

int posture(int i){
    allowed=preAction();
    if(allowed == 2) return 1;
    if(!i) {
        write("Please include the position number desired.");
        return 0;
    }
    this_player()->SetPosition(i);
    return i;
}

int deshadow(string str){
    object target;
    allowed=preAction(1);
    if(allowed == 2) return 1;
    if(str == "me") str = this_player()->GetKeyName();
    if(!target = present(str,this_player())){
        target = present(str,environment(this_player()));
    }
    if(!target){
        write("That's not here.");
        return 1;
    }
    else {
        if(shadow(target,0)){
            remove_shadow(target);
            write("You deshadow "+str);
        }
        else write("No shadows there.");
    }
    return 1;
}

int enshadow(string str){
    object target;
    string s1,s2,s3,file;
    allowed=preAction(1);
    if(allowed == 2) return 1;
    if(sscanf(str,"%sshadow/%s %s",s1,s2,s3) > 0) file = s2;
    else if(sscanf(str,"%sshadow/%s.c %s",s1,s2,s3) > 0) file = s2;
    else if(sscanf(str,"%s %s",s1,s2) > 0) file = s1;
    else {
        write("You must specify a file within /shadows and a target.");
        return 1;
    }
    if( s3 && s3 != "") target = present(s3,environment(this_player()));
    else if(s3 == "me") target = this_player();
    else target = present(s2,environment(this_player()));

    if(!target) {
        write("Please specify a file within /shadows and a target.");
        return 1;
    }

    if(file_size("/shadow/"+file) < 1){
        write("That is not a valid shadow.");
        return 1;
    }

    new("/shadow/"+file)->eventShadow(target);
    write("You enshadow "+target->GetName()+" with /shadow/"+file);
    return 1;
}

int medscan(string str){
    allowed=preAction();
    if(allowed == 2) return 1;
    if(!str || str == "") {
        write("Please rephrase, including your target this time.");
        return 1;
    }
    if(str == "me") str = this_player()->GetKeyName();
    person=present(str,environment(scanner));
    if(!person  || !living(person)){
        write("Disease check: No such being found");
        return 1;
    }
    write("Scanning for germs...");
    this_object()->germ_scan(str);
    write("Germ scan complete.\n");
    write("Wound scan currently offline.");
    return 1;
}

int germ_scan(mixed strob ){
    int i;
    string a,b,c,str;
    if(stringp(strob)) {
        str = strob;
        if(str == "me") str = this_player()->GetKeyName();
        person=present(str,environment(scanner));
        if(!person  || !living(person)){
            write("This being was not found.");
            return 2;
        }
    }
    if(objectp(strob)){
        person = strob;
        str = person->GetName();
    }
    if(!str || str == "") {
        write("Please rephrase, including your target this time.");
        return 1;
    }
    if(!living(person)){
        write("This thing is not alive.");
        return 2;
    }
    ngstuff=all_inventory(person);
    gstuff = ({});
    say(scanner->GetName()+" waves a tricorder at "+person->GetName()+".",({scanner,person}) );
    tell_object(person,scanner->GetName()+" waves a tricorder at you.");

    for(i=0;i<sizeof(ngstuff);i++){
        if(ngstuff[i]->isGerm()== 1 && !sizeof(gstuff)) gstuff = ({ngstuff[i]});
        if(ngstuff[i]->isGerm()== 1 && sizeof(gstuff) > 0 ) gstuff += ({ngstuff[i]});
    }
    if(!sizeof(gstuff)){
        write(person->GetName()+" has no detectable diseases.");
        return 2;
    }
    for(i=0;i<sizeof(gstuff);i++){
        if(!a=gstuff[i]->GetKeyName() || a == ""  ) a = "generic disease";
        if(!b=gstuff[i]->GetType() || b == ""  ) b = "unknown type";
        if(intp(gstuff[i]->GetCure())) c=gstuff[i]->GetCure();
        if(!c) c = 0;
        write("Disease: "+a+", type: "+b+", severity: "+c);
        return 1;
    }
    write("Please specify a living being that shares your current location.");
    return 2;
}

int germ_squash(string str){
    int i,bar,c;
    object array whom;
    allowed=preAction(1);
    if(allowed == 2) return 1;
    if(!str || str == "") {
        write("Please specify a living thing, or \"all\" for everything in "+
                "your environment.");
        return 1;
    }
    if(str == "me") str = this_player()->GetKeyName();
    person=present(str,environment(scanner));
    whom = ({});
    if(!person && str !="all"){
        write("Disease check: No such being found");
        return 1;
    }

    if( str !="all" && !living(person)){
        write("Disease check: No such living being found");
        return 1;
    }
    if(str =="all") whom = get_livings(environment(this_player()));
    if(str !="all") whom = ({person});
    foreach(object ingrate in whom){
        i=germ_scan(ingrate);
        if(i!=2 && intp(gstuff[i]->GetCure()) ) bar=gstuff[i]->GetCure();
        if(i!=2 && !intp(gstuff[i]->GetCure()) ) write("GetCureType: "+typeof(gstuff[i]->GetCure()));
        if(!i) { write("Extremely weird error here."); return 1; }
        for(i=0;i<sizeof(gstuff);i++){
            if(gstuff[i] && gstuff[i]->isGerm() ){
                c=10;
                gstuff[i]->SetCure(c);
                if(intp(gstuff[i]->GetCure()) ){
                    gstuff[i]->eventCure(ingrate,c,gstuff[i]->GetType());
                    write("You cure "+ingrate->GetName()+"'s disease.");
                    say(ingrate->GetName()+" looks healthier.",ingrate);
                    tell_object(ingrate,"You feel less sick.");
                }
                if(gstuff[i] && gstuff[i]->isGerm() && !intp(gstuff[i]->GetCure())){
                    write("This disease uses a functional as its SetCure condition.");
                    write("It will not be cured by this device. You will have to "+
                            "manually review the germ's code to determine what will cure it.");
                    write("SetCure() data type: "+typeof(gstuff[i]->GetCure()));
                    write("Filename: "+base_name(gstuff[i])+"\n");
                }
            }	
        }
        gstuff=({});
    }
    return 1;
}

int infect(string str){
    string whom,what,disease;
    object ob;
    mixed foo;
    allowed=preAction(1);
    if(allowed == 2) return 1;
    if(!str){
        write("Please indicate whom to infect, and with what. Example:");
        write("infect doofus cold");
        write("Current valid diseases: cold, flu, fleas, lice, h1n1");
        return 1;
    }
    if(sscanf(str,"%s %s",whom,what)) { 
        if(!whom || !what || whom == "" || what == ""){
            write("Please indicate whom to infect, and with what. Example:");
            write("infect doofus cold");
            write("Current valid diseases: cold, flu, fleas, lice");
            return 1;
        }
    }
    if(!whom || !what || whom == "" || what == ""){
        write("Please indicate whom to infect, and with what. Example:");
        write("infect doofus cold");
        write("Current valid diseases: cold, flu, fleas, lice");
        return 1;
    }
    if(whom == "me") whom = this_player()->GetKeyName();
    person=present(whom,environment(scanner));
    if(!person  || !living(person)){
        write("This being was not found.");
        return 1;
    }
    if(what == "cold") disease  = "/domains/town/obj/cold";
    else if(what == "flu") disease  = "/domains/town/obj/flu";
    else if(what == "fleas") disease  = "/domains/town/obj/fleas";
    else if(what == "lice") disease  = "/domains/town/obj/lice";
    else if(what == "rage") disease  = "/domains/town/obj/rage";
    else if(what == "h1n1") disease  = "/domains/town/obj/h1n1";
    else if(what != "") {
        write("That isn't a valid disease.");
        return 1;
    }
    if(person){	
        write(person->GetName()+" located. Infecting...");
        say(scanner->GetName()+" waves a tricorder at "+person->GetName()+".",person);
        if(present(whom,environment(scanner))) {
            tell_object(person,scanner->GetName()+" waves a tricorder at you.");
        }
    }
    ob=new(disease);
    if(ob) foo = ob->eventInfect(person);
    else write("There's a problem with that parasite.");
    return 1;
}

int amputate(string str){
    string whom,which,limb,both;
    allowed=preAction();
    if(allowed == 2) return 1;
    if(!str){
        write("Please indicate whose limbs you wish to amputate, and which. Example:");
        write("amputate joey left hand");
        return 1;
    }
    if(sscanf(str,"%s %s %s",whom,which,limb)) { 
        if(!limb) sscanf(str,"%s %s",whom,limb);
    }
    if(whom && whom == "me") whom = this_player()->GetKeyName();
    if(!whom || !limb){
        write("Please indicate whose limbs you wish to amputate, and which. Example:");
        write("amputate joey left hand");
        return 1;
    }
    if(!person=present(whom,environment(scanner))){
        write(capitalize(whom)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    both = limb;
    if(which && which !="") both = which+" "+limb;
    //write("both: "+both);

    if(!person->RemoveLimb(both, scanner)){
        write("You wave your tricorder menacingly, but nothing happens.");
        tell_room(environment(person),scanner->GetName()+" waves a tricorder at "+person->GetName()+" in a threatening manner.", ({ scanner,person }) );
        tell_object(person,scanner->GetName()+" waves "+possessive(scanner)+" tricorder menacingly at you.");
        return 1;
    }

    if(both == "head"){
        tell_room(environment(person),scanner->GetName()+" waves a tricorder at "+person->GetName()+", and "+
                person->GetName()+"'s head falls off!", ({ scanner,person }) );
        write("You decapitate "+person->GetName()+".");
        tell_object(person,scanner->GetName()+" decapitates you with a wave of "+possessive(scanner)+" tricorder!");
        //person->RemoveLimb(both, scanner);
        return 1;
    }
    stumps=person->GetLimbs();
    if(!stumps) { 
        write(person->GetName()+" hasn't any limbs."); 
        say(scanner->GetName()+" waves a tricorder at "+person->GetName()+".",scanner,person);
        tell_object(person,scanner->GetName()+" waves a tricorder at you.");
        return 1;
    }
    if(member_array(both,stumps) != -1){
        person->RemoveLimb(both, scanner);
        write("You amputate "+person->GetName()+"'s "+both); 
        say(scanner->GetName()+" has amputated "+person->GetName()+"'s "+both+" "+
                "with an energy beam from a medical tricorder.",scanner,person);
        tell_object(person,scanner->GetName()+" waves a tricorder at you and "+
                "your "+both+" is severed.");
        return 1;
    }
    if(member_array(limb,stumps)!= -1){
        //person->RemoveLimb(limb, scanner);
        write("You amputate "+person->GetName()+"'s "+limb+"."); 
        say(scanner->GetName()+" has amputated "+person->GetName()+"'s "+both+" "+
                "with an energy beam from a medical tricorder.",scanner,person);
        tell_object(person,scanner->GetName()+" waves a tricorder at you and"+
                "your "+limb+" is severed.");
        return 1;
    }
    //write("There is some sort of problem, it looks like. No "+
    //"limbs are severed.");
    return 1;
}

int regenerate(string str){
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str){
        write("Please indicate whose limbs you wish to regenerate.");
        return 1;
    }
    if(str == "me") str = this_player()->GetKeyName();
    if(!person=present(str,environment(scanner))){
        write(capitalize(str)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    stumps=person->GetMissingLimbs();
    if(!stumps) { 
        write(person->GetName()+" isn't missing any limbs."); 
        say(scanner->GetName()+" waves a tricorder at "+person->GetName()+".",scanner,person);
        tell_object(person,scanner->GetName()+" waves a tricorder at you.");
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    for(i=0;i<sizeof(stumps);i++){
        person->RestoreLimb(stumps[i]);
        write("You regenerate "+person->GetName()+"'s "+stumps[i]+".");
        say(scanner->GetName()+" regenerates "+person->GetName()+"'s "+stumps[i]+".",scanner,person);
        tell_object(person,scanner->GetName()+" regenerates your "+stumps[i]+".");
    }
    return 1;
}

int DoSkillChange(string str, int i){
    int more_or_less;
    if(str == "me") str = this_player()->GetKeyName();
    this_skill = person->GetSkill(str);
    if(this_skill["level"] < i) more_or_less = 1;
    skillclass=this_skill["class"];
    person->SetSkill(str,i,skillclass);
    say(scanner->GetName()+" waves a medical tricorder at "+person->GetName()+".",({person,scanner}));
    tell_object(person,scanner->GetName()+" waves a medical tricorder at you.");	  
    if(more_or_less == 1) tell_room(environment(person), person->GetName()+" looks somehow more experienced.",({person}) );
    if(more_or_less != 1) tell_room(environment(person), person->GetName()+" looks somehow less experienced.",({person}) );
    write("You've set "+person->GetName()+"'s "+str+" to level "+i+".");
    return 1;
}
int DoAllSkills(int foo){
    int i;
    for(i=0;i<sizeof(skilllist);i++){
        this_skill = person->GetSkill(skilllist[i]);
        skillclass=this_skill["class"];
        person->SetSkill(skilllist[i],foo,skillclass);
        write("You've set "+person->GetName()+"'s "+skilllist[i]+" to level "+foo+".");
    }
    say(scanner->GetName()+" waves a medical tricorder at "+person->GetName()+".",({person,scanner}));
    say(person->GetName()+" seems to undergo an almost undetectable, subtle transformation.",({person,scanner}));
    tell_object(person,scanner->GetName()+" waves a medical tricorder at you.");	
    return 1;
}
int setskill(string str){
    string whom,skill;
    string skill1,skill2,rubbish;
    int amt;
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str) { 
        write("Syntax: setskill <person> <skill> <amount>");
        write("If there is a space in the skill name, replace it with "+
                "an underscore. Example:\nsetskill schmucky blade_attack 5");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!sscanf(str,"%s %s %d",whom,skill,amt)) { sscanf(str,"%s %s",whom,skill);  }
    if(sscanf(str,"%s %s_%s ",rubbish,skill1,skill2) && skill1 && skill2){
        whom = rubbish;
        skill = skill1 + " " +skill2;
    }
    write("skill: "+skill);
    if(!whom || whom == ""){
        write("Syntax: setskill <person> <skill> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(whom == "me") whom = this_player()->GetKeyName();
    if(!person=present(whom,environment(scanner))){
        write(capitalize(whom)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    this_skill = ([]);
    skilllist = ({});
    skilllist = person->GetSkills();
    if(amt < 0) amt = 0;
    if(allowed==2) return 1;
    if ( !whom || !skill || !amt || !intp(amt) ){
        write("Syntax: setskill <person> <skill> <amount>");
        write("If there is a space in the skill name, replace it with "+
                "an underscore. Example:\nsetskill schmucky blade_attack 5");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(skill == "all") { DoAllSkills(amt); return 1; }
    if(member_array(skill,skilllist) == -1) {
        write(capitalize(whom)+" doesn't have that skill.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    DoSkillChange(skill, amt);
    return 1;
}
int DoStatChange(string str, int i){
    if(str == "me") str = this_player()->GetKeyName();
    this_stat = person->GetStat(str);
    statclass=this_stat["class"];
    person->SetStat(str,i,statclass);
    say(scanner->GetName()+" waves a medical tricorder at "+person->GetName()+".",({person,scanner}));
    tell_object(person,scanner->GetName()+" waves a medical tricorder at you.");	  
    write("You've set "+person->GetName()+"'s "+str+" to level "+i+".");
    return 1;
}
int DoAllStats(int foo){
    int i;
    for(i=0;i<sizeof(statlist);i++){
        this_stat = person->GetStat(statlist[i]);
        statclass=this_stat["class"];
        person->SetStat(statlist[i],foo,statclass);
        write("You've set "+person->GetName()+"'s "+statlist[i]+" to level "+foo+".");
    }
    say(scanner->GetName()+" waves a medical tricorder at "+person->GetName()+".",({person,scanner}));
    tell_object(person,scanner->GetName()+" waves a medical tricorder at you.");	
    return 1;
}
int setstat(string str){
    string whom,stat;
    int amt;
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str) { 
        write("Syntax: setstat <person> <stat> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!sscanf(str,"%s %s %d",whom,stat,amt)) { sscanf(str,"%s %s",whom,stat);  }
    if(!whom || whom == ""){
        write("Syntax: setstat <person> <stat> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(whom == "me") whom = this_player()->GetKeyName();
    if(!person=present(whom,environment(scanner))){
        write(capitalize(whom)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    this_stat = ([]);
    statlist = ({});
    statlist = person->GetStats();
    if(amt < 0) amt = 0;
    if(allowed==2) return 1;
    if ( !whom || !stat || !amt || !intp(amt) ){
        write("Syntax: setstat <person> <stat> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(stat == "all") { DoAllStats(amt); return 1; }
    if(member_array(stat,statlist) == -1) {
        write(capitalize(whom)+" doesn't have that stat.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    DoStatChange(stat, amt);
    return 1;
}
int modstam(string str, int stamina){
    string whom;
    int tempint,staminalevel;
    object person;
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str){
        write("Syntax: modstam <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    sscanf(str,"%s %d",whom,stamina);
    if (!whom || !stamina){
        write("Syntax: modstam <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(whom == "me") whom = this_player()->GetKeyName();
    if(!intp(stamina)){
        write("Syntax: modstam <person> <amount>");
        write("Where <amount> is an integer. For example:\nmodstam joe 3\nmodstam mike -10\n");
        say(scanner->GetName()+" fumbles stupidly with a medical tricorder.",scanner);
        return 1;
    }
    person=present(whom,environment(scanner));
    if(!person){
        write(capitalize(whom)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    staminalevel=person->GetStaminaPoints();
    tempint = staminalevel + stamina;
    if(tempint < 0) {
        stamina = stamina + absolute_value(tempint);
    }
    person->AddStaminaPoints(stamina);
    if(stamina > 0){
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks a bit more at ease.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel stronger.");
        write("You've raised "+capitalize(whom)+"'s stamina level by "+stamina+" points.");
        return 1;
    }
    if(stamina < 0){
        stamina=absolute_value(stamina);
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks uncomfortable.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel weaker.");
        write("You've lowered "+capitalize(whom)+"'s stamina level by "+stamina+" points.");
        return 1;
    }
    write("Aucun effet.");
    return 1;
}
int modtox(string str, int poison){
    string whom;
    int tempint,poisonlevel;
    object person;
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str){
        write("Syntax: modtox <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    sscanf(str,"%s %d",whom,poison);
    if (!whom || !poison){
        write("Syntax: modtox <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(whom == "me") whom = this_player()->GetKeyName();
    if(!intp(poison)){
        write("Syntax: modtox <person> <amount>");
        write("Where <amount> is an integer. For example:\nmodtox joe 3\nmodtox mike -10\n");
        say(scanner->GetName()+" fumbles stupidly with a medical tricorder.",scanner);
        return 1;
    }
    person=present(whom,environment(scanner));
    if(!person){
        write(capitalize(whom)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    poisonlevel=person->GetPoison();
    tempint = poisonlevel + poison;
    if(tempint < 0) {
        poison = poison + absolute_value(tempint);
    }
    person->AddPoison(poison);
    if(poison > 0){
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks paler and shaky.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel ill.");
        write("You've raised "+capitalize(whom)+"'s poison level by "+poison+" points.");
        return 1;
    }
    if(poison < 0){
        poison=absolute_value(poison);
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks less ill.", ({person,scanner})
           );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel better than before.");
        write("You've lowered "+capitalize(whom)+"'s poison level by "+poison+" points.");
        return 1;
    }
    write("Aucun effet.");
    return 1;
}
int modmag(string str, int magic){
    string whom;
    int tempint,magiclevel;
    object person;
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str){
        write("Syntax: modmagic <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    sscanf(str,"%s %d",whom,magic);
    if (!whom || !magic){
        write("Syntax: modmagic <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(whom == "me") whom = this_player()->GetKeyName();

    if(!intp(magic)){
        write("Syntax: modmagic <person> <amount>");
        write("Where <amount> is an integer. For example:\nmodmagic joe 3\nmodmagic mike -10\n");
        say(scanner->GetName()+" fumbles stupidly with a medical tricorder.",scanner);
        return 1;
    }
    person=present(whom,environment(scanner));
    if(!person){
        write(capitalize(whom)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    magiclevel=person->GetMagicPoints();
    tempint = magiclevel + magic;
    if(tempint < 0) {
        magic = magic + absolute_value(tempint);
    }
    person->AddMagicPoints(magic);
    if(magic > 0){
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" glows briefly.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel more magical.");
        write("You've raised "+capitalize(whom)+"'s magic level by "+magic+" points.");
        return 1;
    }
    if(magic < 0){
        magic=absolute_value(magic);
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks somehow more plain.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel less magical.");
        write("You've lowered "+capitalize(whom)+"'s magic level by "+magic+" points.");
        return 1;
    }
    write("Aucun effet.");
    return 1;
}
int moddrink(string str, int drink){
    string whom;
    int tempint,drinklevel;
    object person;
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str){
        write("Syntax: moddrink <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    sscanf(str,"%s %d",whom,drink);
    if (!whom || !drink){
        write("Syntax: moddrink <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(whom == "me") whom = this_player()->GetKeyName();

    if(!intp(drink)){
        write("Syntax: moddrink <person> <amount>");
        write("Where <amount> is an integer. For example:\nmoddrink joe 3\nmoddrink mike -10\n");
        say(scanner->GetName()+" fumbles stupidly with a medical tricorder.",scanner);
        return 1;
    }
    person=present(whom,environment(scanner));
    if(!person){
        write(capitalize(whom)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    drinklevel=person->GetDrink();
    tempint = drinklevel + drink;
    if(tempint < 0) {
        drink = drink + absolute_value(tempint);
    }
    person->AddDrink(drink);
    if(drink > 0){
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks a bit more at ease.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel more hydrated.");
        write("You've raised "+capitalize(whom)+"'s drink level by "+drink+" points.");
        return 1;
    }
    if(drink < 0){
        drink=absolute_value(drink);
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks uncomfortable.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel thirstier.");
        write("You've lowered "+capitalize(whom)+"'s drink level by "+drink+" points.");
        return 1;
    }
    write("Aucun effet.");
    return 1;
}
int modfood(string str, int food){
    string whom;
    int tempint,foodlevel;
    object person;
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str){
        write("Syntax: modfood <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    sscanf(str,"%s %d",whom,food);
    if (!whom || !food){
        write("Syntax: modfood <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(whom == "me") whom = this_player()->GetKeyName();

    if(!intp(food)){
        write("Syntax: modfood <person> <amount>");
        write("Where <amount> is an integer. For example:\nmodfood joe 3\nmodfood mike -10\n");
        say(scanner->GetName()+" fumbles stupidly with a medical tricorder.",scanner);
        return 1;
    }
    person=present(whom,environment(scanner));
    if(!person){
        write(capitalize(whom)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    foodlevel=person->GetFood();
    tempint = foodlevel + food;
    if(tempint < 0) {
        food = food + absolute_value(tempint);
    }
    person->AddFood(food);
    if(food > 0){
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks a bit more at ease.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel more nourished.");
        write("You've raised "+capitalize(whom)+"'s food level by "+food+" points.");
        return 1;
    }
    if(food < 0){
        food=absolute_value(food);
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks uncomfortable.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel hungrier.");
        write("You've lowered "+capitalize(whom)+"'s food level by "+food+" points.");
        return 1;
    }
    write("Aucun effet.");
    return 1;
}
int modcaff(string str, int caff){
    string whom;
    int tempint,cafflevel;
    object person;
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str){
        write("Syntax: modcaff <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    sscanf(str,"%s %d",whom,caff);
    if (!whom || !caff){
        write("Syntax: modcaff <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(whom == "me") whom = this_player()->GetKeyName();

    if(!intp(caff)){
        write("Syntax: modcaff <person> <amount>");
        write("Where <amount> is an integer. For example:\nmodcaff joe 3\nmodcaff mike -10\n");
        say(scanner->GetName()+" fumbles stupidly with a medical tricorder.",scanner);
        return 1;
    }
    person=present(whom,environment(scanner));
    if(!person){
        write(capitalize(whom)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    cafflevel=person->GetCaffeine();
    tempint = cafflevel + caff;
    if(tempint < 0) {
        caff = caff + absolute_value(tempint);
    }
    person->AddCaffeine(caff);
    if(caff > 0){
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks a bit keener.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel tipsy.");
        write("You've raised "+capitalize(whom)+"'s caffeine level by "+caff+" points.");
        return 1;
    }
    if(caff < 0){
        caff=absolute_value(caff);
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks more focused and alert.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel more awake.");
        write("You've lowered "+capitalize(whom)+"'s caffeine level by "+caff+" points.");
        return 1;
    }
    write("Aucun effet.");
    return 1;
}
int modalc(string str, int alc){
    string whom;
    int tempint,alclevel;
    object person;
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str){
        write("Syntax: modalc <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    sscanf(str,"%s %d",whom,alc);
    if (!whom || !alc){
        write("Syntax: modalc <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(whom == "me") whom = this_player()->GetKeyName();

    if(!intp(alc)){
        write("Syntax: modalc <person> <amount>");
        write("Where <amount> is an integer. For example:\nmodalc joe 3\nmodalc mike -10\n");
        say(scanner->GetName()+" fumbles stupidly with a medical tricorder.",scanner);
        return 1;
    }

    person=present(whom,environment(scanner));
    if(!person){
        write(capitalize(whom)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    alclevel=person->GetAlcohol();
    tempint = alclevel + alc;
    if(tempint < 0) {
        alc = alc + absolute_value(tempint);
    }

    person->AddAlcohol(alc);
    if(alc > 0){
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks dizzier and more disoriented.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel tipsy.");
        write("You've raised "+capitalize(whom)+"'s alcohol level by "+alc+" points.");
        return 1;
    }
    if(alc < 0){
        alc=absolute_value(alc);
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks more focused and alert.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel more sober.");
        write("You've lowered "+capitalize(whom)+"'s alcohol level by "+alc+" points.");
        return 1;
    }
    write("Aucun effet.");
    return 1;
}
int addhp(string str, int hp){
    string whom;
    object person;
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str){
        write("Syntax: addhp <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    sscanf(str,"%s %d",whom,hp);
    if (!whom || !hp){
        write("Syntax: addhp <person> <amount>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(whom == "me") whom = this_player()->GetKeyName();

    if(!intp(hp)){
        write("Syntax: addhp <person> <amount>");
        write("Where <amount> is an integer. For example:\naddhp joe 3\naddhp mike -10\n");
        say(scanner->GetName()+" fumbles stupidly with a medical tricorder.",scanner);
        return 1;
    }
    person=present(whom,environment(scanner));
    if(!person){
        write(capitalize(whom)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    person->AddHP(hp);
    if(hp > 0){
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks stronger and more refreshed.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel stronger and more refreshed.");
        write("You've raised "+capitalize(whom)+"'s health by "+hp+" points.");
        return 1;
    }
    if(hp < 0){
        hp=absolute_value(hp);
        say(scanner->GetName()+" points a medical tricorder at "+capitalize(whom)+", "+
                "and "+nominative(person)+" looks weaker and duller.", ({person,scanner}) );
        tell_object(person,scanner->GetName()+" points a medical tricorder at you and you "+
                "suddenly feel weaker and more frail.");
        write("You've lowered "+capitalize(whom)+"'s health by "+hp+" points.");
        return 1;
    }
    write("Aucun effet.");
    return 1;
}
int extract(string str){
    string what,whom;
    object person, slug;
    int rifleshot_wounds,firearms_wounds,wounds;
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str){
        write("Extract what from whom?");
        write("Example: extract slug from <person>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    sscanf(str,"%s from %s",what,whom);
    if (!what || ! whom){
        write("Extract what from whom?");
        write("Example: extract slug from <person>");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(whom == "me") whom = this_player()->GetKeyName();
    person=present(whom,environment(scanner));
    if(!person){
        write(capitalize(whom)+" isn't here.");
        say(scanner->GetName()+" fumbles with a medical tricorder.",scanner);
        return 1;
    }
    if(!creatorp(this_player()) && strsrch(base_name(person), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    slug=present("firearms_wound",person);
    wounds=0;
    wounds=person->GetLead("gunshot_wounds");
    wounds+=person->GetLead("rifleshot_wounds");
    if(!slug && what=="slug"){
        write(capitalize(whom)+" has no gunshot injury.");
        say(scanner->GetName()+" looks a bit silly waving a medical tricorder at "+capitalize(whom)+".",({ scanner, person }) );
        tell_object(person,scanner->GetName()+" looks a bit silly waving a medical tricorder at you.");
        return 1;
    }
    if(slug && what=="slug" && wounds > 0 ){
        write("You extract a lead slug from "+capitalize(whom)+"'s body.");
        say(scanner->GetName()+" deftly extracts a lead slug from "+capitalize(whom)+" with a medical tricorder.",({ scanner, person }) );
        tell_object(person,scanner->GetName()+" deftly extracts a lead slug from you with a medical tricorder.");
        firearms_wounds=person->GetLead("firearms_wounds");
        rifleshot_wounds=person->GetLead("rifleshot_wounds");
        slug=new("/domains/town/obj/spent");
        if(person->GetLead("gunshot_wounds") > 0) {
            person->AddLead("gunshot_wounds", -1);
            slug->SetShort("a spent pistol slug");
            --wounds;
            slug->eventMove(environment(scanner));
            if(person->GetLead() < 1){
                slug=present("firearms_wound",person);
                if(slug) slug->eventDestruct();
                return 1;
            }
            return 1;
        }
        if(person->GetLead("rifleshot_wounds") > 0) {
            person->AddLead("rifleshot_wounds", -1);
            slug->SetShort("a spent rifle slug");
            --wounds;
            slug->eventMove(environment(scanner));
            if(person->GetLead() < 1){
                if(slug) slug->eventDestruct();
                slug=present("firearms_wound",person);
                if(slug) slug->eventDestruct();
                return 1;
            }
        }
        slug->eventMove(environment(scanner));
        slug=present("firearms_wound",person);
        if(wounds < 1 && slug){
            slug->eventDestruct();
            return 1;
        }
    }
    return 1;
}
int fscan(string str){
    object ob;
    object *stuffs;
    mixed *stuff;
    allowed=preAction();
    if(allowed==2) return 1;
    if(!str || str=="" || str=="here") {
        ob = environment(this_player());
        name = ob->GetShort();
        str = base_name(ob);
    }
    if(str == "me") str = this_player()->GetKeyName();
    if(present(str,this_player())) {
        ob = present(str,this_player());
        name = ob->GetName();
    }
    if(present(str,environment(this_player()))) {
        ob=present(str,environment(this_player()));
        name = ob->GetName();
    }
    if(!ob){
        write("The tricorder fails to locate any such person, object, or environment.\n");
        say(this_player()->GetName()+" scans around aimlessly with "+
                " a medical tricorder.\n");
        return 1;
    }
    write("You perform a file scan of "+name+".\n\n");
    write("\n"+name+" is in posession of the following items:");
    write("-------------------------------------------");
    say(this_player()->GetName()+" performs a tricorder scan of "+name+".\n",ob);
    tell_object(ob,this_player()->GetName()+" scans you with "+
            "a medical tricorder.\n");
    stuffs=deep_inventory(ob);
    filter(stuffs, (: this_object()->analyze(file_name($1)) :), this_object());
    filter(stuffs, (: this_object()->getname($1) :), this_object());
    stuff=deep_inherit_list(ob);
    write("\n"+name+" is composed of the following files:\n");
    write("-------------------------------------------");
    foreach(mixed gene in deep_inherit_list(ob)) {
        if(!stringp(gene)) break;
        if(!gene) gene="";
        write(gene);
    }
    write("\nSCAN COMPLETE");
    return 1;
}
void analyze(string str){
    string temp, temp2;
    sscanf(str,"%s#%s",temp,temp2);
    if(temp=="/lib/corpse"){
        write("Found a corpse. Absolute name:\n"+str);
        return;
    }
    return;
}
string getname(object ob){
    string temp, temp2;
    if(!ob) return "No object found.";
    else
        sscanf(file_name(ob),"%s#%s",temp,temp2);
    write(ob->GetName()+",\t\tfilename: "+temp+".c");
    return ob->GetName();
}
mixed assess(string args) {
    string *lines, *arr, *limbs;
    object ob;
    string tmp1, tmp2;
    int i, x, y, cols;
    if( args == "" || !args ) return 0;
    if(args == "me") args = this_player()->GetKeyName();
    if( !(ob = present(args, environment(this_player()))) )
        if( !(ob = find_player(convert_name(args))) &&
                !(ob = find_living(lower_case(args))) )
            return capitalize(args) + " is nowhere to be found.";
    if( creatorp(ob) ) return "You cannot get stat information on a creator.";
    if(!creatorp(this_player()) && strsrch(base_name(ob), homedir(this_player()))){
        write("Builders can only do this to NPC's that belong to them.");
        return 1;
    }
    if(tmp1 = ob->GetGender()) {
        cols = ((int *)this_player()->GetScreen())[0];
        tmp1 = ob->GetCapName() + " aka " + ob->GetShort() +
            ", level " + ob->GetLevel() + " " + ob->GetGender();
        if( !(tmp2 = ob->GetRace()) ) tmp2 = "blob";
        tmp1 += " " + tmp2;
        if( !(tmp2 = ob->GetClass()) ) tmp2 = "drifter";
        tmp1 += " " + capitalize(tmp2);
        if( tmp2 = ob->GetSpouse() )
            tmp1 += " (spouse: " + tmp2 + ")";
        lines = ({ center(tmp1, cols) });
        if( ob->GetUndead() ) tmp1 = "Undead";
        else tmp1 = "Alive";
        if( ob->GetSleeping() ) tmp1 += " / Sleeping";
        else tmp1 += " / Awake";
        if( ob->GetParalyzed() ) tmp1 += " / Paralyzed";
        lines += ({ center(tmp1, cols), "" });
        lines += ({ center("Health: " +ob->GetHealthPoints() + "/"+
                    ob->GetMaxHealthPoints() + "   Magic: " +
                    ob->GetMagicPoints() + "/" +
                    ob->GetMaxMagicPoints() + "   Stamina: " +
                    ob->GetStaminaPoints() + "/" +
                    to_int((float)ob->GetMaxStaminaPoints()) + "   Carry: " +
                    ob->GetCarriedMass() + "/" +
                    ob->GetMaxCarry(), cols) });
        lines += ({ center("Food: " + ob->GetFood() + "    " +
                    "Drink: " + ob->GetDrink() + "    " +
                    "Alcohol: " + ob->GetAlcohol() + "    " +
                    "Caffeine: " + ob->GetCaffeine() + "    " +
                    "Poison: " + ob->GetPoison() + "    ", cols) });
        lines += ({ center("Training Points: " + ob->GetTrainingPoints()) });
        lines += ({ "", "Limbs:" });
        limbs = ob->GetWieldingLimbs();
        arr = map(ob->GetLimbs(),
                (: sprintf("%:-14s%s (%d) %d/%d", $1,
                           ((member_array($1, $(limbs)) == -1) ? " " : "*"),
                           ($(ob))->GetLimbClass($1),
                           ($(ob))->GetHealthPoints($1),
                           ($(ob))->GetMaxHealthPoints($1)) :));
        i = sizeof(arr);
        while(i--) if( (y = strlen(arr[i])) > x ) x = y;
        x = cols/(x+2);
        lines += explode(format_page(arr, x), "\n") + ({ "", "Skills:" });
        arr = map(ob->GetSkills(),
                function(string skill, object who) {
                mapping mp = who->GetSkill(skill);
                int x;
                x = to_int(percent(mp["points"],
                        who->GetMaxSkillPoints(skill, mp["level"])));
                return sprintf("%:-18s (%d) %:2d%% - %d/%d",
                    skill, mp["class"], x,
                    who->GetSkillLevel(skill), mp["level"] );
                }, ob);
        i = sizeof(arr);
        while(i--) if( (y = strlen(arr[i])) > x ) x = y;
        x = cols/(x+2);
        lines += explode(format_page(arr, x), "\n") + ({ "", "Stats:" });
        arr = map(ob->GetStats(),
                (: sprintf("%:-12s (%d) %d/%d", $1,
                           ($(ob))->GetStatClass($1),
                           ($(ob))->GetStatLevel($1),
                           ($(ob))->GetBaseStatLevel($1)) :));
        i = sizeof(arr);
        x = 0;
        while(i--) if( (y = strlen(arr[i])) > x ) x = y;
        x =cols/(x+2);
        lines += explode(format_page(arr, x), "\n");
        lines += ({ "", ob->GetName()+" has amassed a net worth of " +
                ( ob->GetNetWorth() * currency_rate("gold") ) + " gold."});
        arr = filter( map(ob->GetCurrencies(),
                    (: ($(ob))->GetCurrency($1) &&
                     sprintf("%d %s", ($(ob))->GetCurrency($1), $1) :)),
                (: $1 :));
        lines += ({ "Money on hand: "+implode(arr, ", ") });
        this_player()->eventPage(lines, "system");
        return 1;
    }
    else {
        write("You can't stat a non-living object!");
        return 1;
    }
}
