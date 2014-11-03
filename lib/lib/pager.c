/*    /lib/pager.c
 *    from the Dead Souls Object Library
 *    a system pager for interactive objects
 *    created by Descartes of Borg 951104
 *    Version: @(#) pager.c 1.3@(#)
 *    Last modified: 96/10/18
 */

#include <function.h>
#include <message_class.h>
#include "include/pager.h"

static private int InPager = 0;

mixed more(mixed val, string cl, function f, mixed args){
    log_file("more", sprintf("%O", previous_object()) + "\n");
    return eventPage(val, cl, f, args);
}

varargs mixed eventPage(mixed val, mixed msg_class, function f,mixed args...){
    mixed array files;
    int maxi;

    if( InPager ) return "You are already in the pager.";
    if( !stringp(val) && !arrayp(val) )
        error("Bad argument 1 to eventPage().\n");
    if(!sizeof(val)) return 0;
    if( stringp(msg_class) || !msg_class ) msg_class = MSG_SYSTEM;
    if( arrayp(val) ){
        mapping file = ([]);

        if( !(file["Size"] = sizeof(val)) ) return 1;
        file["MessageClass"] = msg_class;
        file["CurrentLine"] = 0;
        file["Lines"] = val;
        file["Callback"] = f;
        file["Args"] = args;
        file["Name"] = "";
        file["Marks"] = ([]);
        file["LastSearch"] = 0;
        files = ({ file });
    }
    else {
        string tmp;
        val = wild_card(val);
        if( !val || !sizeof(val) ) return "File not found.";
        files = ({});
        foreach(tmp in val){
            mapping file = ([]);
            string str;

            if( file_size(tmp) == -2 ){
                if( sizeof(val) == 1 ) return "That is a directory.";
                else continue;
            }
            if(!file_exists(tmp)) continue;
            str = read_file(tmp);
            if( !str || !stringp(str)) continue;
            file["Name"] = tmp;
            file["Lines"] = explode(str, "\n");
            file["Size"] = sizeof(file["Lines"]);
            file["MessageClass"] = msg_class;
            file["CurrentLine"] = 0;
            file["Callback"] = f;
            file["Args"] = args;
            file["Marks"] = ([]);
            file["LastSearch"] = 0;
            files += ({ file });
        }
    }
    if( (maxi = sizeof(files)) > 1 ){
        int i;
        for(i=0; i<maxi; i++){
            mapping tmp = files[i];

            if( i != maxi-1 ){
                files[i]["Callback"] = (: Page, files[i+1] :);
                files[i]["Args"] = 0;
            }
        }
        if(files[<1]["Args"] && files[<1]["Callback"]) 
            files[<1]["Args"] = ({ files[<1]["Callback"] }) + files[<1]["Args"];
        else if(files[<1]["Callback"]) files[<1]["Args"] = ({ files[<1]["Callback"] });
        files[<1]["Callback"] = (: RazzleDazzle :);
    }
    return Page(files[0]+([]));
}

static int Page(mixed tmpfile){
    string page, prompt, key;
    int endline, tmpcurrline, err;
    mixed tmparr, tmparr2;
    mapping file = ([]);
    int waschar = this_object()->GetCharmode();
    foreach(mixed clef, mixed val in tmpfile){
        if(clef) file += ([ clef : val ]);
    }
    key = "enter";
    endline = file["CurrentLine"] + (GetScreen()[1] - 3);
    if( endline < file["CurrentLine"] ) endline = file["CurrentLine"];
    if( endline > (file["Size"] - 1) ) endline = file["Size"] - 1;
    tmparr = file["Lines"];
    tmpcurrline = file["CurrentLine"];
    tmparr2 = tmparr[tmpcurrline..endline];
    err = catch(page = implode(tmparr2, "\n"));
    eventPrint(page, file["MessageClass"]);
    if( creatorp() && file["Name"] != "" ) prompt = file["Name"] + " ";
    else prompt = "";
    if( endline < file["Size"] - 1 ){
        prompt += "(" + (file["CurrentLine"]+1) + "-" + (endline+1) + " ";
        prompt += ((endline * 100)/(file["Size"] - 1)) + "%) press "+key+": ";
        // Following fix courtesy of Brodbane
        prompt = "%^BOLD%^" + prompt + "%^RESET%^\n";
        file["CurrentLine"] = endline + 1;
        eventPrint(prompt, MSG_PROMPT);
        input_to((: cmdPage :), file);
    }
    else {
        int fp;
        file["CurrentLine"] = endline;
        fp = functionp(file["Callback"]);
        if( !fp || (fp == FP_OWNER_DESTED) ){
            this_object()->CheckCharmode();
            return 1;
        }
        if( file["Args"] ) evaluate(file["Callback"], file["Args"]...);
        else {
            evaluate(file["Callback"]);
        }
    }    
    return 1;
}

