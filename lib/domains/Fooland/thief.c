// An example monster.
// Has a catch_tell. It opens the door for you.
// Comments on monster properties can be found in /obj/foobird.c
// Written by Mobydick, 1-22-93.

#include <mudlib.h>

inherit MONSTER ;

void create () {

	object ob1, ob2, ob3 ;

	seteuid(getuid()) ;
	::create() ;
	set ("short", "A door guard") ;
	set ("long", "The guard is listening intently.\n") ;
	set ("id", ({ "guard" }) ) ;
	set ("capacity", 10000) ;
	set ("volume", 10000) ;
        set ("mass", 3000) ;
        set ("bulk", 250) ;
	 set_name("guard") ;
	set ("gender", "male") ;
	set ("race", "human") ;
	enable_commands() ;
	ob1 = clone_object ("/obj/sword") ;
	ob1 -> move (this_object()) ;
	ob2 = clone_object ("/obj/silver_key") ;
	ob2->move(this_object()) ;
	ob3 = clone_object ("/obj/mask") ;
	ob3 -> move (this_object()) ;
	set_living_name("guard") ;
	enable_commands() ;
	set ("hit_points", 35) ;
	set ("armor_class", 0) ;
	set ("max_hp",35) ;
	set ("attack_strength", 0) ;
	set ("damage", ({ 1,4 }) ) ;
	set ("weapon_name", "fists") ;
	set ("stat/strength", 13) ;
	set ("stat/dexterity", 12) ;
	set_skill("attack", 9, "strength") ;
	set_skill("defense", 11, "dexterity") ;
	set_verbs( ({ "punch at", "swing at" }) ) ;
	set_verbs2( ({ "punches at", "swings at" }) ) ;
  	wield_weapon(ob1) ;
	equip_armor(ob3) ;
}

void receive_message (string Class, string str) {

	if (str!="You hear a knocking sound.\n") return ;
	call_out("unlock_door", 2) ;
}

int unlock_door() {

	object env ;

	env = environment(this_object()) ;
	env->set_status("north", "closed") ;
	env->update_link("north") ;
	say ("The guard unlocks the north door.\n") ;
	call_out ("open_door", 2) ;
}

int open_door() {

	object env ;

	env = environment(this_object()) ;
	env->set_status("north", "open") ;
	env->update_link("north") ;
	say ("The guard opens the north door.\n") ;
	call_out ("close_door", 6) ;
}

int lock_door() {

	object env ;

	env = environment(this_object()) ;
	env->set_status("north", "locked") ;
	env->update_link("north") ;
	say ("The guard locks the north door.\n") ;
}

int close_door() {

	object env ;

	env = environment(this_object()) ;
	env->set_status("north", "closed") ;
	env->update_link("north") ;
	say ("The guard closes the north door.\n") ;
	call_out ("lock_door", 2) ;
}
