#ifndef __BBOARD_H
#define __BBOARD_H

void create();
static private void save_board();
static private void restore_board();
static private int valid_access();
void add_post(string id, string who, string subj, string msg);
void remove_post(string id, int post);
void mark_read(string id, int post, string reader);
mapping query_post(string id, int post);
mapping *query_posts(string id);
int query_number_posts(string id);

#endif /* __BBOARD_H */
