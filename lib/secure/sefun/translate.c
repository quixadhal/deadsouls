//      /adm/simul_efun/translate.c
//      from the Dead Souls Mudlib
//      translates a string into nonsense for non-proficient speakers
//      created by Descartes of Borg 930823
//      based on a speak function by Valodin@Dead Souls

#define FLUBS ({ "womble", "assimilate", "scruvy", "bing", "lait", "krishe",\
"something", "blarg", "gyro", "yellow hephalumps", "darmok", "tanagra",\
"cabbage", "blurgle", "jello", "migrated", "furry", "driznit", "folnar",\
"pamplemousse", "burrito", "eyne", "duh", "hamster",\
})

string translate(string str, int prof) {
    if( prof >= 100 ) return str;
    else {
	string *words;
	string word;
	int fs, i, maxi;

	fs = sizeof(FLUBS);
	words = explode(str, " ");
	for(i=0, maxi = sizeof(words); i < maxi; i++)
	  if( random(100) >= prof ) words[i] = FLUBS[random(fs)];
	return implode(words, " ");
    }
}
