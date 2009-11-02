#include <lib.h>
#include "include/clan.h"

private class ClanClass Clan;

private int isWelcomed;

static void create(){
    Clan = new(class ClanClass);
    Clan->leader = 0;
    Clan->name = 0;
    Clan->objectName = 0;
    Clan->skill = 0;
    isWelcomed = 0;
}

static void init(){
    if(!present(this_object(), this_player())) return;
    if(this_player()->GetClan() != GetClanName()) return;
    if(this_player()->GetKeyName() == GetLeader()){
        add_action("eventBring", "bring");
        add_action("eventInitiate", "initiate");
        add_action("eventRetire", "retire");
    }
    this_player()->eventPrint("\n");
    if(!isWelcomed){
        this_player()->AddChannel(GetClanName());
        eventWelcome(this_player());
        isWelcomed = 1;
    }
}

mixed CanJoin(object ob){ return 1; }

string GetAffectLong(object ob){
    if(!ob || !living(ob)) return 0;
    return ob->GetName() + " is a member of the "
        + pluralize(GetClanName()) + ".";
}

string SetLeader(string str){
    if(!user_exists(str)) error("No such user: " + str
            + ". You must have a real leader.");
    if(!stringp(Clan->leader)) 
        Clan->leader = str;
    return Clan->leader;
}

string GetLeader(){ return Clan->leader; }

string SetClanName(string str){
    if(!stringp(Clan->name)) Clan->name = str;
    return Clan->name;
}

string GetClanName(){ return Clan->name; }

string SetClanObject(string str){
    if(!stringp(Clan->objectName)) Clan->objectName = str;
    return Clan->objectName;
}

string GetClanObject(){ return Clan->objectName; }

string SetClanSkill(string str){
    if(!stringp(Clan->skill)) Clan->skill = str;
    return Clan->skill;
}

string GetClanSkill(){ return Clan->skill; }

int eventBring(string str){
    object who;

    if(!str) return notify_fail("Bring whom?\n");
    who = find_player(lower_case(str));
    if(!who)
        return notify_fail(who->GetName() + " is nowhere to be found.\n");
    if(who->GetClan() != GetClanName())
        return notify_fail(who->GetName() + " is not one of you!\n");
    if(   environment(who)->GetProperty("no teleport")
            || environment(this_player())->GetProperty("no teleport")
            || environment(this_player())->GetProperty("no magic"))
        return notify_fail("A magic force blocks your powers.\n");
    if(present(who, environment(this_player())))
        return notify_fail(capitalize(str) + " is here.\n");
    if(this_player()->GetMagicPoints() < 70)
        return notify_fail("Too low on magic power.\n");
    this_player()->AddMagicPoints(-70);
    who->eventPrint("%^CYAN%^Your clan leader summons you.%^RESET%^");
    who->eventMoveLiving(environment(this_player()));
    if(!present(who, environment(this_player())))
        this_player()->eventPrint("%^CYAN%^" + capitalize(str)
                + " is beyond your reach.%^RESET%^");
    return 1;
}

int eventInitiate(string str){
    object initiate;
    object clanObject;
    mixed ret;

    if(!str) return notify_fail("Initiate whom?\n");
    initiate = present(lower_case(str), environment(this_player()));
    if(!initiate || !living(initiate))
        return notify_fail("No one of that nature here.\n");
    if(stringp(ret = CanJoin(initiate))) return notify_fail(ret);
    else if(!ret) return ret;
    if(this_player()->GetMagicPoints() < 300)
        return notify_fail("Too low on magic power.\n");
    if(initiate->GetClan())
        return notify_fail("You may only initiate people without clan "
                + "affiliation.\n");
    initiate->SetClan(GetClanName());
    initiate->SetSkill(GetClanSkill(), 1, 1);
    if(clanObject = new(GetClanObject()))
        clanObject->eventMove(initiate);
    this_player()->AddMagicPoints(-300);
    eventJoin(initiate);
    return 1;
}

void eventJoin(object ob){
    ob->eventPrint("%^YELLOW%^You are now a member of the "
            + pluralize(GetClanName()) + ".%^RESET%^");
    environment(ob)->eventPrint("%^YELLOW%^" +ob->GetName()
            + " is now a member of the "
            + pluralize(GetClanName()) + ".%^RESET%^", ob);
}

int eventRetire(string str){
    object retiree;
    object clanObject;

    if(!str) return notify_fail("Retire whom?\n");
    retiree = present(lower_case(str), environment(this_player()));
    if(!retiree || !living(retiree))
        return notify_fail("No one of that nature here.\n");
    if(retiree->GetClan() != GetClanName())
        return notify_fail(retiree->GetName() + " is not one of us!\n");
    clanObject = present(GetClanName() + "_clan_object", retiree);
    if(!clanObject) error("Problem with clan object.");
    clanObject->eventDestruct();
    retiree->SetClan(0);
    //  retiree->SetSkill(GetClanSkill(), 1, 1); We need to remove skill here.
    eventUnjoin(retiree);
    return 1;
}

void eventUnjoin(object ob){
    ob->eventPrint("%^RED%^You are no longer a member of the "
            + pluralize(GetClanName()) + ".%^RESET%^");
    environment(ob)->eventPrint("%^RED%^" + ob->GetName()
            + " is no longer a member of the "
            + pluralize(GetClanName()) + ".%^RESET%^", ob);
}

void eventWelcome(object ob){
    ob->eventPrint("%^YELLOW%^Welcome, fellow " + GetClanName()
            + ".%^RESET%^");
}

