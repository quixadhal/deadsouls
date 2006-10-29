#ifndef s_lpu_h
#define s_lpu_h

#define ROOM				"/lib/std/room"
#define DOORS				"/lib/blank"
#define DAEMON				"/lib/std/daemon"
#define OBJECT				"/lib/std/item"
#define OB_SIMUL_EFUN			"/secure/sefun/sefun"
#define OB_DEED                         "/obj/deed"
#define OB_ORDER                        "/obj/order"
#define OB_ESTATE                       "/std/estate"
#define OB_POSTAL                       DIR_SECURE_OBJ "/post"
#define ACCESS                          "/secure/lib/std/access"

#define set_light 			SetAmbientLight
#define set_short			SetShort
#define set_long			SetLong
#define set_items			SetItems
#define set_exits			SetExits
#define query_exit			GetExit
#define set_property			SetProperty
#define set_name			SetKeyName
#define set_id				SetId
#define set_no_clean			SetNoClean
#define set_smell_string		SetSmell
#define set_smell			SetSmell
#define set_properties			SetProperties
#define set_door			SetDoor
#define set_race			SetRace
#define set_gender			SetGender
#define set_level			SetLevel
#define set_body_type			SetProperty
#define set_hp				SetHealthPoints
#define set_class			SetClass
#define set_spell_chance		SetSpellChance
#define set_spell			SetSpell
#define set_skills			SetSkills
#define set_skill			SetSkill
#define set_spell_chance		SetSpellChance
#define set_emotes			SetActions
#define set_wielding_limbs		SetWieldingLimbs
#define set_wc				SetClass
#define set_type			SetWeaponType
#define set_mass			SetMass
#define set_value			SetValue
#define set_listen_string		SetListen
#define set_listen 			SetListen
#define set_edit_ok			valid_edit	
#define set_search			SetSearch
#define set_paralyzed			SetParalyzed
#define set_pre_exit_functions		SetProperty
#define MONSTER				LIB_SENTIENT
#define monster				sentient
#define set_money			SetCurrency
#define add_exit			AddExit
#define set_read			SetRead
#define set_prevent_get			SetPreventGet
#define VAULT				LIB_ROOM
#define set_day_long			SetDayLong
#define set_night_long			SetNightLong
#define set_open			SetOpen
#define query_open			GetOpen
#define remove_item_description		RemoveItem
#define remove_exit			RemoveExit
#define add_item_description		AddItem
#define set_outside                     SetTown
#define create_door                     CreateDoor
#define set_quest_points		SetQuestPoints
#define query_quest_points		GetQuestPoints
#define query_class                     GetClass

#endif /* s_lpu_h */
