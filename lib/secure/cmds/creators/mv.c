/*
// This file is part of the TMI Mudlib distribution.
// Please include this header if you use this code.
// Adapted by Buddha(1-18-91) from unknown source.
// Added check for existance of destination file. Plura 930120
*/

#include <lib.h>
inherit LIB_DAEMON;

int cmd(string str);
int help();


int
cmd(string str) {
   string t1, t2, *tmp;
    if(this_player()->GetForced()) {
        write("Someone tried forcing you to mv "+str+"\n");
        return 1;
    }
   if(!str||sscanf(str,"%s %s",t1,t2)!=2) {
      /* We should add checks for flags here. */
      return help();  
   } else {
#if 0
      if(file_size(t2=absolute_path((string)this_player()->query_cwd(),t2)) > 0)
         {
            notify_fail("mv: "+t2+" already exists.\n");
            return 0;
         }
#endif
      t2=absolute_path((string)this_player()->query_cwd(),t2);
      rename(t1=absolute_path(this_player()->query_cwd(),t1),t2);
      if(file_size(t2) == -2) {
         tmp = explode(t1, "/");
         t2 += "/" + tmp[sizeof(tmp)-1];
        }
      write(
         ((file_size(t1)<0)&&(file_size(t2)!=-1)) ? t1+" -> "+t2+"\n"
         :"mv failed.\n");
   }
   return 1;
}

int help() {
   write(
      "Syntax:\nmv <file1> <file2|directory>\n" +
      "Renames a file or moves it into the directory specified.\n" +
      "It wont overwrite an existing file.\n");
   return 1;
}

/* EOF */
