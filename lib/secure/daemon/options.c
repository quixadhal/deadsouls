/*    /daemon/postal/options.c
 *    from Dead Souls
 *    IIPS options management system
 *    created by Descartes of Borg 940502
 */

#include <lib.h>
#include <dirs.h>
#include <daemons.h>
#include <objects.h>
#include "options.h"

inherit LIB_DAEMON;

private mapping __MyGroups;
private mapping __Options;
static private string __Owner;

void create() {
    daemon::create();
    SetNoClean(1);
    __MyGroups = ([]);
    __Options = ([]);
    __Owner = 0;
}

static private int valid_access() {
    switch(base_name(previous_object(0))) {
        case OBJ_POST: case FOLDERS_D: case LETTERS_D:
        case LOCALPOST_D: case REMOTEPOST_D: case OPTIONS_D:
          return 1;
        default: return 0;
    }
}

void assure_box_exists(string who) {
    string tmp, file;

    if(!valid_access()) return;
    if(__Owner == who) return;
    file = DIR_POSTAL+"/"+who[0..0]+"/"+who+"/postalrc"+__SAVE_EXTENSION__;
    if(unguarded((: file_exists, file :))) return;
    file = DIR_POSTAL+"/"+who[0..0];
    if(unguarded((: file_size, file :)) != -2) unguarded((: mkdir, file :));
    file = DIR_POSTAL+"/"+who[0..0]+"/"+who;
    if(unguarded((: file_size, file :)) != -2) unguarded((: mkdir, file :));
    __MyGroups = ([]);
    __Options = ([]);
    __Owner = who;
    unguarded((: save_object, file+"/postalrc" :));
}

static private void load_options(string who) {
    string file;

    if(who == __Owner) return;
    this_object()->assure_box_exists(who);
    if(__Owner == who) {
        return;
      }
    __Owner = who;
    file = DIR_POSTAL+"/"+who[0..0]+"/"+who+"/postalrc";
    unguarded((: restore_object, file :));
  }

static private void save_options() {
    string file;

    file = DIR_POSTAL+"/"+__Owner[0..0]+"/"+__Owner+"/postalrc";
    unguarded((: save_object, file :));
}

void set_option(string who, string option, mixed value) {
    if(base_name(previous_object()) != OBJ_POST) return;
    load_options(who);
    __Options[option] = value;
    save_options();
}
                                                                             
mixed query_option(string who, string option) {
    string tmp;

    tmp = base_name(previous_object());
    if(tmp != OBJ_POST && tmp != LOCALPOST_D && tmp != FOLDERS_D) return 0;
    load_options(who);
    return __Options[option];
}

mapping query_options(string who) {
    string tmp;

    tmp = base_name(previous_object());
    if(tmp != OBJ_POST && tmp != LOCALPOST_D && tmp != FOLDERS_D) return ([]);
    load_options(who);
    return copy(__Options);
}

void set_group(string who, string group, string *members) {
    if(!valid_access()) return;
    load_options(who);
    if(!members) map_delete(__MyGroups, group);
    else __MyGroups[group] = members;
    save_options();
  }

string *query_group(string who, string group) {
    if(!valid_access()) return ({});
    load_options(who);
    return copy(__MyGroups[group]);
  }

mapping query_groups(string who) {
    if(!valid_access()) return ([]);
    load_options(who);
    return copy(__MyGroups);
  }
