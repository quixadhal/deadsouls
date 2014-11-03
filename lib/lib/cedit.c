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
    if(!str){
#if CED_DEBUG
        str = "screen: "+ScreenData["row"]+","+ScreenData["col"]+
            " file: "+((FileData["topline"]+ScreenData["row"])-1)+
            ","+ScreenData["col"]+", topline: "+FileData["topline"];
#else 
        str = "line: "+((FileData["topline"]+ScreenData["row"])-1)+
            " col: "+ScreenData["col"]+" file: "+ FileData["file"];
#endif
    }
    receive(str);
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
    if(topline > lastline) return 0;
    receive("\r\e[1;1H");
    receive("\r\e[2K");
    while(line < (ScreenData["maxrow"] )){
        string trunky; // patches for linewrapping are welcome
        int extra;
        receive("\r\e["+line+";1H");
        receive("\e[2K");
        if(FileData["map"][(line+topline)]){
            trunky = FileData["map"][(line+topline)];
            extra = sizeof(trunky) - ScreenData["maxcol"];
            if(extra > 0) trunky = truncate(trunky, extra);
            receive("\r\e[7l"+trunky+"\n");
        }
        line++;
    }
    receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H");
    ScreenData["charbuffer"] = "";
    return 1;
}

static int UpdateScreen(){
    if(ScreenData["row"] > (ScreenData["maxrow"] - 1)){ 
        if(!ScreenData["goto"] && !ScreenData["searching"])
            ScreenData["row"] = (ScreenData["maxrow"] - 1);
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
    receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H"); 
    StatReport();
}

static varargs int CeditCollate(int x){ /* when lines need to be resorted */
    int i; 
    string tmp = "";
    string *lns = sort_array(keys(FileData["map"]), 1);
    if(!sizeof(lns)) return 0;
    FileData["contents"] = "";
    foreach(int l in lns){
        if(!undefinedp(x) && !(l + x)) continue; /* remove a line */
        tmp += FileData["map"][l]+"\n";
        if(l == x) tmp += "\n"; /* add a line */
    }
    foreach(mixed key, mixed val in FileData["map"]){
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
    int i;
    string *lns = sort_array(keys(FileData["map"]), 1);
    if(!sizeof(lns)) return 0;
    foreach(int l in lns){
        ret += FileData["map"][l]+"\n";
    }
    foreach(mixed key, mixed val in FileData["map"]){
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
    col = ScreenData["col"];
    row = (ScreenData["row"] + FileData["topline"]) - 1;
    if(FileData["map"][row]){
        begin = FileData["map"][row][0..(col-3)];
        end = FileData["map"][row][(col-1)..];
        FileData["map"][row]=begin+end;
        receive("\r\e[2K");
        receive(FileData["map"][row]);
        ScreenData["col"]--;
        if((ScreenData["maxrow"] - 1) != ScreenData["row"] &&
                FileData["map"][row+1]){
            receive("\r\e["+(ScreenData["row"]+1)+";"+ScreenData["col"]+"H");
            receive("\r\e[2K");
            receive(FileData["map"][row+1]);
        }
        receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H");
    }
    ScreenData["charbuffer"] = "";
    ScreenData["backspace"] = 0;
} 

static int rEnter(){
    if(!ScreenData["sessionbuffer"]) ScreenData["sessionbuffer"] = "";
    if(ScreenData["searching"]){
        int end, i, start = ScreenData["pregoto"][0];
        end = sort_array(keys(FileData["map"]), 1)[<1];
        ScreenData["searching"] = 0;
        if(sizeof(ScreenData["search"])){
            ScreenData["oldsearch"] = ScreenData["search"];
            ScreenData["searchhit"] = 0;
        }
        else {
            ScreenData["search"] = ScreenData["oldsearch"];
            start = ScreenData["searchhit"] + 1;
        }
        i = start;
        while(i <= end){
            if(FileData["map"][i] && grepp(FileData["map"][i],
                        ScreenData["search"])){
                ScreenData["goto"] = 1;
                ScreenData["search"] = 0;
                ScreenData["searchhit"] = i;
                ScreenData["charbuffer"] = itoa(i);
                break;
            }
            i++;
        }
#if CED_DEBUG
        debug_message("ScreenData[\"search\"]: "+ScreenData["search"]);
        debug_message("ScreenData[\"goto\"]: "+ScreenData["goto"]);
        debug_message("ScreenData[\"charbuffer\"]: "+ScreenData["charbuffer"]);
        debug_message("ScreenData[\"oldsearch\"]: "+ScreenData["oldsearch"]);
#endif
        if(!ScreenData["goto"]){
            ScreenData["report"] = "String not found searching from line "+
                start;
        }
    }
    if(ScreenData["goto"]){
        int top;
        int line = atoi(ScreenData["charbuffer"]);
        if(line && FileData["map"][line]){
            ScreenData["report"] = "Going to "+ScreenData["charbuffer"];
            top = line - ScreenData["pregoto"][0]; 
            if(top < 1) top = 1;
            FileData["topline"] = top;
            ScreenData["row"] = (ScreenData["pregoto"][0]) + 1;
            ScreenData["col"] = 1;
        }
        else {
            ScreenData["report"] = "No such line!";
            ScreenData["row"] = ScreenData["pregoto"][0];
            ScreenData["col"] = ScreenData["pregoto"][1];
        }
        ScreenData["charbuffer"] = "";
        ScreenData["goto"] = 0;
        ScreenData["pregoto"] = 0;
        this_object()->SetNoEcho(1);
    }
    else {
        ScreenData["charbuffer"] = "\n";
        ScreenData["sessionbuffer"] += sprintf("%c", 13);
        CeditCollate((FileData["topline"]+ScreenData["row"])-1);
    }
    UpdateScreen();
    RedrawScreen();
    return 1;
}

static int rAscii(string c){
    string tmp = c;
    int row, col, arrowed;
    if(!ScreenData["charbuffer"]) ScreenData["charbuffer"] = "";
    if(!ScreenData["sessionbuffer"]) ScreenData["sessionbuffer"] = "";
    if(ScreenData["searching"]){
        if(!ScreenData["search"]) ScreenData["search"] = "";
        ScreenData["charbuffer"] += c;
        ScreenData["search"] += c;
        return 1;
    }
    else if(ScreenData["goto"]){
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
        }
        if(sizeof(ScreenData["sessionbuffer"]) > 2){
            if(ScreenData["sessionbuffer"][<1] == 91){
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
                if(sizeof(FileData["map"][row]) >= col){
                    FileData["map"][row][col-1] = ScreenData["charbuffer"][0];
                }
                else {
                    FileData["map"][row] += ScreenData["charbuffer"];
                }
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
        if((ScreenData["maxrow"] - 1) != ScreenData["row"] &&
                FileData["map"][row+1]){
            receive("\r\e["+(ScreenData["row"]+1)+";"+ScreenData["col"]+"H");
            receive("\r\e[2K");
            receive(FileData["map"][row+1]);
        }
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
    else if(c == "g"){ /* Ctrl-G for going to a line */
        string tmpstr = "Please type the line number then press enter.";
        int where = sizeof(tmpstr);
        ScreenData["report"] = tmpstr;
        ScreenData["pregoto"] = ({ ScreenData["row"], ScreenData["col"] });
        ScreenData["row"] = (ScreenData["maxrow"]);
        ScreenData["col"] = (where + 1);
        receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H");
        ScreenData["charbuffer"] = "";
        ScreenData["goto"] = 1;
        this_object()->SetNoEcho(0);
    }
    else if(c == "f"){ /* Ctrl-F */
        int targettopline, targetline, maxlines, lasty;
        lasty = sort_array(keys(FileData["map"]), 1)[<1];
        targettopline = FileData["topline"] + (ScreenData["maxrow"] - 1);
        if(targettopline > (lasty - ScreenData["maxrow"]) + 1){
            targettopline = (lasty - ScreenData["maxrow"]) + 1;
        }
        targetline = targettopline + ScreenData["row"];
        maxlines = sizeof(FileData["map"]);
        if(targettopline > maxlines) targettopline = maxlines;
        if(targetline > maxlines) targetline = maxlines;
        FileData["topline"] = targettopline;
        ScreenData["row"] = targetline;
        RedrawScreen(FileData["topline"]);
    }
    else if(c == "b"){ /* Ctrl-B */
        int targettopline = FileData["topline"] - (ScreenData["maxrow"] - 1);
        int targetline = targettopline - ScreenData["row"];
        if(targettopline < 1) targettopline = 1;
        if(targetline < 1) targetline = 1;
        FileData["topline"] = targettopline;
        ScreenData["row"] = targetline;
        RedrawScreen(FileData["topline"]);
    }
    else if(c == "l"){ /* Ctrl-L */
        RedrawScreen(FileData["topline"]);
        ScreenData["charbuffer"] = "";
    }
    else if(c == "d"){ /* Ctrl-D */
        row = (ScreenData["row"] + FileData["topline"]) - 1;
        if(FileData["map"][row]){
            CeditCollate(-((FileData["topline"]+ScreenData["row"])-1));
            RedrawScreen();
        }
        ScreenData["charbuffer"] = "";
        ScreenData["linedel"] = 0;
    }
    else if(c == "i"){ /* Ctrl-I */
        if(!ScreenData["insert"]){
            ScreenData["insert"] = 1;
            ScreenData["report"] = "Insert mode enabled";
        }
    }
    else if(c == "o"){ /* Ctrl-O */
        if(ScreenData["insert"]){
            ScreenData["insert"] = 0;
            ScreenData["report"] = "Overstrike mode enabled";
        }
    }
    else if(c == "x"){ /* Ctrl-X */
        int ret;
#if CED_DISABLED
        ScreenData["report"] = "This alpha editor is not yet in shape to "+
            "save files.";
#else
        if(sizeof(FileData["file"]) && sizeof(FileData["map"])){
            ret = CeditSave();
            if(ret){
                ScreenData["report"] = FileData["file"]+" saved.";
                write("Exiting screen editor.\n");
                ScreenData["cedmode"] = 0;
                FileData = ([ "file" : "", "map" : ([]) ]);
                ClearBuffers();
            }
            else ScreenData["report"] = FileData["file"]+" save FAILED.";
        }
#endif
    }
    else if(c == "s"){ /* Ctrl-S */
        int ret;
#if CED_DISABLED
        ScreenData["report"] = "This alpha editor is not yet in shape "+
            "to save files.";
#else
        if(sizeof(FileData["file"]) && sizeof(FileData["map"])){
            ret = CeditSave();
            if(ret) ScreenData["report"] = FileData["file"]+" saved.";
            else ScreenData["report"] = FileData["file"]+" save FAILED.";
        }
#endif
    }
    else if(c == "c"){ /* Ctrl-C */
        ClearBuffers();
    }
    else if(c == "31"){ /* Ctrl-/ for a search */
        string tmpstr = "Please type the word to search for: ";
        int where = sizeof(tmpstr);
        ScreenData["report"] = tmpstr;
        ScreenData["pregoto"] = ({ ScreenData["row"], ScreenData["col"] });
        ScreenData["row"] = (ScreenData["maxrow"]);
        ScreenData["col"] = (where + 1);
        receive("\r\e["+ScreenData["row"]+";"+ScreenData["col"]+"H");
        ScreenData["charbuffer"] = "";
        ScreenData["searching"] = 1;
        this_object()->SetNoEcho(0);
    }
    else if(c == "28"){ /* Ctrl-\ */
        ScreenData["report"] = "esc-28 received";
    }
    if(ScreenData["cedmode"]) UpdateScreen();
    else {
        this_object()->SetCharmode(1);
        this_object()->erase_prompt();
        this_object()->write_prompt();
    }
    return 1;
}

static int rArrow(string str){
    switch(str){
        int lasty;
        case "up" : ScreenData["row"]--; break;
        case "down" : 
                    if((ScreenData["row"] > (ScreenData["maxrow"] - 2) )){
                        lasty = sort_array(keys(FileData["map"]), 1)[<1];
                        if((FileData["topline"] > (lasty - ScreenData["maxrow"]) + 1)){
                            break;
                        }
                    }
                    ScreenData["row"]++; 
                    break;
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
#ifndef __DSLIB__
    return 0;
#else
    FileData = ([ "file" : "", "map" : ([]) ]);
    if(!this_player() || this_player() != this_object()) return 0;
    if(!sizeof(file)) file = "";
    if(x){
        ClearBuffers();
        ScreenData["cedmode"] = x;
        ScreenData["insert"] = 1;
        FileData["topline"] = 1;
        if(!FileData["map"]) FileData["map"] = ([]);
        if(true()){
            FileData["file"] = file;
            tmp = read_file(FileData["file"]);
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
            tmp = replace_string(tmp,"\t","    ");
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
        ScreenData["report"] = "Editing "+FileData["file"];
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
    return 1;
}

