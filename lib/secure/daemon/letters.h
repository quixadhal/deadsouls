#ifndef __LETTERS_H
#define __LETTERS_H

static private int valid_access();
string create_letter(string str);
static private void save_letter();
static private int restore_letter(string id);
string query_letter(string id);
void delete_folder(string who, string folder, string id);
void add_folder(string who, string folder, string id);
static void manage_letters();
static void manage_postal();

#endif /* __LETTERS_H */
