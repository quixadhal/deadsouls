#ifndef __AUTOSAVE_H
#define __AUTOSAVE_H

int Setup();
void heart_beat();
nomask void save_player(string nom);
void pre_save();
nomask void actually_save_player(string nom);
nomask int restore_player(string nom);
nomask void restore_inventory();
int eventDestruct();
string GetKeyName();

#endif __AUTOSAVE_H


