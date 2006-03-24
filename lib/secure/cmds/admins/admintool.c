#include <lib.h>
#include <privs.h>
#include <daemons.h>
#include <config.h>

inherit LIB_DAEMON;

varargs int Menu(string str);
int MainMenu();
int MainMenuChoice();
int GeneralMenu();
int DaemonMenu();
int UsersMenu();
int DriverMenu();
int GroupsMenu();
int ToggleMudLock();
int ShowLock();
int SetTZ();
int eventSetTZ(string str);
int ChangeEmail();
varargs int eventChangeEmail(string str, int auto);
int SetReboot();
int eventSetReboot(int i);
int AddClass();
int eventAddClass(string str);
int RemoveClass();
int eventRemoveClass(string str);
int AddRace();
int eventAddRace(string str);
int RemoveRace();
int PlayerRace(string str);
int eventRemoveRace(string str);
int AddCurrency();
int eventAddCurrency(string str);
int CurrencyRate(string str);
int CurrencyWeight(string str);
int CurrencyInflation(string str);
int RemoveCurrency();
int eventRemoveCurrency(string str);
int EncrePlayer();
int eventEncrePlayer(string str);
int DecreCreator();
int eventDecreCreator(string str);
mixed EnCre(string args);
mixed DeCre(string args);
int RidUser();
int eventRidUser(string str);
int LogRid(string str);
int DoRid(string who);
int BanishUser();
int eventBanishUser(string str);
int UnBanishUser();
int eventUnBanishUser(string str);
int ChangeName();
int eventChangeName(string str);
int ChangePort();
int eventChangePort(int i);
int ShutDownMud();
int eventShutDownMud(string str);
int RemoveGroup();
int eventRemoveGroup(string str);
int AddGroup();
int eventAddGroup(string str);
int ModGroup();
int eventModGroup(string str);
int eventEditGroup(string str);
int ShowGroups();

string process_input(string str);

string global_group_temp, menu, racepath, currency, ridded, globalstr, file;
string *currencies;

float rate, weight, inflation; 

mixed InvalidChoice(){
    write("Invalid choice. Try again or choose q to quit.\n");
    Menu();
    return 1;
}

int PlaceHolder(){
    write("This function has not yet been defined.");
    Menu();
    return 1;
}

varargs int Menu(string str){

    if(str) menu = str;
    if(!menu) menu = "main";

    switch (menu){
    case "main" : MainMenu(); break;
    case "general" : GeneralMenu(); break;
    case "daemon" : DaemonMenu();break;
    case "users" : UsersMenu();break;
    case "driver" : DriverMenu();break;
    case "groups" : GroupsMenu();break;
    default : menu = "main"; MainMenu(); break;
    }
    return 1;
}

string process_input(string str){
    //write("Input is "+str+".");
    switch (str) {
    case "q" : write("Ok, quitting admintool.\nBye.\n");return " ";
    case "x" : write("Ok, quitting admintool.\nBye.\n");return " ";
    case "y" : write("Ok, quitting admintool.\nBye.\n");return " ";
    case "z" : Menu("main"); break;
    case "1" : Menu("general"); break;
    case "2" : Menu("daemon");break;
    case "3" : Menu("users");break;
    case "4" : Menu("driver");break;
    case "5" : Menu("groups");break;
    case "a" : ToggleMudLock();break;
    case "b" : ShowLock();break;
    case "c" : SetTZ();break;
    case "d" : ChangeEmail();break;
    case "e" : SetReboot();break;
    case "f" : AddClass();break;
    case "g" : RemoveClass();break;
    case "h" : AddRace();break;
    case "i" : RemoveRace();break;
    case "j" : AddCurrency();break;
    case "k" : RemoveCurrency();break;
    case "l" : EncrePlayer();break;
    case "m" : DecreCreator();break;
    case "n" : RidUser();break;
    case "o" : BanishUser();break;
    case "p" : UnBanishUser();break;
    case "r" : ChangeName();break;
    case "s" : ChangePort();break;
    case "t" : AddGroup();break;
    case "u" : RemoveGroup();break;
    case "v" : ModGroup();break;
    case "w" : ShowGroups();break;
    case "SD": ShutDownMud();break;
    default : InvalidChoice();
    }
    return "(process_input fun finished.)";
}


varargs mixed MainMenu(string str) {    
    string tmp;
    tmp = "\tDead Souls Admin Tool\n";
    tmp += "\t=====================\n\n";
    tmp += "\t\t1) general\n";
    tmp += "\t\t2) daemon\n";
    tmp += "\t\t3) users\n";
    tmp += "\t\t4) driver\n";
    tmp += "\t\t5) groups\n\n";
    tmp += "\t\tq) quit\n";

    this_player()->eventPrint(tmp, "foo");

    input_to((: process_input :));
    return 1;
}

