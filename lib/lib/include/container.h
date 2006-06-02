#ifndef l_container_h
#define l_container_h

int CanReceive(object ob);
int CanRelease(object ob);

int eventReceiveObject();
int eventReleaseObject();

int GetAmbientLight();
static int SetAmbientLight(int x);
string GetInternalLong();
static string SetInternalLong(string str);
string GetInternalShort();
static string SetInternalShort(string str);
int GetOpacity();
int SetOpacity(int x);
int GetRadiantLight(int ambient);

#endif /* l_container_h */
