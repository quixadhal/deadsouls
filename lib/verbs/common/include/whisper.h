#ifndef l_whisper_h
#define l_whisper_h

static void create();
mixed can_whisper();
mixed can_whisper_to_liv(object ob);
mixed can_whisper_in_wrd(string wrd);
mixed can_whisper_in_wrd_to_liv(string wrd, object ob);
mixed can_whisper_to_liv_str(object targ, string str);
mixed can_whisper_in_wrd_to_liv_str(string wrd, object targ, string str);
mixed can_whisper_to_liv_in_wrd_str(object targ, string lang, string str);
mixed do_whisper();
mixed do_whisper_to_liv(object ob);
mixed do_whisper_in_wrd(string wrd);
mixed do_whisper_in_wrd_to_liv(string wrd, object ob);
mixed do_whisper_to_liv_str(object targ, string str);
mixed do_whisper_in_wrd_to_liv_str(string lang, object targ, string str);
mixed do_whisper_to_liv_in_wrd_str(object targ, string lang, string str);
string GetHelp(string str);

#endif /* l_whisper_h */
