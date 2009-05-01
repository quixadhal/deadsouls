
//	ansi header Creator	:  Gothic@TMI-2  
//
//	The standard set of ANSI codes for mudlib use.
//
// Hi-intensity black added by Blue, 941130.  Why leave it out?

#define ESC	""
#define CSI	ESC + "["

/*  Foreground Colors  */

#define BLK ESC+"[30m"          /* Black    */
#define RED ESC+"[31m"          /* Red      */
#define GRN ESC+"[32m"          /* Green    */
#define YEL ESC+"[33m"          /* Yellow   */
#define BLU ESC+"[34m"          /* Blue     */
#define MAG ESC+"[35m"          /* Magenta  */
#define CYN ESC+"[36m"          /* Cyan     */
#define WHT ESC+"[37m"          /* White    */

/*   Hi Intensity Foreground Colors   */

#define HIK ESC+"[1;30m"        /* Black    */
// Note hi-black comes out as dark grey.
#define HIR ESC+"[1;31m"        /* Red      */
#define HIG ESC+"[1;32m"        /* Green    */
#define HIY ESC+"[1;33m"        /* Yellow   */
#define HIB ESC+"[1;34m"        /* Blue     */
#define HIM ESC+"[1;35m"        /* Magenta  */
#define HIC ESC+"[1;36m"        /* Cyan     */
#define HIW ESC+"[1;37m"        /* White    */

/*  Background Colors  */

#define BBLK ESC+"[40m"          /* Black    */
#define BRED ESC+"[41m"          /* Red      */
#define BGRN ESC+"[42m"          /* Green    */
#define BYEL ESC+"[43m"          /* Yellow   */
#define BBLU ESC+"[44m"          /* Blue     */
#define BMAG ESC+"[45m"          /* Magenta  */
#define BCYN ESC+"[46m"          /* Cyan     */
#define BWHT ESC+"[47m"          /* White    */

/* High Intensity Background Colors  */

#define HBRED ESC+"[41;1m"       /* Red      */
#define HBGRN ESC+"[42;1m"       /* Green    */
#define HBYEL ESC+"[43;1m"       /* Yellow   */
#define HBBLU ESC+"[44;1m"       /* Blue     */
#define HBMAG ESC+"[45;1m"       /* Magenta  */
#define HBCYN ESC+"[46;1m"       /* Cyan     */
#define HBWHT ESC+"[47;1m"       /* White    */

#define NOR ESC+"[2;37;0m"      /* Puts everything back to normal */

/*  Additional ansi Esc codes added to ansi.h by Gothic  april 23,1993 */
/* Note, these are Esc codes for VT100 terminals, and emmulators */
/*       and they may not all work within the mud               */

#define BOLD ESC+"[1m"          /* Turn on bold mode */
#define CLR ESC+"[2J"           /* Clear the screen  */
#define HOME ESC+"[H"           /* Send cursor to home position */
#define REF CLR+HOME            /* Clear screen and home cursor */
#define BIGTOP ESC+"#3"         /* Dbl height characters, top half */
#define BIGBOT ESC+"#4"         /* Dbl height characters, bottem half */
#define SAVEC ESC+"[s"           /* Save cursor position */
#define REST ESC+"[u"            /* Restore cursor to saved position */
#define REVINDEX ESC+"M"        /* Scroll screen in opposite direction */
#define SINGW ESC+"#5"          /* Normal, single-width characters */
#define DBL ESC+"#6"            /* Creates double-width characters */
#define FRTOP ESC+"[2;25r"      /* Freeze top line */
#define FRBOT ESC+"[1;24r"      /* Freeze bottom line */
#define UNFR ESC+"[r"           /* Unfreeze top and bottom lines */
#define BLINK ESC+"[5m"         /* Initialize blink mode */
#define U ESC+"[4m"             /* Initialize underscore mode */
#define REV ESC+"[7m"           /* Turns reverse video mode on */
#define HIREV ESC+"[1,7m"       /* Hi intensity reverse video  */

#define STRIP_ME ({ BLK,RED,GRN,YEL,BLU,MAG,CYN,WHI, \
        HIK, HIR, HIG, HIY, HIB, HIM, HIC, HIW, \
        BBLK, BRED, BGRN, BYEL, BBLU, BMAG, BCYN, BWHI, \
        HBBLK, HBRED, HBGRN, HBYEL, HBBLU, HBMAG, HBCYN, HBWHI, \
        NOR, BOLD, CLR< HOME, REF, BIGTOP, BIGBOT, SAVEC, REST, \
        REVINDEX, SINGW, DBL, FRTOP, FRBOT, UNFR, BLINK, U ESC, REV ESC, HIREV ESC })

#define BEEP    "^G"

/* Program      : /feature/me.c
 * Author       :Kenny@Eternal.Civilization (3/25/1998)
 *
 * port to chinese wizard group : /feature/vi.c by ken@chinesemud.net (99-3-15)
 */


#define ECHO 0                          // µÂ®í×„®‹×˜Ó¬ÎÕ×¾ÚÙÕ¬
#define HIDE 1                          // µÂ®í×„®‹×˜™©ÎÕ×¾ÚÙÕ¬
#define REFRESH 1                       // µÂ®í×„®‹ßßÓ¬ÙÛ©†©¡È†
#define LOGFILE "static/VI"             // ´´—¹ÓŒµ¯­ÌÃ­´´

void start_edit(string filename);       // ¦Ÿ»Ï˜€­¡Ë•ú¼À¬×­˜€­¡
private void _append(object pl);        // ÕõÔğ˜ŠßßÈ†­ÔÚÙ
private void _append_line(object pl);   // ÕõÔğ˜Š´ÚÎÃ­ÔÓ©ÏÃÏÏ
private void _backspace(object pl);     // Ğ¾ş²Ôğ˜ŠÌ—´ıÚÙ
protected void _confirm_save(string str, object pl); // Á‹À¬˜€­¡Ë•Ì—®¸®Î³†´´
private void _del_char(object pl, int bs);      // Ğ¾ş²Ôğ˜ŠĞÎ´ıÚÙ
private void _del_line(object pl);              // Ğ¾ş²Ø“ÏÏ
private int  _dispatcher(object pl);            // ¸ÙÊĞÙ¹Â´»™©Í¼¦¯×»
private void _down(object pl);                  // Ôğ˜ŠÍ·ÎÃÓËÓ©ÏÏ
private void _end(object pl);                   // »½Ôğ˜ŠÓË´»ÏÏğ™
private void _esc(object pl);                   // ³Ô×„®‹ıú×»©Û´»È’Âıú×»
private void _file_end(object pl);              // »½Ôğ˜ŠÓË´»´´ğ™
private void _file_head(object pl);             // »½Ôğ˜ŠÓË´»´´Í¸
private void _goto_line(object pl);             // Ş°´»ŞÛµ¿ÏÏ®Ñ
protected void _goto_line_done(string str, object pl, int rein); // ®í´ÈÏÏ¦Ê
private void _help(object pl);                  // ¹¿Ù”©¡È†
private void _home(object pl);                  // »½Ôğ˜ŠÓË´»ÏÏ×Ú
protected void _input(string str, object pl, int fresh); // µÂ´È×„®‹ÚÙ³«
private void _insert(object pl);                // ÕõÔğ˜ŠÌ—È†™…ÚÙ
private void _insert_line(object pl);           // ÕõÔğ˜ŠĞÎ¸»­ÔÓ©ÏÃÏÏ
private void _insert_exclam_mark(object pl);    // ÕõÔğ˜ŠÌ—È†™…®‹Ó©¹÷í¹!í ÚÙÕ¬
private void _join(object pl);                  // »½ÎÃÏÏğıÚÙÓË´»³İÏÏßßÈ†
private void _keymap(object pl);                // µ¿Ó…ÏÃ ªı›­’
protected void _keymap_done(string str, object pl); // ®í´Èµ¿Ó… ªı›­’
private void _left(object pl);                  // Ôğ˜ŠÍ·ÚÓËÓ©ÚÙ
// ÕõÚ³Ş¼ÂÏÎÕ×¾ÒµÎó
private varargs void _message(object pl, string str, string callback, int hide,
        mixed arg);
