//	/adm/simul_efun/base_name.c
//	from the Dead Souls mudlib
//	returns the name of the file of an object without extensions
//	based on the simul_efun originally by Truilkan@Basis
//	created by Descartes of Borg 22 december 1992

string base_name(mixed val) {
    string name, base;
    int borg;

    if(stringp(val)) name = val;
    else name = file_name(val);
    if(sscanf(name, "%s#%d", base, borg) ==2) return base;
    else return name;
}
