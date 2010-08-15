/* Character Input/Output handler.
 * Character mode is meant to allow the user to issue special
 * requests to the mud using escape sequences. However, it is
 * not always as simple as "escape character X". 
 *  
 * This inherited object manages a get_char() loop that accumulates
 * input into CharStuff["charbuffer"], and watches for sets of
 * substrings that match known escape sequences, such as "up arrow"
 * and the like, which are not just one character. 
 * 
 * When an ESC is received (it is assumed that the driver translates
 * 0x1b to 0x1e with the ANSI_SUBSTITUTE define, making '30' the
 * escape char) then the subsequent input is not put into 
 * CharStuff["charbuffer"] but rather into CharStuff["escape"]
 * until a recognized escape sequence is completed in
 * CharStuff["escape"] or until some arbitrary number of characters
 * being input indicates that an unknown escape sequence has
 * been received. At that point the escape sequence is either
 * processed or discarded, and accumulation into CharStuff["charbuffer"]
 * resumes. 
 * -Crat 08FEB2009
 *
 *  Wow what's that even mean?
 * -Crat 22SEP2009
 */

#include <daemons.h>
#include <lib.h>
#include "include/nmsh.h"

#define CHAR_DEBUG 0

private static mapping CharStuff;

int rEsc();

void create(){
    CharStuff = ([
            "charmode" : 1, /* are we in character mode */
            "noecho" : 1, /* do we echo input characters */
            "charbuffer" : "", /* what we've typed of this line so far */
            "tempbuffer" : "", /* the last line, before alias expansion */
            "charshell" : 0, /* hell if i remember */
            ]);
}

static int ReceiveChars(string str){
    string s;
    int c;

    if(!this_object()) return 0;

    if(str){
        c = str[0];
        s = str[0..0];
    }

    if(!CharStuff["charbuffer"]) CharStuff["charbuffer"] = "";

#if CHAR_DEBUG
    if(sizeof(str) > 1) debug("unexpected string: "+str);
    else debug("CHARIO received: "+s+", aka: "+c, "black");
#endif

    if(!sizeof(CharStuff["escape"]) && (c > 31 && c < 127)){ 
        /* Printable character, and we're not waiting on an ESC seq,
         * Let's add it to the character buffer, and then
         * "normalize" any Pinkfish sequences.
         */ 
        CharStuff["charbuffer"] += s;
        if(sizeof(CharStuff["charbuffer"]) > 1 && last(CharStuff["charbuffer"],2) == "%^"){
            /* replace pinkfish to avoid weirdness */
            CharStuff["charbuffer"] = truncate(CharStuff["charbuffer"],2);
            CharStuff["charbuffer"] += "%%^^";
        }
        this_object()->rAscii(s);
        get_char("ReceiveChars", CharStuff["noecho"]);
        return 1; 
    }

    if(sizeof(CharStuff["escape"]) && 
            member_array(30, CharStuff["escape"]) != -1){
        int cl;
        string esc;
        CharStuff["escape"] += s;
        esc = CharStuff["escape"];
        if(last(esc,1) == "H" || last(esc,1) == "R" || sizeof(esc) > 10){
            cl = 1;
            this_object()->rAnsi(esc);
        }
        else if(sizeof(esc) > 2 && esc[1] == 91){
            switch(esc[2]){
                case 65 : this_object()->rArrow("up"); cl = 1; break;
                case 66 : this_object()->rArrow("down"); cl = 1; break;
                case 68 : this_object()->rArrow("left"); cl = 1; break;
                case 67 : this_object()->rArrow("right"); cl = 1; break;
                case 51 : 
                          if(sizeof(esc) > 3 && esc[3] == 126){
                              this_object()->rDel();
                              cl = 1;
                          }
                          break;
            }
        }
        if(cl) CharStuff["escape"] = "";
        get_char("ReceiveChars", CharStuff["noecho"]);
        return 1;
    }

    switch(c){
        case 0 : case  8 : case 127 : this_object()->rBackspace(); break;
        case 1 : this_object()->rCtrl("a"); break;
        case 2 : this_object()->rCtrl("b"); break;
        case 3 : this_object()->rCtrl("c"); break;
        case 4 : this_object()->rCtrl("d"); break;
        case 5 : this_object()->rCtrl("e"); break;
        case 6 : this_object()->rCtrl("f"); break;
        case 7 : this_object()->rCtrl("g"); break;
        case 9 : this_object()->rCtrl("i"); break; /* aka Tab */
        case 10 : case 13: this_object()->rEnter(); break;
        case 11 : this_object()->rCtrl("k"); break;
        case 12 : this_object()->rCtrl("l"); break;
        case 14 : this_object()->rCtrl("n"); break;
        case 15 : this_object()->rCtrl("o"); break;
        case 16 : this_object()->rCtrl("p"); break;
        case 17 : this_object()->rCtrl("q"); break;
        case 18 : this_object()->rCtrl("r"); break;
        case 19 : this_object()->rCtrl("s"); break;
        case 20 : this_object()->rCtrl("t"); break;
        case 21 : this_object()->rCtrl("u"); break;
        case 22 : this_object()->rCtrl("v"); break;
        case 23 : this_object()->rCtrl("w"); break;
        case 24 : this_object()->rCtrl("x"); break;
        case 25 : this_object()->rCtrl("y"); break;
        case 26 : this_object()->rCtrl("z"); break;
        case 28 : this_object()->rCtrl("28"); break;
        case 30 : rEsc(); break;
        case 31 : this_object()->rCtrl("31"); break;
    }

    if(!this_object()){
        // Probably a warmboot or userload
        return 0;
    }
    if(in_edit(this_object())) CharStuff["noecho"] = 0;
    else CharStuff["noecho"] = 1;

#ifdef __GET_CHAR_IS_BUFFERED__
    if(CharStuff["charmode"]) get_char("ReceiveChars", CharStuff["noecho"]);
#endif
    this_object()->RedrawPrompt();
    return CharStuff["charmode"];
}

