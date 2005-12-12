#include <lib.h>

inherit LIB_SENTIENT;

int AdvanceDude(mixed arg);
mapping advancement;

int TalkFunc(){
    int rand1;
    string thing1, thing2, thing3, thing4, thing5;

    rand1 = random(5);

    thing1 = "There's no shame in being wimpy. Live to fight another day. Death takes away valuable xp.";
    thing2 = "Let me know if you see Princess Daphne.";
    thing3 = "Don't fight drunk.";
    thing4 = "Learning spells from Herkimer is a good idea.";
    thing5 = "Food, drink, and caffeine help restore health and strength.";

    switch(rand1){
    case 1 : eventForce("say "+thing1);break;
    case 2 : eventForce("say "+thing2);break;
    case 3 : eventForce("say "+thing3);break;
    case 4 : eventForce("say "+thing4);break;
    case 5 : eventForce("say "+thing5);break;
    default :  eventForce("smile");
    }
}

static void create() {
    sentient::create();
    SetKeyName("dirk");
    SetId(({"dirk"}));
    SetShort("Dirk the Tired");
    SetLong("For 20 years, Dirk the Daring has been trying "
      "to defeat the vile dragon Singe, with almost no respite. "
      "Today he just wants to rest and relax, and has accepted "
      "the position of the town's Adventurers' Guild master. "
      "If you feel you deserve it, \"ask dirk to advance\".");
    SetLevel(15);
    SetRace("human");
    SetGender("male");
    SetAction(5, (: TalkFunc :));
    AddTalkResponse("hello", "hi!");
    SetCommandResponses( ([ 
	"advance": (: AdvanceDude :) 
      ]) );
    advancement = ([ 
      1:(["title":"the utter novice","xp":0,"qp":0]),
      2:(["title":"the simple novice","xp":1000,"qp":0]),
      3:(["title":"the beginner","xp":1500,"qp":0]),
      4:(["title":"the adventurer","xp":2300,"qp":0]),
      5:(["title":"the experienced adventurer","xp":3500,"qp":5]),
      6:(["title":"the expert adventurer","xp":5100,"qp":12]),
      7:(["title":"the great adventurer","xp":7700,"qp":21]),
      8:(["title":"the master adventurer","xp":12000,"qp":32]),
      9:(["title":"the Freeman","xp":17000,"qp":45]),
      10:(["title":"the Citizen","xp":26000,"qp":60]),
      11:(["title":"the Knight","xp":39000,"qp":77]),
      12:(["title":"the Baron","xp":59000,"qp":96]),
      13:(["title":"the Count","xp":88000,"qp":117]),
      14:(["title":"the Earl","xp":130000,"qp":140]),
      15:(["title":"the Marquis","xp":198000,"qp":165]),
      16:(["title":"the Duke","xp":297000,"qp":192]),
      17:(["title":"the Arch Duke","xp":444444,"qp":221]),
      18:(["title":"Praetor","xp":666667,"qp":252]),
      19:(["title":"Quaestor","xp":1000000,"qp":285]),
      20:(["title":"Caesar","xp":2000000,"qp":450]),
    ]);
}

int AdvanceDude(mixed arg){
    int level,xp,qp;
    int desired_level,required_xp,required_qp;
    mixed *statlist;
    mapping this_stat;
    int statclass, statlevel, i;

    statlist = this_player()->GetStats();

    if(!level = this_player()->GetLevel()){
	this_object()->eventForce("say You are confusing me.");
	return 1;
    }

    if(level > 19){
	this_object()->eventForce("say Whoa there, big "
	  "stuff. Advancement past level 20 is the "
	  "purview of the Trans-Human Elder Guild. "
	  "I'm sorry but I can't help you.");
	return 1;
    }


    xp = this_player()->GetExperiencePoints();
    if(!qp = this_player()->GetQuestPoints()) qp = 0;
    desired_level = level+1;
    required_xp = advancement[desired_level]["xp"];
    if(!required_qp = advancement[desired_level]["qp"]) required_qp = 0;

    this_object()->eventForce("say Level "+desired_level+" "
      "with the title of \""+advancement[desired_level]["title"]+"\" "
      "requires "+required_xp+" experience points and "+
      required_qp+" quest points.");

    if( xp > required_xp-1 && qp > required_qp-1){
	this_object()->eventForce("say Congratulations! "
	  "You are promoted to level "+desired_level+" and "
	  "have earned the name "+this_player()->GetName()+" "
	  +advancement[desired_level]["title"]+".");

	this_player()->SetLevel(desired_level);
	this_player()->AddTrainingPoints(desired_level);
	this_player()->AddTitle(advancement[desired_level]["title"]);
	this_player()->RemoveTitle(advancement[desired_level-1]["title"]);

	for(i=0;i<sizeof(statlist);i++){
	    this_stat = this_player()->GetStat(statlist[i]);
	    statclass = this_stat["class"];
	    statlevel = this_stat["level"];
	    this_player()->SetStat(statlist[i],statlevel + 1,statclass);
	}
	this_player()->save_player((string)this_player()->GetKeyName());

	return 1;
    }

    else this_object()->eventForce("say I'm sorry, "+
	  this_player()->GetName()+", but you have not "
	  "fulfilled all the requirements of level "+
	  desired_level+". Please come back and try "
	  "again once you have fulfilled them.");
    return 0;
}


