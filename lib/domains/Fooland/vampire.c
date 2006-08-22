// vampire.c
// An example monster which reacts to the time of day.
// It has three forms: resting form, vampire form, and bat form. In
// daytime it is in resting form, during which time it lies in its
// coffin. At nighttime it turns into a vampire, except during the full
// moon in which case it turns into a bat.
// Coded by Mobydick, 7-4-93.

#include <daemons.h>
#include <mudlib.h>

#define INSIDE_COFFIN "/domains/Fooland/coffin_interior.c"
#define COFFIN "/domains/Fooland/coffin.c"
#define COFFIN_ROOM "/domains/Fooland/tower2"
#define RESTING 1
#define VAMPIRE 2
#define BAT 3

inherit MONSTER ;

#define FULL_MOON_MSG "The full moon shines brightly down."
#define START_NIGHT_MSG "Twilight fades and the night begins.\n"
#define END_NIGHT_MSG "A faint glow appears in the east.\n"


int state ;

// We need three functions, to change the monster into each of its three
// forms.

void become_resting() {
	set ("short", "a pale corpse") ;
	set ("long", "This is the pale corpse of a tall man with a widow's peak and some odd\ndental work. He is dressed entirely in black.\n") ;
	set ("mass", 7500) ;
	set ("bulk", 750) ;
	set ("capacity", 5000) ;
	set ("max_cap",5000) ;
	set ("volume", 500) ;
	set ("max_vol", 500) ;
	set ("no_attack", 1) ;
	state = RESTING ;
	remove_call_out("move_around") ;
}

void become_vampire() {
	set ("short", "a vampire") ;
	set ("long", "The vampire is dressed all in black and has long sharp fangs. Blood drops\nfrom the corner of his mouth.\n") ;
	set ("mass", 7500) ;
        set ("bulk", 750) ;
        set ("capacity", 5000) ;
        set ("max_cap",5000) ;
        set ("volume", 500) ;
        set ("max_vol", 500) ;
	set ("hit_points", 70) ;
	set ("max_hp",70) ;
	set ("stat/strength", 4) ;
        set ("stat/dexterity", 6) ;
        set_skill("attack", 3, "strength") ;
        set_skill("defense", 8, "dexterity") ;
	set ("armor_class", 2) ;
	set ("attack_strength", 12) ;
	set ("weapon_name", "teeth") ;
	set ("damage", ({ 6,12 }) ) ;
	set_verbs( ({ "bite at" }) ) ;
	set ("no_attack", 0) ;
	state = VAMPIRE ;
	set ("moving", 1) ;
	set ("speed", 12) ;
}

void become_bat() {
	set ("short", "a bat") ;
	set ("long", "This small bat is flying around the room noisily. It seems as though it\nmight be more intelligent than it appears...\n") ;
	set ("mass",50) ;
	set ("bulk", 15) ;
	set ("capacity",30) ;
	set ("max_cap", 30) ;
	set ("volume", 10) ;
	set ("max_vol", 10) ;
	enable_commands() ;
	state = BAT ;
	set("moving",1) ;
	set ("speed",4) ;
}

void create() {

	int hour ;
	string str,foo ;
	object ob ;

	::create() ;
	seteuid(getuid()) ;
// Load up the coffin and coffin interior, if needed.
	ob = find_object_or_load(COFFIN) ;
	ob = find_object_or_load(INSIDE_COFFIN) ;
	WEATHER_D->request_weather_notices(this_object()) ;
	enable_commands() ;
	set ("id", ({ "corpse", "vampire", "bat" }) ) ;
	set_name("vampire") ;
	set("gender", "male") ;
	sscanf(WEATHER_D->query_game_time(),"%d%s",hour,foo) ;
// hour = hour + 10 ;
	if (hour>12 && hour <24) {
		foo = WEATHER_D->query_current_moon_phase() ;
		if (foo==FULL_MOON_MSG) {
			become_bat() ;
		} else {
			become_vampire() ;
		}
		move(COFFIN_ROOM) ;
	} else {
		become_resting() ;
		move(INSIDE_COFFIN) ;
	}
}

void receive_message (string Class, string message) {

	if (message==START_NIGHT_MSG) {
	    move(COFFIN_ROOM) ;
	    if ((string)WEATHER_D->query_current_moon_phase()==FULL_MOON_MSG) {
		become_bat() ;
		write ("The corpse changes into a bat, and flies from the coffin!\n") ;
	    } else {
		become_vampire() ;
		say ("The corpse rises and becomes a vampire!\n") ;
	    }
	}
	if (message==END_NIGHT_MSG) {
	    become_resting() ;
	    if (state==BAT) {
		say ("The bat flies out of the room.\n") ;
	    } else {
		say ("The vampire turns into a bat, and flies away!\n") ;
	    }
	    move(INSIDE_COFFIN) ;
	}
}

// Generally this monster should not clean up. Otherwise the coffin
// will become confused.

int clean_up() {
	return 1 ;
}

// Need this to get an appropriate message printed.

varargs int move_player(mixed dest, string message, string dir) {

	object env ;

	env = environment(this_object()) ;
	if (file_name(env)==COFFIN_ROOM) {
// we only want him to go out the window in this case.
	message = "The vampire turns into a bat, and flies out the window!" ;
	dir = "window" ;
	dest = env->query("exits/window") ;
		env->out_the_window() ;
	}
	::move_player(dest,message,dir) ;
}
