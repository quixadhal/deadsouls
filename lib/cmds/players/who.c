#include <lib.h>
#include <config.h>
 
inherit LIB_DAEMON;
 
#define Colours ({\
"%^MAGENTA%^",\
"%^BOLD%^%^MAGENTA%^",\
"%^BOLD%^%^CYAN%^",\
"%^CYAN%^",\
"%^BLUE%^",\
"%^BOLD%^%^BLUE%^",\
})
 
#define Position ({\
"arch",\
"creator",\
"newbie",\
"mortal",\
"high mortal",\
"avatar",\
})
 
string AddName(object ob, int x) {
    string str;
    if( query_idle(ob) > 60 ) str = "(idle)  ";
    else if( in_input(ob) || in_edit(ob) ) str = "(edit)  ";
    else str = "        ";
    if( x < 0 || x > 5 ) x = 0;
    return (Colours[x] + str + strip_colours((string)ob->GetShort()) + "%^RESET%^");
}
 
mixed cmd(string args) {
    string *lines;
    mixed group;
    object *avatars, *hms, *norms, *newbies, *adms, *cres, *obs;
    int x, colour, cnt, *screen;
 
    x = sizeof(obs = filter(users(), (: $1->GetName() &&
                     !((int)$1->GetInvis(this_player())) :)));
    obs = obs - (avatars = filter(obs, (: avatarp :)));
    obs = obs - (hms = filter(obs, (: high_mortalp :)));
    obs = obs - (adms = filter(obs, (: archp :)));
    obs = obs - (cres = filter(obs, (: creatorp :)));
    newbies = obs -
      (norms = filter(obs, (: (int)$1->GetLevel() > MAX_NEWBIE_LEVEL :)));
    screen = (int *)this_player()->GetScreen()[0];
    colour = sizeof(Colours) - 1;
    cnt = -1;
    lines = ({});
    foreach(group in ({ adms, cres, newbies, norms, hms, avatars }) ) {
      int i;
      string *tmp;
      cnt++;
      if( !i = sizeof(group) ) continue;
      tmp = ({ capitalize(consolidate(i, Position[cnt])) + ":" });
      while(i--) tmp += ({ AddName(group[i], colour) });
      colour--;
      lines = tmp + ({ "" }) + lines;
    }
    lines = ({ "%^BOLD%^%^BLUE%^" + center(mud_name(), screen),
               center("There are " + x + " members of our reality!", screen),
               "%^RESET%^"  }) + lines;
    this_player()->eventPage(lines, "info");
    return 1;
}
 
