/*    /lib/item.c
 *    from the Dead Souls LPC Library
 *    inheritable object for all tangible game objects
 *    created by Descartes of Borg 950207
 *    Version: @(#) item.c 1.30@(#)
 *    Last Modified: 97/01/01
 */
 
#include <lib.h>
#include <dirs.h>
#include <daemons.h>

inherit LIB_BURY;
inherit LIB_DETECT;
inherit LIB_DETERIORATION;
inherit LIB_DROP;
inherit LIB_GET;
inherit LIB_GIVE;
inherit LIB_LOCK_WITH;
inherit LIB_MASS;
inherit LIB_MOVE;
inherit LIB_OBJECT;
inherit LIB_PERSIST;
inherit LIB_PUT;
inherit LIB_READ;
inherit LIB_SELL;
inherit LIB_STEAL;
inherit LIB_UNIQUENESS;
inherit LIB_VALUE;
inherit LIB_WEAPON;

private int RetainOnDeath;

/* ******************** item.c attributes ******************** */
string GetExternalDesc() {
    string desc = object::GetExternalDesc();
    string tmp;
    
    if( desc == "" ) {
	return "";
    }
    if( tmp = GetEquippedDescription() ) {
	desc += tmp + "\n";
    }
    if( GetBroken() ) {
	desc += "It appears to be broken.";
    }
    else if( tmp = GetItemCondition() ) {
	desc += tmp;
    }
    return desc;
}

string GetItemCondition() {
    if( GetMaxClass() ) {
        float i = to_float(GetClass()) / GetMaxClass() * 100.0;
 
        if( i > 95.0 ) {
	    return "It is as good as new.";
	}
        else if( i > 80.0 ) {
	    return "It is in good condition.";
	}
        else if( i > 70.0 ) {
	    return "It is in decent condition.";
	}
        else if( i > 50.0 ) {
	    return "It is somewhat worn and battered.";
	}
        else if( i > 30.0 ) {
	    return "It is worn down and dented.";
	}
        else if( i > 10.0 ) {
	    return "It is very worn down and in bad shape.";
	}
        else {
	    return "It has worn down completely.";
	}
    }
    else {
	return deterioration::GetItemCondition();
    }
}

int GetRetainOnDeath() {
    return RetainOnDeath;
}
 
int SetRetainOnDeath(int x) {
    return (RetainOnDeath = x);
}
 
static mixed array AddSave(mixed array vars) {
    return persist::AddSave(vars);
}

string array GetSave() {
    return persist::GetSave();
}

/* ************************ item.c modals *********************** */ 
mixed CanRepair(object who) {
    if( GetBroken() ) return 1;
    else if( !GetMaxClass() ) return "It doesn't need repairing.";
    else return 1;
}

mixed CanSteal(object who) {
    if( GetWorn() ) {
	return "You can't steal something equipped!";
    }
    return steal::CanSteal(who);
}
 
varargs mixed CanThrow(object who, object target) {
    return 1;
}

/* ********************* item.c events ************************ */ 
static int Destruct() {
    if( GetWorn() && environment() ) {
	eventUnequip(environment());
    }
    return object::Destruct();
}
 
int eventMove(mixed dest) {
    if( !environment() && GetWorn() ) {
	mixed array limbs = GetWorn();

	SetWorn(0);
	call_out((: eventRestoreEquip :), 0, limbs);
    }
    if( GetWorn() && environment() ) {
	eventUnequip(environment());
    }
    return move::eventMove(dest);
}
 
void eventDeteriorate(int type) {
    weapon::eventDeteriorate();
    SetDestroyOnSell(1);
    SetValue(GetValue()/2);
}
 
void eventRemoveBlessing() {
    SetProperty("blessed", 0);
    if( living(environment()) ) {
	environment()->eventPrint("%^YELLOW%^"
				  + capitalize((string)GetDefiniteShort()) +
				  " returns to its normal state.");
    }
}
    
int eventBless(int amount, int time) {
    if(!amount || !time) return 0;
    if(GetProperty("blessed")) return 0;
    SetProperty("blessed", amount);
    call_out( (: eventRemoveBlessing :), time);
    if(GetProperty("blessed") > 0) {
	 SetProperty("magic item", "blessed");
	}
    if(GetProperty("blessed") < 0) {
	 SetProperty("magic item", "cursed");
	}
    return 1;
}

