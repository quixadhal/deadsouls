/*  Allows players to possibly backstab a targets
 *  created by Rush@Dead Souls
 *  Version: @(#) backstab.c 1.3@(#)
 *  Last Modified: 96/12/21
 */

#include <lib.h>
#include <position.h>
#include "damage_types.h"
#include "include/backstab.h"

inherit LIB_VERB;

static void create() {
  verb::create();
  SetVerb("backstab");
  SetRules("LIV");
  SetErrorMessage("Who do you want to backstab?");
  SetHelp("Syntax: backstab LIVING\n\n"
          "This sneaky attack allows more devious players to "
          "backstab a specified target.");
}

mixed can_backstab_liv() {
    if( !environment(this_player()) ) return 0;
    if( this_player()->GetInCombat() )
       return "You are too busy with combat!";
    else return 1;
}

mixed do_backstab_liv(object ob) {
  object env;
  
  if(!(env = environment(this_player()))) return 0;
  if( (int)this_player()->GetPosition() != POSITION_STANDING )
    this_player()->eventStand();   
  if(!ob || !present(ob, env)) {
    this_player()->eventPrint("Your hapless victim is no longer present.");
    return 1;
  }
  this_player()->eventPrint("%^RED%^You sneak up behind " + ob->GetName() + "...%^RESET%^");
  this_player()->SetAttack(ob, (:eventBackstab, this_player(), ob:));
  return 1;
}

int eventBackstab(object backstabber, object target) {
  object env;
  float tmp;
  int suprise;
  object *weapons;
  int numberOfWeapons;
  int numberOfStabs;
  int i;

  if(!backstabber || !(env = environment(backstabber))) return 0;
  if(!target || !present(target, env)) {
    backstabber->eventPrint("Your hapless victim is no longer present.");
    return 1;
  }

  tmp = backstabber->GetSkillLevel("stealth")*1.5;
  tmp += backstabber->GetStatLevel("charisma")/2.0;
  tmp += backstabber->GetStatLevel("luck")/3.0;
  tmp += backstabber->GetMobility()/3.0;
  tmp -= target->GetStatLevel("luck")/3.0;
  tmp -= target->GetStatLevel("wisdom")/3.0;
  tmp -= target->GetMobility()/5.0;
  tmp *= 1.0;
  if(tmp < 1.0) tmp = 1.0;
  suprise = to_int(tmp);

  backstabber->AddSkillPoints("stealth", to_int(suprise * 3.0));
  backstabber->AddStaminaPoints(-(5 + random(5)));
  moral_act(backstabber, target, -30);

  if(suprise < random(100) || !present(target, environment(backstabber))) {
    backstabber->eventPrint("%^RED%^" + target->GetName()
      + " evades your poorly executed backstab.%^RESET%^");
    target->eventPrint("%^RED%^You easily dodge " + possessive_noun(backstabber)
      + " attempted backstab.%^RESET%^");
    environment(backstabber)->eventPrint("%^RED%^" + target->GetName()
      + " dodges " + possessive_noun(backstabber) + " attempted backstab.%^RESET%^",
      ({backstabber, target}));
    return 1;
  }  

  weapons = backstabber->GetWielded();
  numberOfWeapons = sizeof(weapons);

  if(!numberOfWeapons) {
    backstabber->eventPrint("%^RED%^You beat your fists ineffectively on "
      + possessive_noun(target) + " back.%^RESET%^");
    target->eventPrint("%^RED%^Somebody beats ineffectively upon your back.%^RESET%^");
    environment(backstabber)->eventPrint("%^RED%^" + backstabber->GetName()
      + " backstabs " + target->GetName() + " expertly... unfortunately "
      + nominative(backstabber) + " forgot " + possessive(backstabber) + " knives.%^RESET%^",
      ({backstabber, target}));
    return 1;
  }

  numberOfStabs = 0;
  for(i = 0; i < numberOfWeapons && target; i++)
    numberOfStabs += eventStab(backstabber, target, weapons[i]);
  if(!numberOfStabs) {
    target->eventPrint("%^RED%^You dodge " + possessive_noun(backstabber)
      + " attempted backstab.%^RESET%^");
    return 1;
  }

  backstabber->eventExecuteAttack(target);
  return 1;
}

int eventStab(object backstabber, object target, object weapon) {
  string weaponType;
  int damage;
  float x;
  int percentDamage;
  float adjustment;
  int numberWielded;
    
  if(!(weaponType = (string)weapon->GetWeaponType())) return 0;
  numberWielded = sizeof(backstabber->GetWielded());

  x = weapon->GetClass() * 5.0;
  x += backstabber->GetSkillLevel(weaponType + " attack")/1.0;
  x += backstabber->GetStatLevel("strength")/2.0;
  x += backstabber->GetStatLevel("luck")/3.0;
  x += backstabber->GetMobility()/3.0;
  if(numberWielded > 1) {
    adjustment = backstabber->GetSkillLevel("multi-weapon") / (numberWielded / 2.0);
    x *= adjustment/100.0;
  }
  x -= target->GetStatLevel("luck")/3.0;
  x *= 0.5;
  x = (x + random(x)) / 2.0;
  if(weaponType != "knife") x = (x - 50.0) / 4.0;
  if(x < 1.0) x = 0.0;

  damage = (int)target->eventReceiveDamage(backstabber,
    weapon->GetDamageType(), to_int(x), 0, target->GetTorso());
  if(damage > 0) {
    weapon->eventReceiveDamage(BLUNT, damage, 0, target->GetTorso());
    percentDamage = damage * 100 / target->GetMaxHealthPoints();
    if(percentDamage < 1) percentDamage = 0;
  }
  else percentDamage = 0;
  eventPrintDamage(backstabber, target, weapon, percentDamage);
  backstabber->AddSkillPoints(weaponType + " attack", 7 + damage/10);
  return 1;
}

int eventPrintDamage(object backstabber, object target, object weapon, int percentDamage) {
  string adverb;
  string myVerb;
  string verb;

  if(!percentDamage) {
    backstabber->eventPrint("%^RED%^You fumble awkwardly with your "
      + strip_article(weapon->GetShort()) + ".%^RESET%^");
    environment(backstabber)->eventPrint("%^RED%^" + backstabber->GetName()
      + " fumbles awkwardly with " + possessive(backstabber) + " "
      + strip_article(weapon->GetShort()) + ".%^RESET%^",
      ({backstabber, target}));
    return 1;
  }

  if(percentDamage < 15) {
    myVerb = "jab ";
    verb = "jabs ";
    adverb = "sharply ";
  }
  else if(percentDamage < 30) {
    myVerb = "stab ";
    verb = "stabs ";
    adverb = "painfully ";
  }
  else {
    myVerb = "slam ";
    verb = "slams ";
    adverb = "deep ";
  }

  backstabber->eventPrint("%^RED%^You " + myVerb + "your "
    + strip_article(weapon->GetShort()) + " " + adverb + "into "
    + possessive_noun(target) + " back.%^RESET%^");
  target->eventPrint("%^RED%^Your back sears with pain as a " + weapon->GetWeaponType()
    + " " + verb + adverb + "into you.%^RESET%^");
  environment(backstabber)->eventPrint("%^RED%^" + backstabber->GetName() + " "
    + verb + possessive(backstabber) + " " + strip_article(weapon->GetShort())
    + " " + adverb + "into " + possessive_noun(target) + " back.%^RESET%^",
    ({backstabber, target}));

  return 1;
}



