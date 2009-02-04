#define NOECHO 1
#ifndef CED_DISABLED
#define CED_DISABLED 1
#endif
#define CED_DEBUG 0

private static int cediting;
private static mapping FileData = ([]);
private static mapping ScreenData = ([]);

static int *GetScreen(){ return ({ 79, 24 }); }
private static int ReceiveChars(string c);

void create(){
    mixed tmp = GetScreen();
    if(!sizeof(ScreenData)){
        ScreenData = ([]);
    }
    if(sizeof(tmp)){
        ScreenData["maxcol"] = tmp[0];
        ScreenData["row"] = tmp[1] - 1;
    }
    else {
        ScreenData["maxcol"] = 79;
        ScreenData["row"] = 23;
    }
    ScreenData["maxrow"] = ScreenData["row"];
    ScreenData["col"] = 1;
    ScreenData["charbuffer"] = "";
    ScreenData["sessionbuffer"] = "   ";
    ScreenData["insert"] = 1;
}

varargs int StatReport(string str){
    receive("\r\e["+(ScreenData["maxrow"]+1)+";1H"); /* put cursor at bottom */
    receive("\r\e[2K"); /* Erase the entire bottom row */
    if(str){
        receive(str);
    }
    else {
#if CED_DEBUG
        receive("screen: "+ScreenData["row"]+","+ScreenData["col"]+
                " file: "+((FileData["topline"]+ScreenData["row"])-1)+
                ","+ScreenData["col"]+", topline: "+FileData["topline"]);
#else 
        receive("line: "+((FileData["topline"]+ScreenData["row"])-1)+
                " col: "+ScreenData["col"]+" file: "+ FileData["file"]);
#endif
    }
    /* now return cursor to prev pos */
    receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H");
    return 1;
}

varargs int RedrawScreen(int topline){
    int lastline = sizeof(FileData["map"]);
    int line = 1;
    if(!topline) topline = FileData["topline"];
    topline--;
    //tc("redraw from "+topline);
    if(topline > lastline) return 0;
    //tc("hmm! line: "+line+", ScreenData[\"maxrow\"]: "+ScreenData["maxrow"]);
    receive("\r\e[1;1H");
    receive("\r\e[2K");
    receive("Editing "+FileData["file"]+"\n");
    while(line < (ScreenData["maxrow"])){
        //tc("line "+line);
        //tc("line + topline: "+(line+topline));
        receive("\r\e["+line+";1H");
        receive("\e[2K");
        if(FileData["map"][(line+topline)]){
            //tc("text: "+FileData["map"][line+topline]);
            receive("\r\e[7l"+FileData["map"][line+topline]+"\n");
        }
        line++;
    }
    receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H");
    ScreenData["charbuffer"] = "";
    return 1;
}

static int CeditCollate(){ /* when lines need to be resorted */
    int i, sz = sizeof(FileData["map"]); 
    string tmp = "";
    if(!sz) return 0;
    FileData["contents"] = "";
    for(i = 0; i < sz; i++){
        if(FileData["map"][i]){
            //tc("line["+i+"]: "+FileData["map"][i]);
            tmp += FileData["map"][i]+"\n";
        }
    }
    tmp = replace_string(tmp,"\n","\n"+sprintf("%c",17));
    i = 1;
    foreach(mixed ln in explode(tmp,sprintf("%c",17))){
        FileData["map"][i] = replace_string(ln,"\n","");
        i++;
    }
    return i;
}

static varargs int CeditSave(string file){
    string ret = "";
    int i, sz = sizeof(FileData["map"]);
    for(i = 0; i < sz; i++){
        if(FileData["map"][i]){
            //tc("line["+i+"]: "+FileData["map"][i]);
            ret += FileData["map"][i]+"\n";
        }
    }
#if CED_DISABLED
    StatReport("This alpha editor is not yet in shape to save files.");
#else
    i = write_file(FileData["file"],ret,1); 
#endif
    return i;
}