mixed eventThrow(object who, object target) {
    if( target && living(target) ) {
	int skill;
	
	who->eventPrint("You throw " + GetShort() + " at " +
			target->GetName() + ".");
        target->eventPrint(who->GetName() + " throws " + GetShort() +" at you.");
	environment(who)->eventPrint(who->GetName() + " throws " +
				     GetShort() + " at " + target->GetName() +
				     ".", ({ who, target }) );
	skill = (who->GetSkillLevel("projectile attack") +
	         who->GetStatLevel("coordination"))/2;
	skill -= (target->GetSkillLevel("projectile defense") +
		  target->GetStatLevel("agility"))/4;
	if( GetWeaponType() != "projectile" ) {
	    skill = skill/4;
	}
	if( skill > random(100) + 1 ) {
	    who->AddSkillPoints("projectile attack",
				target->GetSkillLevel("projectile defense") *
				target->GetLevel() + 10);
	    target->AddSkillPoints("projectile defense", 10);
	    target->eventReceiveThrow(who, this_object());
        }
	else {
	    target->AddSkillPoints("proectile defense",
				   who->GetSkillLevel("projectile attack") *
				   who->GetLevel() + 10);
	    who->AddSkillPoints("projectile attack", 10);
	    environment(who)->eventPrint(capitalize(GetShort()) + " does not "
					 "come close to hitting " +
					 target->GetName() + ".");
	    eventMove(environment(who));
	}
        return 1;
    }
    else if( target ) {
	who->eventPrint("You throw " + GetShort() + " at " +
			target->GetShort() + ".");
	environment(who)->eventPrint(who->GetName() + " throws " +
				     GetShort() + " at " + target->GetShort() +
				     ".", who);	
	return target->eventReceiveThrow(who, this_object());
    }
    if( !eventMove(environment(who)) ) {
        who->eventPrint("You are not too good at throwing things.");
        return 1;
    }
    who->eventPrint("You throw " + GetShort() + ".");
    environment(who)->eventPrint(who->GetName() + " throws " +
				 GetShort() + ".", who);
    return 1;
}
 
varargs mixed eventRepair(object who, int strength, int type) {
    if( !who || !strength ) return 0;
    if( !GetMaxClass() ) return 0;
    while(strength--) {
        if( GetClass() < GetMaxClass() ) {
            SetClass(GetClass() + 1);
            SetValue(GetValue() + (GetValue() / 3));
        }
        else break;
    }
    return 1;
}

mixed eventShow(object who, string component) {
    mixed tmp = object::eventShow(who, component);

    if( component || tmp != 1 ) {
	return tmp;
    }
    if( GetPoison() ) {
        if( random(100) < who->GetSkillLevel("stealth") ) {
            who->eventPrint("You notice a strange substance on it.");
	}
    }
    return 1;
}

/* ***************** item.c driver applies ****************** */
static void create() {
    AddSave(weapon::GetSave() + value::GetSave() + mass::GetSave() +
	    deterioration::GetSave());
    steal::create();
    object::create();
}
 
mixed direct_cast_str_on_obj() {
    return 1;
}

mixed direct_cast_str_on_str_of_obj() {
    return 1;
}

mixed direct_balance_obj_to_obj() {
    return 1;
}

mixed indirect_balance_obj_to_obj() {
    return 1;
}

mixed direct_compare_obj_to_obj() {
    return 1;
}

mixed indirect_compare_obj_to_obj() {
    return 1;
}

mixed direct_judge_obj_to_obj() {
    return 1;
}

mixed indirect_judge_obj_to_obj() {
    return 1;
}

mixed direct_use_obj_to_str() {
    if( environment() != this_player() )
      return "#You need better access to it.";
    else return 1;
}

mixed direct_throw_obj_word_obj() {
    if( environment() != this_player() ) {
        return "#Throw something you are not holding?";
    }
    else return 1;
}

mixed indirect_throw_obj_into_obj() {
    return 1;
}

int direct_sacrifice_obj_to_str(string deus) {
    mixed tmp;
    object env;
 
    if( !sizeof(deus) ) return 0;
    deus = lower_case(remove_article(deus));
    if( !env = environment(this_player()) ) return 0;
    tmp = (mixed)env->CanSacrifice(this_player(), this_object(), deus);
    if( !tmp ) {
        this_player()->eventPrint("This is not the place for sacrifices.");
        return 0;
    }
    else return 1;
}


mixed direct_bless_obj() {
    if( environment() != this_player() ) {
	return "#You don't have that!";
    }
    return 1;
}
    
mixed direct_curse_obj() {
    if( environment() != this_player() ) {
	return "#You don't have that!";
    }
    return 1;
}

void init() { }