static int GeneralMenu() {    
    string tmp;

    tmp = "\tDead Souls Admin Tool General Menu\n";
    tmp += "\t\n\n";
    tmp += "\t\ta) toggle mud lock\n";
    tmp += "\t\tb) display current lock status\n";
    tmp += "\t\tc) change timezone\n";
    tmp += "\t\td) change admin email\n";
    tmp += "\t\te) change shutdown interval\n";
    tmp += "\t\tSD) shut down the MUD\n\n";
    tmp += "\t\tz) return to main menu\n";
    tmp += "\t\tq) quit\n";

    this_player()->eventPrint(tmp, "foo");
    input_to((: process_input :));
    return 1;
}

int DaemonMenu() {    
    string tmp;

    tmp = "\tDead Souls Admin Tool Daemon Menu\n";
    tmp += "\t\n\n";
    tmp += "\t\tf) add class\n";
    tmp += "\t\tg) remove class\n";
    tmp += "\t\th) add race\n";
    tmp += "\t\ti) remove race\n";
    tmp += "\t\tj) add currency\n";
    tmp += "\t\tk) remove currency\n\n";
    tmp += "\t\tz) return to main menu\n";
    tmp += "\t\tq) quit\n";

    this_player()->eventPrint(tmp, "foo");
    input_to((: process_input :));
    return 1;
}

int UsersMenu() {    
    string tmp;

    tmp = "\tDead Souls Admin Tool Daemon Menu\n";
    tmp += "\t\n\n";
    tmp += "\t\tl) promote a player to creator\n";
    tmp += "\t\tm) demote a creator to player\n";
    tmp += "\t\tn) completely erase a user\n";
    tmp += "\t\to) banish a username\n";
    tmp += "\t\tp) unbanish a username\n\n";
    tmp += "\t\tz) return to main menu\n";
    tmp += "\t\tq) quit\n";

    this_player()->eventPrint(tmp, "foo");
    input_to((: process_input :));

    return 1;
}

int DriverMenu(){
    string tmp;

    tmp = "\tDead Souls Admin Tool Driver Menu\n";
    tmp += "\t^^^^^^^^^^^^^^^^^^^^^^*******^^^^^\n";
    tmp += "\t\n\n";
    tmp += "\t\tr) change the MUD's name\n";
    tmp += "\t\ts) change the MUD's connection port\n\n";
    tmp += "\t\tz) return to main menu\n";
    tmp += "\t\tq) quit\n";

    this_player()->eventPrint(tmp, "foo");
    input_to((: process_input :));

    return 1;
}

int GroupsMenu(){
    string tmp;

    tmp = "\tDead Souls Admin Tool Groups Menu\n";
    tmp += "\t^^^^^^^^^^^^^^^^^^^^^^*******^^^^^\n";
    tmp += "\t\n\n";
    tmp += "\t\tt) Add a group\n";
    tmp += "\t\tu) Remove a group\n";
    tmp += "\t\tv) Modify a group\n";
    tmp += "\t\tw) Show groups\n\n";
    tmp += "\t\tz) return to main menu\n";
    tmp += "\t\tq) quit\n";

    this_player()->eventPrint(tmp, "foo");
    input_to((: process_input :));

    return 1;
}

string GetHelp(string str) {
    return ("Syntax: <admintool>\n\n"
      "Menu-driven tool used to edit the MUD's configuration.\n"
      "To change mud name or port number, choose the Driver menu.\n"
      "Use the Users menu for promoting a player to creator status.\n"
      "Use the Groups menu to make someone an assistant admin.\n");
}

int ToggleMudLock(){
    string *line_array;
    string lockline, newline, newfile, line_string, junk;
    int num;
    line_string = read_file("/secure/include/config.h");
    if(!sizeof(line_string)) write("Couldn't read file.");
    line_array = explode(line_string, "\n");
    if(!sizeof(line_array)) write("Array is zero length.");

    foreach(string line in line_array){
	if(strsrch(line,"MUD_IS_LOCKED") != -1){
	    lockline = line;
	}
    }
    if(sscanf(lockline,"%s%d",junk, num) < 2) {
	write("Operation failed. You need to hand-"+
	  "edit /secure/include/config.h immediately.");
	return 0;
    }
    if(num == 0) {
	write("Locking mud.\n");	
	num = 1;
    }
    else {
	write("Unlocking mud.\n");
	num = 0;
    }
    newline = junk + num;
    newfile = replace_string(line_string, lockline, newline);
    write_file("/secure/include/config.h",newfile,1);
    load_object("/secure/cmds/creators/update")->cmd("/secure/daemon/master");
    load_object("/secure/cmds/creators/update")->cmd("/secure/lib/connect");
    write("\n");
    Menu();
    return 1;
}

