//      /adm/daemon/player_d.c
//      from the Dead Souls Mudlib
//      a daemon to keep track of player progress
//      created by Descartes of Borg 10 july 1993

#include <objects.h>
#include <save.h>

string *monsters;
mapping *player_list;

void create() {
    monsters = ({});
    player_list = ({});
    if(unguarded((: file_exists, SAVE_PLAYER_LIST+__SAVE_EXTENSION__ :)))
      unguarded((: restore_object, SAVE_PLAYER_LIST :));
}

mapping *query_player_list() { return player_list; }

void add_player_info() {
    int exp, x;
    object ob;
    int i;

    if(!(ob = previous_object()) || !userp(ob)) return;
    if(creatorp(ob)) {
      i = sizeof(player_list);
      while(i--)
        if(player_list[i]["name"] == (string)ob->GetKeyName() && creatorp(ob))
          player_list -= ({ player_list[i] });
      return;
    }
    x = -1;
    i = sizeof(player_list);
    while(i--) if(player_list[i]["name"] == (string)ob->GetKeyName()) x = i;
    if((sizeof(player_list) == 20) && (x == -1) &&
      (((int)ob->query_exp()) < player_list[19]["experience"]))
        return;
    if(x > -1) player_list -= ({ player_list[x] });
    player_list += ({ ([ "name":(string)ob->GetKeyName(), "experience":(int)ob->query_exp(),
      "kills":sizeof((string *)ob->query_kills()),
      "deaths": sizeof((mixed *)ob->query_deaths()),
      "quests": sizeof((string *)ob->query_quests()),
      "major accomplishments": sizeof((mixed *)ob->query_mini_quests()),
      "level": (int)ob->query_level(),
      "class": (string)ob->query_class(),
      "guild": (string)ob->query_guild()
    ]) });
    player_list = sort_array(player_list, "sort_list", this_object());
    if(sizeof(player_list) > 20) player_list = player_list[0..19];
    unguarded((: save_object, SAVE_PLAYER_LIST :));
}

int sort_list(mapping alpha, mapping beta) {
    if(alpha["experience"] > beta["experience"]) return -1;
    else if(alpha["experience"] < beta["experience"]) return 1;
    else return strcmp(alpha["name"], beta["name"]);
}

int add_kill(string str) {
    int x;

    if(!userp(previous_object(0)) || !stringp(str)) return -1;
    if(!monsters) monsters = ({});
    if((x=member_array(str, monsters)) == -1) {
        monsters += ({ str });
        x = member_array(str, monsters);
        unguarded((: save_object, SAVE_PLAYER_LIST :));
    }
    return x;
}

string query_monster(int x) { return monsters[x]; }
