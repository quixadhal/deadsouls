/*
// This file is a part of the TMI distribution mudlib.
// Please retain this header if you use this code.
// Coded by Grog (10/15/91 - 11/03/91)
// Added to the /bin structure by Buddha  (1/18/91) 
// Help added by Brian (1/28/92)
// Single arg cp added by Scythe@Dead Souls (1/25/93)
// Bug in single arg cp fixed by Pallando (5/26/93)
// Added wild card support by Brodbane@Eve (4/3/06)
 */

#include <lib.h>
inherit LIB_DAEMON;

string GetHelp();

string rmSlash(string str) { return replace_string(str,"//","/"); }

int cmd(string str) {
    string s1;
    string file1;
    string file2;
    int localdest;
    string *files;
    int ow;

    if(this_player()->GetForced()) {
        write("Someone has tried forcing you to cp " + str);
        return 1;
    }
    localdest = 0;    /* Assume it's not a local destination */
    if( str && sscanf( str, "-f %s", s1)) {
        ow = 1;
        str = s1;
    }

    if (!str || sscanf(str, "%s %s", file1, file2) != 2) {
        if (str && sscanf(str, "%s", file1)) {
            file2 = "";     // Check to see if it's a one arg
            localdest = 1;  // cp function.  Assume localdest.
        } else {
            write(GetHelp());
            return 1;
        }
    }

    /* check for last parameter == "." */
    if (file2 == ".") {
        localdest = 1;     /* It's a local destination */
        file2 = "";
    }
    /* Given the player's current working directory and the path(s)
       for the file, construct the new full path for both files */
    file1 = absolute_path(this_player()->query_cwd(), file1);
    file2 = absolute_path(this_player()->query_cwd(), file2);

    /* Added by Brodbane to handle wild card options, 3/3/06 */
    if( !sizeof( files = wild_card( file1 ) ) )
        files = ({ file1 });
    if( sizeof(files) > 1 )
    {
        switch( file_size(file2) )
        {
            case -1:
                this_player()->eventForce("mkdir "+file2);
                break;
            case -2:
                break;
            default:
                return(write("Cannot copy wild card to a single file."),1);
        }
    }

    foreach( string file in files ) {
        string dest = (file_size(file2)==-2 ? rmSlash(file2+"/"+
                    explode( file, "/")[<1]) : file2 );


        if( file_size( file ) == -2 ) {
            localdest = 1;
            if( sizeof(files) > 1 )
                continue;
            else write( "cp: "+file+" is a directory." );
        } 
        else if( file_size(file) < 1 ) { write( "cp: couldn't find  "+file ); }
        else if( file == dest ) write( "cp: You cant copy a file on to itself!" );
        else if( file_size(dest) > 0 && !ow && !sscanf(dest, "%*s.save" )) write( "cp: "+dest+" already exists." );
        else if( !master()->valid_read( file, this_player(), "cp" ) ) write( file+": Permission denied." );
        else if( !master()->valid_write( dest, this_player(), "cp" ) ) write( dest+": Permission denied." );
        else { 
            /* Dunno what this was about. Commenting rather
             * than deleting, in case there was a good reason.
             * write_file( dest, read_file(file), 1 );
             */
            if(cp(file, dest) > 0){
                write( "Copied: "+file+" to "+dest );
            }
            else {
                write("Copy failed.");
            }
        }
    }
    return 1;
}


string GetHelp(){
    return ("Command: cp\nSyntax: cp <oldfile> [pathname]<newfile>\n"
            "This command makes a copy of the file using the new name "
            "and location passed.  If a new pathname is not specified "
            "then the copy is put into the present working directory. "
            "Optionally, wild cards can be used by employing the * operator.\n"
            "The -f flag forces overwriting of an existing file.\n\n" 
            "Examples:\n"
            "cp -f workroom.bak workroom.c\n"
            "cp workroom.bak /tmp/");
}
