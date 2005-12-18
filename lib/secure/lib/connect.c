/*    Installation object for Dead Souls V
 *    Copyright 1995 George Reese
 */

#include <lib.h>
#include <cfg.h>
#include <dirs.h>
#include <flags.h>
#include <daemons.h>

string Name, CapName, Password;
object Admin;

static void InputName(string str);

static void logon() {
    receive("Welcome to the Dead Souls " + mudlib_version() +
      " installation process!!\n\n");
    receive("You will be asked a series of questions for creating an "
      "admin character.\n\n");
    receive("What is your MUD admin username? ");
    input_to((: InputName :), I_NOESC);
}

static void InputPassword(string str);

static void InputName(string str) {
    if( !((int)BANISH_D->valid_name(Name = convert_name(CapName = str))) ) {
	receive("That is not a valid name.\n");
	receive("Name: ");
	input_to((: InputName :));
	return;
    }
    Admin = (object)master()->player_object(Name);
    Admin->SetKeyName(Name);
    mkdir(DIR_PLAYERS "/" + Name[0..0]);
    receive("\nPassword: ");
    input_to((: InputPassword :), I_NOECHO | I_NOESC);
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
    receive("Confirm password: ");
    input_to((: ConfirmPassword :), I_NOECHO | I_NOESC);
}

static void InputCapName(string str);

static void ConfirmPassword(string str) {
    if( str != Password) {
	receive("Passwords do not match.  Password: ");
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
	receive("You cannot do that! Display name: ");
	input_to((: InputCapName :), I_NOESC);
	return;
    }
    Admin->SetCapName(CapName = capitalize(str));
    receive("Male or female? ");
    input_to((: InputGender :), I_NOESC);
}

static void InputRealName(string str);

static void InputGender(string str) {
    if( str ) str = lower_case(str);
    if( !str || str == "" || (str[0] != 'f' && str[0] != 'm') ) {
	receive("Male or female? ");
	input_to((: InputGender :));
	return;
    }
    if( str[0] == 'f' ) Admin->SetGender("female");
    else Admin->SetGender("male");
    receive("What is your real name? ");
    input_to((: InputRealName :), I_NOESC);
}

static void InputEmail(string str);

static void InputRealName(string str) {
    if( !str || str == "" ) str = "Unknown";
    Admin->SetRealName(str);
    receive("What is your email address (first char # for privacy)? ");
    input_to((: InputEmail :), I_NOESC);
}

static void InputEmail(string str) {
    object ob, staff, robe, hat;
    string tmp;

    if( !str || str == "" ) str = "Unknown";
    Admin->SetEmail(str);
    Admin->SetRace("human");

    staff = new("/secure/obj/staff");
    if(staff) staff->eventMove(Admin);

    robe = new("/domains/default/armor/robe");
    if(robe) robe->eventMove(Admin);
    hat = new("/domains/default/armor/wizard_hat");
    if(hat) hat->eventMove(Admin);
    robe = present("robe",Admin);
    if(robe) Admin->eventForce("wear robe");
    hat  =  present("wizard hat",Admin);
    if(hat) Admin->eventForce("wear wizard hat");

    Admin->AddCurrency("silver",random(100)+57);
    Admin->SetWimpy(20);
    Admin->SetTerminal("ansi");
    Admin->save_player(Name);

    tmp = read_file(CFG_GROUPS);
    rm(CFG_GROUPS);
    make_workroom(Admin);
    tmp = replace_string(tmp, "ADMIN", Name);
    write_file(CFG_GROUPS, tmp);
    if( ob = find_object(LIB_CONNECT) ) destruct(ob);
    rename(DIR_SECURE_LIB "/connect.real", DIR_SECURE_LIB "/connect.c");
    destruct(Admin);
    mkdir(DIR_CRES "/" + Name[0..0]);
    rename(DIR_PLAYERS "/" + Name[0..0] + "/" + Name + __SAVE_EXTENSION__,
      DIR_CRES "/" + Name[0..0] + "/" + Name + __SAVE_EXTENSION__);
    make_workroom(this_player());
    destruct(master());
    receive("\nYou will be disconnected and the MUD will shut down.\n");
    receive("Restart the MUD and login again as the admin character.\n");
    load_object("/secure/cmds/admins/admintool")->eventChangeName("Dead_Souls_"+Name);
    shutdown();
    destruct(this_object());

}

string GetKeyName() { return Name; }