protected void _message_done(string str, object pl); // »ß×°ÒµÎó´ıÎÕ×¾
private void _next_match(object pl);            // İÒÒ—/Ş†©©ÎÃ¹÷¸“¦ÎÚÙ³«
private void _page_down(object pl);             // Í·ÎÃ¾Ó©Óşú¿22 ÏÏú¨
private void _page_up(object pl);               // Í·ĞÎ¾Ó©Óşú¿22 ÏÏú¨
private void _process(object pl);               // ³ªÁ×„®‹ıú×»´ıÚÙ³«×„®‹
protected void _quit(mixed unused, mixed pl);   // Á‹À¬˜€­¡Ë•
private void _refresh(object pl);               // ¹’ÏÃ©¡È†
private void _refresh_cursor(object pl);        // ¹’ÏÃÔğ˜Šğ©ÙÈ
// ÙÛ©†©¡È†
private void _refresh_screen(object pl, int s_row, int e_row, int s_col);
private void _refresh_status(object pl);        // ¹’ÏÃÚ³Ş¼ÂÏ
private void _replace(object pl);               // ÚÙ³«Ş†©©
protected void _replace_done(string str, object pl); // ®í´ÈŞ†©©ÚÙ³«
private void _right(object pl, int col, int extra);  // Ôğ˜ŠÍ·ÔÓÓËÓ©ÚÙ
private void _search(object pl);                     // ÚÙ³«İÒÒ—
protected void _search_done(string str, object pl);  // ®í´ÈİÒÒ—ÚÙ³«
private void _tab(object pl);                        // È’Âıú×»ÎÃ TAB ­’´ıµ¿ğ©
private void _undef_key(object pl);                  // ®íÎ“ış ªı›­’µ¿Ó…
protected void _undef_key_done(string str, object pl); // ®í´ÈÙ¹µ¿ ªı›­’
private void _up(object pl);                           // Ôğ˜ŠÍ·ĞÎÓËÓ©ÏÏ
private void _write(object pl);                        // ®í´È³ó³†´´—¹È“şË
protected void _write_done(string str, object pl, int quit); // ³ó³†´´—¹

private mapping EditedFiles=([ ]);              // ˜€­¡ÙÏ´ı´´—¹
/*
   int iS_Row, iE_Row,                             // ÀĞ­“×Ô³—´ı×­ı¨ÏÏ¦Ê
   iS_Col, iE_Col,                             // ÀĞ­“×Ô³—´ı×­ğ³Á¹ğ©
   iRow,   iCol,                               // Ôğ˜Šğ©ÙÈ
   is_Command_Mode, is_NewFile, is_Modify,     // ´´—¹Ú³À÷ËŒ˜Š
   is_Substituted;                             // ³”©©ıõÃ‹ËŒ˜Š
   string *sText,                                  // ´´—¹ıõ®œ
   sChar, sOldChar,                         // ×„®‹ÚÙ³«ú¿È’Âú¨
   sCount,                                  // È’Â­Ë×²
   sFileName,                               // ˜€­¡´´È“
   sSearch, sReplace;                       // İÒÒ—/Ş†©©ÚÙ³«
   mapping keymap;                                 //  ªı›­’µ¿Ó…µÕØØ˜
 */

void start_edit(string file)                // ¦Ÿ»Ï˜€­¡Ë•ú¼À¬×­˜€­¡
{
    int  i;
    mapping me;
    //tc("starting edit of "+file);

    if (objectp(EditedFiles[file]))
    {
        write(sprintf("%s Ø²Õõ˜€­¡Ø‚¹÷´´—¹ú¼Ì‹ĞÕ¦øÕã˜€­¡íú\n",
                    capitalize(EditedFiles[file]->query("id"))));
        return;
    } // if
    this_player()->SetProperty("me", ([ ]));
    me = this_player()->GetProperty("me");
    me["sFileName"] = file;
    if (file_size(file) <= 0)
    {        // ´´—¹şñµ®ÏíŒµ 1
        me["sText"] = ({ "" });
        if (file_size(file) < 0)
        {        // ÏÃ´´
            me["is_Command_Mode"] = 0;
            me["is_NewFile"] = 1;
        }
    }
    else
    {
        string content=read_file(file), tabs="        ";

        // ´´—¹şñµ®³Œµ 0
        me["is_Command_Mode"] = 1;
        me["is_NewFile"] = 0;
        if (stringp(content))
        {
            me["sText"] = explode(read_file(file), "\n");
            // »½ TABú¼ÓÕ 8 ¹÷ÀØ—Ú³”Ş†ú¼ÓÕ¸»˜ƒÔğ˜Šğ©ÙÈ´ı­Ëİƒ
            for (i=sizeof(me["sText"])-1; i>=0; i--)
            {
                int len=sizeof(me["sText"][i]);
                reset_eval_cost();
                for (int j=0; j<len; j++)
                    if (me["sText"][i][j] == '\t')
                        me["sText"][i] = replace_string(
                                me["sText"][i], "\t", tabs[0..7-j%8]);
            }
        }
        else me["sText"] = ({ "" });
    } // old file

    me["sCount"] = "";
    me["sSearch"] = me["sReplace"] = me["is_Modify"] = 0;
    me["iE_Row"] = 23;
    me["iS_Row"] = me["iS_Col"] = me["iRow"] = me["iCol"] = 1;
    EditedFiles += ([ file: this_player()->link() ]);

    // ®í´È/Ğˆµ¿ ªı›­’µ¿Ó…µÕØØ˜
    if (!mapp(me["keymap"] = this_player()->query("me_keymap")))
        me["keymap"] = ([ ]);
    if (this_player()->query("me_no_help") ||
            this_player()->GetProperty("no_me_help"))
    {        // ¸Ì´õÓ©³ğ× ÔÈ˜€­¡Ë•ú¼Ù˜»Ô»°®‹˜€­¡Ë•
        _refresh_screen(this_player(), 1, 23, 1);

        get_char("_input", HIDE, this_player());
    }
    else
    {        // ´õÓ©³ğ× ÔÈ˜€­¡Ë•ú¼Î®À³ ªı›İ´È•
        this_player()->SetProperty("no_me_help", 1);
        _help(this_player());
    }
} // start_edit()


private void _refresh_cursor(object pl)        // ¹’ÏÃÔğ˜Šğ©ÙÈ
{
    mapping me=pl->GetProperty("me");
    printf(ESC "[%d;%df", me["iRow"]+1, me["iCol"]);
} // _refresh_cursor()


private void _refresh_status(object pl)        // ¹’ÏÃÚ³Ş¼ÂÏ
{
    mapping me=pl->GetProperty("me");

    // ÓËµ¯Ôğ˜Š´» (1,1)ú¼ÔÈ¸³Î€ÚÙÕ¬ÎÕ×¾Ú³Ş¼ÂÏ
    printf(HOME + NOR + REV + "ÂÏ:%4d ÏÏ:%3d %-43s %10s [È’Âıú×»]" + NOR,
            me["iS_Row"]+me["iRow"]-1, me["iS_Col"]+me["iCol"]-1,
            sprintf("%s %s", me["sFileName"], (me["is_Modify"]? "(ÓÒÏ¹ı)":
                    (me["is_NewFile"]? "(ÏÃ´´)": "")))[0..42],
            (me["is_Command_Mode"]? "?:¹¿Ù”©¡È†": "[×„®‹ıú×»]"));
    _refresh_cursor(pl);
} // _refresh_status()


// ÙÛ©†©¡È†
private void _refresh_screen(object pl, int s_row, int e_row, int s_col)
{
    int  i, temp;
    mapping me=pl->GetProperty("me");


    if (e_row < s_row)                // ­Œ™‰×­ı¨ÏÏ¦Ê´ıÎ®ßß³ğÏø
    {
        temp = e_row;
        e_row = s_row;
        s_row = temp;
    }

    if (s_row < 1) s_row = 1;
    if (s_col < 1) s_col = 1;
    if (e_row > sizeof(me["sText"])) // ™©ı›ş¼ ²´´ğ™
        e_row = sizeof(me["sText"]);
    if (e_row-s_row > 22) e_row = s_row+22;

    me["iS_Row"] = s_row;        me["iE_Row"] = e_row;
    me["iS_Col"] = s_col;        me["iE_Col"] = s_col+79;

    printf(REF+NOR);                // Ì…ş²©¡È†ú¼ÓËµ¯Ôğ˜Š´» (1,1)
    for (i=me["iS_Row"]-1; i<me["iE_Row"]; i++)
        printf("\r\n%s", me["sText"][i][me["iS_Col"]-1..me["iE_Col"]-1]);
    for (i=me["iS_Row"]+22-me["iE_Row"]; i>0; i--)
        printf("\r\n~");        // ™©ÚƒÏÏ¦ÊÔÈ ~ ˜×¾
    _refresh_status(pl);
} // _refresh_screen()


