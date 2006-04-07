
#ifndef __FTP__
#define __FTP__

private void eventCmdUser(string arg);
private void eventCmdPass(string arg);
private void eventCmdRetr(string arg);
private void eventCmdStor(string arg);
private void eventCmdNlst(string arg);
private void eventCmdList(string arg);
private void eventCmdCdup(string arg);
private void eventCmdQuit(string arg);
private void eventCmdType(string arg);
private void eventCmdPort(string arg);
private void eventCmdNoop(string arg);
private void eventCmdDele(string arg);
private void eventCmdRnfr(string arg);
private void eventCmdRnto(string arg);
private void eventCmdStou(string arg);
private void eventCmdSyst(string arg);
private void eventCmdCwd(string arg);
private void eventCmdMkd(string arg);
private void eventCmdPwd(string arg);
private void eventCmdRmd(string arg);

class ftp_session {
    int      connected;
    string   user;
    mixed    priv;
    string   pwd;
    object   dataPipe;
    object   cmdPipe;
    int      idleTime;
    int      binary;
    string   targetFile;
    string   command;
    int      filepos;
    string   renamefrom;
}

#define LTYPE_LIST      0
#define LTYPE_NLST      1
#define MAX_IDLE_TIME	600

#endif  /* __FTP__ */

#ifndef ANON_PREFIX
#define ANON_PREFIX	"/ftp"
#endif /* ANON_PREFIX */

#ifndef FTP_WELCOME
#define FTP_WELCOME	"/news/ftpd"
#endif /* FTP_WELCOME */
