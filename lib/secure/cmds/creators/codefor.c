//      /bin/dev/_codefor.c

#include <lib.h>
inherit LIB_DAEMON;

#define SYNTAX "Syntax: \"codefor <lpc commands>\".\n"

int cmd( string a )
{
    string file, filename;
    mixed err, ret;
    string path;

    if( !a ) { notify_fail( SYNTAX ); return 0; }
    file = "mixed eval() { " + a + "; }\n";
    path = user_path(lower_case(this_player()->GetName()));
    if( file_size( path ) != -2 ) 
	path = "/open/" +lower_case(this_player()->GetName());
    filename = path +"CMD_EVAL_TMP_FILE.c";
    rm( filename );
    if( ret = find_object( filename ) ) destruct( ret );
    write_file( filename, file );
    if( err = catch(  (mixed)call_other( filename, "???" ) ) )
	write( "Error = " + err );
    else 
	ret = find_object(filename);
    dump_prog(ret, 1, "/tmp/CODE_TMP_FILE");
    ret = read_file( "/tmp/CODE_TMP_FILE");
    rm("/tmp/CODE_TMP_FILE");
    rm( filename );

    ret = explode(ret, ";;;  *** Disassembly ***");
    write(ret[1]);
    return 1;
}

