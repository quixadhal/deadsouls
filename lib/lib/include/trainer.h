#ifndef __trainer_h__
#define __trainer_h__

static void create();
static void init();

mixed AddTrainingSkills(string *args...);
mixed RemoveTrainingSkills(string *args...);
string *GetTrainingSkills();
mapping GetStudents();

mapping AddCommandResponses(mapping mp);
mapping AddRequestResponses(mapping mp);

int eventHelp(object who, string unused);
int eventTrain(object who, string verb, string skill);

static int ContinueTraining(object who, string skill, int x);

int eventStart(object who, string skill);
int eventContinue(object who, string skill, int x);
int eventComplete(object who, string skill);

#define TRAINING_WAIT 2
#endif __trainer_h__