static void cmdPage(string str, mapping file){
    string *tmp;
    string cmd, args;
    int fp, x, scrlen;

    if( !str || trim(str) == "" ){
        if( file["CurrentLine"] >= (file["Size"]) ){
            fp = functionp(file["Callback"]);
            if( !fp || (fp == FP_OWNER_DESTED) ) return;
            if( file["Args"] ) evaluate(file["Callback"], file["Args"]...);
            else evaluate(file["Callback"]);
        }
        else Page(file);
        return;
    }
    cmd = str[0..0];
    if( strlen(str) > 1 ) args = str[1..];
    else args = 0;
    switch(cmd){
        case ",":
            if( !args || !(file["Marks"][args]) ){
                receive("\a");
                input_to((: cmdPage :), file);
                return;
            }
            else cmdPage("g" + file["Marks"][args], file);
            return;

        case "/":
            if( file["CurrentLine"] >= (file["Size"] - 1) ){
                eventPrint("\a" + GetPagerPrompt(file), MSG_PROMPT);
                input_to((: cmdPage :), file);
                return;
            }
            if( !args && !(file["LastSearch"]) ){
                eventPrint("\a" + GetPagerPrompt(file), MSG_PROMPT);
                input_to((: cmdPage :), file);
                return;
            }
            if( !args ) args = file["LastSearch"];
            tmp = regexp(file["Lines"][file["CurrentLine"]..], args);
            if( !sizeof(tmp) ){
                eventPrint("\a" + GetPagerPrompt(file), MSG_PROMPT);
                input_to((: cmdPage :), file);
                return;
            }
            for(x = file["CurrentLine"]; x < file["Size"]; x++){
                if( tmp[0] == file["Lines"][x] ) break;
            }
            if( x == file["Size"] ){
                eventPrint("\a" + GetPagerPrompt(file), MSG_PROMPT);
                input_to((: cmdPage :), file);
                return;
            }
            file["CurrentLine"] = (x ? x-1 : x);
            file["LastSearch"] = args;
            Page(file);
            return;

        case "?":
            x = file["CurrentLine"] - GetScreen()[1] - 3;
            if( x < 1 ){
                eventPrint("\a" + GetPagerPrompt(file), MSG_PROMPT);
                input_to((: cmdPage :), file);
                return;
            }
            if( !args && !(file["LastSearch"]) ){
                eventPrint("\a" + GetPagerPrompt(file), MSG_PROMPT);
                input_to((: cmdPage :), file);
                return;
            }
            if( !args ) args = file["LastSearch"];
            tmp = regexp(file["Lines"][0..x], args);
            if( !sizeof(tmp) ){
                x = -1;
            }
            else {
                for( ; x > -1; x--){
                    if( tmp[<1] == file["Lines"][x] ) break;
                }
            }
            if( x == -1 ){
                eventPrint("\a" + GetPagerPrompt(file), MSG_PROMPT);
                input_to((: cmdPage :), file);
                return;
            }
            file["CurrentLine"] = (x ? x - 1 : x);
            if( file["CurrentLine"] < 0 ) file["CurrentLine"] = 0;
            file["LastSearch"] = args;
            Page(file);
            return;

        case "b": 
            scrlen = GetScreen()[1];
            if( (file["CurrentLine"] - (2*(scrlen-3))) < 1 ){
                eventPrint("\a" + GetPagerPrompt(file), MSG_PROMPT);
                input_to((: cmdPage :), file);
                return;
            }
            else {
                file["CurrentLine"] -= (2 * (scrlen-3));
                Page(file);
            }
            return;

        case "g": case "<": case "G": case ">":
            if( cmd == "g" || cmd == "<" ) x = 1;
            else {
                x = (file["Size"] - (GetScreen()[1] - 3));
                if( x < 1 ) x = 1;
            }
            if( !args ) args = x + "";
            else if( args[0] == ' '){
                if( strlen(args) == 1 ) args = x + "";
                else args = trim(args);
            }
            if( ((x = to_int(args)) < 1) || (x > file["Size"]) ){
                eventPrint("\a" + GetPagerPrompt(file), MSG_PROMPT);
                input_to((: cmdPage :), file);
                return;
            }
            file["CurrentLine"] = x - 1;
            Page(file);
            return;

        case "h": case "H":
            eventPrint(GetHelp("pager"), MSG_HELP);
            eventPrint(GetPagerPrompt(file), MSG_PROMPT);
            input_to((: cmdPage :), file);
            return;

        case "m":
            if( !args ){
                eventPrint("\a" + GetPagerPrompt(file), MSG_PROMPT);
                input_to((: cmdPage :), file);
                return;
            }
            file["Marks"][args] = file["CurrentLine"];
            eventPrint("Mark " + args + " set to line " +
                    (file["CurrentLine"] + 1) + ".", file["MessageClass"]);
            eventPrint(GetPagerPrompt(file), MSG_PROMPT);
            input_to((: cmdPage :), file);
            return;

        case "n":
            fp = functionp(file["Callback"]);
            if( !fp || (fp == FP_OWNER_DESTED) ){
                eventPrint("\a" + GetPagerPrompt(file), MSG_PROMPT);
                input_to((: cmdPage :), file);
                return;
            }
            if( file["Args"] ) evaluate(file["Callback"], file["Args"]...);
            else evaluate(file["Callback"]);
            return;

        case "p": case "%":
            if( args ) x = to_int(args);
            if( !args || x < 1 || x > 100 ){
                eventPrint("\a" + GetPagerPrompt(file), MSG_PROMPT);
                input_to((: cmdPage :), file);
                return;
            }
            x = ((file["Size"] - 1) * x)/100 - 1;
            if( x < 0 ) x = 0;
            else if( x > ((file["Size"] - 1) - (GetScreen()[1] - 3)) )
                x = ((file["Size"] - 1) - (GetScreen()[1] - 3));
            file["CurrentLine"] = x;
            Page(file);
            return;

        case "q":
            fp = functionp(file["Callback"]);
            this_object()->CheckCharmode();
            if( !fp || (fp == FP_OWNER_DESTED) ) return;
            if( file["Args"] ) evaluate(file["Callback"], file["Args"]...);
            else evaluate(file["Callback"]);
            return;

        case "v":
            eventPrint("Dead Souls Pager v3.0 by Descartes of Borg 951104",
                    MSG_HELP);
            eventPrint(GetPagerPrompt(file), MSG_PROMPT);
            input_to((: cmdPage :), file);
            return;

        default:
            eventPrint("\a'h' for help", MSG_ERROR);
            eventPrint(GetPagerPrompt(file), MSG_PROMPT);
            input_to((: cmdPage :), file);
            return;
    }
}

