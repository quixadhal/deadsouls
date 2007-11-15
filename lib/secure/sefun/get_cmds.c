/*    /secure/sefun/get_cmds.c
 *    returns an array of available commands
 *    Created by Cratylus 23JAN2005
 *    Version: @(#) get_cmds.c 1.1@(#)
 *    Last modified: 2005/01/23
 */

string *get_cmds(){
    int i;
    string str;
    string *temparr,*cmds,*NULL;

    NULL = ({ "irony" });

    if(!get_dir("/cmds/")) return NULL;

    foreach(str in get_dir("/cmds/"))
    {
        temparr=get_dir("/cmds/"+str+"/*.c");
        for(i=0;i<sizeof(temparr);i++){
            //write(temparr[i]);
            if(temparr[i] && !sizeof(cmds)) cmds = ({temparr[i]});
            if(temparr[i] && sizeof(cmds) > 0)  cmds += ({temparr[i]});
        }
    }

    foreach(str in get_dir("/secure/cmds/"))
    {
        temparr=get_dir("/secure/cmds/"+str+"/*.c");
        for(i=0;i<sizeof(temparr);i++){
            //write(temparr[i]);
            if(temparr[i] && !sizeof(cmds)) cmds = ({temparr[i]});
            if(temparr[i] && sizeof(cmds) > 0)  cmds += ({temparr[i]});
        }
    }

    return cmds;

}
