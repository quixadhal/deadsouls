#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string rep, flag, domain, person;
    string write_perms = read_file("/secure/cfg/write.cfg");
    string *tmp_array = explode(write_perms, "\n");
    string *admin_array = ({});
    mapping DomainsMap = ([]);


    if(!archp(previous_object()) || this_player()->GetForced()){
        return "No.";
    }

    if(!args || args == ""){
        write(this_object()->GetHelp());
        return 1;
    }

    if(sscanf(args,"%s %s %s", flag, domain, person) != 3){
        write(this_object()->GetHelp());
        return 1;
    }

    person = lower_case(person);

    if(!user_exists(person)){
        write("Invalid person.");
        return 1;
    }

    if(!directory_exists("/domains/"+domain)){
        write("Invalid domain.");
        return 1;
    }

    write_perms = replace_string(write_perms, "\n",";\n");

    foreach(string line in tmp_array){
        string where, admins;
        if(sscanf(line,"(/domains/%s/) %s", where, admins)){
            DomainsMap[where] = admins;
            if(sizeof(DomainsMap[domain])) admin_array = explode(DomainsMap[domain],":");
        }
    }

    if(flag == "-a"){

        if(member_array(person, admin_array) != -1){
            write("That person already administers that domain.");
            return 1;
        }

        else admin_array += ({ person });

        rep = "(/domains/"+domain+"/) "+implode(admin_array, ":");

        if(grepp(write_perms, "(/domains/"+domain+"/)")){
            write_perms = replace_matching_line(write_perms, "/domains/"+domain, rep);
            write_perms = replace_string(write_perms, ";\n","\n");
        }
        else{
            write_perms = replace_string(write_perms, ";\n","\n");
            write_perms = newline_trim(write_perms);
            write_perms += "\n(/domains/"+domain+"/) "+person+"\n";
        }
    }

    else if(flag == "-d"){
        if(member_array(person, admin_array) == -1){
            write("That person does not administer that domain.");
            return 1;
        }

        else admin_array -= ({ person });

        if(!sizeof(admin_array)){
            write_perms = remove_matching_line(write_perms, "(/domains/"+domain+"/)",1);
            write_perms = replace_string(write_perms, ";\n","\n");
        }
        else {
            rep = "(/domains/"+domain+"/) "+implode(admin_array, ":");
            write_perms  = replace_matching_line(write_perms, "/domains/"+domain, rep);
            write_perms = replace_string(write_perms, ";\n","\n");
        }
    }

    else {
        write("Invalid flag.");
        return 1;
    }

    write_file("/secure/cfg/write.cfg", write_perms, 1);
    update("/secure/daemon/master");
    write("Ok.");
    return 1;
}

string GetHelp(string args) {
    return ("Syntax: domainadmin [-a | -d] <domain> <person>\n\n"
            "This command manages the write permissions to a specified "
            "domain. For example, to give a creator named Snuffy write "
            "privileges to /domains/SnuffLand, you would type: "
            "\ndomainadmin -a SnuffLand snuffy\n"
            "To revoke those privileges:\n"
            "\domainadmin -d SnuffLand snuffy\n"
            "This permits the creator to use QCS within the domain's "
            "directories. "
            "Write privileges are listed in /secure/cfg/write.cfg\n"
            "See also: domaincreate, admintool");
}
