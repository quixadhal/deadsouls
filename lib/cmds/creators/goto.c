/*    /cmds/creator/goto.c
 *    from the Dead Souls LPC Library
 *    a command to take a cre anywhere in the game
 *    created by Descartes of Borg 930906
 *    modified for NM IV 950331
 */

#include <lib.h>

inherit LIB_DAEMON;
 
mixed cmd(string str) {
    object ob;

    if(!str) return "Goto where?";
    if((ob = find_living(lower_case(str))) && 
      (!ob->GetInvis() || !archp(ob)) && ob=environment(ob)) {
        if(ob == environment(this_player())) {
            message("my_action", "You twitch.", this_player());
            if(hiddenp(this_player())) return 1;
            message("other_action", (string)this_player()->GetName()+
              " twitches.", ob, ({ this_player() }));
            return 1;
        }
    }
    if(ob && ob->GetInvis() && creatorp(ob) && !archp(this_player())) ob = 0;
    if(!ob) str = absolute_path((string)this_player()->query_cwd(), str);
    if(ob) this_player()->eventMoveLiving(ob, "");
      else this_player()->eventMoveLiving(str, "");
    return 1;
}

void help() {
    message("help",
      "Syntax: <goto [living thing|file]>\n\n"
      "This command will move you to where the living thing is if it can "
      "be found, otherwise it will search for the file named and try to "
      "move you into that file.\n\nSee also: home, move, trans.",
      this_player()
    );
}
