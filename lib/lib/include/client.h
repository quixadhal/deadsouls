#ifndef __CLIENT_H__
#define __CLIENT_H__

int eventCreateSocket(string host, int port);
static void eventAbortCallback(int fd);
static void eventReadCallback(int fd, mixed val);
static void eventRead(mixed val);
static void eventWriteCallback(int fd);
void eventWrite(mixed val);
static void eventClose(class client sock);
static void eventSocketClose();
int eventDestruct();
static void eventSocketError(string str, int x);
function SetRead(function f);
int SetSocketType(int x);
int SetDestructOnClose(int x);

#endif /* __CLIENT_H__ */