static int ReceiveChar(string c){
    string tmp;
    int row, col, arrowed;
    if(!ScreenData["charbuffer"]) ScreenData["charbuffer"] = "";
    //tc("SCREENMODE received: "+c+" aka "+c[0]);
    if(!cediting){
        ReceiveChars(c); 
        get_char("ReceiveChars", NOECHO);
    }

    if(ScreenData["goto"]){
        if(c[0] == 13 || c[0] == 10){
            int top;
            int line = atoi(ScreenData["charbuffer"]);
            if(line && FileData["map"][line]){
                StatReport("Going to "+ScreenData["charbuffer"]);
                top = line - ScreenData["pregoto"][0]; 
                if(top < 1) top = 1;
                FileData["topline"] = top;
                ScreenData["row"] = (ScreenData["pregoto"][0]) + 1;
                ScreenData["col"] = 1;
            }
            else {
                StatReport("No such line!");
                ScreenData["row"] = ScreenData["pregoto"][0];
                ScreenData["col"] = ScreenData["pregoto"][1];
            }
            ScreenData["charbuffer"] = "";
            ScreenData["goto"] = 0;
            ScreenData["pregoto"] = 0;
            RedrawScreen();
            //receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H");
        }
        else {
            ScreenData["charbuffer"] += c;
            ScreenData["sessionbuffer"] += c;
            if(sizeof(ScreenData["sessionbuffer"]) > 1024){
                ScreenData["sessionbuffer"]=ScreenData["sessionbuffer"][1..];
            }
        }
        get_char("ReceiveChar", NOECHO);
        return 1;
    }

    /* special chars */
    if(c[0] == 17){ /* Ctrl-Q for quitting without saving */
        write("Cancelling cedit!");
        cediting = 0;
        FileData = ([ "file" : "", "map" : ([]) ]);
        ScreenData["charbuffer"] = "";
        get_char("ReceiveChars", NOECHO);
        return 1;
    }
    if(c[0] == 7){ /* Ctrl-G for going to a line */
        string tmpstr = "Please type the line number then press enter.";
        int where = sizeof(tmpstr);
        StatReport(tmpstr);
        ScreenData["pregoto"] = ({ ScreenData["row"], ScreenData["col"] });
        ScreenData["row"] = (ScreenData["maxrow"]+1);
        ScreenData["col"] = (where + 1);
        receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H");
        ScreenData["charbuffer"] = "";
        ScreenData["goto"] = 1;
        get_char("ReceiveChar", NOECHO);
        return 1;
    }
    if(c[0] == 6){ /* Ctrl-F */
        int targettopline = FileData["topline"] + (ScreenData["maxrow"]);
        int targetline = targettopline + ScreenData["row"];
        int maxlines = sizeof(FileData["map"]);
        if(targettopline > maxlines) targettopline = maxlines;
        if(targetline > maxlines) targetline = maxlines;
        FileData["topline"] = targettopline;
        ScreenData["row"] = targetline;
        RedrawScreen(FileData["topline"]);
        StatReport();
        get_char("ReceiveChar", NOECHO);
        return 1;
    }
    if(c[0] == 2){ /* Ctrl-B */
        int targettopline = FileData["topline"] - (ScreenData["maxrow"]);
        int targetline = targettopline - ScreenData["row"];
        if(targettopline < 1) targettopline = 1;
        if(targetline < 1) targetline = 1;
        FileData["topline"] = targettopline;
        ScreenData["row"] = targetline;
        RedrawScreen(FileData["topline"]);
        StatReport();
        get_char("ReceiveChar", NOECHO);
        return 1;
    }
    if(c[0] == 12){ /* Ctrl-L */
        RedrawScreen(FileData["topline"]);
        StatReport();
        ScreenData["charbuffer"] = "";
        get_char("ReceiveChar", NOECHO);
        return 1;
    }
    if(c[0] == 4){ /* Ctrl-D */
        /* Actual deletion process occurs further down.
         * This bit just marks the line for deletion.
         */
        ScreenData["charbuffer"] = "";
        ScreenData["linedel"] = 1;
    }
    if(c[0] == 9){ /* Ctrl-I */
        if(!ScreenData["insert"]){
            ScreenData["insert"] = 1;
            StatReport("Insert mode enabled");
        }
        get_char("ReceiveChar", NOECHO);
        return 1;
    }
    if(c[0] == 15){ /* Ctrl-O */
        if(ScreenData["insert"]){
            ScreenData["insert"] = 0;
            StatReport("Overstrike mode enabled");
        }
        get_char("ReceiveChar", NOECHO);
        return 1;
    }
    if(c[0] == 24){ /* Ctrl-X */
        int ret;
#if CED_DISABLED
        StatReport("This alpha editor is not yet in shape to save files.");
#else
        if(sizeof(FileData["file"]) && sizeof(FileData["map"])){
            ret = CeditSave();
            if(ret){
                StatReport(FileData["file"]+" saved.");
                write("Exiting screen editor.");
                cediting = 0;
                FileData = ([ "file" : "", "map" : ([]) ]);
                ScreenData["charbuffer"] = "";
                get_char("ReceiveChars", NOECHO);
                return 1;
            }
            else StatReport(FileData["file"]+" save FAILED.");
        }
        get_char("ReceiveChar", NOECHO);
#endif
        return 1;
    }
    if(c[0] == 19){ /* Ctrl-S */
        int ret;
        if(sizeof(FileData["file"]) && sizeof(FileData["map"])){
            ret = CeditSave();
            if(ret) StatReport(FileData["file"]+" saved.");
            else StatReport(FileData["file"]+" save FAILED.");
        }
        get_char("ReceiveChar", NOECHO);
        return 1;
    }

    /* special cases of "enter" or too-large buffer */
    if(sizeof(ScreenData["charbuffer"]) > 126){
        ScreenData["charbuffer"] = last(ScreenData["charbuffer"],126);
    }    
    if(c[0] == 3){
        ScreenData["charbuffer"] = "";
    }
    else if(c[0] == 13 || c[0] == 10 ){
        ScreenData["charbuffer"] = "\n";
        ScreenData["sessionbuffer"] += c;
    }
    /* end special cases of "enter" or too-large buffers */

    if(c[0] == 0){ /* backspace */
        ScreenData["backspace"] = 1;
    }

    if(c[0] == 30){ /* special char that represents an escape */
        ScreenData["charbuffer"] += c;
        ScreenData["sessionbuffer"] += c;
    }
    /* end special chars */

    if(c[0] > 31 && c[0] < 127){ /* normal printable chars */
        ScreenData["charbuffer"] += c;
        ScreenData["sessionbuffer"] += c;
    }
    if(sizeof(ScreenData["sessionbuffer"]) > 1024){
        ScreenData["sessionbuffer"] = ScreenData["sessionbuffer"][1..];
    }
    /* arrow keys and special sequences */
    if(sizeof(ScreenData["charbuffer"]) > 2){
        tmp = last(ScreenData["charbuffer"],3);
        if(tmp[0] == 30 && tmp[1] == 91 && tmp[2] == 65){ /* up */
            //tc("row minus","cyan");
            ScreenData["row"]--;
            ScreenData["charbuffer"] = "";
            arrowed = 1;
        }
        if(tmp[0] == 30 && tmp[1] == 91 && tmp[2] == 66){ /* down */
            //tc("row plus","cyan");
            ScreenData["row"]++;
            ScreenData["charbuffer"] = "";
            arrowed = 2;
        }
        if(tmp[0] == 30 && tmp[1] == 91 && tmp[2] == 68){ /* left */
            //tc("col minus","cyan");
            ScreenData["col"]--;
            ScreenData["charbuffer"] = "";
            arrowed = 3;
        }
        if(tmp[0] == 30 && tmp[1] == 91 && tmp[2] == 67){ /* right */
            //tc("col plus","cyan");
            ScreenData["col"]++;
            ScreenData["charbuffer"] = "";
            arrowed = 4;
        }
        if(!arrowed && sizeof(ScreenData["charbuffer"]) > 3){
            tmp = last(ScreenData["charbuffer"], 4); 
            if(tmp[0] == 30 && tmp[1] == 91 && tmp[2] == 51 &&
                    tmp[3] == 126){ /* delete */
                //tc("col plus","cyan");
                ScreenData["delete"] = 1;
                ScreenData["charbuffer"] = "";
            }
        }
    }
    /* Attempt to drop escapes that eluded us */
    ScreenData["sentinel"] = 0;
    tmp = ScreenData["charbuffer"];
    if(sizeof(tmp) == 3 && tmp[2] == 30){
        ScreenData["sentinel"] = 1;
    }
    else if(sizeof(ScreenData["sessionbuffer"]) > 2){
        if(ScreenData["sessionbuffer"][<2] == 30){
            if(ScreenData["sessionbuffer"][<1] == 91){
                ScreenData["sentinel"] = 1;
            }
        }
        if(ScreenData["sessionbuffer"][<3] == 30){
            if(ScreenData["sessionbuffer"][<2] == 91){
                ScreenData["sentinel"] = 1;
            }
        }
    }
    /* end arrow keys and special sequences */

    /* text input */
    if(sizeof(tmp) && tmp[0] != 30 && !ScreenData["sentinel"]){
        receive(tmp);
        if(tmp[0] == 91){
            //tc("ALART "+tmp+", prev: "+ScreenData["sessionbuffer"][<1],"green");
        }
        if(sizeof(ScreenData["sessionbuffer"]) > 2){
            if(ScreenData["sessionbuffer"][<1] == 91){
                //tc("SHITFUCK "+ScreenData["sessionbuffer"][<3]+" "+
                //  ScreenData["sessionbuffer"][<2]+" "+
                //  ScreenData["sessionbuffer"][<1], "red");
            }
        }
        if(ScreenData["insert"]){
            string begin;
            string end;
            col = ScreenData["col"];
            row = (ScreenData["row"] + FileData["topline"]) - 1;
            if(FileData["map"][row]){
                begin = FileData["map"][row][0..(col-2)];
                end = FileData["map"][row][(col-1)..];
                FileData["map"][row]=begin+ScreenData["charbuffer"]+end;
            }
        }
        else {
            col = ScreenData["col"];
            row = (ScreenData["row"] + FileData["topline"]) - 1;
            if(FileData["map"][row]){
                //tc("wtf "+identify(ScreenData["charbuffer"]));
                //tc("wtf2 "+identify(FileData["map"][row][col-1]));
                if(sizeof(FileData["map"][row]) >= col){
                    FileData["map"][row][col-1] = ScreenData["charbuffer"][0];
                }
                else {
                    FileData["map"][row] += ScreenData["charbuffer"];
                }
                //ScreenData["charbuffer"][sizeof(ScreenData["charbuffer"])-1];
                //ScreenData["charbuffer"];
            }
        }
        if(ScreenData["charbuffer"] == "\n"){
            CeditCollate();
            RedrawScreen();
        }
        receive("\r\e[2K");
        if(FileData["map"][row]) receive(FileData["map"][row]);
        ScreenData["col"]++;
        receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H"); 
        ScreenData["charbuffer"] = "";
    }
    /* end text input */

    /* deletion stuff */
    /* backspace */
    if(ScreenData["backspace"]){
        string begin;
        string end;
        col = ScreenData["col"];
        row = (ScreenData["row"] + FileData["topline"]) - 1;
        if(FileData["map"][row]){
            begin = FileData["map"][row][0..(col-3)];
            end = FileData["map"][row][(col-1)..];
            FileData["map"][row]=begin+end;
            receive("\r\e[2K");
            receive(FileData["map"][row]);
            ScreenData["col"]--;
            receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H");
        }
        ScreenData["charbuffer"] = "";
        ScreenData["backspace"] = 0;
    }
    /* end backspace */
    /* delete */
    if(ScreenData["delete"]){
        string begin;
        string end;
        col = ScreenData["col"];
        row = (ScreenData["row"] + FileData["topline"]) - 1;
        if(FileData["map"][row]){
            begin = FileData["map"][row][0..(col-2)];
            end = FileData["map"][row][(col)..];
            FileData["map"][row]=begin+end;
            receive("\r\e[2K");
            receive(FileData["map"][row]);
            receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H");
        }
        ScreenData["charbuffer"] = "";
        ScreenData["delete"] = 0;
    }
    /* end delete */
    /* line delete */
    if(ScreenData["linedel"]){
        row = (ScreenData["row"] + FileData["topline"]) - 1;
        if(FileData["map"][row]){
            FileData["map"][row] = 0;
            CeditCollate();
            RedrawScreen();
        }
        ScreenData["charbuffer"] = "";
        ScreenData["linedel"] = 0;
    }
    /* end line delete */
    /* end deletion stuff */

    /* redrawing, etc */
    //tc("row: "+ScreenData["row"]+", col: "+ScreenData["col"],"red");
    if(ScreenData["row"] > ScreenData["maxrow"]){ 
        ScreenData["row"] = ScreenData["maxrow"];
        if(FileData["topline"] < sizeof(FileData["map"])){
            FileData["topline"]++;
            RedrawScreen(FileData["topline"]);
        }
    }
    if(ScreenData["row"] < 1){ 
        ScreenData["row"] = 1;
        FileData["topline"]--;
        if(FileData["topline"] < 1){
            FileData["topline"] = 1;
        }
        RedrawScreen(FileData["topline"]);
    }
    if(ScreenData["col"] > ScreenData["maxcol"]) 
        ScreenData["col"] = ScreenData["maxcol"];
    if(ScreenData["col"] < 1) 
        ScreenData["col"] = 1;
    if(ScreenData["col"] > sizeof(FileData["map"][(ScreenData["row"] +
                    FileData["topline"]) - 1])){
        ScreenData["col"] = sizeof(FileData["map"][(ScreenData["row"] +
                    FileData["topline"]) - 1])+1; /* lol */
        if(ScreenData["col"] < 1) ScreenData["col"] = 1;
    }
    //tc("row: "+ScreenData["row"]+", col: "+ScreenData["col"],"green");
    receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H"); 
    StatReport();
    /* end redrawing, etc */

#ifdef __GET_CHAR_IS_BUFFERED__
    get_char("ReceiveChar", NOECHO);
#endif
    return 1;
}

