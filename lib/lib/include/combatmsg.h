#ifndef l_combatmsg_h
#define l_combatmsg_h
 
mixed GetMissData(object target, int type, string limb);
static void eventSendMissMessages(object target, int x, string limb);
mixed GetCombatVerbs(string type, int damage);
mixed GetCombatMove(string type, int skill);
varargs void SendMeleeMessages(object target, int x, string targlimb, string limb);
varargs void SendWeaponMessages(object target, int x, object weapon, string limb);

/* virtual only */
string GetName();
varargs int eventPrint(string msg, mixed arg2, mixed arg3);
 
#endif /* l_combatmsg_h */
