/*    /cmds/creator/events.c
 *    From the Dead Souls Mud Library
 *    show wizard events
 *    Version: @(#) events.c 1.2@(#)
 *    Last modified: 96/12/08
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    mapping mp;
    string *obs;
    string *st2;
    string st3;
    int i,j,k,l;
    string name;

    if(!str && archp(previous_object())) {
        mp = EVENTS_D->GetEvents();
        i = sizeof(obs = sort_array(keys(mp),-1));
        message("info", "The following events are pending:", this_player());
        while(i--) {
            l=atoi(""+obs[i]);
            message("info", arrange_string(""+l, 30) +
                    arrange_string(mp[obs[i]]["function"], 15) +
                    local_ctime(l)+" "+query_tz(), this_player());
        }
        return 1;
    } 
    else if((archp(previous_object()) && str) || creatorp(previous_object())) {
        if (archp(previous_object()))
            name = str;
        else
            name = this_player()->GetKeyName();
        j = 0;
        mp = EVENTS_D->GetEvents();
        i = sizeof(obs = keys(mp));
        if (i) {
            while(i--)
                if((st2 = explode(""+obs[i], "/")))
                    if (st2[0] == "realms" && st2[1] == name) {
                        if (j == 0) {
                            if (archp(previous_object()))
                                message("info", name + " has the following events pending:", this_player());
                            else
                                message("info", "You have the following events pending:", this_player());
                            j = 1;
                        }
                        st3 = "~";
                        for (k = 2; k < sizeof(st2); k++)
                            st3 += "/" + st2[k];
                        message("info", arrange_string(st3, 30) +
                                arrange_string(mp[obs[i]]["function"], 15) +
                                ctime(mp[obs[i]]["time"])+" Mud Time", this_player());
                    }
        }
        if (j == 0) {
            if (archp(previous_object()))
                message("info", name+" doesn't have any events pending.", this_player());
            else
                message("info", "You don't have any events pending.", this_player());
        }
        return 1;
    }
    return 0;
}

string GetHelp() {
    if(archp(this_player())) 
        return ("Syntax: events [creator]\n\n"
                "Gives a list of all pending mud events or all events pending for the specified creator.");
    else
        return ("Syntax: events\n\n"
                "Gives a list of your pending events.");
}
