#ifndef __NOTIFY_H__
#define __NOTIFY_H__
 
#define Date       0
#define Author     1
#define Message    2
 
static void create();
static int eventSaveNotices();
int eventAddNotice(object who, string msg);
int eventRemoveNotice(int x);
int eventPrintNotices(object who, int start_time);
 
#endif
