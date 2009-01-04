inherit "/lib/std/room";

void create() {
    ::create();
    SetShort("between the branches");
    SetLong(
            "Here, between two strong branches, a small platform (with railing) has "+
            "been installed. The wind rustles the leaves of the tree, a pleasant "+
            "sound that helps keep your mind off the fact that if you fell, you would "+
            "make a very unpleasant-looking mess on the pavement below. But don't "+
            "worry, there have been no fatalities from here... yet. A telescope "+
            "stands near each corner of the platform. ");
    SetProperties( (["light"     : 2, 
                "indoors"   : 1 ,
                "no castle" : 1]) );
    SetExits( (["down" : "/domains/Praxis/trunk"]) );
    SetItems( ([
                "telescope" : "Through this magically enhanced telescope, you can see the "+
                "whole world.. you can look at various directions and "+
                "places, broadening your horizons till they overlap those "+
                "of the world. With this telescope, you realize, you can "+
                "add much to your knowledge. ",
                "platform"  : "The platform is very sturdy. Standing on it, you feel "+
                "much more comfortable than if you had been standing a "+
                "few meters to your left--in mid-air. ",
                "railing"   : "Having a railing immensely increases your sense of "+
                "security, quite understandably. ",
                "branches"  : "The broad branches here easily support the platform. ",
                "leaves"    : "The leaves provide a pleasant green canopy. Even though "+
                "this tree is deciduous, it stays green all year long. ",
                "tree"      : "You lean back from the telescope and take a moment to look at "+
                "your surroundings, the branches of the Tree of Mages. As "+
                "legend has it, the Tree is as old as the world; some say "+
                "older, but few believe that. It is a fact, though, that the "+
                "Tree of Mages has, through millenia of association with "+
                "powerful mages, acquired tremendous magic and, according to "+
                "some, even intelligence. Today the Tree serves as the class "+
                "hall of the Class of Mages.",

                ({"n", "north"}) : "North of the Tree lie the dark expanses of the North "+
                                    "Forest, a place of fear and wonder, great danger and "+
                                        "great treasure. Far off in the distance you see a city "+
                                        "consisting mainly of a huge library--probably the "+
                                        "Philosophers' city of Cartesia. Still farther, at the "+
                                        "very horizon, you see the gleaming of pristine snow at "+
                                        "what must be the north pole.",
                                    ({"e", "east"}) : "East of the mage tree lie the northern reaches of Krasna. "+
                                                       "Nearby a few buildings lie, whereas farther off the "+
                                                           "north-south road Centre Path connects with the "+
                                                           "eastward-leading Sun Alley. A hospital lies west of that "+
                                                           "juncture, while Sun Alley harbors a classy restaurant, "+
                                                           "and a small residential area mainly occupied by "+
                                                           "aristocrats fallen on hard times.  Sun Alley ends when "+
                                                           "it connects with East Road. East of Sun Alley is the "+
                                                           "Cemetery, where the restless spirits of the immortals "+
                                                           "make their home. East Road leads northwards, to the "+
                                                           "gates of Newbieland. North of the cemetery, a small "+
                                                           "path connects with East Road and leads yet eastward, "+
                                                           "over a plateau. Dragons are said to make their lair "+
                                                           "beyond the plateau.",
                                                       ({"w", "west"}) : "To the west a vast desert lies, crossed by a range of "+
                                                                          "towering mountains. The Desert Highway slices the "+
                                                                              "barrenness of the desert as it leads westwards, its "+
                                                                              "farthest expanses cloaked by shimmering heat waves... "+
                                                                              "Nearby a strange valley lies, somewhat to the north of "+
                                                                              "the Desert Highway. In the distance, you can barely "+
                                                                              "glimpse the glistening white spires of a desert city.",

                                                                          ({"s", "south"}) : "South of the Mage Tree lies West Road, the north-south "+
                                                                                              "road that marks the western limit of Krasna. It "+
                                                                                                  "connects at the southern end with Boc-La Road, the main "+
                                                                                                  "road of the city.  A hut to the eastern side of the "+
                                                                                                  "road, slightly south of the tree, catches your eye.",
                                                                                              ({"se", "southeast"})
                                                                                                  : "Southeast of the Mage Tree lies the main body of the "+
                                                                                                  "city of Krasna. West Road leads south from the Tree, "+
                                                                                                  "and ends at Boc-La, the east-west road that has been "+
                                                                                                  "called ``the main artery of Krasna''. Boc-La's western "+
                                                                                                  "end begins at West Road's southern end, and from "+
                                                                                                  "there it leads east, passing through Monument "+
                                                                                                  "Square and continuing eastwards toward the dark Eastern "+
                                                                                                  "Jungle, where savages, cannibals and rogues make their "+
                                                                                                  "home. Along the way, Boc-La passes through Lars' Bar, "+
                                                                                                  "the establishment of the Sage of Languages, the local "+
                                                                                                  "Monastery, Horace's Supply shop, and other famous "+
                                                                                                  "sites. At its eastern end, just before heading "+
                                                                                                  "into the jungle, Boc-La road has a small gate leading "+
                                                                                                  "south to the fabled docks of the Flying Galleys. From "+
                                                                                                  "Monument Square another main road, Centre Path, leads "+
                                                                                                  "north and south. North, Centre Path passes by the "+
                                                                                                  "church, a hospital and Sun Alley before heading to the "+
                                                                                                  "North Forest; south, it leads past the bar to south "+
                                                                                                  "Krasna, where the business sector lies (and where the "+
                                                                                                  "bank and post office are located).  Yet southwards, "+
                                                                                                  "Centre Path leads to the headquarters of the class of "+
                                                                                                  "fighters and, eventually, all the way south are the "+
                                                                                                  "piers where fishermen cast nets to provide the local "+
                                                                                                  "restaurant with fresh seafood.",
                                                                                              ({"nw", "northwest"})
                                                                                                  : "The North Forest is abruptly arrested by a range of "+
                                                                                                  "mountains lying west and slightly north of the Mage "+
                                                                                                  "Tree. On the other side of the mountains lies the desert.",
                                                                                              ({"ne", "northeast"})
                                                                                                  : "As it leaves Krasna to the north, Centre Path turns "+
                                                                                                  "into a trail winding through the North Forest. Slightly "+
                                                                                                  "north of the city limits an east-west path leads from "+
                                                                                                  "the remains of Centre Path. Passing by this path, the "+
                                                                                                  "trail continues north, past a hole in the ground, then "+
                                                                                                  "curves northwest for a short while, and when it curves "+
                                                                                                  "back north, it has joined the North Forest Highway. "+
                                                                                                  "The Highway goes north for a long time, passing by in "+
                                                                                                  "turn a bog, an exit going west to some city, a tower "+
                                                                                                  "and a house, and finally reaching the polar ice cap "+
                                                                                                  "at the very horizon.",

                                                                                              ({"valley", "strange valley"})
                                                                                                  : "This strange valley to the north of the Great Western "+
                                                                                                  "Highway (sometimes known as the Desert Highway) is "+
                                                                                                  "obviously artificial. Its clumsy manufacture leads you "+
                                                                                                  "to believe that it was constructed by some particularly "+
                                                                                                  "maladroit race, perhaps by orcs.",
                                                                                              ({"desert highway", "western highway", "great western highway"})
                                                                                                  : "Constructed many years ago to allow trade between "+
                                                                                                  "Krasna and the cities beyond the desert, this highway "+
                                                                                                  "is protected by magic against sand storms. It provides "+
                                                                                                  "no protection against heat, however; sometimes one can "+
                                                                                                  "see a small bag, or trinket, that is all that remains of "+
                                                                                                  "an unfortunate traveller whose body has long since "+
                                                                                                  "become dust.",
                                                                                              ({"spires", "desert city"})
                                                                                                  : "The shimmering is too strong for you to make out any "+
                                                                                                  "details, but you can glimpse the white spires of a "+
                                                                                                  "desert city at the end of the Great Desert Highway.", 
                                                                                              ({"mountains", "desert mountains"})
                                                                                                  : "A mighty mountain range crosses the desert to the west "+
                                                                                                  "of Krasna. It is rumored to harbour orcs, goblin caves, "+
                                                                                                  "stirges and all kinds of nasty creatures.",
                                                                                              "centre path"    : "A road leading north and south from Monument Square, "+
                                                                                                  "the heart of Krasna. Along it lie some of the most "+
                                                                                                  "famous establishments of Krasna, not least being "+
                                                                                                  "Lars' Pub and the Clerics' Hospital.",
                                                                                              "east road"      : "A north-south road at the eastern limit of Krasna, "+
                                                                                                  "East Road connects with Sun Alley at the cemetery. "+
                                                                                                  "Its northern end leads to Newbieland.",
                                                                                              "sun alley"      : "An east-west alley leading from the hospital at its "+
                                                                                                  "western end to the cemetery at its eastern end "+
                                                                                                  "(coincidence?), Sun Alley is home to the Last Dragon "+
                                                                                                  "Restaurant and also to barons and knights who have "+
                                                                                                  "fallen on hard times.",
                                                                                              "cemetery"       : "Praxis Cemetery is already occupied by the ghosts of "+
                                                                                                  "the immortals. Thus, for new entrants it is strictly "+
                                                                                                  "Standing Room Only.",
                                                                                              "newbieland"     : "Newbieland lies north of East Road; a force-field "+
                                                                                                  "surrounding it prevents you from seeing anything else.",
                                                                                              "hospital"       : "The Clerics' Hospital was established many years ago "+
                                                                                                  "by the kind class of Clerics, who have their class "+
                                                                                                  "head-quarters in the church just south of the hospital. "+
                                                                                                  "Normally they charge a fee to cover their expenses, but "+
                                                                                                  "in certain cases they make exceptions for first-level "+
                                                                                                  "players.",
                                                                                              "restaurant"     : "The Last Dragon Restaurant is the classiest restaurant "+
                                                                                                  "in Krasna! Located in Sun Alley, once a prestigious "+
                                                                                                  "neighbourhood, this restaurant uses only the freshest "+
                                                                                                  "ingredients.  Needless to say it also charges a lot.",
                                                                                              "plateau"        : "East of Krasna lies a plateau. Dragons are said to lie "+
                                                                                                  "beyond, and horridly injured travellers returning from "+
                                                                                                  "there confirm the tale.",
                                                                                              "square"         : "Monument Square, once known as Krasna Square, is the "+
                                                                                                  "teeming heart of Krasna City.  People come there to "+
                                                                                                  "converse, meet, receive healing if a cleric is there, "+
                                                                                                  "and generally relax. The immortals have graciously put "+
                                                                                                  "protective forces on the Square to prevent and fighting, "+
                                                                                                  "stealing and any spell-casting except heal-craft.",
                                                                                              ({"krasna", "praxis", "city"}) 
                                                                                                  : "Krasna City is the major city in this reality. All of "+
                                                                                                  "the class halls are located in or near it (except the "+
                                                                                                  "class hall of the rogues, who have been banished to the "+
                                                                                                  "jungle). Once known as Praxis, Krasna has a rich history "+
                                                                                                  "and a lively commerce with other, lesser cities such as "+
                                                                                                  "the desert city, Arberth, Cartesia and many more.\n"+
                                                                                                  "                         To Global Newbieland        A - Mage Tree\n"+
                                                                                                  "                                                     B - Hospital\n"+
                                                                                                  "                                  |                  C - Post Office\n"+
                                                                                                  "                                  +                  D - Square\n"+
                                                                                                  "                                  |                  E - Monastery\n"+
                                                                                                  "                  Here there      |                  F - Free equipment\n"+
                                                                                                  "                  be dragons   F--+                  G - Horace's Store\n"+
                                                                                                  "                      |           |                  H - Boards\n"+
                                                                                                  "                      |           |   Here there     I - Library\n"+
                                                                                                  "          A           +           +-- be dragons\n"+
                                                                                                  "          |           |           |\n"+
                                                                                                  "          |           |           |\n"+
                                                                                                  " T        +--+     B--+---+---+---+-- To Graveyard\n"+
                                                                                                  " o        |           |   |   |   |\n"+
                                                                                                  "          |           |   +   +   |\n"+
                                                                                                  " M        +           +           +---I\n"+
                                                                                                  " o        |           |   E   G   |\n"+
                                                                                                  " u        |           |   |   |   |\n"+
                                                                                                  " n ---+---+---+----+--D---+---+---+---+-- To Outlands\n"+
                                                                                                  " t            |    |  |       |   |\n"+
                                                                                                  " a            +    +--+       H-+ |\n"+
                                                                                                  " i                    |           +\n"+
                                                                                                  " n            +   +   |       To Ships\n"+
                                                                                                  " s            |   |   |\n"+
                                                                                                  "          +---+---+---+---+\n"+
                                                                                                  "              |   |   |\n"+
                                                                                                  "              |   |   |\n"+
                                                                                                  "              +   C   +\n"+
                                                                                                  "                     /\n"+
                                                                                                  "                    /\n"+
                                                                                                  "                   +\n"+
                                                                                                  "                   |\n"+
                                                                                                  "                   |\n"+
                                                                                                  "                   +",
                                                                                              "bar"            : "When this reality was created, Lars' Bar was one of the "+
                                                                                                  "first establishments to operate. Lars spared no expense "+
                                                                                                  "in providing the very best atmosphere for players to "+
                                                                                                  "relax in. Recent anti-alcohol measures, notably a tax on "+
                                                                                                  "drinks, have caused Lars' Bar to lose some of its "+
                                                                                                  "grandeur, however.  Lars tried at first to draw more "+
                                                                                                  "customers by publishing a list of the most powerful "+
                                                                                                  "adventurers in this world, but when that failed to bring "+
                                                                                                  "enough customers Lars had to resort to increasing "+
                                                                                                  "prices. Still, despite ever-rising prices, if you really "+
                                                                                                  "need a drink then Lars' Bar is *the* place to go; his "+
                                                                                                  "profits may have gone down but his drinks are still as "+
                                                                                                  "powerful as they ever were.",
                                                                                              "church"         : "The local church is where the class of clerics make "+
                                                                                                  "their home. Ever willing to help their kindred, the "+
                                                                                                  "clerics of Nightmare have established a hospital just "+
                                                                                                  "north of the church, where for a fee one can get healing "+
                                                                                                  "and magical energy, and even limbs can be restored.",
                                                                                              "monastery"      : "Home of the powerful class of monks, the monastery is a "+
                                                                                                  "proud and solemn place where many of the sacred rituals "+
                                                                                                  "of Nightmare take place. It is in the main hall of the "+
                                                                                                  "monastery that ghosts come to pray for resurrection "+
                                                                                                  "after being killed, and it is in a chapel in the "+
                                                                                                  "monastery that couples are joined in marriage. Located "+
                                                                                                  "north of Boc-La, slightly to the east of Monument "+
                                                                                                  "Square, the monastery is awarded deep respect by most "+
                                                                                                  "members of society.",
                                                                                              ({"docks", "galleys"})
                                                                                                  : "At the very east end of Boc-La a gate leads south to "+
                                                                                                  "the magic docks of Krasna where flying galleys are "+
                                                                                                  "purchased, to bring travellers to different realms.",
                                                                                              "shop"           : "Horace, one of the first true-blooded elves to come to "+
                                                                                                  "Nightmare, promptly set up shop near the east end of "+
                                                                                                  "Krasna. Taking advantage of his position, Horace buys "+
                                                                                                  "low and sells high. He is noted for his cold-blooded "+
                                                                                                  "mercantility and his crass profiteering; since his store "+
                                                                                                  "is, however, one of the few places where newbies can "+
                                                                                                  "sell their gear, the High Mortals of Nightmare "+
                                                                                                  "discourage people from killing him.",
                                                                                              "piers"          : "Located at the very southern end of Krasna, the piers "+
                                                                                                  "are frequently visited by hopeful players with fishing "+
                                                                                                  "poles on their shoulders and a lot of time on their "+
                                                                                                  "hands.",
                                                                                              ({"bank",
                                                                                               "post office"})  : "South of Monument Square an east-west alley marks the "+
                                                                                                                   "business sector of Krasna. To the east it leads to the "+
                                                                                                                       "sewers, and to the west it leads to the bank and the "+
                                                                                                                       "post office. The bank is where people open an account to "+
                                                                                                                       "store their cash; no interest payments, unfortunately. "+
                                                                                                                       "At the post office mail is handled, sent, and received.",
                                                                                                                   "jungle"         : "To the east of Krasna lies the dark Eastern Jungle. Its "+
                                                                                                                       "inhabitants include cannibals, savages, rogues, and "+
                                                                                                                       "other vermin.  At the far end of the jungle a mysterious "+
                                                                                                                       "tower is rumored to lie, but few have returned to tell "+
                                                                                                                       "the tale.  The few who have speak of death traps, doors "+
                                                                                                                       "that lock behind you and other chilling details.",
                                                                                                                   "sage"           : "The Sage dwells in a small hut north of Boc-La, near "+
                                                                                                                       "the western end of the city. He is the person in charge "+
                                                                                                                       "of instructing players in the proper use of various "+
                                                                                                                       "languages.",

                                                                                                                   ({"forest", "north forest"})
                                                                                                                       : "A dark place of many legends and a thousand tales, the "+
                                                                                                                       "North Forest is home to a myriad of creatures, ranging "+
                                                                                                                       "from evil faeries to wicked wolves, from trolls to "+
                                                                                                                       "dragons to centaurs.  It is even said that the gates of "+
                                                                                                                       "hell lie somewhere in the North Forest. And yet the "+
                                                                                                                       "forest harbours heros, and scholars, paladins and "+
                                                                                                                       "seekers of truth...  and treasures beyond the wildest "+
                                                                                                                       "dreams of avarice...  Fortunately for travellers, the "+
                                                                                                                       "North Forest Highway provides magical protection from "+
                                                                                                                       "monsters, but even the highway offers no shield from "+
                                                                                                                       "the bandits and highwaymen who frequently beset the "+
                                                                                                                       "unwary.",
                                                                                                                   ({"north pole", "pole", "snow"})
                                                                                                                       : "At the north horizon, where sky embraces earth in a "+
                                                                                                                       "white-grey swirl, lies the North Pole.  Once the "+
                                                                                                                       "residence of one Mr. S. Claus and his elves, sometimes "+
                                                                                                                       "referred to as subordinate clauses, the North Pole has "+
                                                                                                                       "been ruthlessly taken over by a tribe of ferocious "+
                                                                                                                       "giants who evicted Mr. Claus and turned his helpers "+
                                                                                                                       "into yummy bite-sized (by giant standards) pieces of "+
                                                                                                                       "raw meat.  At least, that is what is claimed by the few "+
                                                                                                                       "who have returned from there; but they may well have "+
                                                                                                                       "suffered trauma from the cold and entered a "+
                                                                                                                       "hallucinatory state.",
                                                                                                                   "west road"      : "This north-south road harbours two important magical "+
                                                                                                                       "establishments, the legendary Tree of Mages and the "+
                                                                                                                       "less well-known hut of Mora the Teller of Fortunes. The "+
                                                                                                                       "Tree is at the north end of the road, Mora's hut is "+
                                                                                                                       "somewhat to the south. The south end of the road "+
                                                                                                                       "connects to the western end of Boc-La.",
                                                                                                                   "boc-la"         : "This road runs east-west through the heart of the "+
                                                                                                                       "city. To the west it merges with the western highway, "+
                                                                                                                       "and to the east it dissappears in the jungle. The road "+
                                                                                                                       "passes through the square, and many important "+
                                                                                                                       "establishments can be found just off it. Horace's shop "+
                                                                                                                       "and the adventurer's hall flank the road in the east. "+
                                                                                                                       "Nearer to the tree you can see Lars' pub, and the "+
                                                                                                                       "Sage's house is opposite a bit of farmlands.",
                                                                                                                   "hut"            : "Mora's hut lies south and east of the Mage Tree.  "+
                                                                                                                       "Training the telescope on the sign above the door, you "+
                                                                                                                       "make out that Mora is a fortune-teller specializing in "+
                                                                                                                       "horoscopes.",
                                                                                                                   "library"        : "You cannot clearly make out the library, as your view "+
                                                                                                                       "is obstructed.",
                                                                                                                   "cartesia"       : "Far north of Krasna lies the famous Philosopher City "+
                                                                                                                       "of Cartesia, home to the largest known library in the "+
                                                                                                                       "world.  The Philosophers of Cartesia, or Seekers of "+
                                                                                                                       "Truth as they are sometimes called, have long been a "+
                                                                                                                       "major force, exploring new areas, seeking out new life, "+
                                                                                                                       "and new civilizations, boldly going where no player has "+
                                                                                                                       "gone before.",
                                                                                                                   "north highway"  : "A magical path leading through the North Forest, the "+
                                                                                                                       "North Forest Highway is magically protected against "+
                                                                                                                       "monsters, thus ensuring free passage to those who are "+
                                                                                                                       "not stupid.. err..  bold enough to wander off it. "+
                                                                                                                       "However, it does not offer protection against bandits "+
                                                                                                                       "and highwaymen. The Highway connects with a path "+
                                                                                                                       "leading north from Krasna, forming a single, long road "+
                                                                                                                       "from the heart of civilization (Krasna) to, "+
                                                                                                                       "essentially, the polar icecap. A ways north of its "+
                                                                                                                       "southern end, the Highway has an exit leading to the "+
                                                                                                                       "rationally-designed, empirically-built and dogmatically-"+
                                                                                                                       "defended city of philosophers, Cartesia.",
                                                                                                                   "horizon"        : "You do not notice that here. duh.",
                                                                                                                   "hole"           : "North of the city, right before the remains of Centre "+
                                                                                                                       "Path curve northwest, lies a hole in the ground with a "+
                                                                                                                       "reddish glow coming out of it. That hole is reputed to "+
                                                                                                                       "be the lair of the evil demon-worshipping class known "+
                                                                                                                       "as the Class of Kataans.",
                                                                                                                   "tower"          : "The tower is obscured by trees.",
                                                                                                                   "house"          : "A fairly large mansion on the lies west of the North "+
                                                                                                                       "Highway, somewhat to the north of the exit to the "+
                                                                                                                       "Philosopher City. Not much is known about it, except "+
                                                                                                                       "that a necromancer might be living in it.",
                                                                                                                   "bog"            : "At the south end of the North Forest Highway a path "+
                                                                                                                       "leads east to a diseased-looking bog. Many explorers "+
                                                                                                                       "have gone there, and quite a few have come back.  Those "+
                                                                                                                       "who came back all had one thing in common: they had "+
                                                                                                                       "wimpy set ``on''.  They report of nasty beasts, horrid "+
                                                                                                                       "insects, ghoulish graveyards and some other.. well.. "+
                                                                                                                       "weird things.",
                                                                                                                   "exit"           : "Several miles north of where the North Highway "+
                                                                                                                       "connects with the trail that is the northern extension "+
                                                                                                                       "of Centre Path, an exit leads west from the Highway "+
                                                                                                                       "toward a small city with a big library."]) ) ; }
                                                                                                                       void init(){
                                                                                                                           ::init();
                                                                                                                       }
