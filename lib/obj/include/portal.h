#ifndef __portal_obj_h__
#define __portal_obj_h__
 
static void create();
string eventShowPortal();
int SetLink(mixed dest);
string GetLink();
int SetLifeSpan(int x);
int GetLifeSpan();
mixed CanEnter(object who, string what);
int eventEnter(object who);
int eventDestruct();
 
#endif
