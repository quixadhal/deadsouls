#ifndef __COMMAND_H__
#define __COMMAND_H__

static void create();
void eventRehash(mixed paths);
string GetCommand(string cmd, string *path);
varargs string *GetCommands(string path);
varargs string *GetPaths(string cmd);

#endif /* __COMMAND_H__ */