protected void _input(string str, object pl, int fresh) // µÂ´È×„®‹ÚÙ³«
{
    int    i, size, bs=0;
    string *chars;
    mapping me=pl->GetProperty("me");
    //tc("_input("+identify(str)+", "+identify(pl)+", "+identify(fresh)+")");
    if (fresh)
    {        // ÙÛ©†©¡È†
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
        get_char("_input", HIDE, pl);
        return;
    }
    //tc("1");
    me["is_Substituted"] = 1;       // Î®Ğˆµ¿³”©©ıõÃ‹ËŒ˜Š
    if (str == "")                  // ³”©©´Ÿ Backspace/Ctrl-Backspace
    {
        str = "BACKSPACE";
        bs = 1;
    }
    if (str[0] == 27)
    {
        chars = explode(str, ESC);      // ÓœÍÒÃ‹´ıÚÙ³«ú¼ÓÕ ESC µÎÀ¬
        for (i=sizeof(chars)-1; i>=0; i--)
            chars[i] = ESC + chars[i];      // ­Ô©Û ESC
        if (sizeof(chars) == 0)                 // ®ŸÙ©×Ì´ÑÓ©ÚÙÕ¬ ESC
            chars = ({ ESC });
    }
    else    chars = ({ str });

    size = sizeof(chars);
    for (i=0; i<size; i++)
    {        // Ú¬©©ÓœÍÒÃ‹ğ¬˜€­¡Ë•ıõÃ‹
        switch (chars[i])
        {
            case "\t":       me["sChar"] = "TAB";   break;
            case "\r":       me["sChar"] = "ENTER"; break;
            case ESC:        me["sChar"] = "ESC";   break;
                             case ESC "OA":
                                 case ESC "[A":   me["sChar"] = "UP";    break;
                             case ESC "OB":
                                 case ESC "[B":   me["sChar"] = "DOWN";  break;
                             case ESC "OC":
                                 case ESC "[C":   me["sChar"] = "RIGHT"; break;
                             case ESC "OD":
                                 case ESC "[D":   me["sChar"] = "LEFT";  break;
                             case ESC "OH":
                                 case ESC "[H":
                                 case ESC "1~":
                                 case ESC "[1~":  me["sChar"] = "HOME";  break;
                             case ESC "[2~":  me["sChar"] = "INSERT";break;
                             case ESC "[3~":  me["sChar"] = "DELETE";break;
                             case ESC "4~":   me["sChar"] = "END";   break;
                             case ESC "[4~":  me["sChar"] = "END";   break;
                             case ESC "[5~":  me["sChar"] = "PAGEUP";break;
                             case ESC "[6~":  me["sChar"] = "PAGEDOWN";break;
                             case ESC "OP":   me["sChar"] = "F1";    break;
                             case ESC "OQ":   me["sChar"] = "F2";    break;
                             case ESC "OR":   me["sChar"] = "F3";    break;
                             case ESC "OS":   me["sChar"] = "F4";    break;
                             case ESC "m":    me["sChar"] = "F5";    break;
                             case ESC "[17~": me["sChar"] = "F6";    break;
                             case ESC "[18~": me["sChar"] = "F7";    break;
                             case ESC "[19~": me["sChar"] = "F8";    break;
                             case ESC "[20~": me["sChar"] = "F9";    break;
                             case ESC "[21~": me["sChar"] = "F10";   break;
                             case ESC "[23~": me["sChar"] = "F11";   break;
                             case ESC "[24~": me["sChar"] = "F12";   break;
            default:
                             if (sizeof(chars[i])==1 &&
                                     0<chars[i][0] && chars[i][0]<27)
                                 switch (chars[i][0])
                                 { // ³”©©ıõÃ‹ú¼× Õõ×„®‹ıú×»Ó™ı›× ÔÈÙ¹Â
                                     case  1: me["sChar"]="Ctrl-A"; break;
                                     case  2: me["sChar"]="Ctrl-B"; break;
                                     case  4: me["sChar"]="Ctrl-D"; break;
                                     case  6: me["sChar"]="Ctrl-F"; break;
                                     case  7: me["sChar"]="Ctrl-G"; break;
                                     case 10: me["sChar"]="Ctrl-J"; break;
                                     case 11: me["sChar"]="Ctrl-K"; break;
                                     case 12: me["sChar"]="Ctrl-L"; break;
                                     case 14: me["sChar"]="Ctrl-N"; break;
                                     case 15: me["sChar"]="Ctrl-O"; break;
                                     case 16: me["sChar"]="Ctrl-P"; break;
                                     case 17: me["sChar"]="Ctrl-Q"; break;
                                     case 18: me["sChar"]="Ctrl-R"; break;
                                     case 19: me["sChar"]="Ctrl-S"; break;
                                     case 20: me["sChar"]="Ctrl-T"; break;
                                     case 21: me["sChar"]="Ctrl-U"; break;
                                     case 22: me["sChar"]="Ctrl-V"; break;
                                     case 23: me["sChar"]="Ctrl-W"; break;
                                     case 24: me["sChar"]="Ctrl-X"; break;
                                     case 25: me["sChar"]="Ctrl-Y"; break;
                                     case 26: me["sChar"]="Ctrl-Z"; break;
                                 } // switch
                             else
                             {
                                 me["sChar"] = chars[i];            // ³İğ¬ËØÍ¿ÚÙ³«
                                 if (!bs) me["is_Substituted"] = 0; // Ì…ş²³”©©ıõ
                             } // else
        } // switch
        // »½Ú¬©©ßß´ıÚÙ³«İÍ´»¸ÙÊĞÙ¹Â¦¯×»³ªÁú¼™ó´®³²µÂ®íÎÃ¹÷×„®‹
        //tc("me[\"sChar\"]: "+me["sChar"], "blue");
        if (_dispatcher(pl) && i==size-1) get_char("_input", HIDE, pl);
    } // for
} // input()


