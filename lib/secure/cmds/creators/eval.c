//      /bin/dev/_eval.c
//      Part of the TMI distribution mudlib and now part of Dead Souls's
//      allows wizards to execute LPC code without writing new objects.
//      Created by Douglas Reay (Pallando @ TMI-2, Dead Souls, etc) 92-12-07

#include <privs.h>
#include <lib.h>

inherit LIB_DAEMON;

#define SYNTAX "Syntax: \"eval <lpc commands>\".\n"

int cmd( string a )
{
    string file, filename;
    mixed err, ret;
    string x,y;

    if(!member_group(previous_object(), PRIV_SECURE)) {
	log_file("adm/eval", query_privs(previous_object())
	  +" ("+ctime(time())+"): eval "+a+"\n");
    }
    if( !a ) { notify_fail( SYNTAX ); return 0; }

    // The includes in the file arn't necessary (and can be removed if the
    // include files on your mud are called something different).  They
    // just to make things like   "eval return children( USER )"    possible.
    file =
    ""+
    "#include <lib.h>\n"+
    "#include <privs.h>\n"+
    "#include <cfg.h>\n"+
    "#include <save.h>\n"+
    "#include <rooms.h>\n"+
    "#include <config.h>\n"+
    "#include <daemons.h>\n"+
    "#include <objects.h>\n"+
    "#include <commands.h>\n"+
    "#include <armor_types.h>\n"+
    "#include <damage_types.h>\n"+
    "#include <position.h>\n"+
    "#include <runtime_config.h>\n"+
    "inherit LIB_ITEM;\n"+
    "mixed eval() { " + a + "; }\n"+
    "";
    filename = user_path((string)previous_object()->GetKeyName());
    if( file_size( filename ) != -2 ) {
	notify_fail( "You must have a valid home directory!\n" );
	return 0;
    }
    filename += "CMD_EVAL_TMP_FILE.c";
    if(securep(previous_object())) filename = "/secure/tmp/"+previous_object()->GetKeyName()+"_CMD_EVAL_TMP_FILE.c";
    rm( filename );
    if( ret = find_object( filename ) ) destruct( ret );
    write_file( filename, file,1 );
    ret = (mixed)call_other(filename, "eval");
    write( wrap( "Result = " + identify( ret ) ) );
    if( ret = find_object( filename ) ) destruct( ret );
    return 1;
}

int help()
{
    write( SYNTAX + @EndText
Effect: calls a function containing <lpc commands>
Example: If you type:
  eval return 1 + cos( 0.0 )
the command creates a temporary file in your home dir containing the line:
  eval() { return 1 + cos( 0.0 ); }
then does call_other on the files's eval() function, giving:
  Result = 2.000000
EndText
    );
    return 1;
}
