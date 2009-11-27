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
        "meh", "nehi", "samastahum", "tora", "kia-ab", "boltehe", \
        "ata", "mevin", "ivrit", "ktzat", "lo", "ani", "tov","aval", \
        "verstehen", "sie", "bisschen", "sehr", "aber", "etwas", "keinen",\
        "shomo", "mifahmin", "mifahmam", "katalavenete", "hellenika",\
        "spreek", "uw", "vrauw", "nit", "shiz", "se","pericolo",\
        "iskit", "imhud", "halas", "sagud", "imche", "eigoga", "nihongo",\
        "wakarimaska", "wakarimasen", "samastihenh", "iye", "hai",\
        "ni", "huei", "shwa", "potong-hwa", "mah", "wa", "ideahr",\
        "khairei", "hoc", "propter", "auch", "vielen","lurgid",\
        "forshtor", "deuw", "noshk", "puedo", "pero", "debajo" , "atras",\
        "mientras", "desde", "nunca", "haka", "silflay", "embleer", "hraka",\
        "mbembe", "mau-mau","tiki","meka","leka","hai","haini","ho", \
        "chonny","mola","chala","hala","hei","thlon","mey","nikto",\
        "ecgum", "eyne", "sobre","dupa","zum","schlampe","jodio","con",\
        "narfle", "garthak", "hokanda-matuso","oota-goota","mafi","makka",\
        "shukran", "akbar", "mokele", "dapku", "loituma","plurdled",\
    "curiae", "aethelingas", "eorlas","carajo","oder",\
    "ae","aroha","awarua","atua","tenei", "dsfargeg",\
    "tangata","koa","kora","hupane","upane","krankor",\
    "baardaap","droogkloot","reetketelsteenbeeldhouwer","asbak","haka",\
    "zaadje","verkloten","teef","moffie","shele",\
    "anasi","atouche","ayir","boos","teezee","shawty",\
    "durka","haista","vittu","kyrpä","räkä","shawtay",\
    "suoro","voi","debil","kunel","shinel","sampo",\
    "sasikumea","zakila","txakurra","soustat","irrumator",\
    "mentula","koproskilo","malakas","la'a'za'zel","zayin",\
    "haakskeekah","noko","shizzle","nizzle","hizzle",\
    "desu","desu","desu","desu","desu","desu",\
    "desu","desu","desu","desu","desu","desu",\
    "desu","desu","desu","desu","desu","desu",\
    "UNITINU","STENDEC","CROATOAN","LATUALATUKA",\
    "LMKUU","puka","degozaimasu","szysznyk", "syzygy",\
    "darklock","confutatis","maledictis","flammis","acribus",\
    "addictis","XYNTA","durian","adzuki","flocka",\
    "Ð¥Ð£Ð˜Ð¢Ð", "NIBIRU", "tatonka","tralfamadore",\
    "sihulm","koyaanisqatsi","tanstaafl","misunderestimate",\
    "ar","ot","ti","fi","na",\
    "ar","ot","ti","fi","na",\
    "ar","ot","ti","fi","na",\
    "ar","ot","ti","fi","na",\
    "ar","ot","ti","fi","na",\
    "ar","ot","ti","fi","na",\
    "o","u","y","e",\
    "o","u","y","e",\
    "o","u","y","e",\
    "o","u","y","e",\
    "o","u","y","e",\
    "o","u","y","e",\
    "ki","ika","ita","ite","ni",\
    "ki","ika","ita","ite","ni",\
    "ip","ip","ip","ip","ip","ip",\
    "ip","ip","ip","ip","ip","ip",\
    "iq","iq","iq","iq","iq","iq",\
    "iq","iq","iq","iq","iq","iq",\
    "ig","ig","ig","ig","ig","ig",\
    "ig","ig","ig","ig","ig","ig",\
    "kii-oto","kii-oto","kii-oto","kii-oto","kii-oto",\
    "kii-ateh","kii-ateh","kii-ateh","kii-ateh","kii-ateh",\
    "galanga","wanga","ivlivs","kokoh-chula","birichoto",\
    "bukkakke","santorum","habeebit","yotsuba", "obama",\
    "mccain","biden","palin","chitonw","wachovia","wamu",\
    "dogoodi","idoogod","yamaglonchi","hotchka","tacgnol",\
    "zangano","zafacon","doushnou","yanaglonchi","rowsdower",\
    "gleepy","gloop","gloopy","neeby","nabba","nooby","la","la","la",\
    "lo","lo","sabba","sibby","sabba","nooby","abba","nabba",\
    "le","le","le","lo","lo","tooby","ooby","walla","dede","scozzafava",\
    "AUSCANZUKUS",\
})

string translate(string str, int prof) {
    if( prof >= 100 ) return str;
    else {
        string *words;
        int fs, i, maxi;

        fs = sizeof(FLUBS);
        words = explode(str, " ");
        for(i=0, maxi = sizeof(words); i < maxi; i++){
            string prefix = "";
            string suffix = "";
            if(!alphap(first(words[i],1))) prefix = first(words[i],1);
            if(!alphap(last(words[i],1))) suffix = last(words[i],1);
            if( random(100) >= prof ) words[i] = prefix+FLUBS[random(fs)]+suffix;
        }
        return implode(words, " ");
    }
}
