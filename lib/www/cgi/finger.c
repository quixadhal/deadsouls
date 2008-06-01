#include <daemons.h>

varargs string gateway(mixed who, int strip_html){
    string ret = "";
    string *ret_array;

    if(!who || !stringp(who)) who = "123456789101112";

    if(!strsrch(who,"user=")) who = lower_case(replace_string(who,"user=","",1));

    if(!user_exists(who)){
        ret = "No such user.";
    }

    else {
        ret_array = FINGER_D->GetRemoteFinger(who);
        ret += "Name: "+ret_array[0]+"<br>";
        ret += "Title: "+replace_string(ret_array[1],"$N",ret_array[0])+"<br>";
        ret += (find_player(who) ? "On since: " : "Last on: ")+ ret_array[4]+"<br>";
        ret += "Level: "+ret_array[7]+"<br>";
        ret += (ret_array[8] ? ret_array[8]+"<br>" : "");
    }

    ret += "<br><br>";
    ret += "<FORM ACTION=\"finger.html\">Finger a user: <INPUT name=\"user\">";
    ret += "<INPUT TYPE=SUBMIT VALUE=\"Submit\"></FORM><br><br>";
    ret += "<a href=\"/index.html\">Home</a><br><br>";
    ret += "<a href=\"http://dead-souls.net\">Dead Souls Home</a>";

    return ret;
}
