/*    /daemon/postal/letters.c
 *    from Dead Souls
 *    letter management daemon
 *    created by Descartes of Borg 940503
 *    based upon the original letter.c created by Descartes 940130
 */

#include <lib.h>
#include <dirs.h>
#include <daemons.h>
#include <objects.h>
#include "letters.h"
 
inherit LIB_DAEMON;

private string __Letter;
private mapping __Folders; 
private string *__Undeleted; 
static private string __LetterId;
static private mapping __LettersDir, __PostalDir; 
static private mixed *__LetterPtr, *__PostalPtr;
 
void create() { 
    string *tmp;
    int i;

    daemon::create();
    SetNoClean(1);
    __Letter = ""; 
    __Folders = ([]);
    __Undeleted = ({}); 
    __LettersDir = ([]); 
    __PostalDir = ([]); 
    __LetterId = 0;
    __LetterPtr = ({ 0, 0}); 
    for(i=0; i<10; i++) { 
        if(unguarded((: file_size, DIR_LETTERS+"/"+i :)) == -2)
          __LettersDir[i] = unguarded((: get_dir, DIR_LETTERS+"/"+i+"/", 0 :));
    } 
    i = sizeof(tmp = unguarded((: get_dir, DIR_POSTAL+"/", 0 :)));
    __PostalPtr = ({ tmp[0], 0 }); 
    while(i--) { 
        if(unguarded((: file_size, DIR_POSTAL+"/"+tmp[i] :)) == -2)
          __PostalDir[tmp[i]] = unguarded((: get_dir, DIR_POSTAL+"/"+tmp[i]+"/", 0 :));
      } 
  } 
 
static private int valid_access() {
    switch(base_name(previous_object(0))) {
        case OBJ_POST: case LETTERS_D: case FOLDERS_D:
        case LOCALPOST_D: case REMOTEPOST_D: case OPTIONS_D:
          return 1;
        default: return 0;
    }
}

string create_letter(string str) { 
    int tmp; 
    string id, dir; 
 
    if(!valid_access()) return "";
    id = sprintf("%d", tmp = time()); 
    dir = DIR_LETTERS+"/"+(tmp%10); 
    if(unguarded((: file_size, dir+"/" :)) != -2) unguarded((: mkdir, dir :));
    while(unguarded((: file_size, dir+"/"+id+__SAVE_EXTENSION__ :)) != -1)
      id = sprintf("%d", tmp += 10);
    __LetterId = id;
    __Letter = str; 
    __Folders = ([]);
    save_letter();
    return id; 
} 
 
static private void save_letter() { 
    string file;
    int x;

    x = strlen(__LetterId)-1;
    file = sprintf("%s/%s/%s", DIR_LETTERS,__LetterId[x..x],__LetterId); 
    unguarded((: save_object, file :));
} 
 
static private int restore_letter(string id) { 
    string dir; 
    int x; 
 
    if(id == __LetterId) return 1;
    x = strlen(id) - 1; 
    dir = DIR_LETTERS+"/"+id[x..x]; 
    if(!unguarded((: file_exists, dir+"/"+id+__SAVE_EXTENSION__ :))) {
        return 0; 
      }
    unguarded((: restore_object, dir+"/"+id :));
    __LetterId = id;
    if(__Undeleted) {
        x = sizeof(__Undeleted);
        __Folders = ([]);
        while(x--) __Folders[__Undeleted[x]] = ({ "new" });
        __Undeleted = 0;
        save_letter();
    } 
    return 1; 
  } 
 
string query_letter(string id) { 
    string base;

    if((base = base_name(previous_object(0))) != OBJ_POST &&
      base != FOLDERS_D) return "Illegal access.";
    if(!restore_letter(id)) return "Invalid message.\n"; 
    return __Letter; 
  } 
 
void delete_folder(string who, string folder, string id) {
    string *tmp; 
    string file;
    int i; 
 
    if(!valid_access()) return;
    if(!restore_letter(id)) return; 
    if(!__Folders[who]) return;
    __Folders[who] -= ({ folder });
    if(!sizeof(__Folders[who])) map_delete(__Folders, who);
    save_letter();
    i = sizeof(tmp = keys(__Folders)); 
    while(i--) { 
        file = DIR_POSTAL+"/"+tmp[i][0..0]+"/"+tmp[i]+"/postalrc"+
          __SAVE_EXTENSION__;
        if(!user_exists(tmp[i])) {
            FOLDERS_D->delete_user(tmp[i]);
            restore_letter(id);
	  }
        else if(!unguarded((: file_exists, file :))) {
            map_delete(__Folders, tmp[i]);
            save_letter();
	  }
    } 
    if(!sizeof(keys(__Folders))) {
        file = DIR_LETTERS+"/"+id[strlen(id)-1..strlen(id)-1]+"/"+id+__SAVE_EXTENSION__;
        unguarded((: rm, file :));
    }
}

void add_folder(string who, string folder, string id) {
    if(!valid_access()) return;
    if(!restore_letter(id)) return;
    if(!__Folders[who]) __Folders[who] = ({ folder });
    else __Folders[who] += ({ folder });
    save_letter();
}

static void manage_letters() { 
    string str, ext, file; 
    string *tmp; 
    int x, i; 
 
    if(!(x = sizeof(keys(__LettersDir)))) return;
    if(__LetterPtr[1] >= sizeof(__LettersDir[__LetterPtr[0]])) {
        if(++__LetterPtr[0] >= x) __LetterPtr = ({ 0, 0 });
        else __LetterPtr[1] = 0;
      }
    sscanf(__LettersDir[__LetterPtr[0]][__LetterPtr[1]], "%s\.%s", str, ext); 
    __LetterPtr[1]++;
    if(!restore_letter(str)) { 
        return; 
      } 
    i = sizeof(tmp = keys(__Folders)); 
    while(i--) { 
        file = DIR_POSTAL+"/"+tmp[i][0..0]+"/"+tmp[i]+__SAVE_EXTENSION__;
        if(!user_exists(tmp[i])) { 
            FOLDERS_D->delete_user(tmp[i]);
            restore_letter(str);
        } 
        else if(!unguarded((: file_exists, file :))) {
            map_delete(__Folders, tmp[i]);
            save_letter();
	  }
    } 
    if(!sizeof(keys(__Folders))) { 
        file = DIR_LETTERS+"/"+str[strlen(str)-1..strlen(str)-1]+"/"+str+__SAVE_EXTENSION__;
        unguarded((: rm, file :));
    }
} 
    
static void manage_postal() { 
    string pl, ext, file; 
    string *tmp;
    int x, i; 

    if(!(x = sizeof(tmp = keys(__PostalDir)))) return;
    if(__PostalPtr[1] >= sizeof(__PostalDir[__PostalPtr[0]])) {
        i = member_array(__PostalPtr[0], tmp);
        if(++i == x) i = 0;
        __PostalPtr = ({ tmp[i], 0 });
      }
    if(!pl) {
        __PostalPtr[1]++;
        return;
    }
    sscanf(__PostalDir[__PostalPtr[0]][__PostalPtr[1]], "%s\.%s", pl, ext); 
    file = DIR_POSTAL+"/"+pl[0..0]+"/"+pl+__SAVE_EXTENSION__;
    if(!unguarded((: file_exists, file :))) {
        __PostalPtr[1]++;
        return; 
      } 
    if(!user_exists(pl)) FOLDERS_D->delete_user(pl);
    else FOLDERS_D->check_folders(pl);
    __PostalPtr[1]++;
 } 
