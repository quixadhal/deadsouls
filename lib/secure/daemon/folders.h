#ifndef __FOLDERS_H
#define __FOLDERS_H

static private void load_folder(string who, string folder);
static private void save_folder();
mapping *query_box_info(string who, string folder);
void add_post(string who, string folder, mapping borg);
void delete_posts(string who, string folder, int *del);
void mark_read(string who, string folder, int x);
mapping mail_status(string who);
int sort_folder(mapping a, mapping b);
int filter_folder(mapping borg);

#endif /* __FOLDERS_H */
