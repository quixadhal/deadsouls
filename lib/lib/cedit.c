#define NOECHO 1
#ifndef CED_DISABLED
#define CED_DISABLED 0
#endif
#define CED_DEBUG 0

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
        ScreenData["row"] = 24;
    }
    ScreenData["maxrow"] = ScreenData["row"];
    ScreenData["col"] = 1;
    ScreenData["charbuffer"] = "";
    ScreenData["sessionbuffer"] = "   ";
    ScreenData["insert"] = 1;
    ScreenData["cedmode"] = 0;
}

int ClearBuffers(){
    ScreenData["charbuffer"] = "";
    ScreenData["sessionbuffer"] = "";
    this_object()->SetCharbuffer("");
}

varargs int StatReport(string str){
    receive("\r\e["+(ScreenData["maxrow"])+";1H"); /* put cursor at bottom */
    receive("\r\e[2K"); /* Erase the entire bottom row */
    if(!str && sizeof(ScreenData["report"])){
        str = ScreenData["report"];
        ScreenData["report"] = "";
    }
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
    mixed tmp = GetScreen();

    if(sizeof(tmp)){
        ScreenData["maxcol"] = tmp[0];
        ScreenData["maxrow"] = tmp[1];
    }
    else {
        ScreenData["maxcol"] = 79;
        ScreenData["maxrow"] = 24;
    }

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

static int UpdateScreen(){
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

static int rBackspace(){
    int col, row;
    string begin, end;
    //tc("cedit rBackspace()", "green");
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
    //RedrawScreen();
} 

static int rEnter(){
    if(!ScreenData["sessionbuffer"]) ScreenData["sessionbuffer"] = "";
    if(ScreenData["goto"]){
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
        this_object()->SetNoEcho(1);
        RedrawScreen();
    }
    else {
        ScreenData["charbuffer"] = "\n";
        ScreenData["sessionbuffer"] += sprintf("%c", 13);
    }
    return 1;
}

static int rAscii(string c){
    string tmp = c;
    int row, col, arrowed;
    if(!ScreenData["charbuffer"]) ScreenData["charbuffer"] = "";
    if(!ScreenData["sessionbuffer"]) ScreenData["sessionbuffer"] = "";
    if(ScreenData["goto"]){
        ScreenData["charbuffer"] += c;
        return 1;
    }
    else {
        ScreenData["charbuffer"] += c;
        ScreenData["sessionbuffer"] += c;
    }
    if(sizeof(ScreenData["sessionbuffer"]) > 1024){
        ScreenData["sessionbuffer"] = ScreenData["sessionbuffer"][1..];
    }
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
    return 1;
}

static int rDel(){
    string begin, end;
    int col, row;
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
    return 1;
}

int rCtrl(string c){
    string tmp;
    int row, col, arrowed;
    if(!ScreenData["charbuffer"]){
        ScreenData["charbuffer"] = this_object()->GetCharbuffer();
    }

    /* special chars */
    if(c == "q"){ /* Ctrl-Q for quitting without saving */
        write("Cancelling cedit!");
        ScreenData["cedmode"] = 0;
        FileData = ([ "file" : "", "map" : ([]) ]);
        ClearBuffers();
    }
    if(c == "g"){ /* Ctrl-G for going to a line */
        string tmpstr = "Please type the line number then press enter.";
        int where = sizeof(tmpstr);
        //StatReport(tmpstr);
        ScreenData["report"] = tmpstr;
        ScreenData["pregoto"] = ({ ScreenData["row"], ScreenData["col"] });
        ScreenData["row"] = (ScreenData["maxrow"]);
        ScreenData["col"] = (where + 1);
        receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H");
        ScreenData["charbuffer"] = "";
        ScreenData["goto"] = 1;
        this_object()->SetNoEcho(0);
    }
    if(c == "f"){ /* Ctrl-F */
        int targettopline = FileData["topline"] + (ScreenData["maxrow"]);
        int targetline = targettopline + ScreenData["row"];
        int maxlines = sizeof(FileData["map"]);
        if(targettopline > maxlines) targettopline = maxlines;
        if(targetline > maxlines) targetline = maxlines;
        FileData["topline"] = targettopline;
        ScreenData["row"] = targetline;
        RedrawScreen(FileData["topline"]);
        StatReport();
    }
    if(c == "b"){ /* Ctrl-B */
        int targettopline = FileData["topline"] - (ScreenData["maxrow"]);
        int targetline = targettopline - ScreenData["row"];
        if(targettopline < 1) targettopline = 1;
        if(targetline < 1) targetline = 1;
        FileData["topline"] = targettopline;
        ScreenData["row"] = targetline;
        RedrawScreen(FileData["topline"]);
        StatReport();
    }
    if(c == "l"){ /* Ctrl-L */
        RedrawScreen(FileData["topline"]);
        StatReport();
        ScreenData["charbuffer"] = "";
    }
    if(c == "d"){ /* Ctrl-D */
        row = (ScreenData["row"] + FileData["topline"]) - 1;
        if(FileData["map"][row]){
            FileData["map"][row] = 0;
            CeditCollate();
            RedrawScreen();
        }
        ScreenData["charbuffer"] = "";
        ScreenData["linedel"] = 0;
    }
    if(c == "i"){ /* Ctrl-I */
        if(!ScreenData["insert"]){
            ScreenData["insert"] = 1;
            StatReport("Insert mode enabled");
        }
    }
    if(c == "o"){ /* Ctrl-O */
        if(ScreenData["insert"]){
            ScreenData["insert"] = 0;
            StatReport("Overstrike mode enabled");
        }
    }
    if(c == "x"){ /* Ctrl-X */
        int ret;
#if CED_DISABLED
        StatReport("This alpha editor is not yet in shape to save files.");
#else
        if(sizeof(FileData["file"]) && sizeof(FileData["map"])){
            ret = CeditSave();
            if(ret){
                StatReport(FileData["file"]+" saved.");
                write("Exiting screen editor.");
                ScreenData["cedmode"] = 0;
                FileData = ([ "file" : "", "map" : ([]) ]);
                ClearBuffers();
            }
            else StatReport(FileData["file"]+" save FAILED.");
        }
#endif
    }
    if(c == "s"){ /* Ctrl-S */
        int ret;
#if CED_DISABLED
        StatReport("This alpha editor is not yet in shape to save files.");
#else
        if(sizeof(FileData["file"]) && sizeof(FileData["map"])){
            ret = CeditSave();
            if(ret) StatReport(FileData["file"]+" saved.");
            else StatReport(FileData["file"]+" save FAILED.");
        }
#endif
    }
    if(c == "c"){ /* Ctrl-C */
        ClearBuffers();
    }
    UpdateScreen();
    return 1;
}

static int rArrow(string str){
    switch(str){
        case "up" : ScreenData["row"]--; break;
        case "down" : ScreenData["row"]++; break;
        case "left" : ScreenData["col"]--; break;
        case "right" : ScreenData["col"]++; break;
    }
    ScreenData["charbuffer"] = "";
    UpdateScreen();
    return 1;
}

varargs int SetCedmode(int x, string file){
    int num;
    mixed tmp, lines;
    //tc("SetCedmode("+x+", "+identify(file)+")", "green");
#ifndef __DSLIB__
    return 0;
#else
    FileData = ([ "file" : "", "map" : ([]) ]);
    if(!this_player() || this_player() != this_object()) return 0;
    if(!sizeof(file)) file = "";
    if(x){
        ClearBuffers();
        ScreenData["cedmode"] = x;
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
    }
    else {
        ScreenData["cedmode"] = 0;
    }
    return ScreenData["cedmode"];
#endif
}

int GetCedmode(){
    return ScreenData["cedmode"];
}

static int rAnsi(string str){
    debug_message("cedit ansi received: "+str);
    return 1;
}

