/*    /secure/sefun/get_verbs.c
 *    returns an array of available verbs
 *    Created by Cratylus 23JAN2005
 *    Version: @(#) get_verbs.c 1.1@(#)
 *    Last modified: 2005/01/23
 */

string *get_verbs(){
    int i;
    string str;
    string *temparr,*verbs;

    foreach(str in get_dir("/verbs/"))
    {
        temparr=get_dir("/verbs/"+str+"/*.c");
        for(i=0;i<sizeof(temparr);i++){
            //write(temparr[i]);
            if(temparr[i] && !sizeof(verbs)) verbs = ({temparr[i]});
            if(temparr[i] && sizeof(verbs) > 0)  verbs += ({temparr[i]});
        }
    }
    return verbs;

}

