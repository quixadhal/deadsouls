#ifndef l_character_h
#define l_character_h

static void create();

mixed eventConnect(string who);
mixed eventLink(string primary, string secondary, string email);
mixed eventUnlink(string primary, string who);

mapping GetLinks();
mapping GetLink(string who);

#endif /* l_character_h */
