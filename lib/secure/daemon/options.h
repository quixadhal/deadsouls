#ifndef __OPTIONS_D_H
#define __OPTIONS_D_H

static private int valid_access();
void assure_box_exists(string who);
static private void load_options(string who);
static private void save_options();
void set_option(string who, string option, mixed value);
mixed query_option(string who, string option);
void set_group(string who, string group, string *members);
string *query_group(string who, string group);
mapping query_groups(string who);

#endif /* __OPTIONS_D_H */
