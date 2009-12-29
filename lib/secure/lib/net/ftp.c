/* /secure/lib/net/ftp.c
 * a ftp server using the DeadSouls /secure/daemon/inet.c daemon
 * Constructed from lima-1.0a8 /secure/daemons/ftp_d.c
 * Dvarsk@Nightmare 990126
 *  o Redesigned for Nightmare LPMud/DeadSouls socket system.
 *    o created new RETR file retrieve function.
 *    o general conversion from daemon server to server socket
 *  o Added access checking.
 *  o Added RNFR, RNTO file rename.
 *  o Added RMD directory delete.
 *
 * Code implemented under lima.org general usage policy 
 *     as per Lima 1.0a8 USAGE document.
 */

/* 
 ** First draft / quickly hacked FTPD.
 **
 ** Rust (rust@lima.mudlib.org) July 12, 1996
 **
 ** There are commands this doesn't support.  If your client
 ** Seems to want some other command to work for some weird reason,
 ** and this FTPD won't do it, let me know and I'll add it in.
 **
 ** Myth@Icon of Sin - Jan 19, 1997
 **  o Fixed STOR:
 **    o ascii: removed carriage returns
 **    o binary: added a fileposition flag
 **  o Added the SYST command.
 **  o Added line checking to the read callback.
 **  o Fixed send so that larger files can be handled without difficulty.
 ** Jan 21, 1997
 **  o Fixed the mkd command.
 ** 
 ** Tigran Sept 16, 1997
 **  o Fixed nlst and list (which are now identicle) so that they accept
 **    flags.  This ftp server should now be able to be used by most 
 **    GUI based clients, and should also work w/ ange-ftp and efs (from 
 **    emacs and Xemacs.  Note there is still work that can be done here.
 **    Acceptable flags are -a -l -C and -F
 **
 ** Naebator (sgt@israel.ru) Jan 9, 1998
 **  o Fixed LIST and NLST:
 **    o Added -1 flag
 **    o unless specified otherwize, forces -l flag on LIST and -1 flag on NLST.
 **    o directories report size 0.
 **  o If home directory does not exist, it defaults to "/".
 **
 */

#include <lib.h>
#include <dirs.h>
#include NETWORK_H
#include <runtime_config.h>
#include "include/ftp.h"

inherit LIB_SOCKET;

private static class  ftp_session Session;
private        string      Password  = 0;
private static mixed       outfile   = ([]);
private static int         MaxBuffer = get_config(__MAX_BYTE_TRANSFER__);
private static int         MaxFile   = get_config(__MAX_READ_FILE_SIZE__);
private static mapping     dispatch  = ([
        "user" : (: eventCmdUser :), "pass" : (: eventCmdPass :),
        "retr" : (: eventCmdRetr :), "stor" : (: eventCmdStor :),
        "nlst" : (: eventCmdNlst :), "list" : (: eventCmdList :),
        "cdup" : (: eventCmdCdup :), "quit" : (: eventCmdQuit :),
        "type" : (: eventCmdType :), "port" : (: eventCmdPort :),
        "noop" : (: eventCmdNoop :), "dele" : (: eventCmdDele :),
        "syst" : (: eventCmdSyst :), "rnfr" : (: eventCmdRnfr :),
        "rnto" : (: eventCmdRnto :), "stou" : (: eventCmdStou :),
        "cwd"  : (: eventCmdCwd  :), "mkd"  : (: eventCmdMkd  :),
        "pwd"  : (: eventCmdPwd  :), "rmd"  : (: eventCmdRmd  :),
        ]);

static void create(int fd, object owner){
    socket::create(fd, owner); 
    Session = new(class ftp_session);
    Session->cmdPipe = owner;
    Session->idleTime = 0;
}

nomask static int check_privs(string file, string oper) {
    string nom, tmp;
    int x;

    if(oper == "read" && (file == "/doc" || sscanf(file,"/doc/%*s" ))){ 
        return 1;
    }
    if( !sscanf(file, REALMS_DIRS "/%s", nom) ) {
        return 0;
    }
    if(file_privs(file) == Session->user){
        return 1;
    }
    if( sscanf(nom, "%s/%*s", tmp) ) {
        nom = tmp;
    }
    nom = user_path(nom)+"adm/access";
    if( file_size(nom+".c") < 0 ) {
        return 0;
    }
    catch(x = call_other(nom, "check_access", this_object(), "foo", 
                file, oper));
    return x;
}