private int _dispatcher(object pl)                // ¸ÙÊĞÙ¹Â´»™©Í¼¦¯×»
{
    int  i, count=1;
    mapping me=pl->GetProperty("me");


    if ( me["is_Command_Mode"] ||
            (me["is_Substituted"] && // ®È×„®‹ıú×»Ó™ı›× ÔÈışÏ¨Ù¹Â
             // Ø‚µ‘¹÷Ù¹Â™©ÕõØ‚³ªÁ
             me["sChar"]!="ENTER" && me["sChar"]!="TAB") )
    {        // ÎÙÕõ×ÌÈ’Âıú×»
        if (sizeof(me["sChar"])!=1 && !me["is_Substituted"])
            return 1;
        // ­Œ™‰ ªı›­’µ¿Ó…µÕØØ˜ú¼ÙÛÏÃµ¿Ó…È’Â
        if (!undefinedp(me["keymap"][me["sChar"]]))
            me["sChar"] =
                me["keymap"][me["sChar"]];
        if (!stringp(me["sChar"])) return 1; // Type checking...
        // ­Œ™‰×Ì×²ÚÙ´ı©—ú¼¾Í×Ì¹»­Ô´»È’Â­Ë×²Á
        if ('0'<=me["sChar"][0] && me["sChar"][0]<='9')
            me["sCount"] += me["sChar"];
        else
        {
            sscanf(me["sCount"], "%d", count);
            me["sCount"] = ""; // ­Ë×²Ë• ‰Âƒ
            // ÙÛ¹™ĞÎ¹÷Ù¹Â
            if (me["sChar"] == ".")
                me["sChar"] = me["sOldChar"];
            else        // ­ÌÃ­Ø‚³ğ´ıÙ¹Â
                me["sOldChar"] = me["sChar"];
        }

        for (i=0; i<count; i++)
            switch (me["sChar"]) // ¸ÙÊĞÙ¹Â´»™©Í¼¦¯×»
            {
                case "a":_append(pl);                           break;
                case "d":
                    case "Ctrl-D": _del_line(pl);                  break;
                case "f":
                    case "Ctrl-Z": _refresh(pl);                break;
                case "g":
                    case "Ctrl-G": _goto_line(pl);                break;
                case "h":
                    case "LEFT":
                    case "Ctrl-J": _left(pl);                   break;
                case "i":      _insert(pl);                break;
                case "j":
                    case "DOWN":
                    case "Ctrl-K": _down(pl);                break;
                case "k":
                    case "UP":
                    case "Ctrl-O": _up(pl);                        break;
                case "l":
                    case "RIGHT":
                    case "Ctrl-L": _right(pl, 1,
                            (me["is_Command_Mode"]?0:1));
                break;
                case "m": _keymap(pl);                        break;
                case "n":
                    case "Ctrl-N": _next_match(pl);                break;
                case "o": _append_line(pl);                    break;
                case "q":        // ˜×¾™©ÕãµÂ®íÏÃ×„®‹ú¼»ß×°˜€­¡
                    case "F4":
                    case "Ctrl-Q": _quit(0, pl);                return 0;
                case "r":
                    case "Ctrl-R": _replace(pl);                break;
                case "s":
                    case "Ctrl-S": _search(pl);                   break;
                case "u": _undef_key(pl);                break;
                case "w":
                    case "F2":
                    case "Ctrl-W": _write(pl);                      break;
                case "x":
                    case "DELETE":
                    case "Ctrl-X":   _del_char(pl, 0);        break;
                case "A":
                    case "HOME":
                    case "Ctrl-A":   _home(pl);        break;
                case "B":
                    case "PAGEUP":
                    case "Ctrl-B":   _page_up(pl);        break;
                case "E":
                    case "END":
                    case "Ctrl-E":   _end(pl);        break;
                case "F":
                    case "PAGEDOWN":
                    case "Ctrl-F":   _page_down(pl);        break;
                case "J":
                    case "Ctrl-P": _join(pl);         break;
                case "O": _insert_line(pl);         break;
                case "U":
                    case "Ctrl-U": _file_head(pl);        break;
                case "V":
                    case "Ctrl-V": _file_end(pl);        break;
                case "?":
                    case "F1":
                    case "Ctrl-Y": _help(pl);         break;
                case "~":
                    case "Ctrl-T": _insert_exclam_mark(pl);        break;
                case "BACKSPACE": _backspace(pl);          break;
                case "ESC": _esc(pl);                         break;
                case "TAB": _tab(pl);                        break;
            } // switch
    } // if me["is_Command_Mode"]
    else
        _process(pl);                // ³ªÁ×„®‹ıú×»´ıÚÙ³«×„®‹

    return 1;                        // ˜×¾­ŞÏ°µÂ®íÏÃ×„®‹ú¼­ŞÏ°˜€­¡
} // dispatcher()


private void _append(object pl)                // ÕõÔğ˜ŠßßÈ†­ÔÚÙ
{
    mapping me=pl->GetProperty("me");

    if (sizeof(me["sText"]) == 0) return;
    me["is_Command_Mode"] = 0;
    _refresh_status(pl);
    _right(pl, 1, 1);
} // _append()


private void _append_line(object pl)        // ÕõÔğ˜Š´ÚÎÃ­ÔÓ©ÏÃÏÏ
{
    string *text;
    mapping me=pl->GetProperty("me");

    me["is_Command_Mode"] = me["is_NewFile"] = 0;
    me["is_Modify"] = 1;
    text = me["sText"][0..me["iS_Row"]+me["iRow"]-2] + ({ "" }) +
        me["sText"][me["iS_Row"]+me["iRow"]-1..<1];
    me["sText"] = text;
    _refresh_screen(pl, me["iS_Row"], sizeof(me["sText"]), 1);
    me["iCol"] = 1;
    _down(pl);
} // _append_line()


private void _backspace(object pl)        // Ğ¾ş²Ôğ˜ŠÌ—´ıÚÙ
{
    mapping me=pl->GetProperty("me");

    if (me["iS_Col"] == me["iCol"])        return;
    _left(pl);
    _del_char(pl, 1);
} // _backspace()


protected void _confirm_save(string str, object pl) // Á‹À¬˜€­¡Ë•Ì—®¸®Î³†´´
{
    mapping me=pl->GetProperty("me");

    if (str=="y" || str=="Y") return _write_done("", pl, 1);
    me["is_Modify"] = 0;                // Ğˆµ¿í¹ğ³Ï¹ıí ËŒ˜Š
    _quit(0, pl);
} // _confirm_save()


private void _del_char(object pl, int bs)                // Ğ¾ş²Ôğ˜ŠĞÎ´ıÚÙ
{
    int    go_left=0;                // ×Ì¸±ÕõĞ¾ş²ßßÓ¬ÚÓËÔğ˜Š
    string str;
    mapping me=pl->GetProperty("me");


    if (sizeof(me["sText"])==0 ||
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])==0)
        return;
    if (me["is_NewFile"] || !me["is_Modify"])
    {
        me["is_NewFile"] = 0;
        me["is_Modify"] = 1;
        _refresh_status(pl);
    }
    if (me["iS_Col"]+me["iCol"]-2 >=
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2]))
        return;
    str = me["sText"][me["iS_Row"]+me["iRow"]-2][0..me["iS_Col"]+
        me["iCol"]-3];
    // ­Œ™‰×Ì¸±ÏˆÓ¬¹»­Ô×úÎÃ´ııõ®œ
    if (me["iS_Col"]+me["iCol"]-1 <
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2]))
        str += me["sText"][me["iS_Row"]+
            me["iRow"]-2][me["iS_Col"]+me["iCol"]-1..<1];
    else        go_left = 1;                // Óøğ¬×ÌĞ¾ş²ÏÏğ™´ıÚÙú¼İ·ÓÕÓ¬Ú 
    me["sText"][me["iS_Row"]+me["iRow"]-2] = str;
    printf(ESC "[%d;1f%-80s", me["iRow"]+1,        me["sText"][me["iS_Row"]+
            me["iRow"]-2][me["iS_Col"]-1..me["iE_Col"]-1]);
    if (go_left && !bs) _left(pl);
    else        _refresh_cursor(pl);
} // _del_char()


private void _del_line(object pl)        // Ğ¾ş²Ø“ÏÏ
{
    string *text=({ });
    mapping me=pl->GetProperty("me");
    me["is_NewFile"] = 0;
    me["is_Modify"] = 1;
    if (me["iS_Row"]+me["iRow"]-2 > 0)
        text = me["sText"][0..me["iS_Row"]+me["iRow"]-3];
    text += me["sText"][me["iS_Row"]+me["iRow"]-1..<1];
    me["sText"] = text;
    _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
    if (me["iS_Row"]+me["iRow"]-1 > sizeof(me["sText"]))
        _up(pl);                // ®Ÿ×ÌĞ¾ş²ÚßßÓ©ÏÏú¼¾ÍÓ¬ĞÎÓËÔğ˜Š
    // ®ŸÔğ˜Š´ıÏÃğ©ÙÈş¼ ²ÏÏğ™ú¼ÕøÓËµ¯Ôğ˜Š´»ÏÏğ™
    else if (me["iS_Col"]+me["iCol"]-1 >
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2]))
        _end(pl);
} // _del_line()


