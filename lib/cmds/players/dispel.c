#include <lib.h>
#include <cfg.h>
#include <magic_protection.h>
#include <function.h>

inherit LIB_DAEMON;


mixed cmd(string args) {

    if(!args){
        write("Dispel what?");
        return 1;
    }
    if(args == "meditate"){
        object mojo = present("meditate mojo", this_player());
        if(!mojo){
            write("You have no meditation mojo to dispel.");
            return 1;
        }
        write("You dispel the meditation mojo from yourself.");
        mojo->eventDispel();
    }
    if(args == "whip"){
        object whip = present_file("/obj/whip", this_player());
        if(!whip){
            write("You have no whip to dispel.");
            return 1;
        }
        write("You dispel the whip from yourself.");
        whip->eventDispel();
    }
    if(args == "buffer"){
        class MagicProtection *Protections;
        Protections = this_player()->GetMagicProtection();
        foreach(class MagicProtection tmp in Protections){
            if(!tmp->obname) continue;
            if(tmp->obname == "/powers/spells/buffer"){
                if(!(functionp(tmp->hit) & FP_OWNER_DESTED)){
                    write("You dispel your buffer.");
                    this_player()->RemoveMagicProtection("/powers/spells/buffer");
                    return 1;
                }
            }
        }
        write("You have no buffer to dispel.");
        return 1;
    }
    if(args == "greater buffer"){
        class MagicProtection *Protections;
        Protections = this_player()->GetMagicProtection();
        foreach(class MagicProtection tmp in Protections){
            if(!tmp->obname) continue;
            if(tmp->obname == "/powers/spells/greater_buffer"){
                if(!(functionp(tmp->hit) & FP_OWNER_DESTED)){
                    write("You dispel your greater buffer.");
                    this_player()->RemoveMagicProtection("/powers/spells/greater_buffer");
                    return 1;
                }
            }
        }
        write("You have no greater buffer to dispel.");
        return 1;
    }
    return 1;
}


string GetHelp(){
    return ("Syntax: dispel <spellname>\n\n"
            "Dismisses a given magical effect, if appropriate.\n"
            "\nSee also: cast");
}