mixed* clean_array(mixed* r) {
    int i, n;

    r = r & r; // sort.  sort_array() can't hack it.  And no, &= doesn't work.

    n = sizeof(r) - 1;
    while (i < n) {
        if (r[i] == r[i+1]) {
            int j = i+1;

            while (j < n && r[i] == r[j + 1])
                j++;

            r[i..j-1] = ({});
            n -= j - i;
        }
        i++;
    }

    return r;
}

private string find_flags(string arg){
    string array parts;
    string array flags=({});

    parts = filter(explode(arg, " "), (: $1[0]=='-' :));
    foreach(string part in parts) flags += explode(part,"");
    clean_array(flags);
    return implode(flags -= ({"-"}) ,"");
}

private string strip_flags(string arg){
    string array parts;

    parts = filter(explode(arg," "), (: $1[0] != '-' :));
    return implode(parts, " ");
}

string FindPrevDir( string path ) {
    string array parts = explode(path, "/");

    if(sizeof(parts) == 1) return path;
    parts = parts [0..<2];
    return "/" + implode(parts, "/");   
}

private void idle_time_out(){
    if(Session->dataPipe){ /* Data connections are still active. */
        Session->idleTime = 0;
    }
    else{
        Session->idleTime += 60;
        if(Session->idleTime > MAX_IDLE_TIME + 60){
            eventWrite("426 Idle time too long, connection closed.\n",1);
            Destruct();
            return;
        }
    }
    call_out("idle_time_out", 60);
}

private string GetFtpWelcomeMsg(){
    return sprintf("220- %s FTP server ready.\n%s"
            "220 Please login with your creator name or anonymous.\n",
            mud_name(),
            file_exists(FTP_WELCOME) ? "220- " +
            replace_string(read_file(FTP_WELCOME), "\n", "\n220- ") 
            +"\n": "");

}

string GetKeyName(){ return Session->user; }

string GetUniqueFileName(string arg){
    string array parts = explode(arg, "/");
    string path, file, sufx = "";
    int i = 0;

    if(sizeof(parts) == 1){
        path = "/";
        file = parts[0];
    }
    else{
        path = "/" + implode(parts [0..<2], "/");
        file  = parts [<1];
    }
    if(sscanf(file, "%s.%s", file, sufx) ==2){
        sufx = "." + sufx;
    }
    sscanf(file, "%s~%d", file, i);
    i++;
    arg = absolute_path(path, sprintf("%s~%d%s", file, i, sufx));
    while( file_exists(arg) ){
        i++;
        arg = absolute_path(path, sprintf("%s~%d%s", file, i, sufx));
    }
    return arg;
}

void StartService(){
    eventWrite(GetFtpWelcomeMsg(),0);
    call_out("idle_time_out", 60);
}

private void eventDestructDataPipe(mixed f){
    if(Session->dataPipe){
        Session->dataPipe->SetClose(f);
        Session->dataPipe->eventDestruct();
    }
}

void Destruct(){
    remove_call_out("idle_time_out");
    eventDestructDataPipe(0);
    ::Destruct();
}

private void eventCmdUser(string arg){
    if(!arg){ 
        eventWrite("500 command not understood.\n",0); 
        return; 
    }
    arg = lower_case(arg);
    if(Session->connected){
        eventWrite(sprintf("530 User %s access denied.\n", arg),0);
        return;
    }
    Session->user = arg;
#ifdef ALLOW_ANON_FTP
    if(member_array(arg, ({"anonymous", "ftp"})) != -1){
        eventWrite("331 Guest login ok, send your complete e-mail "
                "address as password.\n",0);
        return;
    }
#endif
    eventWrite(sprintf("331 Password required for %s.\n", arg),0);
    return;
}

