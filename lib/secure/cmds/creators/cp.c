/*
// This file is a part of the TMI distribution mudlib.
// Please retain this header if you use this code.
// Coded by Grog (10/15/91 - 11/03/91)
// Added to the /bin structure by Buddha  (1/18/91) 
// Help added by Brian (1/28/92)
// Single arg cp added by Scythe@Dead Souls (1/25/93)
// Bug in single arg cp fixed by Pallando (5/26/93)
*/

#include <lib.h>
inherit LIB_DAEMON;

int help();

int
cmd(string str) {
    string line;
    string file1;
    string file2;
    int localdest;
    if(this_player()->GetForced()) {
        write("Someone has tried forcing you to cp " + str);
        return 1;
    }
    localdest = 0;    /* Assume it's not a local destination */
    if (!str || sscanf(str, "%s %s", file1, file2) != 2) {
       if (str && sscanf(str, "%s", file1)) {
          file2 = "";     // Check to see if it's a one arg
          localdest = 1;  // cp function.  Assume localdest.
       } else {
          help();
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

    /* This used to give and lstat error, fixed 930208 by Plura */
   if(file_size(file1) < 1)
   {
      notify_fail("cp: couldn't find "+file1+"\n");
      return 0;
   }

  /* Check if the destination is a directory */
  if (!localdest) {
    if (file_size(file2) == -2)
      localdest = 1;
  }
  if (localdest) {
    /* Extract the root file name from the source and use
       it for the destination.  file2 should be just a
       directory path at this point so that the rootname can
       be appended. */
    string newroot;
    string path;
    string rootname;
    rootname = file1;
    while(sscanf(rootname, "%s/%s", path, newroot) == 2) {
      rootname = newroot;
    }
    file2 = file2 + "/" + rootname;
  }
   if(file_size(file1) == -2)   /*Plura 930120*/
   {
      notify_fail("cp: "+file1+" is a directory.\n");
      return 0;
   }
#if 0
   if(file_size(file2) > 0)  /* Plura 930120 */
  {
      notify_fail("cp: "+file2+" already exists.\n");
      return 0;
   }
#endif
  if (file1 == file2) {
    notify_fail("cp: can not copy a file on top of itself!\n");
    return 0;
  }
   if(!master()->valid_read(file1, this_player(), "cp"))
    {
    notify_fail(file1 + ": Permission denied.\n");
    return 0;
  }
    if(!master()->valid_write(file2, this_player(), "cp"))
    {
    notify_fail(file2 + ": Permission denied.\n");
    return 0;
  }
  line = read_file(file1);
  if (line == 0) {
    /* Uhoh.  Can't read source file. */
    notify_fail("cp: file not found: " + file1 + "\n");
    return 0;
  }
  if (file_size(file2) == -2)
    {
    notify_fail("Tried to overwrite the directory "+ file2 + "\n");
    return 0;
  }
  rm(file2);
  write_file(file2, line);
  message("system", "Copied: " + file1 + " to " + file2, this_player());
  return 1;
}

int 
help() {
  message("help", "Command: cp\nSyntax: cp <oldfile> [pathname]<newfile>\n"
	  "This command makes a copy of the file using the new name "
	  "and location passed.  If a new pathname is not specified "
	  "then the copy is put into the present working directory.", 
	  this_player());
  return 1;
}

/* EOF */