private void _down(object pl)                // Ôğ˜ŠÍ·ÎÃÓËÓ©ÏÏ
{
    int  acc;
    mapping me=pl->GetProperty("me");


    // ­Œ™‰×Ì¸±ÓÒÕõ´´ğ™
    if (me["iS_Row"]+me["iRow"]-1 >= sizeof(me["sText"]))
        // ®ŸÓÒÎÕ×¾´´ğ™ÎÃÓ©ÏÏú¼Õø™©Ú÷×Ã
        if (me["iRow"] < 23) return;
        else
        {        // µ€ÎÕ×Ô´´ğ™ÎÃÓ©ÏÏú¼ÓÕÍ©ÎÕ´´ğ™´ı³†Õõ
            me["iS_Row"]++;
            printf(ESC "[24;1f\r\n~");
            me["iRow"] = 22;
        }
    else if (++me["iRow"] > 23)
    {        // ©¡È†ĞÎ¾ú¼ÎÕ×¾ÎÃÓ©ÏÏ
        me["iS_Row"]++;                me["iE_Row"]++;
        printf(ESC "[24;1f\r\n%s", me["sText"][me["iE_Row"]-1]
                [me["iS_Col"]-1..me["iE_Col"]-1]);
        me["iRow"] = 23;
    }

    // ®ŸÔğ˜Š´ıÏÃğ©ÙÈş¼ ²ÏÏğ™ú¼ÕøÓËµ¯Ôğ˜Š´»ÏÏğ™
    if (me["iS_Col"]+me["iCol"]-1 >
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2]))
        me["iCol"] = sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-
            me["iS_Col"]+(me["is_Command_Mode"]?1:2);
    // ®Ÿ³İÏÏğÚÙú¼Õøµ¿ğ©Õõ´õÓ©Á¹
    if (me["iCol"]==0 && me["iS_Col"]==1) me["iCol"] = 1;
    if (me["iCol"] > 0)_refresh_status(pl);
    else
    {        // Ôğ˜Š´ıÏÃğ©ÙÈÕõÕ¡©¡È†Í‚ú¼ÏˆÓ¬ÙÛ©†©¡È†
        acc = me["iCol"];        me["iCol"] = 1;
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"],
                me["iS_Col"]+acc-1);
    }
} // _down()


private void _end(object pl)                // »½Ôğ˜ŠÓË´»ÏÏğ™
{
    mapping me=pl->GetProperty("me");

    if (sizeof(me["sText"]) == 0) return;
    // ­ËİƒÏÏğ™´ığ©ÙÈ
    me["iCol"] = sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-
        me["iS_Col"]+(me["is_Command_Mode"]?1:2);
    if (me["iCol"] == 0) me["iCol"] = 1; // ®Ÿ³İÏÏğÚÙú¼Õøµ¿ğ©Õõ´õÓ©Á¹
    if (sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2]) < me["iE_Col"])
        _refresh_status(pl);
    else
    {        // Ôğ˜Š´ıÏÃğ©ÙÈÕõÕ¡©¡È†Í‚ú¼ÏˆÓ¬ÙÛ©†©¡È†
        me["iCol"] = 79;
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]+
                (sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-
                 me["iE_Col"]+1));
    }
} // _end()


private void _esc(object pl)                // ³Ô×„®‹ıú×»©Û´»È’Âıú×»
{
    mapping me=pl->GetProperty("me");
    me["is_Command_Mode"] = 1;
    _left(pl);
} // _esc()


private void _file_end(object pl)        // »½Ôğ˜ŠÓË´»´´ğ™
{
    mapping me=pl->GetProperty("me");
    if (sizeof(me["sText"]) == 0) return;
    _goto_line_done(sprintf("%d", sizeof(me["sText"])), pl, 0);
} // _file_end()


private void _file_head(object pl)        // »½Ôğ˜ŠÓË´»´´Í¸
{
    mapping me=pl->GetProperty("me");
    if (sizeof(me["sText"]) == 0) return;
    _goto_line_done("1", pl, 0);
} // _file_head()


private void _goto_line(object pl)        // Ş°´»ŞÛµ¿ÏÏ®Ñ
{
    mapping me=pl->GetProperty("me");
    _message(pl, sprintf("Ì‹×„®‹ÏÏ¦Êú¿Ù˜»Ô—³ ENTER ®íÎ“ú¼1~%d)ú¦",
            sizeof(me["sText"])), "_goto_line_done", ECHO, 1);
} // _goto_line()


protected void _goto_line_done(string str, object pl, int rein) // ®í´ÈÏÏ¦Ê
{
    int        no;
    mapping me=pl->GetProperty("me");

    if (str == "") return _message_done("", pl);
    if (sscanf(str, "%d", no)!=1 || no<1 || no>sizeof(me["sText"]))
        return _message(pl, "³ğ´ıÏÏ¦Êúí", "_message_done", HIDE);

    me["iCol"] = 1;
    if (me["iS_Row"]<=no && no<=me["iE_Row"])
    {        // Ù¹µ¿ÏÏ¦ÊÕõÕ¡ÀĞ×Ô¸µğºÙÏ
        me["iRow"] = no-me["iS_Row"]+1;
        _refresh_status(pl);
    }
    else
    {        // Ù¹µ¿ÏÏ¦Ê™©ÕõÕ¡ÀĞ×Ô¸µğºÙÏú¼ÏˆÓ¬ÙÛ©†©¡È†
        me["iS_Row"] = no-11;
        if (me["iS_Row"] < 1)        me["iS_Row"] = 1;
        if (me["iE_Row"] < no+11)        me["iE_Row"] = no+11;
        me["iRow"] = no-me["iS_Row"]+1;
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"], 1);
    }
    if (rein) get_char("_input", HIDE, pl); // ­Œ™‰×Ì¸±ÏˆÓ¬µÂ®íÎÃ³ğ×„®‹
} // _goto_line_done()


