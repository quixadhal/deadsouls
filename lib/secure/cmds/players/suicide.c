/*  /secure/cmds/player/suicide.c
 *  from the Dead Souls Object Library
 *  Allows players to obliterate their character
 *  created by Blitz@Dead Souls
 */

#include <lib.h>
#include <dirs.h>
#include <flags.h>
#include <privs.h>
#include <daemons.h>
#include ROOMS_H
#include <message_class.h>

inherit LIB_DAEMON;

static private void GetPassword(string input);
static private void GetYesOrNo(string input);
static private void EndSuicide(string who);

string home_dir = "";
string newfile, tmp, gwho = "";

int eventHoseDude(object dude){
    if(dude) dude->eventDestruct();
    if(dude) destruct(dude);
    return 1;
}

mixed cmd(string str) {
    string who;
    object ob;

    if( sizeof(str) ) return "Suicide does not require any arguments.";
    ob = previous_object();
    if( this_player(1) != ob || !userp(ob) ) {
        log_file("security", "** Illegal suicide attempt **\n "
                "Call stack:\n"+ sprintf("%O\n", previous_object(-1)));
        return "Suicide failed.";
    }
    if( this_player()->GetForced() ) {
        log_file("security", "*** Illegal \"Forced\" Suicide **\n"
                "Call stack:\n"+ sprintf("%O\n", previous_object(-1)));
        return 0;
    }
    who = this_player()->GetKeyName();
    if( who == "guest" ) return "Guest is not suicidal!";
    if( member_group(who, PRIV_SECURE) || member_group(who, PRIV_ASSIST) )
        return "You must first have your security privileges removed.";
    this_player()->eventPrint("Committing suicide means having your character "
            "completely erased from "+mud_name()+"'s database.  If you "
            "are certain this is what you want, enter in your correct "
            "password.  Otherwise enter in a wrong password.");
    this_player()->eventPrint("Password: ", MSG_EDIT);
    input_to((: GetPassword :), I_NOECHO | I_NOESC);
    return 1;
}

static private void GetPassword(string input) {
    string tmp;
    if( !sizeof(input) ) {
        this_player()->eventPrint("Suicide aborted.");
        return;
    }
    tmp = this_player()->GetPassword();
    if( tmp != crypt(input, tmp) ) {
        this_player()->eventPrint("Wrong password.  Suicide aborted.");
        return;
    }
    this_player()->eventPrint("\nLeave a suicide note? (a)bort, (y)es, (N)o :\n",
            MSG_EDIT);
    input_to((: GetYesOrNo :)); 
    return;
}

static private void GetYesOrNo(string input) {
    tmp = this_player()->GetKeyName();
    if( !sizeof(input) || (input = lower_case(input))[0] != 'y' ) {
        if( input && input[0] == 'a' ) {
            this_player()->eventPrint("Suicide has been aborted.");
            return;
        }
        EndSuicide(tmp);
        return;
    }
    this_player()->eventPrint("\nYou may now enter a letter "
            "explaining why you suicided.  If you do not wish to write a "
            "letter, simply exit the editor without writing anything. "
            "(enter \".\" on a blank line to exit editor.)\n");
    this_player()->eventEdit(DIR_TMP + "/" + tmp, (: EndSuicide, tmp :));
}

static private void EndSuicide(string who) {
    string file;
    object *ob;
    string whocheck = cleaned_end(base_name(this_player()));
    tmp = "";
    newfile = "";
    if(who != whocheck){
        write("There seems to be a conflict in determining your identity.");
        write("Suicide aborted.");
        return;
    }
    gwho = who;
    home_dir = homedir(this_player()); 
    if(!directory_exists(DIR_TMP + "/suicide/")) mkdir (DIR_TMP + "/suicide/");

    file = DIR_TMP + "/" + who;
    newfile = "/log/suicides/" + who;
    if( file_size(file) > 0 ) {
        tmp = possessive_noun(who)+" suicide note.\n"
            "Dated: "+ctime(time())+"\n";
        tmp += read_file(file);
        if(!directory_exists("/log/suicides")){
            unguarded( (: mkdir("/log/suicides") :) );
        }
        unguarded( (: write_file(newfile, tmp, 1) :) );
    }
    if( file_exists(file) ) rm(file);
    log_file("suicide", who+" suicided at "+ctime(time())
            +". (from "+query_ip_name(this_player())+")\n");
    tmp = player_save_file(who);
    unguarded((: rename, tmp, save_file(DIR_SUICIDE + "/" + who) :));
    if(home_dir && directory_exists(home_dir)){
        object *purge_array = filter(objects(), (: !strsrch(base_name($1), home_dir) :) );
        foreach(object tainted in purge_array){
            if(clonep(tainted)){
                tainted->eventMove(ROOM_FURNACE);
                purge_array -= ({ tainted });
            }
        }
        foreach(object tainted in purge_array){
            tainted->eventDestruct();
        }
        unguarded( (: rename(home_dir,"/secure/save/decre/"+gwho+"."+timestamp()) :) );
        home_dir = "";
        gwho = "";
    }
    this_player()->eventPrint("You have suicided.  Please try " 
            "again another time.");
    environment(this_player())->eventPrint(
            this_player()->GetName()+" has ended "+
            possessive(this_player())+" own life before your very eyes.",
            this_player() );
    if( sizeof( ob = filter(users(), (: archp :)) ) )
        ob->eventPrint("["+this_player()->GetName()+" has "
                "committed suicide]");
    PLAYERS_D->RemoveUser(who);
    this_player()->eventMove(ROOM_FURNACE);
    this_player()->eventDestruct();
    return;
}

string GetHelp(){
    return "Syntax: suicide\n\n"
        "Ends your character's existence on "+mud_name()+" FOREVAR.  "
        "It is non-reversible.  Once you issue this command, you will be asked"
        "for a password to confirm your identity.  Once password is confirmed, "
        "you will no longer exist in the MUD's database.  You may also opt "
        "to write a final suicide note.\n\n"
        "NOTE: If you suicide from a restricted site, you will have to "
        "reregister!";
}