int ShowLock(){
    if(load_object("/secure/daemon/master")->is_locked()){
	write("Mud is locked. Only admins and designated personnel can log in.\n");
    }
    else write("Mud is unlocked. Players are free to login.\n");
    Menu();
    return 1;
}


int SetTZ(){
    write("Please enter the desired time zone: \n");
    input_to( (: eventSetTZ :) );
    return 1;
}

int eventSetTZ(string str){
    string ret;

    ret = set_tz(str);
    write(ret+"\n");
    load_object("/secure/cmds/creators/update")->cmd("/daemon/time");
    Menu();
    return 1;
}

int ChangeEmail(){
    write("Current admin email is: "+admin_email());
    write("Please enter the new email address of the mud administrator: \n");
    input_to( (: eventChangeEmail :) );
    return 1;
}

varargs int eventChangeEmail(string str, int auto){
    string *line_array;
    string lockline, newline, newfile, line_string, junk, email;
    if(strsrch(str,"@") == -1 ) {
	write("That isn't a valid email address.");
	if(!auto) Menu();
	return 0;
    }
    str == replace_string(str, "#", "");
    line_string = read_file("/secure/include/config.h");
    if(!sizeof(line_string)) write("Couldn't read file.");
    line_array = explode(line_string, "\n");
    if(!sizeof(line_array)) write("Array is zero length.");
    foreach(string line in line_array){
	if(strsrch(line,"ADMIN_EMAIL") != -1){
	    lockline = line;
	}
    }
    if(sscanf(lockline,"%s\"%s\"",junk, email) < 2) {
	write("Operation failed. You need to hand-"+
	  "edit /secure/include/config.h immediately.");
	if(!auto) Menu();
	return 0;
    }

    newline = junk + "\""+str+"\"";
    newfile = replace_string(line_string, lockline, newline);
    write_file("/secure/include/config.h",newfile,1);
    load_object("/secure/cmds/creators/update")->cmd("/secure/sefun/sefun");
    write("\n");
    if(!auto) Menu();
    return 1;
}

int SetReboot(){
    write("Current reboot interval is set to "+EVENTS_D->GetRebootInterval()+" hours.");
    write(load_object("/cmds/players/nextreboot")->cmd("string"));
    write("Please enter the new desired interval in hours:\n");
    input_to( (: eventSetReboot :) );	
    return 1;
}

int eventSetReboot(mixed i){
    int num;
    if(!intp(i) && !sscanf(i,"%d",num)){
	write("Failed to set new interval.\n");
	Menu();
	return 1;
    }
    if(intp(i)) num = i;
    EVENTS_D->SetRebootInterval(num);
    write("Reboot interval set to "+EVENTS_D->GetRebootInterval()+" hours.");
    Menu();
    return 1;
}

int AddClass(){
    write("Classes currently available: "+implode(load_object(CLASSES_D)->GetClasses()," "));
    write("Please enter the name of the class you'd like to add: \n");
    input_to( (: eventAddClass :) );
    return 1;
}

int eventAddClass(string str){
    string classpath;
    array *classes;
    if(!str) {
	write("Invalid class name.\n");
	Menu();
	return 1;
    }

    classpath = "/secure/cfg/classes/"+str;
    write("Looking for "+classpath+"...");
    classes = load_object(CLASSES_D)->GetClasses();
    if(member_array(str,classes) != -1) {
	write("That class is already available.\n");
	Menu();
	return 1;
    }
    if(file_size(classpath) < 1) {
	write("For a class to be added, you must create and ");
	write("edit a class file, then place it in /secure/cfg/classes ");
	write("with the other class files.\n");
	write("Operation Failed: no such class file\n");
	Menu();
	return 1;
    }
    CLASSES_D->AddClass(classpath);
    write("Classes currently available: "+implode(load_object(CLASSES_D)->GetClasses()," ")+"\n");

    Menu();
    return 1;
}

int RemoveClass(){
    write("Classes currently available: "+implode(load_object(CLASSES_D)->GetClasses()," "));
    write("Please enter the class you'd like to remove: \n");
    input_to( (: eventRemoveClass :) );
    return 1;
}

int eventRemoveClass(string str){
    if(!str){
	write("That is not a valid class.\n");
	Menu();
	return 1;
    }

    if(member_array(str,load_object(CLASSES_D)->GetClasses()) == -1) {
	write("That class is already unavailable.\n");
	Menu();
	return 1;
    }

    load_object(CLASSES_D)->RemoveClass(str);
    write("Classes currently available: "+implode(load_object(CLASSES_D)->GetClasses()," ")+"\n");
    Menu();
    return 1;
}

int AddRace(){
    write("Races currently available: "+implode(load_object(RACES_D)->GetRaces()," "));
    write("Please enter the name of the race you'd like to add: \n");
    input_to( (: eventAddRace :) );
    return 1;
}

