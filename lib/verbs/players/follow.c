/*  Allows players to possibly follow a target into another room
 *  created by Rush@Dead Souls
 */

#pragma save_binary

#include <lib.h>
#include "include/follow.h"

inherit LIB_VERB;

static void create() {
  ::create();
  SetVerb("follow");
  SetRules("", "LIV");
  SetErrorMessage("Who would you like to follow?");
  SetHelp("Syntax: follow\n"
    "        follow LIV\n"
    "\n"
    "If a living object is specified, this command allows "
    "you to begin trailing the living being as it moves "
    "through Dead Souls.\n"
    "Otherwise, your following status is reported.\n"
    "\n"
    "See also: evade, lead, tracking, stealth\n");
}

mixed can_follow() { return 1; }

mixed do_follow() {
  object* followers;
  string* obs;
  object leader;
  int size;
  string tmp;

  // Format follow string.
  if(this_player()->CanLead() && leader = this_player()->GetLeader()) {
    tmp = "You are ";
    if(leader->GetAllowed(this_player())) tmp += "following";
    else tmp += "trailing";
    tmp += " " + leader->GetName() + ".\n";
  }
  else tmp = "You are trailing no one." + "\n";

  // Get the followers.
  followers = this_player()->GetFollowers();
  if(!followers) followers = ({});

  // Format lead string.
  tmp += "You are leading ";
  obs = map(
    filter(followers, (:this_player()->GetAllowed($1):)),
    (:$1->GetName():));
  size = sizeof(obs);
  if(size) tmp += conjunction(obs);
  else tmp += "no one";
  tmp += ".\n";

  // Format evasion string.
  tmp += "You are evading ";
  obs = map(
    filter(followers, (:!this_player()->GetAllowed($1):)),
    (:$1->GetName():));
  size = sizeof(obs);
  if(size) tmp += conjunction(obs);
  else tmp += "no one";
  tmp += ".\n";

  this_player()->eventPrint(tmp);
  return 1;
}

mixed can_follow_liv() { return 1; }

mixed do_follow_liv(object ob) {
  object leader;

  if(leader = this_player()->GetLeader()) {
    leader->RemoveFollower(this_player());
    this_player()->eventPrint("You stop trailing " + leader->GetName() + ".");
  }

  if(member_array(this_player(), ob->AddFollower(this_player())) == -1)
    this_player()->eventPrint("You are not empowered to follow " + ob->GetName() + ".");
  else this_player()->eventPrint("You are now trailing " + ob->GetName() + ".");

  return 1;
}


