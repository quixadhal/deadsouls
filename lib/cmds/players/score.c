/*  /verbs/players/score.c
 *  from the Dead Soulsr2 Library
 *  Displays a list of score and related info
 *  created by Blitz@Dead Souls
 */
 
#include <lib.h>
 
inherit LIB_DAEMON;
inherit LIB_HELP;
 
mixed eventScore();
 
static void create() {
  daemon::create();
  SetNoClean(1);
  SetHelp("Syntax: score\n\n"
         "Displays information about your character.\n\n"
         "See also: status\n");
}
 
static string *FoodDegree =
({ "could eat a horse right now!", "could eat plenty more.",
   "could eat some more.", "are partially hungry.",
   "are feeling full.", "feel quite full." });
 
static string *DrunkDegree =
({ "sober", "tipsy", "drunk", "blitzed",
   "smashed out of your gord", "FUBAR" });
 
static string *DrinkDegree =
({ "parched", "extremely thirsty", "very thirsty", "thirsty",
   "somewhat thirsty", "not thirsty" });
 
mixed cmd(string unused) {
  if( creatorp(this_player()) )
    return "Creators have no score, get a life.";
  eventScore();
  return 1;
}
 
mixed eventScore() {
  string *str;
  int birth, age, x, y, z;
 
  str  = ({ "You are "+(string)this_player()->GetShort() + " (" +
          (string)this_player()->GetMoralityDescription() + ")." });
  str += ({ sprintf("You are a level %d %s%s %s.",
           (int)this_player()->GetLevel(),
           ( (int)this_player()->GetUndead() ? "undead " : ""),
           capitalize((string)this_player()->GetRace() || "nothing"),
           capitalize((string)this_player()->GetClass() || "commoner")) });
  str += ({ "Your native town is "+(string)this_player()->GetTown()+", and "
              "you are " + ((string)this_player()->GetReligion() ||
                            "agnostic") + " in faith." });
  str += ({ sprintf("You've solved %s, and have %s.",
            consolidate(sizeof((string *)this_player()->GetQuests()),
              "one quest"),
            consolidate(sizeof((string *)this_player()->GetTitles()),
              "one title") ) });
  birth = (int)this_player()->GetBirth();
  age = ( query_year(time()) - query_year(birth) );
  str += ({ sprintf("You were born on the %d%s day of %s, year %d. "
            "(%d years old)", query_date(birth), ordinal(query_date(birth)),
            query_month(birth), query_year(birth), age) });
  if( x = (int)this_player()->GetTrainingPoints() < 1 ) {
      y = (int)this_player()->GetLevel() + 1 + (x / -4);
      str += ({ "Training points await you at level " + y + "." });
  }
  else str += ({ "You have " + consolidate(
                (int)this_player()->GetTrainingPoints(),
                "one training point") + "." });
  if( (int)this_player()->GetWimpy() )
    str += ({ "You are feeling wimpy." }); else
    str += ({ "You are feeling brave." });
  if( (int)this_player()->GetPoison() > 0 )
    str += ({ "You are poisoned." });
  x = (int)this_player()->GetFood() / 17;
  if( x > sizeof(FoodDegree) - 1 ) x = (sizeof(FoodDegree) - 1);
  y = (int)this_player()->GetDrink() / 17;
  if( y > sizeof(DrinkDegree) - 1 ) y = (sizeof(DrinkDegree) - 1);
  z = (int)this_player()->GetAlcohol() / 17;
  if( z > sizeof(DrunkDegree) - 1 ) z = (sizeof(DrunkDegree) - 1);
  str += ({ "You "+FoodDegree[x] });
  str += ({ sprintf("You are %s and %s.", DrinkDegree[y], DrunkDegree[z]) });
  this_player()->eventPage(str, "info");
  return 1;
}
