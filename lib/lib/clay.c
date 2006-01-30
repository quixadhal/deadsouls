#include <lib.h>
inherit LIB_STORAGE;
inherit LIB_SURFACE;

private static string Composition, Name, ShortDesc, Desc;
private static string *Nouns, *Adjectives;

varargs string SetComposition(string comp, string name, string *nouns, string *adjs, string
  short, string desc);
void InitComposition();

void create(){
    string *tmpsave1, *tmpsave2, *tmpsave3, *tmpsave4;
    surface::create();
    storage::create();
    tmpsave1 = storage::GetSave(); 
    tmpsave2 = surface::GetSave(); 
    tmpsave3 = ({ "Composition","Name","Nouns","ShortDesc", "Desc","Adjectives" });
    tmpsave4 = tmpsave1 + tmpsave2 + tmpsave3;
    //tc("AddSave: "+identify(tmpsave4));
    AddSave( tmpsave4 );
    SetKeyName("lump");
    SetId( ({"lump","clay","mass"}) );
    SetAdjectives( ({"lump of","clay","mass of","heavy", "shapeless"}) );
    SetShort("a lump of clay");
    SetLong("This is a heavy, shapeless mass of clay.");
    call_out( (: InitComposition :), 0);
    SetNoCondition(1);
    SetCanClose(0);
    SetMaxCarry(200);
    SetMass(200);
}

void InitComposition(){
    //tc("made it to initcomp.","red");
    SetComposition(Composition, Name, Nouns, Adjectives, ShortDesc, Desc);
}

varargs string SetComposition(string comp, string name, string *nouns, string *adjs, string short, string desc){
    string tmp;
    //tc("made it to setcomp.","blue");

    if(name) {
	Name = name;
	SetKeyName(name);
    }

    if(comp) Composition = comp;

    if(comp && !Desc){
	tmp = replace_string(GetLong(),"clay",comp);
	SetLong(tmp);
    }

    if(comp && !ShortDesc){
	tmp = replace_string(GetShort(),"clay",comp);
	SetShort(tmp);
    }

    if(sizeof(nouns)){
	Nouns = nouns;
	SetId(nouns);
    }

    if(desc){
	Desc = desc;
	SetLong(desc);
    }

    if(short){
	ShortDesc = short;
	SetShort(short);
    }

    if(sizeof(adjs)){
	Adjectives = adjs;
	SetAdjectives(adjs);
    }

    return Composition;
}


void init(){
    ::init();
}
