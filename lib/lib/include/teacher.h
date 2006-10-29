#ifndef __trainer_h__
#define __trainer_h__

static void create();
static void init();

mixed AddTeachingLanguages(string *args...);
mixed RemoveTeachingLanguages(string *args...);
string *GetTeachingLanguages();
mapping GetStudents();

int eventHelp(object who, string unused);
int eventTeachLanguage(object who, string verb, string language);

static int ContinueTeaching(object who, string language, int x);

int eventStart(object who, string language);
int eventContinue(object who, string language, int x);
int eventComplete(object who, string language);

#define TEACHING_WAIT 10
#endif __trainer_h__