private void eventCmdPswd(string arg){
    string path;
    if(!arg){ 
        eventWrite("500 command not understood.\n",0); 
        return; 
    }
    if(Session->connected || !Session->user){
        eventWrite("503 Login with USER first.\n",0);
        return;
    }

#ifdef ALLOW_ANON_FTP
    if(member_array(arg, ({"anonymous", "ftp"})) != -1){
        eventWrite("230 guest login ok, access restrictions apply.\n",0);
        Session->connected = 1;
        Session->priv = 0;
        Session->pwd = ANON_PREFIX;
        log_file("reports/network_connect", "Anomymous login "
                "(email = %s)\n", arg); 
        return;
    }
#endif
    path = DIR_CRES "/" + Session->user[0..0] + "/" + Session->user;
    RestoreObject(DIR_CRES "/" + Session->user[0..0] + "/" +
            Session->user, 1 );
    if(!Password || Password != crypt(arg, Password) ) {
        log_file("reports/network_connect", "Attempted login as %s\n",
                Session->user);
        eventWrite("530 Login incorrect.\n", 1);
        return;
    }
    eventWrite(sprintf("230 User %s logged in.\n", Session->user),0);
    log_file("reports/network_connect", "User %s logged in.\n",Session->user);
    Session->connected = 1;
    Session->priv = "" + Session->user;
    Session->pwd = absolute_path(REALMS_DIRS, Session->user);
    if (file_size(Session->pwd) != -2) Session->pwd = ANON_PREFIX;
    return;
}

private void eventCmdQuit(string arg){
    eventWrite("221 Goodbye.\n", 1);
}

string RetrieveCmdCallback(object ob){
    int start,length;
    mixed ret;

    if (!ob || undefinedp(outfile[ob])) return 0;

    start = outfile[ob][2];
    length = MaxBuffer;
    outfile[ob][2] += length;

    if (start + length > outfile[ob][4]) 
        length = outfile[ob][4] - start;

    ret = read_buffer(outfile[ob][0], start, length);
    if (outfile[ob][2] >= outfile[ob][4]) {
        map_delete(outfile, ob);
    }
    return ret;
}

private void eventReadFtpData(mixed text){
    switch(Session->binary){
        case 0:
            text=replace_string(text, CARRIAGE_RETURN, "");
            write_file(Session->targetFile, text);
            return;
        case 1:
            write_buffer(Session->targetFile, Session->filepos, text);
            Session->filepos += sizeof(text);
            return;
        default:
            error(sprintf("Assertion failed: \"##x\" (File: %s)\n",  __FILE__));
    }
}

private void eventCmdPort(string arg){
    string ip, *parts;
    int port;

    if(!arg){ 
        eventWrite("500 command not understood.\n",0); 
        return; 
    }
    parts = explode(arg, ",");
    if(sizeof(parts) != 6){
        eventWrite("550 Failed command.\n",0);
        return;
    }
    ip = implode(parts[0..3],".");
    port = (to_int(parts[4]) << 8) + to_int(parts[5]);
    if(Session->dataPipe) eventDestructDataPipe(0);
    Session->dataPipe = new(LIB_FTP_CLIENT);
    Session->dataPipe->SetOwner(this_object());
    Session->dataPipe->SetSocketType(Session->binary?STREAM_BINARY:STREAM);
    Session->dataPipe->eventCreateSocket(ip, port);
    Session->dataPipe->SetDestructOnClose(1);
    Session->dataPipe->SetRead((:eventReadFtpData:));
    Session->dataPipe->SetClose((:eventWrite("226 Transfer complete.\n",0):));
    eventWrite("200 PORT command successful.\n",0);
    return;
}

