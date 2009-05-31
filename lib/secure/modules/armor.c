#include <lib.h>
#include <modules.h>
#include <daemons.h>
#include <commands.h>
#include ROOMS_H
#include <armor_types.h>
#include <damage_types.h>


int GetProts(string str);
int eventProcessValues();
int eventStartQuery(string str);
int eventReceiveValue(string str);
mixed eventReadProtectionSettings(string str);
varargs int eventModifyProtections(mapping Protecciones, string filename, object ob);

string globaltmp, globalstr, protections, current;
object armor_item;
string *prot_array = ({});
mapping ProtectionsMap = ([]);

string array Protections = ({ "BLUNT" ,"BLADE", "KNIFE", "WATER", "SHOCK", "COLD", "HEAT", "GAS", "ACID", "MAGIC", "POISON", "DISEASE", "TRAUMA" });


varargs int eventStartArmorQuestions(string what, object ob){
    string filename;
    mixed tempvar;

    armor_item = ob;
    filename = base_name(ob)+".c";

    if(file_exists(filename)) tempvar = eventReadProtectionSettings(filename);
    if(tempvar && mapp(tempvar)) ProtectionsMap = tempvar;
    else ProtectionsMap = ([]); 

    protections = "";
    prot_array = ({});
    if(what && what != "") prot_array = ({ what });
    foreach(string prot in Protections) {
        protections += lower_case(prot)+", ";
    }

    truncate(protections,2);
    protections += ".";

    write("Your armor can protect against one or more of the following types of damage: ");
    write(protections+".");
    write("\nPlease enter which ones your armor should protect from, one at a time. ");
    write("When you are done, please type a dot on a blank line.");
    if(sizeof(prot_array))  write("So far, we have: "+identify(prot_array));

    input_to( (: GetProts :) );
    return 1;
}

int GetProts(string str){
    string *temp_array;
    if(!str || str == "" || str == "."){
        prot_array -= ({0});
        if(sizeof(prot_array)) foreach(string element in prot_array){
            if(member_array(upper_case(element),Protections) == -1) prot_array -= ({ element });
        }

        if(!sizeof(prot_array)){
            write("Modification cancelled.");
            return 1;
        }

        else {
            write("Protections list complete.");
            eventProcessValues();
            return 1;
        }
    }

    if(grepp(str," ")){
        temp_array = explode(str," ");
        prot_array += temp_array;
    }
    else prot_array += ({ str });

    if(sizeof(prot_array))	write("You may now enter the next value. So far, we have: "+identify(prot_array));
    else write("You may now enter the next value. So far, it is blank.");
    write("If you're done entering values, enter a dot on a blank line.");

    input_to( (: GetProts :) );
    return 1;
}

int eventProcessValues(){
    if(sizeof(prot_array)){
        current = upper_case(prot_array[0]);
        prot_array -= ({ prot_array[0] });
        eventStartQuery(current);
        return 1;
    }
    eventModifyProtections(ProtectionsMap, base_name(armor_item)+".c", armor_item);
    return 1;
}

int eventStartQuery(string str){
    write("Please enter the protection value for: "+str);
    input_to( (: eventReceiveValue :) );
    return 1;
}

int eventReceiveValue(string str){
    int val;
    if(!sscanf(str,"%d",val)) {
        write("Please enter an integer value.");
        eventStartQuery(current);
        return 1;
    }

    else {
        ProtectionsMap[current] = val;
        eventProcessValues();
        return 1;
    }
}

mixed eventReadProtectionSettings(string str){
    string prot, num, s1, junk;
    int val;
    string *temp_array = ({});
    string *fun_array = ({});
    string tmp_str = "";
    if(file_exists(str+".c")) str += ".c";
    if(file_exists(str)) {
        if(!check_privs(this_player(),str)) {
            write("You lack sufficient privileges for this.");
            return 1;
        }
        else {
            globalstr = str;
            unguarded( (: globalstr = read_file(globalstr) :) );
        }
        if(globalstr && globalstr != ""){
            str = globalstr;
            globalstr = "";
        }
    }

    fun_array = this_object()->eventReadFunctions(str);   
    if(!sizeof(fun_array)) {
        write("Unknown error.");
        return 1;
    }
    foreach(string element in fun_array){
        if(grepp(element,"SetProtection(")) tmp_str +=  element ;
                }

                temp_array = explode(tmp_str,"\n");
                if(!sizeof(temp_array)){
                return ([]);
                }
                else {
                foreach(string element in temp_array){
                if(sscanf(element,"SetProtection(%s)%s",s1,junk) == 2){
                sscanf(s1,"%s,%s",prot,num);
                num = trim(num);
                prot = trim(prot);
                sscanf(num,"%d",val);
                ProtectionsMap[prot] = val;
                }
                }
                }

                return ProtectionsMap;
                }

varargs int eventModifyProtections(mapping Protecciones, string filename, object ob){
    string new_lines;

    if(!check_privs(this_player(),filename)){
        write("You do not have sufficient privileges to perform this action.");
        return 1;
    }
    globaltmp = filename;
    unguarded( (: globalstr = read_file(globaltmp) :) );

    globalstr = remove_matching_line(globalstr, "SetProtection" , 1);
    globaltmp = generate_tmp();
    new_lines = "\n";
    foreach(string key, string val in ProtectionsMap){
        new_lines += "SetProtection("+key+", "+val+");\n";
    }
    globalstr = this_object()->eventAppend(globalstr,({"SetArmorType","SetRestrictLimbs","SetLong"}),new_lines);
    unguarded( (: write_file(globaltmp, globalstr,1) :) );
    this_object()->eventGeneralStuff(globaltmp);
    globalstr = filename;
    unguarded( (: cp(globaltmp, globalstr) :) );
    rm(globaltmp);
    if(ob && grepp(filename,base_name(ob))){
        reload(ob);
    }
    return 1;
}
