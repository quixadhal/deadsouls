
#include <lib.h>
#include "include/guild.h"

private static class GuildClass Guild;
private int isWelcomed;

static void create() {
  Guild = new(class GuildClass);
  Guild->leader = 0;
  Guild->name = 0;
  Guild->objectName = 0;
  Guild->skill = 0;
  isWelcomed = 0;
}

static void init() {
  if(!present(this_object(), this_player())) return;
  if((string)this_player()->GetGuild() != (string)GetGuildName()) return;
  if((string)this_player()->GetKeyName() == (string)GetLeader()) {
    add_action((:eventBring:), "bring");
    add_action((:eventInitiate:), "initiate");
    add_action((:eventRetire:), "retire");
  }
  this_player()->eventPrint("\n");
  if(!isWelcomed) {
    this_player()->AddChannel(GetGuildName());
    eventWelcome(this_player());
    isWelcomed = 1;
  }
}

mixed CanJoin(object ob) { return 1; }

string GetAffectLong(object ob) {
  if(!ob || !living(ob)) return 0;
  return ob->GetName() + " is a member of the "
    + pluralize(GetGuildName()) + ".";
}

string SetLeader(string str) {
  if(!user_exists(str)) error("No such user: " + str
    + ". You must have a real leader.");
  if(!stringp(Guild->leader)) Guild->leader = str;
  return Guild->leader;
}

string GetLeader() { return Guild->leader; }

string SetGuildName(string str) {
  if(!stringp(Guild->name)) Guild->name = str;
  return Guild->name;
}

string GetGuildName() { return Guild->name; }

string SetGuildObject(string str) {
  if(!stringp(Guild->objectName)) Guild->objectName = str;
  return Guild->objectName;
}

string GetGuildObject() { return Guild->objectName; }

string SetGuildSkill(string str) {
  if(!stringp(Guild->skill)) Guild->skill = str;
  return Guild->skill;
}

string GetGuildSkill() { return Guild->skill; }

int eventBring(string str) {
  object who;

  if(!str) return notify_fail("Bring whom?\n");
  who = find_player(lower_case(str));
  if(!who)
    return notify_fail(who->GetName() + " is nowhere to be found.\n");
  if((string)who->GetGuild() != (string)GetGuildName())
    return notify_fail(who->GetName() + " is not one of you!\n");
  if(   environment(who)->GetProperty("no teleport")
     || environment(this_player())->GetProperty("no teleport")
     || environment(this_player())->GetProperty("no magic"))
    return notify_fail("A magic force blocks your powers.\n");
  if(present(who, environment(this_player())))
    return notify_fail(capitalize(str) + " is here.\n");
  if((int)this_player()->GetMagicPoints() < 70)
    return notify_fail("Too low on magic power.\n");
  this_player()->AddMagicPoints(-70);
  who->eventPrint("%^CYAN%^Your guild leader summons you.%^RESET%^");
  who->eventMoveLiving(environment(this_player()));
  if(!present(who, environment(this_player())))
    this_player()->eventPrint("%^CYAN%^" + capitalize(str)
      + " is beyond your reach.%^RESET%^");
  return 1;
}

int eventInitiate(string str) {
  object initiate;
  object guildObject;
  mixed ret;

  if(!str) return notify_fail("Initiate whom?\n");
  initiate = present(lower_case(str), environment(this_player()));
  if(!initiate || !living(initiate))
    return notify_fail("No one of that nature here.\n");
  if(stringp(ret = CanJoin(initiate))) return notify_fail(ret);
  else if(!ret) return ret;
  if((int)this_player()->GetMagicPoints() < 300)
    return notify_fail("Too low on magic power.\n");
  if(initiate->GetGuild())
    return notify_fail("You may only initiate people without guild "
      + "affiliation.\n");
  initiate->SetGuild((string)GetGuildName());
  initiate->SetSkill(GetGuildSkill(), 1, 1);
  if(guildObject = new((string)GetGuildObject()))
    guildObject->eventMove(initiate);
  this_player()->AddMagicPoints(-300);
  eventJoin(initiate);
  return 1;
}

void eventJoin(object ob) {
  ob->eventPrint("%^YELLOW%^You are now a member of the "
    + pluralize((string)GetGuildName()) + ".%^RESET%^");
  environment(ob)->eventPrint("%^YELLOW%^" +(string)ob->GetName()
    + " is now a member of the "
    + pluralize((string)GetGuildName()) + ".%^RESET%^", ob);
}

int eventRetire(string str) {
  object retiree;
  object guildObject;

  if(!str) return notify_fail("Retire whom?\n");
  retiree = present(lower_case(str), environment(this_player()));
  if(!retiree || !living(retiree))
    return notify_fail("No one of that nature here.\n");
  if((string)retiree->GetGuild() != (string)GetGuildName())
    return notify_fail(retiree->GetName() + " is not one of us!\n");
  guildObject = present(GetGuildName() + "_guild_object", retiree);
  if(!guildObject) error("Problem with guild object.");
  guildObject->eventDestruct();
  retiree->SetGuild(0);
//  retiree->SetSkill(GetGuildSkill(), 1, 1); We need to remove skill here.
  eventUnjoin(retiree);
  return 1;
}

void eventUnjoin(object ob) {
  ob->eventPrint("%^RED%^You are no longer a member of the "
    + pluralize((string)GetGuildName()) + ".%^RESET%^");
  environment(ob)->eventPrint("%^RED%^" + (string)ob->GetName()
    + " is no longer a member of the "
    + pluralize((string)GetGuildName()) + ".%^RESET%^", ob);
}

void eventWelcome(object ob) {
  ob->eventPrint("%^YELLOW%^Welcome, fellow " + (string)GetGuildName()
    + ".%^RESET%^");
}

