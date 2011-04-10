#include <lib.h>
#include <cfg.h>

inherit LIB_DAEMON;

string *zones_cfg = explode((read_file(CFG_TIMEZONES)|| ""), "\n");

mixed cmd(string args) {
    string tz = this_player()->GetProperty("timezone");
    if(!tz || tz == "NONE") tz = "No timezone set.";

    if(!args){
        write("Your timezone is: "+tz);
        return 1;
    }
    args = upper_case(args);
    if(member_array(args, (zones_cfg + ({"NONE"}))) == -1){
        write("That is an unknown timezone.");
        return 1;
    }
    this_player()->SetProperty("timezone", args);
    write("Your timezone is now set to: "+args);
    return 1;
}

string GetHelp(){
    return ("Syntax: timezone [zone | none]\n\n"
            "Without an argument, timezone will display your current "
            "timezone. If provided a valid timezone known to the mud, "
            "it will attempt to set your personal timezone to that."
            "\nSee also: env");
}
