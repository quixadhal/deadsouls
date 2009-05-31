#include <lib.h>
#include <vendor_types.h>
#define DIR_TXT "/domains/campus/txt"
inherit LIB_ITEM;
int Spam(string spam);
string SpamSpam(string spamspam);
int SpamSpamSpam(string spamspamspam);
string spam1, spam2;
void create(){
    ::create();
    SetKeyName("spam can");
    SetId(({"spam","can"}));
    SetAdjectives(({"can of","can","spam"}));
    SetShort("a can of spam");
    SetLong( (: SpamSpam :) );
    SetMass(20);
    SetDollarCost(10);
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
    add_action("SpamSpamSpamSpam","spam");
}

string SpamSpam(string spamspam){
    string spamspamspam;
    spamspamspam=read_file("/domains/campus/txt/spam.txt");
    return spamspamspam;
}
int SpamSpamSpamSpam(string spam){
    string spamspam;
    object ob;
    if(!spam || spam == ""){
        this_object()->Spam();
        return 1;
    }
    if(!sscanf(spam,"%s with %s",spam1,spam2)){
        write("You'd like to spam who with what?");
        say(this_player()->GetName()+" fumbles with "+possessive(this_player())+
                " can of spam.\n");
        return 1;
    }
    ob=present(spam1,environment(this_player()));
    if(!ob){
        write("There is no such thing here to spam!\n");
        say(this_player()->GetName()+" looks around for something to spam.\n");
        return 1;
    }
    if(!living(ob)){
        write(ob->GetShort()+" is not impressed by spam.\n");
        say(this_player()->GetName()+" spams around foolishly.\n");
        return 1;
    }
    if(file_size(DIR_TXT+"/"+spam2+".txt") == -1){
        write("You cannot spam "+ob->GetName()+" that way. You look silly.\n");
        say(this_player()->GetName()+" looks silly trying to spam incorrectly.\n");
        return 1;
    }
    spamspam=unguarded((: read_file(DIR_TXT+"/"+spam2+".txt") :));
    say(this_player()->GetName()+" spams "+ob->GetName()+" with a "+
            "giant "+spam2+".\n");
    tell_object(ob, this_player()->GetName()+" spams you with:\n"+spamspam+"\n");
    write("You spam "+ob->GetName()+" with a giant "+spam2+"!\n");
    return 1;
}
int Spam(string spamspamspamspam){
    string but_I_dont_like_spam;
    but_I_dont_like_spam=unguarded((:read_file("/secure/obj/txt/spam.txt") :));
    this_player()->SetLong("$N is a can of spam:\n\n"+but_I_dont_like_spam);
    this_player()->AddTitle("the can of spam (Spammy!)");
    write("You spam all over yourself!\n");
    say(this_player()->GetName()+" spams all over "+objective(this_player())+"self!\n");
    return 1;
}
