/*    /daemon/postal/folders.c
 *    from Dead Souls
 *    folder management daemon
 *    created by Descartes of Borg 940502
 */

#include <lib.h>
#include <dirs.h>
#include <iips.h>
#include <daemons.h>
#include <objects.h>
#include "folders.h"

inherit LIB_DAEMON;

private mapping *__BoxInfo;
static private string __Folder, __Owner;

void create() {
    daemon::create();
    SetNoClean(1);
    __BoxInfo = ({});
    __Folder = "new";
    __Owner = 0;
}

static private void load_folder(string who, string folder) {
    string file;

    if(who == __Owner && folder == __Folder) return;
    OPTIONS_D->assure_box_exists(who);
    file = sprintf("%s/%s/%s/%s", DIR_POSTAL, who[0..0], who, folder);
    if(!unguarded((: file_exists, file+__SAVE_EXTENSION__ :))) {
        __BoxInfo = ({});
        __Folder = folder;
        __Owner = who;
      }
    else {
        __Folder = folder;
        __Owner = who;
        if(!unguarded((: restore_object, file :))) __BoxInfo = ({});
      }
  }

static private void save_folder() {
    string file;

    if(!__Owner || !__Folder) return;
    file= sprintf("%s/%s/%s/%s", DIR_POSTAL, __Owner[0..0], __Owner, __Folder);
    unguarded((: save_object, file :));
  }

mapping *query_box_info(string who, string folder) {
    if(base_name(previous_object(0)) != OBJ_POST) return ({});
    load_folder(who, folder);
    return __BoxInfo;
  }

void add_post(string who, string folder, mapping borg) {
    string fwd, a, b, msg, tmp;
    object ob, pl;

    tmp = base_name(previous_object(0));
    if(previous_object(0) != this_object() && tmp != OBJ_POST &&
      tmp != LOCALPOST_D) return;
    if(folder=="new" && (fwd=(string)OPTIONS_D->query_option(who, "forward")) &&
      strsrch(borg["subject"], "[FORWARD]") == -1) {
        borg["subject"] += " [FORWARD]";
        if(sscanf(fwd, "%s@%s", a, b) == 2) {
            borg["message"] = (string)LETTERS_D->query_letter(borg["id"]);
            LOCALPOST_D->send_post(borg, fwd);
            return;
	  } 
        else if(user_exists(fwd)) {
            this_object()->add_post(fwd, "new", borg);
            return;
	  }
      }
    load_folder(who, folder);
    if(__Folder != "new") borg["read"] = 1;
    else borg["read"] = 0;
    __BoxInfo = sort_array(__BoxInfo+({ borg }), "sort_folder",this_object());
    LETTERS_D->add_folder(__Owner, __Folder, borg["id"]);
    save_folder();
    if(folder != "new") return;
    if((pl=find_player(who)) && (int)OPTIONS_D->query_option(who, "notify")) {
        msg = (string)OPTIONS_D->query_option(who, "message");
        if( !stringp(msg) ) msg = "New mail from $N!!!\n";
        msg = replace_string(replace_string(msg, "$S", borg["subject"]),
          "$N", capitalize(borg["from"]));
        message("system", msg, pl);
    }
    if(pl && ob = present(POSTAL_ID, pl)) {
        ob->incoming_post();
    }
}

void delete_posts(string who, string folder, int *del) {
    int i;

    if(base_name(previous_object(0)) != OBJ_POST) return;
    load_folder(who, folder);
    if((i = sizeof(del)) != sizeof(__BoxInfo)) return;
    while(i--) {
        if(del[i]) {
            LETTERS_D->delete_folder(__Owner, __Folder, __BoxInfo[i]["id"]);
            __BoxInfo[i] = 0;
        }
    }
    __BoxInfo = sort_array(filter(__BoxInfo, "filter_folder", 
      this_object()), "sort_folder", this_object());
    save_folder();
}

void mark_read(string who, string folder, int x) {
    if(base_name(previous_object(0)) != OBJ_POST) return;
    load_folder(who, folder);
    __BoxInfo[x]["read"] = 1;
    save_folder();
}

mapping mail_status(string who) {
    int unread, total, i;

    load_folder(who, "new");
    i = sizeof(__BoxInfo);
    while(i--) {
        total++;
        if(!__BoxInfo[i]["read"]) unread++;
    }
    return ([ "unread" : unread, "total" : total ]);
}

int sort_folder(mapping a, mapping b) {
    string alpha, beta;

    if(!stringp(a["date"])) alpha = ""+a["date"];
    else alpha = a["date"];
    if(!stringp(b["date"])) beta = ""+b["date"];
    else beta = b["date"];
    return strcmp(beta, alpha);
}

int filter_folder(mapping borg) { return (borg ? 1 : 0); }