varargs int SetCediting(int x, string file){
    int num;
    mixed tmp, lines;
    FileData = ([ "file" : "", "map" : ([]) ]);
    if(!this_player() || this_player() != this_object()) return 0;
    if(!sizeof(file)) file = "";
    if(x){
        cediting = 1;
        FileData["topline"] = 1;
        if(!FileData["map"]) FileData["map"] = ([]);
        if(true()){
            FileData["file"] = file;
            tmp = read_file(FileData["file"]);
            //tc("tmp: "+identify(tmp),"cyan");
            if(!tmp){
                string line = repeat_string(" ", ScreenData["maxcol"])+"\n";
                lines = ScreenData["maxrow"];
                FileData["contents"] = "";
                while(lines){
                    lines--;
                    FileData["contents"] += line;
                }
            }
            tmp = replace_string(tmp,"\n","\n"+sprintf("%c",17));
            lines = 1;
            foreach(mixed ln in explode(tmp,sprintf("%c",17))){
                FileData["map"][lines] = replace_string(ln,"\n","");
                lines++;
            }
        }
        receive("\r\e[2J"); /* cls */
        receive("\r\e[7l"); /* no wrapping */
        ScreenData["row"] = 1;
        ScreenData["col"] = 1;
        RedrawScreen(FileData["topline"]);
#ifdef __DSLIB__
        remove_get_char(this_object());
#endif
        get_char("ReceiveChar",1);
    }
    else {
        cediting = 0;
    }
    return cediting;
}

int GetCediting(){
    return cediting;
}
