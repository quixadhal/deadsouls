// A courthouse where people can get married and you can check out the
// list of marriages.
// Due to TMI's strict non-sectarian policy (and, probably, the fact that
// so many of the coders are from California) marriages take place here
// rather than in a church or synagogue or what-have-you. You can change
// it if you feel you must... :)
// Leto did a 83s/0/1 22/5/95

#define MARRIAGE_FILE "/domains/Fooland/marriages.o"

#include <mudlib.h>

inherit ROOM ;

string *couples, *pending ;
int i, j, numcouples, numpending ;
string str ;

void add_pending(string str) ;
void perform_marriage(string s1, string s2) ;

void create() {
	::create();
	seteuid(getuid()) ;
	set ("objects", ([ "justice" : "/domains/Fooland/justice" ]) ) ;
	reset() ;
	set ("light", 1) ;
	set ("short", "The Footown Court House") ;
        set ("long",
	"You are in the Footown Court House. The Justice of the Peace here\n"+
	"will perform marriages between any two denizens of the MUD.\n"+
	"Type 'help marriage' to learn how to get married or see who has\n"+
	"gotten married.\n") ;
	set ("exits", ([ "south" : "/domains/Fooland/ebazz" ]) ) ;
	seteuid(getuid()) ;
	numpending = 0 ;
	pending = allocate(10) ;
	str = read_file(MARRIAGE_FILE,1,1) ;
	if (!str) {
		numcouples = 0 ;
		couples = allocate(1) ;
		return ;
	}
	if (sscanf(str,"%d",numcouples)!=1) {
		write ("Marriages file corrupt! Tell a wizard!\n") ;
		return ;
	}
	couples = allocate(numcouples+1) ;
	for (i=0;i<numcouples;i++) {
		str = read_file(MARRIAGE_FILE,i+2,i) ;
		sscanf (str,"%s\n",couples[i]) ;
	}
}

void init() {
	add_action("list_couples", "couples") ;
	add_action("married_to_whom","spouse") ;
	add_action ("get_married", "marry") ;
	add_action ("help", "help") ;
}

int help (string str) {
	if (str=="marriage") {
		write(
	"To get married, both MUDders should come to the court house and\n"+
	"type 'marry <spouse>'. When both have typed it, the marriage will\n"+
	"be performed.\n"+
	"Type 'couples' to get a list of recent marriages, or 'spouse\n"+
	"<name>' to find out who's married to whom.\n") ;
		return 1 ;
	}
	return 0 ;
}

int list_couples() {

	int i ;
	string s1, s2, date ;

	if (numcouples==0) {
		write ("No one has ever gotten married...<sniff>\n") ;
		return 1 ;
	}
	for (i=1;i<numcouples;i++) {
		sscanf (couples[i],"%s-%s %s",s1,s2,date) ;
		write (s1+" and "+s2+" got married at "+date+".\n") ;
	}
	return 1 ;
}

string find_spouse (string fellow) {

	object spouse ;
	string s1,s2,s3 ;

	if (numcouples==0) return "none" ;
	for (i=0;i<numcouples;i++) {
		sscanf(couples[i],"%s-%s %s",s1,s2,s3) ;
		if (s1==capitalize(fellow)) return s2 ;
		if (s2==capitalize(fellow)) return s1 ;
	}
	return "none" ;
}

int married_to_whom (string str) {

	string spouse ;

	if (!str) str = this_player()->query("cap_name") ;
	spouse = find_spouse(str) ;
	if (spouse=="none") {
		write (capitalize(str)+" is not married to anyone.\n") ;
		return 1 ;
	}
	write (capitalize(str)+" is married to "+spouse+".\n") ;
	return 1 ;
}

int get_married (string str) {

	object person ;
	string s1,s2 ;

	if (!str) {
		notify_fail("To whom do you want to get married?\n") ;
		return 0 ;
	}
	if (capitalize(str)==(string)this_player()->query("cap_name")) {
		notify_fail ("I'm sorry, the law does not permit you to marry yourself.\n") ;
		return 0 ; 
	}
	person = find_living ("justice") ;
	if (!person) {
		write ("The justice of the peace is not here to do that.\n") ;
		return 1 ;
	}
	if (this_player()->query("spouse")) {
		notify_fail("The justice looks through his records and says, 'I'm afraid you are already\nmarried.'\n") ;
		return 0 ;
	}
	if (find_spouse(capitalize(str))!="none") {
		notify_fail ("The justice looks through his records and says, 'I am sorry to have to tell\nyou this... but "+capitalize(str)+" is already married to someone else.'\n") ;
		return 0 ;
	}
	if (!find_player(str) || !present(str,environment(this_player()))) {
		notify_fail ("The justice peers at you and says, \"I'm sorry, but your beloved is not here.\"\n") ;
		return 0 ;
	}
	if (numpending==0) {
		add_pending(str) ;
		return 1 ;
	}
	for (i=0;i<numpending;i++) {
		sscanf(pending[i],"%s-%s",s1,s2) ;
		if (capitalize(str)==s2 && (string)this_player()->
			query("cap_name") == s1) {
			perform_marriage(s1,s2) ;
			if (i==numpending) {
				numpending = numpending - 1 ;
				return 1 ;
			}
			for (j=i+1;j<numpending;j++) {
				pending[j-1]=pending[j] ;
			}
			numpending = numpending - 1 ;
			return 1 ;
		}
	}
	add_pending(str) ;
	return 1 ;
}

void add_pending(string str) {

	string str2 ;
	object fiancee ;

	if (numpending==10) {
		notify_fail("There are already too many proposals pending. Try again a little later.\n") ;
		return 0 ;
	}
	str2 = this_player()->query("cap_name") ;
	pending[numpending] = capitalize(str)+"-"+str2 ;
	numpending = numpending + 1 ;
	write ("You proposal has been noted by the justice.\n") ;
	fiancee = find_player(str) ;
	tell_object(fiancee,str2+" has proposed marriage to you!\n") ;
}

void perform_marriage(string s1, string s2) {

	object spouse ;

	spouse = find_player(lower_case(s2)) ;
	if (!spouse) {
		notify_fail("That person has logged out... you can't marry them now.\n") ;
		return 0 ; 
	}
	spouse->set("spouse",s1) ;
	this_player()->set("spouse", s2) ;
	tell_object(spouse,"Congratulations! You have just become married to "+s1+"!\n") ;
	write ("Congratulations! You have just become married to "+s2+"!\n") ;
	shout ("You hear bells ringing in the distance.\n") ;
	couples[numcouples] = s1+"-"+s2+" "+ctime(time()) ;
	numcouples = numcouples + 1 ;
write ("ping") ;
	rm(MARRIAGE_FILE) ;
	write_file(MARRIAGE_FILE, numcouples+"\n") ;
	for (i=0;i<numcouples;i++) {
		write_file (MARRIAGE_FILE, couples[i]+"\n") ;
	}
	return ;
}