private void do_list( string arg, int ltype){
    string array 	files;
    string flags;
    string output;
    buffer data = allocate_buffer(MaxBuffer);

    if(arg){
        flags = find_flags(arg);
        arg   = strip_flags(arg);
    }

    if(!arg || arg == "") arg = ".";
    /* This hack added by Tigran because things like /secure/master.c/.
     * evaluate and cause havoc w/ ftp clients like efs for Xemacs and
     * ange-ftp for Emacs.  Besides it shouldn't happen anyways */
    if(arg[<2..]=="/." && file_exists(arg[0..<3]) ){
        eventWrite(sprintf("550 %s: No such file OR directory.\n",arg),0);
        eventDestructDataPipe(0);
        return;
    }
    arg = absolute_path(Session->pwd, arg);

#ifdef ALLOW_ANON_FTP
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1 
            && arg[0..(strlen(ANON_PREFIX)-1)] != ANON_PREFIX) { 
        eventDestructDataPipe((:eventWrite("550 Pemission denied.\n",0):); 
                return; 
                }
#endif
                if(file_size(arg) == -2) {      
                arg = (arg[<1] != '/')? arg+"/"+"*":arg+"*";
                }
                if(file_size(FindPrevDir(arg)) == -1){
                eventWrite(sprintf("550 %s: No such file OR directory.\n", arg),0);
                eventDestructDataPipe(0);
                return;
                }
                if(!(files = get_dir(arg, -1))){
                eventWrite(sprintf("550 %s: Permission denied.\n",arg),0);
                eventDestructDataPipe(0);
                return;
                }
                if(flags){
                if(strsrch(flags,'a') == -1)
                files = filter(files, (: member_array($1[0], ({".",".."})) == -1 :));
                }
                if(!sizeof(files)){
                    eventWrite("550 No files found.\n",0);
                    eventDestructDataPipe(0);
                    return;
                }

                /* in case of LIST imply -l */
                /* in case of NLST imply -1 */
                if (ltype == LTYPE_LIST){
                    if (flags){
                        if ( (strsrch(flags, 'l') == -1) &&
                                (strsrch(flags, 'C') == -1) &&
                                (strsrch(flags, '1') == -1) )
                            flags += "l"; 
                    }
                    else
                        flags = "l";
                }
                else{
                    if (flags){
                        if ( (strsrch(flags, 'l') == -1) &&
                                (strsrch(flags, 'C') == -1) &&
                                (strsrch(flags, '1') == -1) )
                            flags += "1";
                    }
                    else flags = "1";
                }
                if(strsrch(flags,'F') > -1){
                    foreach(mixed array file in files)
                        if(file[1]==-2) file[0]=sprintf("%s/",file[0]);
                }
                if(strsrch(flags,'C')>-1){
                    int lines;
                    int size;
                    int i;

                    if((strsrch(flags,'l') > -1 ) || (strsrch(flags,'1') > -1)){
                        eventWrite("550: LIST -C flag is incompatible with -1 or -l.\n",0);
                        eventDestructDataPipe(0);
                        return;
                    }
                    lines=((size = sizeof(files)) / 3 ) + 1;
                    output="";
                    for(i=0;i<lines;i++){
                        mixed array these_files;

                        if((i*3+2) < size){
                            these_files=files[(i*3)..(i*3+2)];
                        }
                        else if(i*3 < size){
                            these_files = files[(i*3)..];
                            while(sizeof(these_files)<3)
                                these_files += ({ ({"",0,0}) });
                        }
                        else break;
                        output = sprintf("%s%-=25s %-=25s %-=25s\n",
                                output, these_files[0][0],
                                these_files[1][0], these_files[2][0] );
                    }
                }
                if(strsrch(flags,'l') > -1){
                    if(strsrch(flags,'1')>-1){
                        eventWrite("550: LIST -l and -1 flags incompatible.\n",0);
                        eventDestructDataPipe(0);
                        return;
                    }
                    output = implode(map(files, 
                                (:sprintf("%s %3i %=9s %=8s %=7s %s%5s %s",
                                          $1[1]==-2?"drwxrwxr-x":"-rw-rw-r--",
                                          1,
                                          lower_case(replace_string(mud_name(), " ", "_"))[0..7],
                                          lower_case(replace_string(mud_name(), " ", "_"))[0..7],
                                          $1[1]==-2?"0":sprintf("%d",$1[1]),
                                          ctime($1[2])[4..10],
                                          (time()-$1[2])>31536000?ctime($1[2])[20..]:ctime($1[2])[11..15],
                                          $1[0]) :)),"\n");
                }
                if(strsrch(flags,'1') > -1)
                    output=implode(map(files,(:sprintf("%s",$1[0]) :)),"\n");
                eventWrite("150 Opening ASCII mode data connection for file list."+CARRIAGE_RETURN+"\n",0);
                Session->dataPipe->eventWrite(implode(explode(output,"\n"), CARRIAGE_RETURN+"\n")+CARRIAGE_RETURN+"\n");
                return;
}

private void eventCmdList(string arg){
    if(!check_privs(absolute_path(Session->pwd, arg), "read")){
        eventDestructDataPipe((:eventWrite("550 Pemission denied.\n",0):));
        return;
    }
    do_list(arg, LTYPE_LIST); 
}

private void eventCmdNlst(string arg){
    if(!check_privs(absolute_path(Session->pwd, arg), "read")){
        eventDestructDataPipe((:eventWrite("550 Pemission denied.\n",0):));
        return;
    }
    do_list(arg, LTYPE_NLST); 
}