int eventAddRace(string str){
    array *races;
    if(!str) {
	write("Invalid race name.\n");
	Menu();
	return 1;
    }

    racepath = "/secure/cfg/races/"+str;
    write("Looking for "+racepath+"...");
    races = load_object(RACES_D)->GetRaces();
    if(member_array(str,races) != -1) {
	write("That race is already available.\n");
	Menu();
	return 1;
    }
    if(file_size(racepath) < 1) {
	write("For a race to be added, you must create and ");
	write("edit a race file, then place it in /secure/cfg/races ");
	write("with the other race files.\n");
	write("Operation Failed: no such race file\n");
	Menu();
	return 1;
    }

    write("Should players be able to play as this race?");
    write("A response other than yes will be interpreted as a no.\n");
    input_to( (: PlayerRace :) );
    return 1;
}

int PlayerRace(string str){
    string *response_array = ({"yes","ok","y","yeah","yes, please"});
    if(!str) str = "no";
    if(member_array(str,response_array) != -1) RACES_D->AddRace(racepath, 1);
    else RACES_D->AddRace(racepath);
    write("Races currently available: "+implode(load_object(RACES_D)->GetRaces()," ")+"\n");

    Menu();
    return 1;
}

int RemoveRace(){
    write("Races currently available: "+implode(load_object(RACES_D)->GetRaces()," "));
    write("Please enter the race you'd like to remove: \n");
    input_to( (: eventRemoveRace :) );
    return 1;
}

int eventRemoveRace(string str){
    if(!str){
	write("That is not a valid race.\n");
	Menu();
	return 1;
    }

    if(member_array(str,load_object(RACES_D)->GetRaces()) == -1) {
	write("That race is already unavailable.\n");
	Menu();
	return 1;
    }

    load_object(RACES_D)->RemoveRace(str);
    write("Races currently available: "+implode(load_object(RACES_D)->GetRaces()," ")+"\n");
    Menu();
    return 1;
}

int AddCurrency(){
    write("Currencies currently available: "+implode(ECONOMY_D->__QueryCurrencies()," "));
    write("Please enter the name of the curency you'd like to add: \n");
    input_to( (: eventAddCurrency :) );
    return 1;
}

int eventAddCurrency(string str){
    string query;

    currency = str;
    if(!str || str == "") {
	write("Invalid currency name.\n");
	Menu();
	return 1;
    }

    currencies = ECONOMY_D->__QueryCurrencies();
    if(member_array(str,currencies) != -1) {
	write("That currency is already available.\n");
	Menu();
	return 1;
    }

    query = "What should its exchange rate, or value be? For comparison, "+currencies[0]+" ";
    query += "has a rate of "+ ECONOMY_D->__Query(currencies[0],"rate")+", "+currencies[1]+" ";
    query += "has a rate of "+ ECONOMY_D->__Query(currencies[1],"rate")+", and "+currencies[2]+" ";
    query += "has a rate of "+ ECONOMY_D->__Query(currencies[2],"rate")+".\n";

    write(query);
    input_to( (: CurrencyRate :) );
    return 1;
}

int CurrencyRate(string str){
    string query;
    if(!str || !sscanf(str,"%f",rate) ){
	write("Invalid rate. Currency not added.\n");
	Menu();
	return 1;
    }

    query = "What should its weight be? For comparison, "+currencies[0]+" ";
    query += "has a weight of "+ ECONOMY_D->__Query(currencies[0],"weight")+", "+currencies[1]+" ";
    query += "has a weight of "+ ECONOMY_D->__Query(currencies[1],"weight")+", and "+currencies[2]+" ";
    query += "has a weight of "+ ECONOMY_D->__Query(currencies[2],"weight")+".\n";

    write(query);
    input_to( (: CurrencyWeight :) );
    return 1;
}


int  CurrencyWeight(string str){
    string query;
    if(!str || !sscanf(str,"%f",weight) ){
	write("Invalid weight. Currency not added.\n");
	Menu();
	return 1;
    }

    query = "What should its inflation rate be? For comparison, "+currencies[0]+" ";
    query += "has an inflation rate of "+ ECONOMY_D->__Query(currencies[0],"inflation")+", "+currencies[1]+" ";
    query += "has an inflation rate of "+ ECONOMY_D->__Query(currencies[1],"inflation")+", and "+currencies[2]+" ";
    query += "has an inflation rate of "+ ECONOMY_D->__Query(currencies[2],"inflation")+".\n";

    write(query);
    input_to( (: CurrencyInflation :) );
    return 1;
}

int CurrencyInflation(string str){
    if(!str || !sscanf(str,"%f",inflation) ){
	write("Invalid inflation rate. Currency not added.\n");
	Menu();
	return 1;
    }
    write(identify(previous_object(-1)));
    write("ECONOMY_D->add_currency(\""+currency+"\", "+rate+", "+inflation+", "+weight+");");
    ECONOMY_D->add_currency(currency, rate, inflation, weight);
    write("Thank you. Currency added.\n");
    Menu();
    return 1;
}

