#include <lib.h>
#include <daemons.h>

int QuestPoints = 0;
mapping QuestBook = ([]);

int AddQuestPoints(mixed x){
    if( !intp(x)) error("Bad argument 1 to AddQuestPoints().\n");
    if((QuestPoints+= x) < 0) QuestPoints = 0;
    return QuestPoints;
}

int GetQuestPoints(){ return QuestPoints; }


int SetQuestPoints(int i){
    QuestPoints = i;
    return QuestPoints;
}

void AddQuest(string title, string desc){
    mapping tmp = ([]);
    object * PartyMember;
    object ob;
    string pname;

    if( !title || !desc ) return;

    if( this_object()->GetParty() ){
        pname = this_object()->GetParty();
        PartyMember = PARTY_D->GetPartyMembers(pname) - ({ this_player() });;
        foreach(ob in PartyMember){
            ob->AddPartyQuest(title,desc);
        }
    }
    tmp["Date"] = time();
    tmp["Description"] = desc;
    if(!QuestBook) QuestBook = ([]);
    QuestBook[title] = tmp;
    this_object()->AddTitle(title);
}

void AddQuestSkillPoints(string skill, int amount){
    object * PartyMember;
    object ob;
    string pname;

    if(!skill || !amount) return;

    if( this_object()->GetParty() ){
        pname = this_object()->GetParty();
        PartyMember = PARTY_D->GetPartyMembers(pname);
        foreach(ob in PartyMember){
            ob->AddSkillPoints(skill, amount);
        }
    }
    else this_object()->AddSkillPoints(skill, amount);
}

void AddQuestStatPoints(string stat, int amount){
    object * PartyMember;
    object ob;
    string pname;

    if(!stat || !amount) return;

    if( this_object()->GetParty() ){
        pname = this_object()->GetParty();
        PartyMember = PARTY_D->GetPartyMembers(pname);
        foreach(ob in PartyMember){
            ob->AddStatPoints(stat, amount);
        }
    }
    else this_object()->AddStatPoints(stat, amount);
}

void AddQuestCurrency(string type, int amount){
    object * PartyMember;
    object ob;
    string pname;

    if(!type || !amount) return;

    if( this_object()->GetParty() ){
        pname = this_object()->GetParty();
        PartyMember = PARTY_D->GetPartyMembers(pname);
        foreach(ob in PartyMember){
            ob->AddCurrency(type, amount);
        }
    }
    else this_object()->AddCurrency(type, amount);
}

void AddPartyQuest(string title, string desc){
    mapping tmp = ([]);

    if( !title || !desc ) return;
    if( member_array(title, this_object()->GetTitles()) != -1 ) return;
    tmp["Date"] = time();
    tmp["Description"] = desc;
    if(!QuestBook) QuestBook = ([]);
    QuestBook[title] = tmp;
    this_object()->AddTitle(title);
}

mixed *GetQuests(){
    if(!QuestBook) QuestBook = ([]);
    return copy(QuestBook);
}

int GetQuest(string str){
    if(!QuestBook) QuestBook = ([]);
    foreach(string key, mapping val in QuestBook){
        if(str == val["Description"]) return 1;
    }
    return 0;
}

mapping RemoveQuest(string str){
    if(!QuestBook) QuestBook = ([]);
    if(QuestBook[str]){
        map_delete(QuestBook,str);
    }
    return copy(QuestBook);
}

void create(){
    QuestBook = ([]);
}
