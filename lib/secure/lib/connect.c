/*    Installation object for Dead Souls
 *    Original author: George Reese
 */

#include <lib.h>
#include <cfg.h>
#include <dirs.h>
#include <flags.h>
#include <daemons.h>
#include <runtime_config.h>

string Name, CapName, Password;
object Admin;
int blindmode;

static void InputName(string str);

static void logon() {
    receive("Welcome to the Dead Souls " + mudlib_version() +
            " installation process!!\n\n");
    receive("You will be asked a series of questions for creating an "
            "admin character.\n\n");
    receive("What is your MUD admin username?\n ");
    input_to((: InputName :), I_NOESC);
}

static void InputPassword(string str);

static void CheckBlind(string str){
    if( !str || str == "" || lower_case(str)[0..0] == "y" ) {
        receive("\nOk, disabling default overhead map.\n");
        blindmode = 1;
    }
    else receive("\Ok, allowing default overhead map behavior.\n");
    receive("\nCreate a password of at least 5 letters: \n");
    input_to((: InputPassword :), I_NOECHO | I_NOESC);
}

    static void InputName(string str) {
        if( !(BANISH_D->valid_name(Name = convert_name(CapName = str))) 
                || lower_case(str) == "guest") {
            receive("That is not a valid name.\n");
            receive("Name: ");
            input_to((: InputName :));
            return;
        }
        Admin = master()->player_object(Name);
        Admin->SetKeyName(Name);
        mkdir(DIR_PLAYERS "/" + Name[0..0]);
        receive("\nDo you use a screen reader for the visually impaired? (y/n)\n");
        input_to((: CheckBlind :), I_NOESC);
    }

static void ConfirmPassword(string str);

static void InputPassword(string str) {
    if( strlen(str) < 5 ) {
        receive("Password must be at least 5 letters.\n");
        receive("Password: ");
        input_to((: InputPassword :), I_NOECHO | I_NOESC);
        return;
    }
    Password = str;
    receive("\nConfirm password: ");
    input_to((: ConfirmPassword :), I_NOECHO | I_NOESC);
}

static void InputCapName(string str);

static void ConfirmPassword(string str) {
    if( str != Password) {
        receive("\nPasswords do not match.  Password: ");
        input_to((: InputPassword :), I_NOECHO | I_NOESC);
        return;
    }
    Admin->SetPassword(crypt(Password, 0));
    CapName = capitalize(CapName);
    receive("\nEnter your display name (" + CapName + " is default): ");
    input_to((: InputCapName :), I_NOESC);
}

static void InputGender(string str);

static void InputCapName(string str) {
    if( !str || str == "" ) str = CapName;
    if( convert_name(str) != Name ) {
        receive("\nYou cannot do that! Display name (hit Enter for default): ");
        input_to((: InputCapName :), I_NOESC);
        return;
    }
    Admin->SetCapName(CapName = capitalize(str));
    receive("\nPlease choose a gender (male, female, neutral, or none): ");
    input_to((: InputGender :), I_NOESC);
}

static void InputRealName(string str);

static void InputGender(string str) {
    if( str ) str = lower_case(str);
    if( !str || str == "" || ((str[0] != 'f' && str[0] != 'm') &&
                member_array(str, ({"male","female","neutral","none"})) == -1)){
        receive("\nPlease choose a gender (male, female, neutral, or none): ");
        receive("Male, female, neutral or none? ");
        input_to((: InputGender :));
        return;
    }
    if( str[0] == 'f' ) Admin->SetGender("female");
    else if( str[0] == 'm' ) Admin->SetGender("male");
    else if( str == "none" ) Admin->SetGender("neuter");
    else Admin->SetGender("neutral");
    receive("What is your real name? ");
    input_to((: InputRealName :), I_NOESC);
}

static void InputEmail(string str);

static void InputRealName(string str) {
    if( !str || str == "" ) str = "Unknown";
    Admin->SetRealName(str);
    receive("What is your email address? ");
    input_to((: InputEmail :), I_NOESC);
}

static void InputEmail(string str) {
    object ob, tool;
    string tmp = "";
    string filep = DIR_PLAYERS "/"+Name[0..0]+"/"+Name+__SAVE_EXTENSION__;
    string filec = DIR_CRES "/"+Name[0..0]+"/"+Name+__SAVE_EXTENSION__;
    int foo, err;

    if( !str || str == "" ) str = "Unknown";
    load_object("secure/cmds/admins/admintool")->eventChangeEmail(str,1);
    Admin->SetEmail(str);
    Admin->SetRace("human");
    Admin->SetPrompt("cwd");
    Admin->AddCurrency("silver",random(100)+57);
    Admin->SetTown("FirstAdmin");
    Admin->eventForce("cd");
    Admin->eventForce("home");
    Admin->SetWimpy(20);
    Admin->SetTerminal("ansi");
    Admin->SetLanguage("Common",100);
    Admin->SetDefaultLanguage("Common");
    if(blindmode){
        Admin->SetProperty("wizmapping", 0);
        Admin->SetProperty("minimapping", 0);
        Admin->SetProperty("screen reader", 1);
    }
    Admin->save_player(Name);
    make_workroom(Name,1);
    PLAYERS_D->AddPlayerInfo(Name);

    tmp = read_file(CFG_GROUPS);

    if(sizeof(tmp)) cp(CFG_GROUPS, "/secure/save/backup/groups.orig");

    rm(CFG_GROUPS);
    tmp = replace_string(tmp, "ADMIN", Name);
    write_file(CFG_GROUPS, tmp);

    tmp = "";

    tmp = read_file(CONFIG_H);

    if(sizeof(tmp)){
        cp(CONFIG_H, "/secure/save/backup/config."+__PORT__+".bak");
        rm(CONFIG_H);
        tmp = replace_string(tmp, "DEBUG_NAME", Name);
        write_file(CONFIG_H, tmp);
    }

    catch(cp(NETWORK_H,"/secure/save/backup/network."+__PORT__+".orig"));

    if( ob = find_object(LIB_CONNECT) ) destruct(ob);
    cp(DIR_SECURE_LIB "/connect.c", DIR_SECURE_LIB "/connect.first.c");
    rm(DIR_SECURE_LIB "/connect.c");
    cp(DIR_SECURE_LIB "/connect.real", DIR_SECURE_LIB "/connect.c");
    destruct(Admin);
    mkdir(DIR_CRES "/" + Name[0..0]);
#if ENABLE_INSTANCES
    filec = new_savename(filec);
    filep = new_savename(filep);
#endif
    err = rename(filep, filec);
    if(err){
        receive("\nAn unfortunate error has ocurred. Mojo meditation: QQ\n");
    }
    receive("\nYou will be disconnected and the MUD will shut down.\n");
    receive("Restart the MUD and login again as the admin character.\n");
    flush_messages();
    destruct(master());
    tool = load_object("/secure/cmds/admins/admintool");
    if(tool) foo = tool->eventChangeName("Dead_Souls_"+Name, 1); 
    if(foo){
        receive("\n\nMud name changed. Use admintool to customize it.");
        receive("\nFor more info, log in and type: help admintool\n");
    }
    else {
        receive("Mud name unchanged.\n");
    }
    flush_messages();
    cp(IMC2_D+".c", "/secure/save/backup/imc2.orig");
    PLAYERS_D->AddPlayerInfo(Name);
    shutdown();
    destruct(this_object());
}

string GetKeyName() { return Name; }