int RemoveCurrency(){
    currencies = ECONOMY_D->__QueryCurrencies();
    write("Available currencies: "+ identify(currencies) +".");
    write("Which currency would you like to remove?\n");

    input_to( (: eventRemoveCurrency :) );
    return 1;
}

int eventRemoveCurrency(string str){
    if(!str || member_array(str, currencies) == -1) {
	write("Invalid currency. No currency removed.\n");
	Menu();
	return 1;
    }
    ECONOMY_D->remove_currency(str);
    write("Remaining currencies: "+identify(ECONOMY_D->__QueryCurrencies())+"\n");
    Menu();
    return 1;
}

int EncrePlayer(){
    write("Please enter the name of the player you'd like to promote: \n");
    input_to( (: eventEncrePlayer :) );
    return 1;
}

int eventEncrePlayer(string str){
    EnCre(str);
    Menu();
    return 1;
}

int DecreCreator(){
    write("Please enter the name of the player you'd like to demote: \n");
    input_to( (: eventDecreCreator :) );
    return 1;
}

int eventDecreCreator(string str){
    DeCre(str);	
    Menu();
    return 1;
}

mixed EnCre(string args) {
    object ob;
    string file, nom;

    nom = convert_name(args);
    if( !user_exists(nom) ){
	write(capitalize(nom) + " is not a member of " +
	  possessive_noun(mud_name()) + " reality.");
	Menu();
	return 1;
    }
    if( !strsrch(file = save_file(nom), DIR_CRES) ) {
	Menu();
	write("You cannot make "+capitalize(args)+" a creator.");
	return 1;
    }

    if( ob = find_player(nom) ) {
	tell_player(nom,"You are about to be promoted to creator. This requires a reset of "+
	  "your character. You will be disconnected from the mud. When you "+
	  "reconnect, you will be a creator.");
	make_workroom(ob);
	ob->eventForce("quit");
    }
    else make_workroom(args);
    if( file_size(DIR_CRES+"/"+nom[0..0]) != -2) mkdir(DIR_CRES+"/"+nom[0..0]);
    rename(file+__SAVE_EXTENSION__, DIR_CRES+"/"+nom[0..0]+"/"+nom+__SAVE_EXTENSION__);
    if( file_size(file+__SAVE_EXTENSION__) > -1 ) rm(file+__SAVE_EXTENSION__);
    write(capitalize(args)+" is now a creator.\n");
    return 1;
}

mixed DeCre(string args) {
    object ob;
    string nom, file;
    nom = convert_name(args);
    if( !user_exists(nom) ){
	write(capitalize(nom) + " is not a member of " +
	  possessive_noun(mud_name()) + " reality.");
	Menu();
	return 1;
    }
    if( !strsrch(file = save_file(nom), DIR_PLAYERS) ) {
	Menu();
	write("You cannot make "+capitalize(args)+" a player.");
	return 1;
    }
    if( ob = find_player(nom) ) {
	tell_player(nom,"You are about to be demoted to player status. This requires a reset of "+
	  "your character. You will be disconnected from the mud. When you "+
	  "reconnect, you will be a player.");
	ob->eventForce("quit");
    }

    if( file_size(DIR_PLAYERS+"/"+nom[0..0]) != -2) 
	mkdir(DIR_PLAYERS+"/"+nom[0..0]);
    rename(file+__SAVE_EXTENSION__, DIR_PLAYERS+"/"+nom[0..0]+"/"+nom+__SAVE_EXTENSION__);

    if( file_size(file+__SAVE_EXTENSION__) > -1 ) rm(file+__SAVE_EXTENSION__);
    write(capitalize(args)+" is now a lowly player.");
    return 1;
}

int RidUser(){
    write("Please enter the name of the player you'd like to erase.\n");
    //write("You will be asked to enter a reason for this ridding.");
    //write("When you have finished, Type a single period on a blank line, ");
    //write("then the letter x and enter.\n");
    input_to( (: eventRidUser :) );
    return 1;
}

int eventRidUser(string str){
    if(!str){
	write("Invalid entry.\n");
	Menu();
	return 1;
    }

    if( !user_exists(str) ) {
	write("No such person: " + str + ".\n");
	Menu();
    }
    DoRid(str);
    return 1;
}