private void _help(object pl)                // ¹¿Ù”©¡È†
{
    printf(REF +
            "í¾ LPMud ®½Ô¨ı©˜€­¡Ë• íÀ1.3 —† 3/25/1998  by Ï©ş‡Î‰(Kenny Hsieh), —†®¿İ·ÔÏ.\r
            \r
            a            ÕõÔğ˜ŠßßÈ†­ÔÚÙ               d  Ctrl-D    Ğ¾ş²Ø“ÏÏ\r
            f  Ctrl-Z    ¹’ÏÃ©¡È†                     g  Ctrl-G    Ş°´»ŞÛµ¿ÏÏ®Ñ\r
            h  Ctrl-J    Ôğ˜ŠÍ·ÚÓËÓ©ÚÙ               i            ÕõÔğ˜ŠÌ—È†™…ÚÙ\r
            j  Ctrl-K    Ôğ˜ŠÍ·ÎÃÓËÓ©ÏÏ               k  Ctrl-O    Ôğ˜ŠÍ·ĞÎÓËÓ©ÏÏ\r
            l  Ctrl-L    Ôğ˜ŠÍ·ÔÓÓËÓ©ÚÙ               m            µ¿Ó…ÏÃ ª\ı›­’\r
            n  Ctrl-N    İÒÒ—/Ş†©©ÎÃ¹÷¸“¦ÎÚÙ³«        o            ÕõÔğ˜Š´ÚÎÃ­ÔÓ©ÏÃÏÏ\r
            q  Ctrl-Q    Á‹À¬˜€­¡Ë•                   r  Ctrl-R    ÚÙ³«Ş†©©\r
            s  Ctrl-S    ÚÙ³«İÒÒ—                     u            ®íÎ“ış ª\ı›­’µ¿Ó…\r
            w  Ctrl-W    ³ó³†´´—¹                     x  Ctrl-X    Ğ¾ş²Ôğ˜ŠĞÎ´ıÚÙ\r
            A  Ctrl-A    »½Ôğ˜ŠÓË´»ÏÏ×Ú               B  Ctrl-B    Í·ĞÎ¾Ó©Óşú¿22 ÏÏú¨\r
            E  Ctrl-E    »½Ôğ˜ŠÓË´»ÏÏğ™               F  Ctrl-F    Í·ÎÃ¾Ó©Óşú¿22 ÏÏú¨\r
            J  Ctrl-P    »½ÎÃÏÏğıÚÙ»Ô´»³İÏÏßßÈ†       O            ÕõÔğ˜ŠĞÎ¸»­ÔÓ©ÏÃÏÏ\r
            U  Ctrl-U    »½Ôğ˜ŠÓË´»´´Í¸               V  Ctrl-V    »½Ôğ˜ŠÓË´»´´ğ™\r\n");
    printf(
            ".            ÙÛ¹™ĞÎ¹÷Ù¹Â                 ?  Ctrl-Y    ¹¿Ù”©¡È†ú¿³İ©¡È†ú¨\r
            ~  Ctrl-T    ÕõÔğ˜ŠÌ—È†™…®‹Ó©¹÷í¹!í ÚÙÕ¬\r\n
            ¦Î¸¿´ıµ¿Ó…­’×ÌÀĞÔíÚÙı¹¦Í ESCíóF1í½F12íóBACKSPACEíóTABíóENTERíóINSERTíóDELETEíó\r
            HOMEíóENDíóPAGEUPíóPAGEDOWNíóUPíóDOWNíóLEFTíóRIGHTíóCtrl-Aí½Ctrl-Zíú\r
            \r
            ©µÔ¡³½™Ñ³İ˜€­¡Ë•ú¼´½Ì‹˜úÂŸ—†®¿Ï“¹†ú¼ÔÏ®ğ¦ğğ×Ş‚¦Í»¿Ó‰Ì‹Â¬Ã‡ğÓú¦\r
            kenny@cindy.cis.nctu.edu.twíú\r
            [—³®ğÓ‚­’©Û´»˜€­¡Ë•]"
          );
    get_char("_input", HIDE, pl, REFRESH); // µÂ®í—³­’ÓÕÙÛ©†©¡È†
} // _help()


private void _home(object pl)                // »½Ôğ˜ŠÓË´»ÏÏ×Ú
{
    mapping me=pl->GetProperty("me");

    me["iCol"] = 1;
    if (me["iS_Col"] == 1)_refresh_status(pl);
    else        _refresh_screen(pl, me["iS_Row"], me["iE_Row"], 1);
} // _home()


private void _insert(object pl)                // ÕõÔğ˜ŠÌ—È†™…ÚÙ
{
    mapping me=pl->GetProperty("me");

    if (sizeof(me["sText"]) == 0) return;
    me["is_Command_Mode"] = 0;
    _refresh_status(pl);
} // _insert()


private void _insert_line(object pl)        // ÕõÔğ˜ŠĞÎ¸»­ÔÓ©ÏÃÏÏ
{
    string *text=({ });
    mapping me=pl->GetProperty("me");

    me["is_Command_Mode"] = me["is_NewFile"] = 0;
    me["is_Modify"] = 1;
    // ­Œ™‰×Ì¸±Ôğ˜ŠĞÎ¸»×Ì¸±ÔÏıõ®œ
    if (me["iS_Row"]+me["iRow"]-3 >= 0)
        text = me["sText"][0..me["iS_Row"]+me["iRow"]-3];
    text += ({ "" });
    text += me["sText"][me["iS_Row"]+me["iRow"]-2..<1];
    me["sText"] = text;
    me["iCol"] = 1;
    _refresh_screen(pl, me["iS_Row"], sizeof(me["sText"]), 1);
} // _insert_line()


private void _insert_exclam_mark(object pl) // ÕõÔğ˜ŠÌ—È†™…®‹Ó©¹÷í¹!í ÚÙÕ¬
{
    mapping me=pl->GetProperty("me");

    // Óøğ¬ğ¸¿Ø²şú×„®‹í¹!í ú¼İ·ÓÕŞÛ´Û­Ô³İ¦¯×»ÓÕ×„®‹í¹!í íú
    if (sizeof(me["sText"]) == 0) return;
    me["sChar"] = "!";
    _process(pl);
} // _insert_!()


private void _join(object pl)                // »½ÎÃÏÏğıÚÙÓË´»³İÏÏßßÈ†
{
    string *text=({ });
    mapping me=pl->GetProperty("me");


    if (me["iS_Row"]+me["iRow"]-1 >= sizeof(me["sText"])) return;

    me["is_NewFile"] = 0;
    me["is_Modify"] = 1;
    _end(pl);                        // Î®»½Ôğ˜ŠÓË´»ÏÏğ™
    text = me["sText"][0..me["iS_Row"]+me["iRow"]-2];
    if (sizeof(me["sText"][me["iS_Row"]+me["iRow"]-1]))
    {        // ®ŸÎÃÏÏÔÏıõ®œ¹»­ÔĞÎÁ³
        text[me["iS_Row"]+me["iRow"]-2] += (" "+
                me["sText"][me["iS_Row"]+me["iRow"]-1]);
        _right(pl, 1, 1);
    }
    text += me["sText"][me["iS_Row"]+me["iRow"]..<1];
    me["sText"] = text;
    _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
} // _join()


private void _keymap(object pl)                // µ¿Ó…ÏÃ ªı›­’
{
    _message(pl, "Ì‹×„®‹ÏÃµ¿Ó…­’¦ÍÕ¡ ª\ı›­’ú¿Ù˜»Ô—³ ENTER ®íÎ“ú¼ÓÕÀØ—Ú¹ŸÀ¬ú¨",
            "_keymap_done", ECHO);
} // _keymap()


protected void _keymap_done(string str, object pl) // ®í´Èµ¿Ó… ªı›­’
{
    string old, _new;
    mapping me=pl->GetProperty("me");

    _refresh_status(pl);
    if (str != "")
    {        // ®ŸÔÏÚ×ÂÎ×„®‹
        if (sscanf(str, "%s %s", _new, old) != 2)
            return _message(pl, "×„®‹¹±×»³ğíú", "_message_done",
                    HIDE);
        me["keymap"][_new] = old;
        _message(pl, sprintf("¦ÈÂİú¼³Ô³İÓÕßßÏÃµ¿Ó…­’í¹%sí ¾ÍÔÏÕ¡ ª\ı›­’?
                    ?sí ´ı ª\ı›Âİúí",
                    _new, old), "_message_done", HIDE);
    }
    else        get_char("_input", HIDE, pl); // ®íÎ“×„®‹ú¼­ŞÏ°µÂ®íÎÃ¹÷×„®‹
} // _keymap_done()


private void _left(object pl)                // Ôğ˜ŠÍ·ÚÓËÓ©ÚÙ
{
    mapping me=pl->GetProperty("me");

    if (me["iS_Col"]==1 && me["iCol"]==1) return _refresh_status(pl);
    if (--me["iCol"] < 1)
    {        // Ôğ˜Š´ıÏÃğ©ÙÈÕõÕ¡©¡È†Í‚ú¼ÏˆÓ¬ÙÛ©†©¡È†
        // ®ŸÚÓË 8 Á¹Ş½µ€´ı©—ú¼¾ÍÙ©ÚÓËÓ©Á¹
        if (me["iS_Col"]-8 < 1)
            me["iCol"] = me["iS_Col"]-1;
        else        me["iCol"] = 8; // ™©®©¾ÍĞˆµ¿Ôğ˜ŠÕõ´õ 8 Á¹ğ©ÙÈ
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"],
                me["iS_Col"]-me["iCol"]);
        return;
    }
    _refresh_status(pl);
} // _left()


