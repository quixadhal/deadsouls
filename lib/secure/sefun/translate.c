//      /adm/simul_efun/translate.c
//      from the Dead Souls Mudlib
//      translates a string into nonsense for non-proficient speakers
//      created by Descartes of Borg 930823
//      based on a speak function by Valodin@Dead Souls

#define FLUBS ({ "lacunae", "uns", "sur",\
"HWAET","shaka","jalad", "darmok", "tanagra",\
"scyld", "sweorda", "narthex", "luft", "mit", "folnar",\
"temba", "temak", "le","la","nous","avec","mais","foutre",\
"cyning", "thara", "stanas", "seo", "stanum", "res", "ipso","stana",\
"meh", "nehi", "samastahum", "tora", "kia ab", "boltehe", \
"ata", "mevin", "ivrit", "ktzat", "lo", "ani", "tov","aval", \
"verstehen", "sie", "bisschen", "sehr", "aber", "etwas", "keinen",\
"shomo", "mifahmin", "mifahmam", "katalavenete", "hellenika",\
"spreek", "uw", "vrauw", "nit", "shiz", "se","pericolo",\
"iskit", "imhud", "halas", "sagud", "imche", "eigoga", "nihongo",\
"wakarimaska", "wakarimasen", "samastihenh", "iye", "hai",\
"ni", "huei", "shwa", "potong hwa", "mah", "wa", "ideahr",\
"khairei", "hoc", "propter", "auch", "vielen",\
"forshtor", "deuw", "noshk", "puedo", "pero", "debajo" , "atras",\
"mientras", "desde", "nunca", "haka", "silflay", "embleer", "hraka",\
"mbembe", "mau mau","tiki","meka","leka","hai","haini","ho", \
"chonny","mola","chala","hala","hei","thlon","mey","nikto",\
"ecgum", "eyne", "sobre","dupa","zum","schlammpe","jodio","con",\
"narfle", "garthak", "hokanda matuso","oota goota","mafi","makka",\
"shukran", "akbar",\
"curiae", "aethelingas", "eorlas","carajo","oder",\
})

string translate(string str, int prof) {
    if( prof >= 100 ) return str;
    else {
	string *words;
	int fs, i, maxi;

	fs = sizeof(FLUBS);
	words = explode(str, " ");
	for(i=0, maxi = sizeof(words); i < maxi; i++)
	    if( random(100) >= prof ) words[i] = FLUBS[random(fs)];
	return implode(words, " ");
    }
}