private void eventCmdRetr(string arg){
    string target_file;
    int i;

    if(!arg){ 
        eventDestructDataPipe((:eventWrite("500 command not understood.\n",0):)); 
        return; 
    }
    target_file = absolute_path(Session->pwd, arg);
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1 
            && target_file[0..(strlen(ANON_PREFIX)-1)] != ANON_PREFIX) { 
        eventDestructDataPipe((:eventWrite("550 Pemission denied.\n",0):));
        return; 
    }
    i = file_size(target_file);
    switch(i){
        case -2:
            eventWrite(sprintf("550 %s: Can't retrieve (it's a directory).\n",
                        target_file),0);
            eventDestructDataPipe(0);
            return;
        case -1:
            eventWrite(sprintf("550 %s: No such file OR directory.\n", 
                        target_file),0);
            eventDestructDataPipe(0);
            return;
        case 0:
            eventWrite(sprintf("550 %s: File contains nothing.\n",
                        target_file),0);
            eventDestructDataPipe(0);
            return;
        default:
            if(i > MaxFile){ 
                eventWrite(sprintf("550 %s: File size too large.\n",
                            target_file),0);
                eventDestructDataPipe(0);
                return;
            }
    }
    if(!check_privs(target_file, "read")){
        eventDestructDataPipe((:eventWrite("550 Pemission denied.\n",0):));
        return;
    }
    switch(Session->binary){
        case 0:	
            outfile[Session->dataPipe]=({target_file,0,0,Session->cmdPipe, i});
            eventWrite(sprintf("150 Opening ascii mode data connection for "
                        "%s (%d bytes).\n", target_file, i),0);
            Session->dataPipe->SetWrite((: RetrieveCmdCallback :));

            Session->dataPipe->eventWrite(RetrieveCmdCallback(Session->dataPipe));
            break;
        case 1:
            outfile[Session->dataPipe]=({target_file,1,0,Session->cmdPipe, i});
            eventWrite(sprintf("150 Opening binary mode data connection "
                        "for %s (%d bytes).\n", target_file, i),0);
            Session->dataPipe->SetWrite((: RetrieveCmdCallback :));

            Session->dataPipe->eventWrite(RetrieveCmdCallback(Session->dataPipe));
            break;
        default:
            error(sprintf("Assertion failed: \"##x\" (File: %s)\n",  __FILE__));
    }
}

void eventCmdPwd(string arg){
    eventWrite(sprintf("257 \"%s\" is current directory.\n", Session->pwd),0);
}

void eventCmdNoop(string arg){
    eventWrite("221 NOOP command successful.\n",0);
}

private void eventCmdStor(string arg){
    if(!arg){ 
        eventDestructDataPipe((:eventWrite("500 command not understood.\n",0):)); 
        return; 
    }
    arg = absolute_path(Session->pwd, arg);
#ifndef ANON_CAN_PUT
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1){
        eventDestructDataPipe((:eventWrite, "550 Pemission denied.\n", 0:)); 
        return;
    }
#else
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1 
            && arg[0..(strlen(ANON_PREFIX)-1)] != ANON_PREFIX) { 
        eventDestructDataPipe((:eventWrite, "550 Pemission denied.\n", 0:)); 
        return; 
    }
#endif
    if(file_size(FindPrevDir(arg)) != -2 ){
        eventDestructDataPipe((:eventWrite, 
                    "553 No such directory to store into.\n", 0:));
        return;
    }
    if(!check_privs(arg, "write")){
        eventDestructDataPipe((:eventWrite("550 Pemission denied.\n",0):));
        return;
    }
    Session->targetFile = arg;
    if( file_exists(arg) ){
        if( !rm( arg ) ){
            eventWrite(sprintf("550 %s: Permission denied.\n", arg),0);
            eventDestructDataPipe(0);
            return;
        }
    }
    else if(!write_file( arg, "") ){
        eventWrite(sprintf("550 %s: Permission denied.\n", arg),0);
        eventDestructDataPipe(0);
        return;
    }
    Session->filepos = 0;
    eventWrite(sprintf("150 Opening %s mode data connection for %s.\n",
                Session->binary ? "binary" : "ascii", arg),0);    
}

private void eventCmdStou(string arg){
    if(!arg){ 
        eventDestructDataPipe((:eventWrite("500 command not understood.\n",0):)); 
        return; 
    }
    arg = absolute_path(Session->pwd, arg);
#ifndef ANON_CAN_PUT
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1){
        eventDestructDataPipe((:eventWrite, "550 Pemission denied.\n", 0:)); 
        return;
    }
#else
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1 
            && arg[0..(strlen(ANON_PREFIX)-1)] != ANON_PREFIX) { 
        eventDestructDataPipe((:eventWrite, "550 Pemission denied.\n", 0:)); 
        return; 
    }
