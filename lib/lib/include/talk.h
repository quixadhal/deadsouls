#ifndef l_talk_h
#define l_talk_h

varargs mixed CanSpeak(object target, object *bystanders, string verb, string msg, string lang);
varargs mixed eventHearTalk(object who, object target, int cls, string verb, string msg, string lang);

mixed eventTalkRespond(object who, object targ, int cls, string msg, string lang);
varargs mixed eventSpeak(object target, int cls, string msg, string lang);

/* pure virtual */ varargs int eventPrint(string msg, mixed arg2, mixed arg3);
/* pure virtual */ int GetLanguageLevel(string lang);
/* pure virtual */ string GetLanguageName(string str);
/* pure virtual */ varargs string GetMessage(string msg, mixed arg);
/* pure virtual */ int *GetScreen();

#endif /* l_talk_h */