int DoRid(string who) {
    object ob;
    string str;
    ridded = lower_case(who);
    str = convert_name(who);
    who = capitalize(who);
    if( member_group(str, PRIV_SECURE) || member_group(str, PRIV_ASSIST) ){
	write("You must first remove this person from a secure group.");
	Menu();
	return 1;
    }
    if( ob = find_player(ridded)) {
	who = (string)ob->GetCapName();
	message("system", "You are being ridded from " + mud_name() + ".",
	  ob);
	ob->eventForce("quit");
	//if( !((int)ob->eventDestruct()) ) destruct(ob);
    }
    file = save_file(ridded) + __SAVE_EXTENSION__;
    //if( rename(file, DIR_RID + "/" + str + __SAVE_EXTENSION__) ) {
    //	write("Rename failed, security violation logged.");
    //	log_file("security", "\n*****\nRid violation attempted\n"
    //	  "Target: " + who + "\nCall stack:\n" + 
    //	  sprintf("%O\n", previous_object(-1)));
    //	return 1;
    //   }
    write("Target is: "+ridded);
    write("Please enter the reason for ridding " + ridded + ".\n");
    unguarded( (: rm(file) :) );
    input_to( (: LogRid :) );
    return 1;
}

int LogRid(string str){
    globalstr = str;
    log_file("rid", "\n" + ridded + " by " + (string)this_player()->GetCapName() + "\n" + str + "\n");
    //unguarded( (: write_file("/log/rid",timestamp()+" "+globalstr) :) );
    write(ridded + " has been ridded from " + mud_name() + ".");
    Menu();
    return 1;
}

int BanishUser(){
    write("Please enter the name to banish: \n");
    input_to( (: eventBanishUser :) );
    return 1;
}

int evenBanishUser(string str){
    if(member_array(str,BANISH_D->query_banished()) != -1) {
	write("That name is already banished.\n");
	Menu();
	return 1;
    }
    if(!user_exists(str = lower_case(str))) {
	write(capitalize(str)+" is now banished.\n");
	catch(call_other(BANISH_D, "banish_name", str));
    }
    else {
	write("A player by that name already exists.\n");
	this_player()->eventPrint((string)FINGER_D->GetFinger(str));
    }
    Menu();
    return 1;
}

int UnBanishUser(){
    write("Please enter the name to unbanish: \n");
    input_to( (: eventUnBanishUser :) );
    return 1;
}

int eventUnBanishUser(string str){
    if(member_array(str,BANISH_D->query_banished()) == -1) {
	write("That is not a banished name.");
	Menu();
	return 1;
    }
    catch(call_other(BANISH_D, "unbanish_name", str));
    write(str+" is unbanished.\n");
    Menu();
    return 1;
}

int ChangeName(){
    write("Current MUD name is "+mud_name());
    write("Please enter the new name for your MUD:\n");
    input_to( (: eventChangeName :) );
    return 1;
}

varargs int eventChangeName(string newname, int automated){
    string *line_array;
    string nameline, newline, newfile, line_string, junk, name;

    if(!newname || newname == "") {
	write("Name change cancelled.\n");
	Menu();
	return 1;
    }

    if(automated){
	if(mud_name() != "DeadSoulsNew" &&
	  mud_name() != "DeadSoulsWin")
	    return 0;
    }

    line_string = read_file("/secure/cfg/mudos.cfg");
    if(!sizeof(line_string)) write("Couldn't read file.");
    line_array = explode(line_string, "\n");
    if(!sizeof(line_array)) write("Array is zero length.");

    if(!sizeof(line_array) || !sizeof(line_string)) {
	if(!automated) Menu();
	return 0;
    }

    foreach(string line in line_array){
	if(strsrch(line,"name :") != -1){
	    nameline = line;
	}
    }

    if(!nameline || sscanf(nameline,"%s : %s",junk, name) < 2) {
	write("Operation failed. You need to copy over "+
	  "/secure/cfg/mudos.cfg immediately with an original.");
	if(!automated) Menu();
	return 0;
    }

    if(automated){
	if(name != "DeadSoulsWin" && name != "DeadSouls" &&
	  name != "Dead Souls" && name != "DeadSoulsNew") newname = name;
	cp("/secure/cfg/mudos.cfg","/secure/cfg/mudos.orig");
    }

    newline = junk + " : " + newname;
    newfile = replace_string(line_string, nameline, newline);
    write_file("/secure/cfg/mudos.cfg",newfile,1);
    cp("/secure/cfg/mudos.cfg","/secure/cfg/mudos.autobak");
    write("\n");
    if(!automated)  {
	write("\nMUD's name changed. Reboot the MUD to activate new name.\n");
	write("\n Do you want to shut down the MUD now to activate the change?\n");
	input_to( (: eventShutDownMud :) );
    }
    return 1;
}

int ChangePort(){
    write("Current MUD network port is "+query_host_port());
    write("Please enter the new network port for your MUD:\n");
    input_to( (: eventChangePort :) );
    return 1;
}

