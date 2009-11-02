#include <lib.h>
#include <objects.h>

mixed performMarriage(object spouse1, object spouse2) ;
int performDivorce(object ob1);
int MarriageRequest(mixed arg1, mixed arg2, mixed arg3);
int eventRequestDivorce(mixed arg1, mixed arg2, mixed arg3);

inherit  LIB_MAYOR;
void create(){
    ::create();
    SetKeyName("clerk");
    SetId("clerk");
    SetAdjectives("town");
    SetGender("male");
    SetRace("human");
    SetShort("the town clerk");
    SetLong("An officious-looking clerk."); 
    SetLocalCurrency("silver");
    SetClass("priest");
    AddCommandResponse("marry", (: MarriageRequest :));
    AddCommandResponse("wed", (: MarriageRequest :));
    AddCommandResponse("join", (: MarriageRequest :));
    AddCommandResponse("divorce", (: eventRequestDivorce :));
    SetLevel(4);
    SetTax(5);
}

int eventRequestDivorce(mixed arg1, mixed arg2, mixed arg3){
    this_object()->performDivorce(arg1);
    return 1;
}

int MarriageRequest(mixed arg1, mixed arg2, mixed arg3){
    string s1, s2, dudename;
    object ob1, ob2;
    if(strsrch(arg2,"divorce") != -1){
        this_object()->performDivorce(arg1);
        return 1;
    }

    if(sscanf(arg3,"%s and %s", s1, s2) !=2 &&
            sscanf(arg3,"%s to %s", s1, s2) !=2){
        eventForce("say "+arg1->GetName()+", you are confusing me.");
        return 1;
    }

    dudename = arg1->GetKeyName();
    if(s1 == "me" || s1 == "i") s1 = dudename;
    if(s2 == "me" || s2 == "i") s2 = dudename;

    if(s1 != dudename && s2 != dudename ){
        eventForce("say Those people will have to decide "+
                "to get married on their own. It isn't "+
                "any of your business.");
        return 1;
    }

    if( s1 == s2){
        eventForce("say we don't do single-member marriages here.");
        return 1;
    }
    if(! ob1 = present(s1, environment(this_object()))){
        eventForce("say "+capitalize(s1)+" isn't here.");
        return 1;
    }
    if(! ob2 = present(s2, environment(this_object()))){
        eventForce("say "+capitalize(s2)+" isn't here.");
        return 1;
    }

    if(!living(ob1) || !living(ob2)){
        eventForce("say I do not perform marriages with inanimate objects.");
        return 1;
    }
    this_object()->performMarriage(ob1, ob2);
    return 1;
}

mixed performMarriage(object spouse1, object spouse2){
    mixed tmp;
    eventForce("say hmmm...");
    tmp = environment(this_player())->CanMarry(this_player(),
            spouse1, spouse2);
    if( !tmp ){
        this_player()->eventPrint("This place is not holy to you.");
        return 1;
    }
    else if( stringp(tmp) ){
        this_player()->eventPrint(tmp);
        return 1;
    }
    tmp = environment(this_player())->eventMarry(this_player(),
            spouse1, spouse2);
    if( tmp == 1 ){
        object ring;

        ring = new(OBJ_WED_RING);
        ring->SetSpouse(spouse2->GetCapName());
        ring->eventMove(spouse1);
        ring = new(OBJ_WED_RING);
        ring->SetSpouse(spouse1->GetCapName());
        ring->eventMove(spouse2);
    }
}

int performDivorce(object ob1){
    string spouse1, spouse2;
    object ring1, ring2, ob2;

    spouse1 = ob1->GetKeyName();
    if(!ob1->GetSpouse()){
        eventForce("say You don't appear to be married.");
        return 1;
    }
    spouse2 = lower_case(ob1->GetSpouse());
    ob2 = find_player(spouse2);

    if(!ob1->CanDivorce(ob1)){
        eventForce("say I cannot perform this divorce. Are you sure "+
                "you are still married?");
        return 1;
    }

    if(!find_player(spouse1) || !ob1 ){
        eventForce("say I'm sorry. Both spouses must be logged "+
                "on for a divorce to take place.");
        return 1;
    }

    ob1->eventDivorce(ob1);
    ob2->eventDivorce(ob2);

    ring1 = present("official wedding ring",ob1);
    ring2 = present("official wedding ring",ob2);

    if(ring1) ring1->eventDestruct();
    if(ring2) ring2->eventDestruct();

    eventForce("say The divorce is complete.");
    tell_player(spouse1,"%^RED%^You are now divorced from "+capitalize(spouse2)+"%^RESET%^!");
    tell_player(spouse2,"%^RED%^You are now divorced from "+capitalize(spouse1)+"%^RESET%^!");
    eventForce("shout this office duly records and announces that "+capitalize(spouse1) +" has divorced "+capitalize(spouse2)+"!");
    return 1;
}


