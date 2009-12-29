//      /bin/dev/_eval.c
//      Part of the TMI distribution mudlib and now part of Dead Souls's
//      allows wizards to execute LPC code without writing new objects.
//      Created by Douglas Reay (Pallando @ TMI-2, Dead Souls, etc) 92-12-07
//      evaldefs stuff by Raudhrskal, 2009-01-02

#include <privs.h>
#include <lib.h>

inherit LIB_DAEMON;

#define SYNTAX "Syntax: \"eval <lpc commands>\".\n"

int cmd( string a )
{
    string file, filename, evaldefs;
    mixed ret;

    if(!member_group(previous_object(), PRIV_SECURE)) {
        log_file("adm/eval", query_privs(previous_object())
                +" ("+ctime(time())+"): eval "+a+"\n");
    }
    if( !a ) { notify_fail( SYNTAX ); return 0; }

    evaldefs = "";

    filename = user_path(previous_object()->GetKeyName());
    if( file_size( filename ) != -2 && !securep(previous_object()) ) {
        notify_fail( "You must have a valid home directory!\n" );
        return 0;
    }

    if(file_exists(filename + "evaldefs.h"))
        evaldefs = "#include \"" + filename + "evaldefs.h\"\n";

    // The includes in the file aren't necessary (and can be removed if the
    // include files on your mud are called something different). They're
    // just to make things like "eval return children( USER )" possible.
    file =
        ""+
        "#include <lib.h>\n"+
        "#include <cgi.h>\n"+
        "#include <dirs.h>\n"+
        "#include <privs.h>\n"+
        "#include <logs.h>\n"+
        "#include <virtual.h>\n"+
        "#include <cfg.h>\n"+
        "#include <clock.h>\n"+
        "#include <save.h>\n"+
        "#include ROOMS_H\n"+
        "#include <localtime.h>\n"+
        "#include <daemons.h>\n"+
        "#include <network.h>\n"+
        "#include <news.h>\n"+
        "#include <objects.h>\n"+
        "#include <commands.h>\n"+
        "#include <armor_types.h>\n"+
        "#include <damage_types.h>\n"+
        "#include <position.h>\n"+
        "#include <runtime_config.h>\n"+
        "#include <terrain_types.h>\n"+
        "#include <medium.h>\n"+
        "#include <body_types.h>\n"+
        "#include <size_types.h>\n"+
        "#include <respiration_types.h>\n"+
        "#include <message_class.h>\n"+
        "inherit LIB_ITEM;\n"+
        evaldefs+
        "mixed eval() { " + a + "; }\n"+
        "";
    filename += "CMD_EVAL_TMP_FILE.c";
    if(securep(previous_object())){
        filename = "/secure/tmp/" + previous_object()->GetKeyName() +
            "_CMD_EVAL_TMP_FILE.c";
    }
    rm( filename );
    if( ret = find_object( filename ) ) destruct( ret );
    write_file( filename, file,1 );
    ret = call_other(filename, "eval");
    write( wrap( "Result = " + identify( ret ) ) );
    if( ret = find_object( filename ) ) destruct( ret );
    return 1;
}

string GetHelp(){
    return SYNTAX + "\nCalls a function containing <lpc commands>\n"
        "Example: If you type:\n"
        "eval return 1 + cos( 0.0 )\n"
        "the command creates a temporary file in your "
        "home dir containing the line:\n"
        "eval() { return 1 + cos( 0.0 ); }\n"
        "then does call_other on the files's eval() "
        "function, giving:\n"
        "Result = 2.000000\n\n"
        "Note: You can add custom defines for yourself "
        "with the file 'evaldefs.h' in your home directory.";
}