// ÕõÚ³Ş¼ÂÏÎÕ×¾ÒµÎó
private varargs void _message(object pl, string str, string callback, int hide,
        mixed arg)
{
    printf(HOME + NOR + REV);        // ÓËµ¯Ôğ˜Š´» (1,1)ú¼Ğˆµ¿¸³Î€ÚÙÕ¬ÎÕ×¾
    // Ì…ş²Õ¡Ú³Ş¼ÂÏ´ıÒµÎó
    printf("
            ");
    printf(HOME + str);                // ÓËµ¯Ôğ˜Š´» (1,1)ú¼ÕãÎÕ×¾ÒµÎó
    input_to(callback, hide, pl, arg); // µÂ®í×„®‹Ú×ÂÎ
} // _message()


protected void _message_done(string str, object pl) // »ß×°ÒµÎó´ıÎÕ×¾
{
    _refresh_status(pl);
    get_char("_input", HIDE, pl);        // ­ŞÏ°µÂ®í×„®‹ÚÙ³«
} // _message_done()


private void _next_match(object pl)                // İÒÒ—/Ş†©©ÎÃ¹÷¸“¦ÎÚÙ³«
{
    mapping me=pl->GetProperty("me");
    string str="";
    // Ğˆµ¿Ë×­İÒÒ—ğ©ÙÈğ¬Ôğ˜ŠÎÙÕõğ©ÙÈ
    int    j=me["iS_Col"]+me["iCol"]-1,
           i, ofs;


    if (!stringp(me["sSearch"])) return; // ®ŸÈ©Î®Ğˆµ¿İÒÒ—ÚÙ³«ú¼™©Ú÷×Ã

    for (i=me["iS_Row"]+me["iRow"]-2; i<sizeof(me["sText"]); i++)
    {
        if ((ofs = strsrch(me["sText"][i][j-1..<1], me["sSearch"]))== -1)
            j = 1; // Ø‚ÏÏÈ©ØÓ´»´ı©—ú¼Ğˆµ¿ÎÃÏÏ³Ô´õÓ©Á¹À¬×­ØÓ
        else
        {        // ØÓ´»Ù¹µ¿ÚÙ³«Âİ, ÓËµ¯Ôğ˜Š´»ıÀ´ıÏÏ
            _goto_line_done(sprintf("%d", i+1), pl, 0);
            me["iCol"] = j+ofs; // ­ËİƒÙ¹µ¿ÚÙ³«´ı¾°µÕğ©ÙÈ
            // ®ŸÙ¹µ¿ÚÙ³«Ã„ÕõÕ¡©¡È†Í‚ú¼ÕøÙÛ©†©¡È†
            if (me["iCol"]<me["iS_Col"] ||
                    me["iCol"]+strlen(me["sSearch"])>me["iS_Col"]+79)
                _refresh_screen(pl, me["iS_Row"], me["iE_Row"],
                        me["iCol"]);
            if (stringp(me["sReplace"]))
            {        // ÏˆÓ¬Ş†©©ÚÙ³«
                me["is_NewFile"] = 0;
                me["is_Modify"] = 1;
                // ¹»­ÔÔğ˜ŠÌ—ıõ®œ
                str = me["sText"][i][0..me["iCol"]-2];
                str += me["sReplace"];
                str += (me["sText"][i][me["iCol"]+
                        strlen(me["sSearch"])-1..<1]);
                me["sText"][i] = str;
                // ®ŸŞ†©©ÚÙ³«Ã„ÕõÕ¡©¡È†Í‚ú¼ÕøÙÛ©†©¡È†
                if (me["iCol"]<me["iS_Col"] ||
                        me["iCol"]+strlen(me["sReplace"])
                        > me["iS_Col"]+79)
                    _refresh_screen(pl, me["iS_Row"],
                            me["iE_Row"], me["iCol"]);
                // »½ÔğË¯ÓË´»Ş†©©ÚÙ³«ßßÈ†
                me["iCol"] = me["iCol"]+strlen(me["sReplace"])-
                    me["iS_Col"]+1;
                printf(ESC "[%d;1f%-80s", me["iRow"]+1,
                        me["sText"][me["iS_Row"]+
                        me["iRow"]-2][me["iS_Col"]-1
                        ..me["iE_Col"]-1]);
            }
            // »½ÔğË¯ÓË´»İÒÒ—ÚÙ³«ßßÈ†
            else        
                me["iCol"] = me["iCol"]+strlen(me["sSearch"])-me["iS_Col"]+1;
            _refresh_status(pl);
            break;
        } // if found
    } // for each row
    if (i >= sizeof(me["sText"]))
        _message(pl, "ØÓ™©´»Ù¹µ¿ÚÙ³«úí", "_message_done", HIDE);
    else        get_char("_input", HIDE, pl); // µÂ®íÎÃ¹÷×„®‹ÚÙ³«
} // _next_match()


private void _page_down(object pl)        // Í·ÎÃ¾Ó©Óşú¿22 ÏÏú¨
{
    mapping me=pl->GetProperty("me");

    if (me["iS_Row"]+22 > sizeof(me["sText"])) return; // ­Œ™‰×Ì¸±™©ı›Õã¾Âİ
    me["iS_Row"] += 22;        me["iE_Row"] += 22;
    // ­Œ™‰Ôğ˜Š´ıÏÃğ©ÙÈ×Ì¸±ş¼ ²´´ğ™
    if (me["iS_Row"]+me["iRow"]-1 > sizeof(me["sText"]))
        me["iRow"] = sizeof(me["sText"])-me["iS_Row"]+1;
    _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
    // ®ŸÔğ˜Š´ıÏÃğ©ÙÈş¼ ²ÏÏğ™ú¼ÕøÓËµ¯Ôğ˜Š´»ÏÏğ™
    if (me["iS_Col"]+me["iCol"]-1 > sizeof(me["sText"][me["iS_Row"]+
                me["iRow"]-2]))
        _end(pl);
} // _page_down()


private void _page_up(object pl)        // Í·ĞÎ¾Ó©Óşú¿22 ÏÏú¨
{
    mapping me=pl->GetProperty("me");

    if (me["iS_Row"] <= 1) return; // ­Œ™‰×Ì¸±™©ı›Õã¾Âİ
    me["iS_Row"] -= 22;
    if (me["iS_Row"] < 1) me["iS_Row"] = 1; // Úµ€Ù©ı›¸¡´»´õÓ©ÂÏ
    _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
    // ®ŸÔğ˜Š´ıÏÃğ©ÙÈş¼ ²ÏÏğ™ú¼ÕøÓËµ¯Ôğ˜Š´»ÏÏğ™
    if (me["iS_Col"]+me["iCol"]-1 >
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2]))
        _end(pl);
} // _page_up()


private void _process(object pl)        // ³ªÁ×„®‹ıú×»´ıÚÙ³«×„®‹
{
    int    i, size;
    string str, rest, *input;
    mapping me=pl->GetProperty("me");


    if (me["is_Modify"]==0 || me["is_NewFile"]==1)
    {
        me["is_NewFile"] = 0;
        me["is_Modify"] = 1;
        _refresh_status(pl);
    }
    if (!me["is_Substituted"] && (strsrch(me["sChar"], "\r")!=-1 ||
                strsrch(me["sChar"], "\n")!=-1))
        me["is_Substituted"] = 1; // Ğˆµ¿ıõÃ‹³”©©ËŒ˜Š
    me["sChar"] = replace_string(me["sChar"], "\t", "        ");
    me["sChar"] = replace_string(me["sChar"], "\r", "\rENTER\r");
    me["sChar"] = replace_string(me["sChar"], "\n", "\rENTER\r");
    input = explode(me["sChar"], "\r");

    size = sizeof(input);
    for (i=0; i<size; i++)                // Ó©³ğ³ªÁÓ©ÏÏ×„®‹
    {
        if (me["is_Substituted"] && input[i] == "ENTER")
        {        // ´˜­’®‹ ENTER ×˜ú¼Ó¬»½Ôğ˜ŠÙ«ßß´ııõ®œÓË´»ÎÃÈ†´ıÏÃÏÏ
            if(!me || !me["sText"] || !me["iS_Row"]){
                continue;
            }
            rest = me["sText"][me["iS_Row"]+
                me["iRow"]-2][me["iS_Col"]+me["iCol"]-2..<1];
            me["sText"][me["iS_Row"]+me["iRow"]-2] =
                me["sText"][me["iS_Row"]+
                me["iRow"]-2][0..me["iS_Col"]+me["iCol"]-3];
            _append_line(pl); // ÕõÔğ˜Š´ÚÎÃ­ÔÓ©ÏÃÏÏ
            // ÏÃÏÏ´ııõ®œ×ÌÕ¡Ôğ˜Šğ©ÙÈÓÕßß´ııõ®œ
            me["sText"][me["iS_Row"]+me["iRow"]-2] = rest;
            continue;
        }

        // ×„®‹ÚÙ³«™©×Ì ENTER ×˜, ³ªÁ TAB ´ıµ¿ğ©
        if (me["is_Substituted"] && input[i] == "TAB")
            input[i] = "        "[0..7-(me["iCol"]-1)%8];
        str = me["sText"][me["iS_Row"]+me["iRow"]-2][0..me["iS_Col"]+
            me["iCol"]-3] + input[i] + me["sText"][me["iS_Row"] +
            me["iRow"]-2][me["iS_Col"]+ me["iCol"]-2..<1];
        me["sText"][me["iS_Row"]+me["iRow"]-2] = str;
        _right(pl, sizeof(input[i]), 1);
    } // for

    printf(ESC "[%d;1f%s", me["iRow"]+1, me["sText"][me["iS_Row"]+
            me["iRow"]-2][me["iS_Col"]-1..me["iE_Col"]-1]);
    _refresh_cursor(pl);
} // _process()