varargs int eventChangePort(string newport, int automated){
    string *line_array;
    string nameline, newline, newfile, line_string, junk, name;
    int num;

    if(!newport || newport == "") {
	write("Port change cancelled.\n");
	Menu();
	return 1;
    }

    if(!sscanf(newport,"%d", num) > 0 || num < 1 || num > 65535 ) {
	write("Invalid port: numeral between 1 and 65535 expected.");
	write("Port change cancelled.\n");
	Menu();
	return 1;
    }

    line_string = read_file("/secure/cfg/mudos.cfg");
    if(!sizeof(line_string)) write("Couldn't read file.");
    line_array = explode(line_string, "\n");
    if(!sizeof(line_array)) write("Array is zero length.");

    foreach(string line in line_array){
	if(strsrch(line,"external_port_1 :") != -1){
	    nameline = line;
	}
    }

    if(!nameline || sscanf(nameline,"%s : %s",junk, name) < 2) {
	write("Operation failed. You need to copy over "+
	  "/secure/cfg/mudos.cfg immediately with an original.");
	if(!automated) Menu();
	return 0;
    }

    newline = junk + " : telnet " + newport;
    //write("newline is: "+newline);
    newfile = replace_string(line_string, nameline, newline);
    write_file("/secure/cfg/mudos.cfg",newfile,1);
    if(!automated)  {
	write("\nMUD's port changed. Reboot the MUD to activate new port.");
	write("NOTE: If the port you selected is 1024 or below, your OS "+
	  "may require the MUD to run as a privileged user.");
	write("\n Do you want to shut down the MUD now to activate the change?\n");
	input_to( (: eventShutDownMud :) );
    }
    return 1;
}

int ShutDownMud(){
    write("Are you sure?");
    input_to( (: eventShutDownMud :) );
    return 1;
}

int eventShutDownMud(string str){
    string *response_array = ({"yes","ok","y","yeah","yes, please"});
    if(!str) str = "no";
    if(member_array(str,response_array) == -1){
	write("Shutdown cancelled.\n");
	Menu();
	return 1;
    }

    write("Shutdown initiated.\n");
    if(this_player()->GetForced()) return 0;

    shout("Game is shut down by " + this_player()->GetKeyName() + ".\n");
    log_file("game_log", ctime(time())+" Game shutdown by "+
      this_player()->GetKeyName()+"(admintool)\n");
    foreach(object dude in users()){
	if(sizeof(base_name(dude)) && !archp(dude)) dude->eventForce("quit");
    }
    shutdown();
    write("Shutdown complete.");
    Menu();
    return 1;
}

int AddGroup(){
    write("\nCurrent groups file: \n"+read_file("/secure/cfg/groups.cfg")+"\n\n");
    write("\nWhat is the name of the group you'd like to add?\n");
    input_to( (: eventAddGroup :) );
    return 1;
}

int eventAddGroup(string str){
    string config_file, new_config_file;
    string *line_array;
    string *top_array;
    string *bottom_array;

    top_array = ({});
    bottom_array = ({});

    if(!str || str == "") {
	write("\nGroup addition cancelled.\n");
	Menu();
	return 1;
    }
    config_file = read_file("/secure/cfg/groups.cfg");
    if(!sizeof(config_file)) {
	write("Couldn't read file. Addition cancelled.\n");
	Menu();
	return 1;
    }

    str = upper_case(str);
    if(strsrch(config_file,"("+str+")") != -1 ) {
	write("\nThat group already exists. Addition cancelled.\n");
	Menu();
	return 1;
    }

    line_array = explode(config_file, "\n");
    if(!sizeof(line_array)) {
	write("\nArray is zero length. Addition cancelled.\n");
	Menu();
	return 1;
    }

    foreach(string line in line_array){
	if(strsrch(line,"(") != -1 && 
	  first(line,1) != "#"){
	    bottom_array += ({ line });
	}
	else top_array += ({ line });
    }

    bottom_array += ({"("+str+") "});

    new_config_file = implode(top_array,"\n");
    new_config_file += "\n";
    new_config_file += implode(bottom_array,"\n");
    write_file("/secure/cfg/groups.cfg",new_config_file,1);
    write("\nGroup "+str+" added.\n");
    load_object("/secure/cmds/creators/update")->cmd("/secure/daemon/master");
    load_object("/secure/cmds/creators/update")->cmd("/secure/lib/connect");
    Menu();
    return 1;
}

int RemoveGroup(){
    write("\nCurrent groups file: \n"+read_file("/secure/cfg/groups.cfg")+"\n\n");
    write("\nWhat is the name of the group you'd like to remove?\n");
    input_to( (: eventRemoveGroup :) );
    return 1;
}