#endif
    if(file_size(FindPrevDir(arg)) != -2 ){
        eventDestructDataPipe((:eventWrite, 
                    "553 No such directory to store into.\n", 0:));
        return;
    }
    if(!check_privs(arg, "write")){
        eventDestructDataPipe((:eventWrite("550 Pemission denied.\n",0):));
        return;
    }
    if( file_exists(arg)){
        arg = GetUniqueFileName(arg);
    }
    Session->targetFile = arg;
    if(!write_file( arg, "") ){
        eventWrite(sprintf("550 %s: Permission denied.\n", arg),0);
        eventDestructDataPipe(0);
        return;
    }
    Session->filepos = 0;
    eventWrite(sprintf("150 Opening %s mode data connection for %s.\n",
                Session->binary ? "binary" : "ascii", arg),0);    
}

private void eventCmdCwd(string arg){
    string newpath;
    if(!arg){ 
        eventWrite("500 command not understood.\n",0); 
        return; 
    }
    newpath = absolute_path(Session->pwd, arg);
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1 
            && newpath[0..(strlen(ANON_PREFIX)-1)] != ANON_PREFIX) { 
        eventWrite("550 Pemission denied.\n",0); 
        return; 
    }
    if(!check_privs(newpath, "read")){
        eventWrite("550 Pemission denied.\n",0);
        return;
    }
    if( file_size(newpath) != -2 ){
        eventWrite(sprintf("550 %s: No such directory.\n", newpath),0);
        return;
    }
    Session->pwd = newpath;
    eventWrite("250 CWD command successful.\n",0);
}

private void eventCmdCdup(string arg){ eventCmdCwd(".."); }

private void eventCmdMkd(string arg){
    if(!arg){ 
        eventWrite("500 command not understood.\n",0); 
        return; 
    }
    arg = absolute_path(Session->pwd, arg);
#ifndef ANON_CAN_PUT
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1){
        eventWrite("550 Permission denied.\n",0);
        return;
    }
#else
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1 
            && arg[0..(strlen(ANON_PREFIX)-1)] != ANON_PREFIX) { 
        eventWrite("550 Pemission denied.\n",0); 
        return; 
    }
#endif
    if( file_size(FindPrevDir(arg)) != -2 ){
        eventWrite(sprintf("550 %s: No such directory.\n",
                    FindPrevDir(arg)),0);
        return;
    }
    if(unguarded((:file_size($(arg)):)) != -1){
        eventWrite(sprintf("550 %s: File exists.\n", arg),0);
        return;
    }
    if(!check_privs(arg, "write")){
        eventWrite("550 Pemission denied.\n",0);
        return;
    }
    if( !mkdir(arg) ){
        eventWrite(sprintf("550 %s: Permission denied.\n", arg),0);
        return;
    }
    eventWrite("257 MKD command successful.\n",0);
    return;
}

private void eventCmdType(string arg){
    if(!arg){ 
        eventWrite("500 command not understood.\n",0); 
        return; 
    }
    arg = lower_case(arg);
    switch(arg){
        case "a":
            Session->binary = 0;
        eventWrite("200 Type set to A.\n",0);
        return;
        case "i":
            Session->binary = 1;
        eventWrite("200 Type set to I.\n",0);
        return;
        default:
        eventWrite("550 Unknown file type.\n",0);
        return;
    }
}

private void eventCmdDele(string arg){
    if(!arg){ 
        eventWrite("500 command not understood.\n",0); 
        return; 
    }
    arg = absolute_path( Session->pwd, arg);
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1 
            && arg[0..(strlen(ANON_PREFIX)-1)] != ANON_PREFIX) { 
        eventWrite("550 Pemission denied.\n",0); 
        return; 
    }
    if( !file_exists(arg) ){
        eventWrite(sprintf("550 %s: No such file OR directory.\n", arg),0);
        return;
    }
    if(!check_privs(arg, "write")){
        eventWrite("550 Pemission denied.\n",0);
        return;
    }
    if( !rm(arg) ){
        eventWrite(sprintf("550 %s: Permission denied.\n",arg),0);
        return;
    }
    eventWrite("250 DELE command successful.\n",0);
}

private void eventCmdSyst(string arg) {
    eventWrite("215 UNIX Mud Name: "+mud_name()+"\n",0);
}

