#ifndef __REMOTE_H__
#define __REMOTE_H__

static void create();
static void Setup();

static void eventRead(int fd, string str);
static void eventProcess(int fd, string str);

#endif /* __REMOTE_H__ */
