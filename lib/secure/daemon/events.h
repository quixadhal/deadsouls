#ifndef __EVENTS_H__
#define __EVENTS_H__

static void create();
varargs static int eventSave(int ung);
void eventReboot(int x);
static void eventAnnounceReboot(int x);
void eventShutdown();
static void Shutdown();
static void eventPollEvents();

int SetRebootInterval(int x);
int GetRebootInterval();
void AddEvent(string c, string s, string f, mixed a, int w, int r);
mapping GetEvents();

#endif /* __EVENTS_H__ */