protected void _quit(mixed unused, mixed pl) // Á‹À¬˜€­¡Ë•
{
    mapping me;

    seteuid(geteuid(pl));
    me = pl->GetProperty("me");
    if (me["is_Modify"])
        return _message(pl, "³İ´´—¹ÓÒ¹’¹ıú¼Ì‹ğ×ÏˆÓ¬³†´´Ãú¿y/n)úÀ[n]",
               "_confirm_save", ECHO);
    me["iRow"] = 23;
    _refresh_cursor(pl);                // ÓËµ¯Ôğ˜Š´»ÚßßÓ©ÏÏ
    printf(NOR "\r\n");
    // ®Ÿ ªı›­’µ¿Ó…µÕØØ˜×ÌÀØ´ıú¼¾ÍÌ…ş²Í†­ÓĞĞÎ´ı­ÌÃ­ú¼™©®©¾Í­ÌÃ­ÕõÍ†­ÓĞĞÎ
    if (sizeof(me["keymap"]) == 0) pl->delete("me_keymap");
    else pl->set("me_keymap", me["keymap"]);
    map_delete(EditedFiles, pl->GetProperty("me/sFileName"));
    pl->delete_temp("me"); // Ì…ş²˜€­¡Ë•ÔÈ´»´ıÕœ×˜˜„×²
} // _quit()


private void _refresh(object pl)        // ¹’ÏÃ©¡È†
{
    mapping me=pl->GetProperty("me");
    _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
} // _refresh()


private void _replace(object pl)        // ÚÙ³«Ş†©©
{
    _message(pl, "İÒÒ—/Ş†©©ÚÙ³«(Ù˜»Ô—³ENTER®íÎ“,ÓÕÀØ—Ú¹ŸÀ¬):",
            "_replace_done", ECHO);
} // _replace()

protected void _replace_done(string str, object pl) // ®í´ÈŞ†©©ÚÙ³«
{
    mapping me=pl->GetProperty("me");

    _refresh_status(pl);
    if (str != "")
    {        // ÔÏ×„®‹Ú×ÂÎ×˜
        if (sscanf(str, "%s %s", me["sSearch"],        me["sReplace"]) != 2)
        {        // Ù©ÔÏÓ©¹÷ÚÙ³«ú¼¾ÍÙ¹µ¿ğ¬İÒÒ—ÚÙ³«ú¼Ó‚­³ıÈ´ŸÙ¹µ¿ÚÙ³«
            me["sSearch"] = str;
            me["sReplace"] = "";
        }
        _next_match(pl);
    }
    else        get_char("_input", HIDE, pl); // ­ŞÏ°µÂ®íÎÃ¹÷×„®‹ÚÙ³«
} // _replace_done()


private void _right(object pl, int col, int extra)        // Ôğ˜ŠÍ·ÔÓÓËÓ©ÚÙ
{
    int    acc;
    mapping me=pl->GetProperty("me");


    if (sizeof(me["sText"])==0 || me["iS_Col"]+me["iCol"]-1>
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-1+extra)
        return;
    me["iCol"] += col;
    if (me["iS_Col"]+me["iCol"]-1 > sizeof(me["sText"][me["iS_Row"]+
                me["iRow"]-2])+extra)
        me["iCol"] = sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-
            me["iS_Col"]+1+extra;
    if (me["iCol"] > 80)
    {        // Ôğ˜Š´ıÏÃğ©ÙÈ™©ÕõÀĞ­“×Ô³—ıõú¼ÏˆÓ¬Í·ÔÓÓËµ¯×Ô³—
        // ®ŸÔÓÓË 8 Á¹Ş½µ€´ı©—ú¼¾ÍÙ©ÔÓÓË´»ÏÏğ™
        if (me["iE_Col"]+8 > sizeof(me["sText"][me["iS_Row"]+
                    me["iRow"]-2])+1)
            acc = sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-
                me["iE_Col"]+1;
        else        acc = 8;
        me["iCol"] = 80-acc+1;         // ­ËİƒÔğ˜Š´ıÏÃğ©ÙÈ
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]+acc);
        return;
    }
    _refresh_status(pl);
} // _right()


private void _search(object pl)                // ÚÙ³«İÒÒ—
{
    _message(pl, "Ì‹×„®‹İÒÒ—ÚÙ³«ú¿Ù˜»Ô—³ ENTER ®íÎ“)ú¦", "_search_done",
        ECHO);
} // _search()


protected void _search_done(string str, object pl) // ®í´ÈİÒÒ—ÚÙ³«
{
    mapping me=pl->GetProperty("me");

    _refresh_status(pl);
    if (str != "")
    {        // ÔÏ×„®‹Ú×ÂÎ×˜
        me["sSearch"] = str;        me["sReplace"] = 0;
        _next_match(pl);
    }
    else        get_char("_input", HIDE, pl); // ­ŞÏ°µÂ®íÎÃ¹÷×„®‹ÚÙ³«
} // _search_done()


private void _tab(object pl)                 // È’Âıú×»ÎÃ TAB ­’´ıµ¿ğ©
{
    mapping me=pl->GetProperty("me");
    _right(pl, 8-(me["iCol"]-1)%8, 0);
} // _tab()


private void _undef_key(object pl)        // ®íÎ“ış ªı›­’µ¿Ó…
{
    _message(pl, "Ì‹×„®‹Ô“®íÎ“µ¿Ó…Ù« ª\ı›­’ú¿Ù˜»Ô—³ ENTER ®íÎ“)ú¦",
        "_undef_key_done", ECHO);
} // undef_key()


protected void _undef_key_done(string str, object pl) // ®í´ÈÙ¹µ¿ ªı›­’
{
    mapping me=pl->GetProperty("me");
    if (str != "")        map_delete(me["keymap"], str);
    _message_done("", pl);
} // undef_key_done()


private void _up(object pl)                // Ôğ˜ŠÍ·ĞÎÓËÓ©ÏÏ
{
    int  acc;
    mapping me=pl->GetProperty("me");


    if (me["iRow"]==1 && me["iS_Row"]==1)        return;
    if (--me["iRow"] < 1)
    {        // Ôğ˜ŠÏÃğ©ÙÈ™©ÕõÀĞ­“×Ô³—ıõú¼¾ÍĞÎÓË×Ô³—
        me["iS_Row"]--;
        me["iE_Row"]=me["iS_Row"]+22;
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
        me["iRow"] = 1;
    }
    // ®ŸÔğ˜ŠÏÃğ©ÙÈş¼ ²ÏÏğ™ú¼ÕøÓË´»ÏÏğ™
    if (me["iS_Col"]+me["iCol"]-1 > sizeof(me["sText"][me["iS_Row"]+
                me["iRow"]-2]))
        me["iCol"] = sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-
            me["iS_Col"]+(me["is_Command_Mode"]?1:2);
    // ®Ÿ³İÏÏğÚÙú¼Õøµ¿ğ©Õõ´õÓ©Á¹
    if (me["iCol"]==0 && me["iS_Col"]==1) me["iCol"] = 1;
    if (me["iCol"] > 0) _refresh_status(pl);
    else
    {        // Ôğ˜ŠÏÃğ©ÙÈ™©ÕõÀĞ­“×Ô³—ıõú¼ÙÛ©†©¡È†
        acc = me["iCol"];        me["iCol"] = 1;
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"],
                me["iS_Col"]+acc-1);
    }
} // _up()


private void _write(object pl)                // ®í´È³ó³†´´—¹È“şË
{
    _message(pl, "Ì‹×„®‹ÏÃ´´È“ú¿Ù˜»Ô—³ ENTER ÓÕ× ÔÈ¾Ğ´´È“)ú¦",
        "_write_done", ECHO, 0);
} // _write()


protected void _write_done(string str, object pl, int quit) // ³ó³†´´—¹
{
    string callback;
    mapping me=pl->GetProperty("me");

    if (str == "") str = me["sFileName"];
    else        // ®í´È´´È“
        str = absolute_path(pl->query("cwd"), str);
    if (quit) callback = "_quit";
    else callback = "_message_done";
    if (write_file(str, implode(me["sText"], "\n")+"\n", 1))
    {
        if (me["is_Modify"])
            log_file(LOGFILE, sprintf("[%s] %s %s %s\n",
                        ctime(time()), getuid(pl),
                        me["is_NewFile"]? "+": "*", str));
        me["is_Modify"] = me["is_NewFile"] = 0;
        me["sFileName"] = str;
        _message(pl, "³†´´şĞ ª\íú", callback, HIDE);
    }
    else
        _message(pl, "³†´´×º—›íú", callback, HIDE);
} // write_done()