int eventRemoveGroup(string str){
    string config_file, new_config_file;
    string *line_array;
    string *top_array;
    string *bottom_array;

    top_array = ({});
    bottom_array = ({});

    if(!str || str == "") {
	write("\nGroup removal cancelled.\n");
	Menu();
	return 1;
    }
    config_file = read_file("/secure/cfg/groups.cfg");
    if(!sizeof(config_file)) {
	write("Couldn't read file. removal cancelled.\n");
	Menu();
	return 1;
    }

    str = upper_case(str);
    if(strsrch(config_file,"("+str+")") == -1 ) {
	write("\nThat group doesn't exist. removal cancelled.\n");
	Menu();
	return 1;
    }

    if(str == "SECURE" || str == "ASSIST"){
	write("\nThis is a configured administrative group. You "+
	  "can't remove it with admintool. If you really, really, really "+
	  "know what you're doing and you want to remove that group, you'll "+
	  "have to do it manually with an editor.\n");
	Menu();
	return 1;
    }

    line_array = explode(config_file, "\n");
    if(!sizeof(line_array)) {
	write("\nArray is zero length. removal cancelled.\n");
	Menu();
	return 1;
    }

    foreach(string line in line_array){
	if(strsrch(line,"(") != -1 && first(line,1) != "#"){
	    if(strsrch(line,"("+str+")") == -1 ) bottom_array += ({ line });
	}
	else if(strsrch(line,"("+str+")") == -1 ) top_array += ({ line });
    }

    new_config_file = implode(top_array,"\n");
    new_config_file += "\n";
    new_config_file += implode(bottom_array,"\n");
    write_file("/secure/cfg/groups.cfg",new_config_file,1);
    load_object("/secure/cmds/creators/update")->cmd("/secure/daemon/master");
    load_object("/secure/cmds/creators/update")->cmd("/secure/lib/connect");
    write("\nGroup "+str+" removed.\n");
    Menu();
    return 1;
}

int ModGroup(){
    write("\nCurrent groups file: \n"+read_file("/secure/cfg/groups.cfg")+"\n\n");
    write("\nWhat is the name of the group you'd like to modify?\n");
    input_to( (: eventModGroup :) );
    return 1;
}

int eventModGroup(string str){
    string config_file;

    if(!str || str == "") {
	write("\nGroup modification cancelled.\n");
	Menu();
	return 1;
    }
    config_file = read_file("/secure/cfg/groups.cfg");
    if(!sizeof(config_file)) {
	write("Couldn't read file. modification cancelled.\n");
	Menu();
	return 1;
    }

    str = upper_case(str);
    if(strsrch(config_file,"("+str+")") == -1 ) {
	write("\nThat group doesn't exist. modification cancelled.\n");
	Menu();
	return 1;
    }

    if(!sizeof(explode(config_file, "\n"))) {
	write("\nArray is zero length. modification cancelled.\n");
	Menu();
	return 1;
    }

    write(str);
    global_group_temp = str;
    write("The group format must be entered correctly. If the members are larry, moe, and curly, enter:\n");
    write("larry:moe:curly\n");
    write("\nIt's important to put those colons between names. If the only member is shemp, enter:\n");
    write("shemp\n");
    write("\nWhat would you like that group to contain?\n");
    input_to ( (: eventEditGroup :) );
    return 1;
}

int eventEditGroup(string members){
    string config_file, new_config_file, str;
    string *line_array;
    string *top_array;
    string *bottom_array;
    string *dudes;

    top_array = ({});
    bottom_array = ({});

    config_file = read_file("/secure/cfg/groups.cfg");
    line_array = explode(config_file, "\n");
    str = global_group_temp;
    global_group_temp = "";

    if(str == "SECURE"){
	if(!members || members == "") {
	    write("You're not leaving the SECURE group empty. Modification cancelled.\n");
	    Menu();
	    return 1;
	}
	if(strsrch(members,":") != -1) dudes = explode(members,":");
	else dudes = ({ members });

	if(member_array(this_player()->GetKeyName(),dudes) == -1) {
	    write("You can only modify this line with admintool if you include yourself in it. Modification cancelled.\n");
	    Menu();
	    return 1;
	}
    }

    foreach(string line in line_array){
	if(strsrch(line,"(") != -1 && first(line,1) != "#"){
	    if(strsrch(line,"("+str+")") == -1 ) bottom_array += ({ line });
	}
	else if(strsrch(line,"("+str+")") == -1 ) top_array += ({ line });
    }

    new_config_file = implode(top_array,"\n");
    new_config_file += "\n";
    new_config_file += implode(bottom_array,"\n");
    new_config_file += "\n";
    new_config_file += "("+str+") "+members;
    write_file("/secure/cfg/groups.cfg",new_config_file,1);
    load_object("/secure/cmds/creators/update")->cmd("/secure/daemon/master");
    load_object("/secure/cmds/creators/update")->cmd("/secure/lib/connect");
    write("\nGroup "+str+" modified.\n");
    Menu();
    return 1;
}

int ShowGroups(){
    write("\nCurrent groups file: \n"+read_file("/secure/cfg/groups.cfg")+"\n\n");
    Menu();
    return 1;
}

mixed cmd(string args) {
    Menu("main");
    return 1;
}

