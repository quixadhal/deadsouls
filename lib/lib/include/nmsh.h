#ifndef __NMSH_H
#define __NMSH_H

int Setup();
nomask static int cmd_alias(string str);
nomask static int cmd_cd(string str);
nomask static int cmd_nickname(string str);
nomask static int cmd_nmsh(string str);
nomask static int cmd_pushd(string str);
nomask static int cmd_popd();
nomask string write_prompt();
nomask string process_input(string str);
nomask static void process_request(string request, string xtra);
static int request_vis(object ob);
static string user_name(object ob);
static private int set_cwd(string str);
static private void pushd(string str);
static private string popd();
nomask static private string do_nickname(string str);
nomask static private string do_alias(string str);
nomask static string replace_nickname(string str);
void reset_prompt();
string query_cwd();
string GetPrompt();
int query_mp();
int query_max_mp();
int query_hp();
int query_max_hp();
int query_sp();
int query_max_sp();
string get_path();
string GetClient();
varargs int GetInvis(object ob);
string GetKeyName();
static string cache_commands(string str);

#endif /* __NMSH_H */


