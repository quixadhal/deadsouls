/* blame OotS */

#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;

mapping alpha1 = ([
        1 : "a",
        2 : "b",
        3 : "c",
        4 : "d",
        5 : "e",
        6 : "f",
        7 : "g",
        8 : "h",
        9 : "i",
        10 : "j",
        11 : "k",
        12 : "l",
        13 : "m",
        14 : "n",
        15 : "o",
        16 : "p",
        17 : "q",
        18 : "r",
        19 : "s",
        20 : "t",
        21 : "u",
        22 : "v",
        23 : "w",
        24 : "x",
        25 : "y",
        26 : "z",
        ]);

mapping alpha2 = ([]);

string doRotate(int code, string str){
    string *str_array = explode(str,"");
    string ret = "";

    foreach(string element in str_array){
        if(alpha2[element]){
            int operand;
            operand = alpha2[element]+code;
            if(operand > 26){
                operand -= 26;
            }
            if(operand < 1){
                operand = 26 - abs(operand);
            }
            ret += alpha1[operand];
        }
        else ret += element;
    }
    return ret;
}    

int cmd(string str){
    int code;
    string tmp, tmp2;
    string ret = "";
    string *str_array = ({});

    if(!alpha2 || !sizeof(alpha2)){
        alpha2 = ([]);
        foreach(mixed key, mixed val in alpha1) alpha2[val] = key;
    }

    if(sscanf(str,"%s %s",tmp, tmp2)){
        if(tmp = "-q"){
            for(int i=0;i<26;i++){
                write(i+": "+doRotate(i, tmp2));
            }
        }
    }

    else if(!sscanf(str,"%d %s",code, tmp)){
        str_array = explode(str,"");
        ret = doRotate(code, str);
    }

    write("Output: "+ret);
    return 1;
}

string GetHelp() {
    return "Syntax: cryptogram -q FIRSTLETTER SECONDLETTER\n"
        "        cryptogram NUMBER STRING\n\n"
        "Tool for decoding cryptograms";
}
