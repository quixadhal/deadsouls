#ifndef __MASTER_H
#define __MASTER_H

void create();
void new_read();
void new_write();
void new_groups();
static private void load_access(string cfg, mapping ref);
void flag(string str);
string *epilog(int x);
string privs_file(string file);
void preload(string str);
int valid_write(string file, object ob, string fun);
int valid_read(string file, object ob, string fun);
int valid_apply(string *ok);
int check_access(object ob, string fun, mixed file, string *ok, string oper);
nomask static int check_user(object ob, string fun, string file, string oper);
nomask static int check_domain(object ob, string fun, string file, string o);
object connect(int port);
object compile_object(string str);
static void crash(string err);
int valid_hide(object who);
int valid_override(string file, string nom);
int valid_socket(object ob, string fun, mixed *info);
mixed apply_unguarded(function f);
string error_handler(mapping mp, int caught);
void log_error(string file, string msg);
varargs string standard_trace(mapping mp, int flag);
string trace_line(object obj, string prog, string file, int line);
int different(string fn, string pr);
void master_log_file(string file, string msg);
string make_path_absolute(string file);
int player_exists(string str);
string domain_file(string str);
string author_file(string str);
static int slow_shutdown();
int save_ed_setup(object who, int code);
int retrieve_ed_setup(object who);
string get_save_file_name(string file);
int is_locked();
string *parse_command_id_list();
string *parse_command_plural_id_list();
string *parse_command_adjectiv_id_list();
string *parse_command_prepos_list();
string parse_command_all_word();
void create_save();
object player_object(string nom);
static void eventReset();
int GetResetNumber();

#endif /* __MASTER_H */

