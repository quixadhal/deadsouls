#ifndef __HTTP_H__
#define __HTTP_H__

static void create();
static void Setup();
static void eventRead(int fd, string str);
private static void eventGetFile(int fd, string file);

#endif /* __HTTP_H__ */