varargs static private void RazzleDazzle(mixed args...){
    function f;
    InPager = 0;
    if(args && sizeof(args)){
        f = args[0];
        if( !functionp(f) || functionp(f) == FP_OWNER_DESTED ) return;
        if( sizeof(args) > 1 ) args = args[1..];
        else args = 0;
        if( args ) evaluate(f, args...);
        else evaluate(f);
    }
    this_object()->CheckCharmode();
}

static private string GetPagerPrompt(mapping file){
    int x;

    if( creatorp() && file["Name"] != "" )
        return "%^BOLD%^" + file["Name"] + ":%^RESET%^ ";
    if( file["CurrentLine"] >= (file["Size"] - 1) ) return "END: ";
    x = ((100 * file["CurrentLine"])/(file["Size"] - 1));
    if( x < 0 ) x = 0;
    if( x > 100 ) x = 100;
    return "%^BOLD%^" + x + "%:%^RESET%^ ";
}

string GetHelp(string str){
    if( str != "pager" ) return 0;
    return ("/<pattern>, ?<pattern>\n"
            "The pattern is optional.  / searches forward for a pattern.  "
            "If no pattern is specified, the last search pattern is "
            "repeated.  ? does the same, except searching backwards.\n"
            "b\n"
            "Move back one page.\n"
            "<return>\n"
            "Move ahead one page.\n"
            "n\n"
            "Move to the next file in the group of files being paged.\n"
            "m<tag>\n"
            "Mark the current line and asociate it with tag.  You can mark "
            "any number of lines.\n"
            ",<tag>\n"
            "Move to the named tag.\n"
            "g<line>, G<line>\n"
            "Go to the named line.  If no line is named, 'g' goes to the "
            "beginning of the file.  In contras, 'G' will go to the end.\n"
            "p<percent>\n"
            "Move to the line <percent> lines into the file.\n"
            "q\n"
            "Quit out of the pager.");
}
