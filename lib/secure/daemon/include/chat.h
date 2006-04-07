#ifndef __CHAT_H__
#define __CHAT_H__

static void create();
string *eventRegisterMember(string *chans);
string *eventRemoveMember(string *chans);
int cmdChannel(string verb, string str);
varargs void eventSendChannel(string who, string ch, string msg, int emote,
  string target, string targmsg);
string *GetChannelList(string ch);
string GetLocalChannel(string ch);
string GetRemoteChannel(string ch);

#endif /* __CHAT_H__ */
