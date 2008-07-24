/**
 *
 * 2006-03-21, jonez
 *  - changed prototype for update to return int instead of object
 *  - changed prototype for reload_room to remove the "recurse" argument
 *  - changed prototype for itoa to return string instead of int
 */

#ifndef l_sefun_h
#define l_sefun_h

#ifndef CARRIAGE_RETURN
#define CARRIAGE_RETURN "\r"
#endif

varargs string socket_address(mixed arg, int foo);
string absolute_path(string curr, string newpth);
int absolute_value(int x);
varargs string add_article(string str, int def);
int ambassadorp(object ob);
string architecture();
int archp(object ob);
string arrange_string(string str, int x);
string base_name(mixed val);
string cardinal(int x);
varargs string center(string str, int x);
varargs string conjunction(mixed expressions, string coordinator);
string consolidate(int x, string str);
string convert_name(string str);
varargs mixed convert_string(string str, int flag);
mixed copy(mixed val);
varargs int creatorp(object ob);
float currency_inflation(string type);
float currency_rate(string type);
int currency_mass(int x, string type);
int currency_value(int x, string str);
int destruct(object ob);
mixed *distinct_array(mixed *arr);
string domain(mixed val);
int domain_exists(string dmn);
object domain_master(mixed val);
varargs int effective_light(object ob);
varargs void event(string fun, int when, mixed *args, int reg);
mapping expand_keys(mapping mp);
int event_pending(object ob);
int exec(object target, object src);
int file_exists(string str);
string file_privs(string file);
varargs string format_page(string *items, int columns, int modifier);
varargs object get_object(string str, object player, int living);
varargs mixed get_objects(string str, object player, int no_arr);
int hiddenp(object ob);
int high_mortalp(object ob);
//string identify(mixed a);
varargs string identify( mixed a );
string item_list(mixed *items);
int leaderp(object ob);
object *livings();
//object load_object(string str);
void log_file(string fl, string msg);
int member_group(mixed who, string grp);
int moral_act(object who, object target, int amount);
string *mud_currencies();
string mud_name();
varargs string imc2_mud_name(string name);
string mudlib();
string mudlib_version();
varargs int newbiep(object ob);
string nominative(mixed val);
string objective(mixed val);
string ordinal(int x);
object parse_objects(object where, string str);
mixed *path_file(mixed full_path);
float percent(mixed numerator, mixed denominator);
varargs int playerp(object ob);
string possessive(mixed val);
string possessive_noun(mixed val);
int query_date(int x);
string query_day_name(int x);
int query_host_port();
int query_hour(int x);
int query_minutes(int x);
string query_month(int x);
int query_night();
int query_reset_number();
string query_season(int x);
object query_snoop(object snoopee);
object query_snooping(object ob);
string query_time_of_day();
int query_year(int x);
string reflexive(mixed val);
string remove_article(string str);
string save_file(string str);
varargs void say(mixed str, object ob);
void set_eval_limit(int x);
void set_privs(object ob, string str);
varargs void shout(mixed str, mixed exclude);
varargs void shutdown(int code);
varargs object snoop(object who, object target);
string strip_article(mixed val);
string strip_colours(string str);
void tell_object(object ob, mixed str);
varargs void tell_room(object ob, mixed str, mixed exclude);
varargs object this_agent(mixed val);
function time_event(string tod, function f);
object to_object(mixed target);
varargs int total_light(object ob);
string translate(string str, int prof);
string trim(string str);
mixed unguarded(function f);
object unique(string fn, int rare);
int user_exists(string str);
string user_path(string name);
string version();
varargs int visibility(object ob);
varargs int visible(object detectee_obj, object detector_obj);
varargs string wrap(string str, int width);
void write(string str);
int tail(string file);
void tell_player(mixed player, string msg);
string *query_custom_path();
string query_custom_command(string str);
string dump_socket_status();
string flat_map(mapping mp);
string *get_cmds();
string *get_verbs();
varargs object array get_livings(object ob,int foo);
string local_time(string str);
void reap_dummies();
varargs void reap_other(string str);
mixed reap_list();
string timestamp();
object *get_dupes(object ob, object where);
varargs string get_stack(int x);
mixed hobbled(object ob);
string query_base_currency();
int query_base_rate();
int query_player_money(object who);
int query_base_value(string currency, int amount);
varargs int make_workroom(mixed dude, int cre);
int valid_currency(string str);
varargs int query_invis(object lookee, object looker);
string set_tz(string str);
string admin_email();
int query_night_light(object ob);
int query_day_light(object ob);
int query_ambient_light(object ob);
int reload_room(object ob);
void tc(string str);
string opposite_dir(string str);
mixed *singular_array(mixed *arr);
varargs mixed remove_matching_line(string target, string substring, int i, string exclude);
string truncate(string str, int i);
int check_privs(mixed pretender, mixed target);
varargs int true(mixed args...);
varargs int false(mixed args...);
int indent_file(string filename);
string path_prefix(string str);
varargs string first_string_element(string str, string delimiter, int stripfirst);
string last_string_element(string str, string delimiter);
string replace_line(string file, string *params, string repl);
int memberp(mixed *primary, mixed sub);
int reverse_memberp(string primary, string *sub);
int mgrepp(string primary, string *sub);
int grepp(string primary, string sub);
varargs mixed replace_matching_line(string target, string substring, string replace, int i, string exclude);
varargs mixed remove_matching_line(string target, string substring, int i, string exclude);
string first(string str, int i);
varargs string last(string str, int i, int significant);
int starts_with_arr(string primary, string *sub);
int starts_with(string primary, string sub);
string reverse_string(string str);
varargs mixed homedir(mixed ob, int cre);
varargs string generate_tmp(mixed arg);
string newline_trim(string str);
int directory_exists(string str);
mixed reload(object ob);
string query_os_type();
varargs mixed read_matching_line(string target, string substring, int i, string exclude);
int abs(int x);
int wipe_inv(mixed dude);
int bool_reverse(mixed arg);
mixed check_light(object who);
int query_carrying(object who, mixed thing);
mixed *findobs(mixed arg);
int atoi(string str);
string itoa(int i);
string *query_names(object whom);
int answers_to(string name, object what);
varargs void add_event(string source, string ob, string fun, mixed *args, int when, int repeat);
varargs void remove_event(int i);
int update(string str);
mixed local_ctime(int i);
int numericp(mixed arg);
int basic_punctuationp(mixed arg);
mixed mapping_member(mapping Map, string sub);
mixed find_inheritors(mixed arg);
mixed find_deep_inheritors(mixed arg);
int imud_privp(mixed guy);
int securep(mixed guy);
string convert_ascii(int i);
varargs object get_random_living(object room, int foo);
varargs int debug(mixed msg, mixed val, string color);
int valid_timezone(string str);
varargs int tell_creators(string msg, string color);
varargs int tn(string str, string col, object room);
varargs int trr(string str, string col);
varargs string *wild_card(string str);
nomask int remove_dots(string str);
int domain_admin(string pretender, string domain);
int compare_array(mixed *arr1, mixed *arr2);
int arrcmp(mixed *arr1, mixed *arr2);
string append_line(string file, string *params, string repl);
int alphap(mixed arg);
varargs mixed alpha_crypt(mixed arg1, mixed arg2);
void set(mixed arg1, mixed arg2);
void query(mixed arg1, mixed arg2);
void personal_log(string str);
void add_sky_event(function f);
int assistp(mixed guy);
int elderp(object ob);
int testp(object ob);
string alpha_strip(mixed arg);
mixed seteuid(mixed args);
mixed getuid(mixed args);
mixed geteuid(mixed args);string *query_local_functions(mixed arg);
int check_string_length(string str);
varargs mixed print_long_string(object who, string str, int catted);
void add(mixed arg1, mixed arg2);
varargs int wizardp(object ob);
int adminp(object ob);
varargs mixed dump_variable(mixed foo, mixed bar);
string day();
int date();
string month();
int year();
varargs int minutes(int x);
varargs string season(int x);
string query_time();
varargs int hour(int x);
varargs string time_elapsed(int arg1, int arg2);
int bitshiftedp(int x);
#if CALL_OUT_LOGGING
int call_out(mixed args...);
#endif
string strip_colors(string str);
string strip_colors_old(string str);
varargs string *chunk_string(string str, int width);
string array groups();
string query_intermud_ip();
string *atomize_string(string str);
string *atomize_array(string *temp_arr);
string match_command(string verb);
varargs mixed random_numbers(int n, int integer);
int snooperp(mixed guy);
string native_version();
string convert_newline(string str);
int clean_newline_file(string str);
mixed convert_into_ascii(string str);
int check_read(string file);
varargs string simple_map(mixed arg);
string query_door(object what, string direction);
int mkdir_recurse(string path);
mixed *socket_names();
string morse(string msg);
string unmorse(string msg);
int *roll(string alea);
mixed *scramble_array(mixed *arr);
int builderp(object ob);
string socket_ip(int fd);
string unpinkfish(string str);
string repinkfish(string str);
string web_translate(string str);
string *efuns();
string *sefuns();
int efun_exists(string str);
int sefun_exists(string str);
varargs int estatep(object ob);
string dbz_colors(string str, int annoying);
mixed lpc_file(string str);
int telnet_privp(mixed guy);
object *containers(object ob);
varargs mixed FAKE_NEW(string str, mixed args...);
int ordinalp(string str);
object *get_dummies(mixed where);

#endif /* l_sefun_h */
