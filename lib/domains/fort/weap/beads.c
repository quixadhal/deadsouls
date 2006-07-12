// A magical weapon created by Javelin @ M*UD

#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>
#include <magic_protection.h>

inherit LIB_ITEM;

static int wieldme();
static int hitCallback(object who, object agent, int x, class MagicProtection cl);
static void endCallback(object who);
static void reprotect(object target);

private int count = 0;
private int protecting = 0;

static void create() {
    item::create();
    SetKeyName("beads");
    SetId( ({ "beads", "blue beads", "string", "string of beads",
	"string of blue beads" }));
    SetAdjectives(({ "blue" }));
    SetShort("a string of blue beads");
    SetLong("A set of small beads of blue stone are strung on a cord.");
    // SetDamagePoints(200);
    SetClass(20);
    SetBaseCost("silver", 50);
    SetMass(10);
    SetWeaponType("blunt");
    SetDamageType(MAGIC);
    SetHands(1);
    SetWield((: wieldme :));
}

void init() {
    ::init();
}

static int wieldme() {
    debug(this_player()->GetName() + " wields some beads.",0,"blue");
    this_object()->reprotect(this_player());
    return 1;
}

static void reprotect(object target) {
    class MagicProtection protection;
    int prot_level, skill, wis;

    protection = new(class MagicProtection);
    protection->bits = BLUNT | BLADE | MAGIC;
    protection->caster = target;
    protection->absorb = 30;
    protection->args = target->GetLevel();
    protection->hit = (: hitCallback   :) ;
    protection->end = (: endCallback   :) ;
    target->AddMagicProtection(protection);
    // tell_object(target, "You feel a warm glow from the beads.");
    send_messages("", "$agent_possessive_noun beads glow.",
      target, target, environment(target));
    protecting = 1;
}

static int hitCallback(object who, object agent, int x, class MagicProtection cl) {
    int left = cl->absorb;
    debug("I am "+file_name(this_object())+", held by " +
      environment(this_object())->GetName() + ". I have "+left+" pts left.",0,"blue");
    if(who && environment(who)) 
	send_messages("", "$agent_name's blue beads flash as $target_name strikes.",
	  who, agent, environment(who));
    return x;
}

static void endCallback(object who) {
    protecting = find_call_out("reprotect") > -1;
    count++;
    debug("I am "+environment(this_object())->GetName()+ "'s " + file_name(this_object())+". find_call_out = " +
      find_call_out("reprotect"),count, "red");
    debug("who is " + who->GetName() + " and t_p is " + this_player()->GetName(),0,"green");
    reprotect(who);
}
