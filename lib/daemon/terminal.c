/*    /daemon/terminal.c
 *    from the Dead Souls Object Library
 *    daemon storing terminal information
 *    created by Pinkfish@Discworld
 *    rewritten for Dead Souls by Descartes of Borg 930903
 *    Version: @(#) terminal.c 1.5@(#)
 *    Last modified: 96/11/11
 */

#include <lib.h>

inherit LIB_DAEMON;

#define ANSI(p) sprintf("%c["+(p)+"m", 27)
#define ESC(p) sprintf("%c"+(p), 27)

static mapping term_info;

void create() {
    mapping nohtml;
    
    daemon::create();
    SetNoClean(1);
    nohtml = ([ "BR": "", "P" : "", "/P" : "", "HREF" : "", ">" : "",
	      "NAME" : "", "/A" : "", "I" : "", "/I" : "", "PRE" : "",
	      "/PRE" : "", "STRONG" : "", "/STRONG" : "", "TABLE" : "",
	      "/TABLE" : "", "TR" : "", "/TR" : "", "TD" : "", "/TD" : ""]);
    term_info = ([ ]);
    term_info["unknown"] =
	([ "RESET": "", "BOLD": "", "FLASH":"", "BLACK":"", "RED":"",
	 "BLUE":"", "CYAN":"", "MAGENTA":"", "ORANGE":"", "YELLOW":"",
	 "GREEN":"", "WHITE":"", "BLACK":"", "B_RED":"", "B_ORANGE":"",
	 "B_YELLOW":"", "B_BLACK":"", "B_CYAN":"","B_WHITE":"", "B_GREEN":"",
	 "B_MAGENTA":"", "STATUS":"", "WINDOW":"", "INITTERM": "",
	 "ENDTERM":""]) + nohtml;
    term_info["ansi"] =
	([ "RESET":ANSI("0"), "BOLD":ANSI(1), "FLASH":ANSI(5),
	 "BLACK":ANSI(30), "RED":ANSI(31), "GREEN":ANSI(32),
	 "ORANGE":ANSI(33),     "YELLOW":ANSI(1)+ANSI(33), "BLUE": ANSI(34),
	 "CYAN":ANSI(36), "MAGENTA":ANSI(35), "BLACK":ANSI(30),
	 "WHITE": ANSI(37), "B_RED":ANSI(41), "B_GREEN":ANSI(42),
	 "B_ORANGE":ANSI(43), "B_YELLOW":ANSI(1)+ANSI(43), "B_BLUE":ANSI(44),
	 "B_CYAN":ANSI(46), "B_BLACK":ANSI(40), "B_WHITE": ANSI(47),
	 "CLEARLINE":ESC("[L")+ESC("[G"), "B_MAGENTA":ANSI(45), "STATUS":"",
	 "WINDOW":"", "INITTERM":ESC("[H")+ESC("[2J"), "ENDTERM":"" ]) +
	nohtml;
    term_info["freedom"] =
	([ "RESET": ESC("G0"), "BOLD":ESC("G@"), "FLASH":ESC("G2"),
	 "BLACK":"", "RED":"", "GREEN":"", "ORANGE":"", "YELLOW":"", "BLUE":"",
	 "CYAN":"", "MAGENTA":"", "BLACK":"", "WHITE":"", "B_RED":ESC("GD"),
	 "B_GREEN": ESC("GD"), "B_ORANGE":ESC("G4"), "B_YELLOW":ESC("G4"),
	 "B_BLUE":ESC("G4"), "B_CYAN":ESC("GD"), "B_BLACK": ESC("GD"),
	 "B_WHITE":ESC("G4"), "B_MAGENTA":("G4"), "STATUS":"", "WINDOW":"",
	 "CLEARLINE":"\r", "INITTERM":"", "ENDTERM":"" ]) + nohtml;
    term_info["ansi-status"] =
	([ "RESET": ANSI("0;37;40"), "BOLD":ANSI(1), "FLASH":ANSI(5),
	 "BLACK":ANSI(30), "RED":ANSI(31), "GREEN":ANSI(32), "ORANGE":ANSI(33),
	 "YELLOW":ANSI(33), "BLUE":ANSI(34), "CYAN": ANSI(36),
	 "MAGENTA": ANSI(35), "BLACK":ANSI(30), "WHITE":ANSI(37),
	 "B_RED":ANSI(41), "B_GREEN":ANSI(42), "B_ORANGE":ANSI(43),
	 "B_YELLOW": ANSI(1)+ANSI(43), "B_BLUE":ANSI(44),
	 "B_CYAN": ANSI(46), "B_BLACK":ANSI(40), "B_WHITE":ANSI(47),
	 "B_MAGENTA":ANSI(45), "STATUS":ESC("[23;24r")+ESC(8), 
	 "WINDOW":ESC(7)+ESC("[0;22r")+ESC("[22H\n"),
	 "INITTERM":ESC("[H")+ESC("[J")+ESC("[23;24r")+ESC("23H\n"),
	 "CLEARLINE":"\r", "ENDTERM":ESC("[0r")+ESC("[H")+ESC("[J") ]) +nohtml;
    term_info["xterm"] =
	([ "RESET" :  ANSI("0;37;40"), "BOLD": ESC("[7m"),
	 "FLASH":ESC("[5m$<2>"), "BLACK":ANSI(30), "RED":ANSI(31),
	 "GREEN":ANSI(32), "ORANGE":ANSI(33), "YELLOW":ANSI(33),
	 "BLUE":ANSI(34), "CYAN":ANSI(36), "MAGENTA":ANSI(35),"BLACK":ANSI(34),
	 "WHITE":ANSI(38), "B_RED":ANSI(41), "B_GREEN":ANSI(42), 
	 "B_ORANGE":ANSI(43), "B_YELLOW":ANSI(43), "B_BLUE":ANSI(44), 
	 "B_CYAN":ANSI(46), "B_BLACK":ANSI(40), "B_WHITE": ANSI(47),
	 "B_MAGENTA": ANSI(45), "STATUS":"", "WINDOW":"", "INITTERM":"",
	 "CLEARLINE":"\r", "ENDTERM":"" ]) + nohtml;
    term_info["html"] = term_info["unknown"] +
	([ "RED"  : "<FONT COLOR=\"#FF0000\">",
	 "GREEN" : "<FONT COLOR=\"#00FF00\">",
	 "BLUE"  : "<FONT COLOR=\"#0000FF\">",
	 "YELLOW" : "<FONT COLOR=\"#FFFF00\">",
	 "CYAN" : "<FONT COLOR=\"#00A0DD\">",
	 "MAGENTA" : "<FONT COLOR=\"#C50067\">",
	 "YELLOW" : "<FONT COLOR=\"#FFFF00\">",
	 "RESET": "</FONT>",
	 "BR" : "<BR>", "P" : "<P>", "/P" : "</P>", ">" : ">",
	 "HREF" : "<A HREF=", "NAME" : "<A NAME=", "/A" : "</A>",
	 "I" : "<I>", "/I" : "</I>", "PRE" : "</PRE>", "/PRE" : "</PRE>",
	 "STRONG" : "<STRONG>", "/STRONG" : "</STRONG>",
	 "TABLE" : "<TABLE>", "/TABLE" : "</TABLE>", "TR" : "<TR>",
	 "/TR" : "</TR>", "TD" : "<TD>" , "/TD" : "</TD>" ]);
}

mapping query_term_info(string type) {
    return (term_info[type] ? term_info[type] : term_info["unknown"]);
}

string *query_terms() { return keys(term_info); }

int query_term_support(string str) {
    return (term_info[str] ? 1 : 0);
}

string no_colours(string str) {
    return terminal_colour(str, term_info["unknown"]);
}

string GetHTML(string str) {
    int i, tot, fcount = 0, ncount = 0;
    string tmp;
    
    str = terminal_colour(str, term_info["html"]);
    tmp = str;
    while( (i = strsrch(tmp, "<FONT")) != -1 ) {
	fcount++;
	tmp = tmp[(i+5)..];
    }
    if( fcount < 1 ) {
	return str;
    }
    tmp = str;
    while( (i = strsrch(tmp, "</FONT")) != -1 ) {
	ncount++;
	tmp = tmp[(i+6)..];
    }
    tot = fcount - ncount;
    if( tot > 0 ) {
	while( tot-- ) {
	    str += "</FONT>";
	}
    }
    return str;
}

