// A little orc who is the punchline of a pretty stupid joke.
// Mobydick, 10-2-92.

inherit "/std/monster" ;

void create () {

	object ob1, ob2, ob3 ;

	::create() ;
	seteuid(getuid()) ;
	set ("short", "a wet orc") ;
	set("long", "This orc is taking a shower and has a fresh clean smell.\n") ;
	set ("id", ({ "orc", "wet orc" }) ) ;
	set ("capacity", 10000) ;
	set ("volume", 10000) ;
        set ("mass", 3000) ;
        set ("bulk", 250) ;
	set_name("orc") ;
	enable_commands() ;
	ob1 = clone_object ("/obj/sword") ;
	ob1 -> move (this_object()) ;
	ob2 = clone_object ("/obj/shield") ;
	ob2 -> move (this_object()) ;
	ob3 = clone_object ("/domains/Fooland/items/soap") ;
	ob3 -> move (this_object()) ;
	set_living_name("orc") ;
	enable_commands() ;
	set ("hit_points", 10) ;
	set ("armor_class", 2) ;
	set ("max_hp",10) ;
	set ("attack_strength", 6) ;
	set ("damage", ({ 2,7 }) ) ;
	set ("weapon_name", "sword") ;
	set ("stat/strength", 4) ;
	set ("stat/dexterity", 6) ;
	set_skill("attack", 3, "strength") ;
	set_skill("defense", 8, "dexterity") ;
	set_verbs( ({ "poke at", "hack at", "wave at" }) ) ;
}