varargs int CancelCharmode(int extra){
    int ret = 1;
#if CHAR_DEBUG
    debug("cancelling! stack: "+get_stack());
#endif
    if(!CharStuff) CharStuff = ([]);
    flush_messages();
    CharStuff["charbuffer"] = "";
    if(!CharStuff["charmode"] && !query_charmode(this_object())) return 0;
    CharStuff["charmode"] = 0;
#ifdef __DSLIB__
    remove_get_char(this_object());
    remove_charmode(this_object());
#else
    ReceiveChars(sprintf("%c",13));
#endif
    if(extra) ReceiveChars(sprintf("%c",13));
    this_object()->SetProperty("was_charmode", 0);
    this_object()->SetProperty("reprompt", 0);
    return ret;
}

int SetCharmode(int x){
    if(!x) CharStuff["charmode"] = 0;
#ifdef __GET_CHAR_IS_BUFFERED__
    else CharStuff["charmode"] = 1;
    remove_get_char(this_object());
    if(!(this_object()->GetCedmode())){
        get_char("ReceiveChars", CharStuff["noecho"]);
    }
    else get_char("ReceiveChar", CharStuff["noecho"]);
#endif
    return CharStuff["charmode"];
}

int GetCharmode(){
    if(!CharStuff) CharStuff = ([]);
    return CharStuff["charmode"];
}

static string GetCharbuffer(){
    if(!CharStuff) CharStuff = ([]);
    return (CharStuff["charbuffer"] || "" );
}

static string SetCharbuffer(string str){
    if(!CharStuff) CharStuff = ([]);
    return CharStuff["charbuffer"] = ( str || "" );
}

static string GetTempbuffer(){
    if(!CharStuff) CharStuff = ([]);
    return (CharStuff["tempbuffer"] || "" );
}

static string SetTempbuffer(string str){
    if(!CharStuff) CharStuff = ([]);
    return CharStuff["tempbuffer"] = ( str || "" );
}

void CheckCharmode(){
    if(!in_edit() && !in_input() && this_object()->GetProperty("was_charmode")){
        CharStuff["charmode"] = 1;
    }
    if(CharStuff["charmode"] && !query_charmode(this_object())){
        if(!in_edit() && !in_input()) SetCharmode(CharStuff["charmode"]);
        if(!this_player() || this_player() != this_object()){
            SetCharmode(1);
            this_object()->RedrawPrompt();
        }
    }
    if(!CharStuff["charmode"] && query_charmode(this_object()) > 0){
        CancelCharmode();
    }
}

void heart_beat(){
    CheckCharmode();
}

int rEsc(){
    CharStuff["escape"] = sprintf("%c", 30);
    if(in_edit(this_object())) CharStuff["noecho"] = 0;
    else CharStuff["noecho"] = 1;
    return 1;
}

int SetNoEcho(int x){
    if(x) CharStuff["noecho"] = 1;
    else CharStuff["noecho"] = 0;
    return CharStuff["noecho"];
}

int GetNoEcho(){
    return CharStuff["noecho"];
}
