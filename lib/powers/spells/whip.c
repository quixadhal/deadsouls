#include <lib.h>
#include <magic.h>
#include <damage_types.h>

inherit LIB_SPELL;

static void create() {
    spell::create();
    SetSpell("whip");
    SetRules("");
    SetSpellType(SPELL_OTHER);
    SetRequiredMagic(5);
    SetSkills(([ "conjuring" : 0 ]));
    SetMagicCost(50, 50);
    SetStaminaCost(10, 10);
    SetDifficulty(0);
    SetHelp("Syntax: <cast whip>\n\n"
            "This spell conjures a whip made of magical energy and "
            "moves it to a wielding limb. If unwielded, it will vanish. "
            "The power of the whip as a weapon depends on your spellcasting "
            "abilities.");
}

varargs int eventCast(object who, int level, string limb, object array targs){
    object whip;
    object *limbs = this_player()->GetWieldingLimbs();
    object *whips = filter(all_inventory(this_player()),
            (: base_name($1) == "/obj/whip" :) );
    if(sizeof(whips)){
        write("You already have an energy whip.");
        return 1;
    }
    whip = new("/obj/whip");
    write("You make a powerful motion with your hand and conjure "+
            "an energy whip!");
    say(this_player()->GetName()+" makes a powerful motion with "+
            possessive(this_player())+" hand and conjures "+
            "an energy whip!");
    whip->eventMove(this_player());
    limbs = filter(limbs, (: !(this_player()->GetWielded($1)) :) );
    if(sizeof(limbs)){
        int weapclass = (this_player()->GetLevel());
        weapclass += (this_player()->GetStatLevel("intelligence") / 4);
        weapclass += (this_player()->GetSpellLevel("whip") / 10);
        weapclass += (this_player()->GetSkillLevel("magic attack") / 3);
        weapclass += (this_player()->GetSkillLevel("conjuring") / 3);
        whip->eventEquip(this_player(),limbs[0]);
        whip->SetClass(weapclass);
        whip->SetDuration(this_player()->GetSkillLevel("conjuring") * 20);
    }
    return 1;
}