private void eventCmdRnfr(string arg) {
    if(!arg){ 
        eventWrite("500 command not understood.\n",0); 
        return; 
    }
    arg = absolute_path( Session->pwd, arg);
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1 
            && arg[0..(strlen(ANON_PREFIX)-1)] != ANON_PREFIX) { 
        eventWrite("550 Pemission denied.\n",0); 
        return; 
    }
    if( !file_exists(arg) ){
        eventWrite(sprintf("550 %s: No such file OR directory.\n", arg),0);
        return;
    }
    if(!check_privs(arg, "write")){
        eventWrite("550 Pemission denied.\n",0);
        return;
    }
    Session->renamefrom = arg;
    eventWrite("350 Input new name for " + arg + ".\n",0);
}

private void eventCmdRnto(string arg) {
    if(!arg){ 
        eventWrite("500 command not understood.\n",0); 
        return; 
    }
    arg = absolute_path( Session->pwd, arg);
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1 
            && arg[0..(strlen(ANON_PREFIX)-1)] != ANON_PREFIX) { 
        eventWrite("550 Pemission denied.\n",0); 
        return; 
    }
    if( !Session->renamefrom ){
        eventWrite(sprintf("550 You must first specify a file to rename.\n", 
                    arg),0);
        return;
    }
    if( !file_exists(Session->renamefrom) ){
        eventWrite(sprintf("550 %s: No such file OR directory.\n", arg),0);
        Session->renamefrom = 0;
        return;
    }
    if(!check_privs(Session->renamefrom, "write")){
        eventWrite("550 Pemission denied.\n",0);
        Session->renamefrom = 0;
        return;
    }
    if(!check_privs(FindPrevDir(arg), "write")){
        eventWrite("550 Pemission denied.\n",0);
        Session->renamefrom = 0;
        return;
    }
    if(rename(Session->renamefrom, arg)){
        eventWrite(sprintf("553 %s: Invalid desitination name.\n", arg),0);
    }
    eventWrite(sprintf("250 RNTO %s --> %s.\n", Session->renamefrom, arg),0);
    Session->renamefrom = 0;
}

private void eventCmdRmd(string arg) {
    if(!arg){ 
        eventWrite("500 command not understood.\n",0); 
        return; 
    }
    arg = absolute_path( Session->pwd, arg);
    if(member_array(Session->user, ({"anonymous", "ftp"})) != -1 
            && arg[0..(strlen(ANON_PREFIX)-1)] != ANON_PREFIX) { 
        eventWrite("550 Pemission denied.\n",0); 
        return; 
    }
    if( file_size(arg) != -2 ){
        eventWrite(sprintf("550 %s: No such file OR directory.\n", arg),0);
        return;
    }
    if(!check_privs(arg, "write")){
        eventWrite("550 Pemission denied.\n",0);
        return;
    }
    if( !rmdir(arg) ){
        eventWrite(sprintf("550 %s: Directory not empty.\n",arg),0);
        return;
    }
    eventWrite("250 RMD command successful.\n",0);
}

void eventRead(string data){
    string cmd, arg;
    function dispatchTo;
    int i;

    if (!(Session->command)) Session->command = "";

    data = replace_string(data, CARRIAGE_RETURN, "");
    Session->command += data;
    if ((i = strsrch(Session->command, "\n")) == -1) return;
    data=Session->command[0..i-1];
    Session->command=Session->command[i+1..];
    Session->command = trim(Session->command);
    if (!sscanf(data, "%s %s", cmd, arg)) cmd = data;
    cmd = lower_case(cmd);

    if (!Session->connected){
        switch(cmd){
            case "user":
                eventCmdUser(arg);
            return;
            case "pass":
                eventCmdPswd(arg);
            return;
            case "quit":
                eventCmdQuit(arg);
            return;
            case "noop":
                eventCmdNoop(arg);
            return;
            default:
            eventWrite("503 Log in with USER first.\n",0);
            return;
        }    
    }
    Session->idleTime = 0;
    dispatchTo = dispatch[cmd];
    if (!dispatchTo){
        log_file("reports/network_error",
                sprintf("ftp - unknown command: %s\n",cmd)); 
        eventWrite(sprintf("502 Unknown command %s.\n", cmd),0);
        return;
    }
    if(catch(evaluate(dispatchTo, arg))){
        eventWrite("550 Unknown failure.  Please report what you were doing "
                "to the mud admin.\n",0);
    }
    return;
}
