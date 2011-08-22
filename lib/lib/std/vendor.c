#include <lib.h>
#include <armor_types.h>
#include <vendor_types.h>
#include <daemons.h>
#include "/lib/include/vendor.h"

inherit LIB_SENTIENT;
inherit LIB_MANYCOINS;

private static int MaxItems, VendorType, bargain;
private static string StorageRoom, LocalCurrency;
private static mapping Costs, Values;

static void create(){
    sentient::create();
    Costs = ([]);
    Values = ([]);
    LocalCurrency = "gold";
    VendorType = VT_TREASURE;
    MaxItems = 40;
}

mixed direct_buy_str_from_liv(string str){ 
    return CanSell(this_player(), str); 
}

int indirect_sell_obj_to_liv(){ return 1; }

int indirect_sell_obs_to_liv(){ return 1; }

int CanCarry(int amount){ return 1; }

    mixed CanBuy(object who, object *obs){
        if( !load_object(StorageRoom) ) 
            return "There is a bug with the vendor's storage, please report it.";
        return 1;
    }

    mixed CanSell(object who, string what){
        if( !load_object(StorageRoom) ) 
            return "There is a bug with the vendor's storage, please report it.";
        return 1;
    }

mixed eventBuy(object who, object *obs){
    object *tmp;
    object sroom, ob;
    int cost;

    sroom = load_object(StorageRoom);
    if( sizeof(all_inventory(sroom)) > GetMaxItems() ){
        eventForce("say I am having trouble getting rid of the things I "
                "have to sell right now.");
        return 1;
    }
    if( !sizeof(obs) ){
        eventForce("say you have nothing to sell!");
        return 1;
    }
    tmp = ({});
    foreach(ob in obs){
        int value;

        if( !(ob->GetShort()) ) continue;
        if( !(ob->GetVendorType() & GetVendorType()) ){
            eventForce("say I do not buy things like " +
                    ob->GetShort());
            continue;
        }
        cost = to_int(ob->GetBaseCost(GetLocalCurrency()));
        value = to_int(ceil(GetValue(ob, who)));
        if(ob->GetValue()) bargain = 1;
        else bargain = 0;
        if(!bargain) value = cost;

        if( !cost || cost < 1 || !value || value < 1){
            eventForce("say " + ob->GetShort() + " is worthless!");
            continue;
        }
        cost = cost / 2;
        value = value / 2;
        if(!cost) cost = 1;
        if(!value) value = 1;
        if( !(ob->CanSell(who)) ){
            eventForce("say You cannot sell " + ob->GetShort() +".");
            continue;
        }
        if( sizeof(filter(all_inventory(sroom),
                        (: $1->GetShort() == $(ob)->GetShort() :)))
                > 3 ){
            if( this_player()->AddCurrency(GetLocalCurrency(),value) == -1 ){
                eventForce("say you cannot carry "+value+" "+
                        GetLocalCurrency()+"!");
                ob->eventMove(environment());
                continue;
            }
            eventForce("say " + ob->GetShort() + "! Great!");
            tell_player(this_player(),GetShort()+" gives you "+value+
                    " "+GetLocalCurrency()+".");
            if(bargain) who->AddSkillPoints("bargaining", value*5);
            message("my_action", "You sell " + ob->GetShort() + ".", who);
            message("other_action", capitalize(who->GetKeyName()) + " sells " +
                    ob->GetShort() + ".", environment(),
                    ({ who, this_object() }));
            ob->eventDestruct();
            return 1;
        }
        eventForce("say " + ob->GetShort() + "! Excellent!");
        tell_player(this_player(),GetShort()+" pays you "+value+
                " "+GetLocalCurrency()+".");

        if( !(ob->eventMove(sroom)) ){
            eventForce("say I cannot seem to carry that");
            return 1;
        }
        if( this_player()->AddCurrency(GetLocalCurrency(), value) == -1 ){
            eventForce("say you cannot carry "+value+" "+
                    GetLocalCurrency()+"!");
            ob->eventMove(environment());
            continue;
        }
        if(bargain) who->AddSkillPoints("bargaining", value*5);
        tmp += ({ ob });
        message("my_action", "You sell " + ob->GetShort() + ".", who);
        message("other_action", capitalize(who->GetKeyName()) + " sells " +
                ob->GetShort() + ".", environment(),
                ({ who, this_object() }));
    }
    if( !sizeof(tmp) )
        eventForce("say I am sorry, " + capitalize(who->GetKeyName()) + ", "
                "that we could not come to a better agreement.");
    else map(tmp, function(object ob){
            if( ob->GetDestroyOnSell() )
            ob->eventDestruct();
            });
    return 1;
}

int cmdShow(object who, string args){
    object ob, sroom;
    int x;

    if( !args || args == "" ){
        eventForce("say show you what?");
        return 1;
    }
    if( !(sroom = load_object(StorageRoom)) ){
        eventForce("say I am having troubles right now");
        return 1;
    }
    if( x = to_int(args) ){
        object *obs;
        object *obs2;
        string *list2;
        int maxi;

        x--;
        obs = all_inventory(sroom);
        maxi = sizeof(obs);
        list2 = ({(obs[0]->GetKeyName())});
        for(int counter = 1;counter < maxi;++counter){
            if(member_array((obs[counter]->GetKeyName()),list2) < 0){
                list2 += ({ ( obs[counter]->GetKeyName()) });
            }
        }
        obs2 = ({ present(list2[0],sroom) });
        maxi = sizeof(list2);
        for(int counter = 1;counter < maxi;++counter){
            obs2 += ({ present(list2[counter],sroom) });
        }
        if((x >= 0) && (x < sizeof(obs2))){
            ob = obs2[x];
        }
    }
    else ob = present(args = lower_case(args), sroom);
    if( !ob ){
        eventForce("say I have no such thing to show you");
        return 1;
    }
    message("other_action", capitalize(GetKeyName())+" shows you "+
            ob->GetShort()+".", who);
    message("system", ob->GetLong(), who);
    return 1;
}

int cmdBrowse(object who, string args){
    object *obs;
    object *obs2;
    string *list;
    object sroom;
    int i, ii, maxi, number;

    if( !args || args == "" ) args = "all";
    if( !(sroom = load_object(StorageRoom)) ){
        eventForce("say I am having terrific difficulties today");
        return 1;
    }
    if(!(maxi = sizeof(obs = filter(all_inventory(sroom), (: !userp($1) && !$1->isDummy() :))))){
        eventForce("say I have nothing to sell right now.");
        return 1;
    }
    list = ({ "item #  Description                         Price ("+GetLocalCurrency()+")", "" });
    obs2 = ({});
    foreach(object tempob in obs){
        string *base_names = ({});
        foreach( object tempob2 in obs2 ){
            base_names += ({ base_name(tempob2) });
        }
        if(member_array(base_name(tempob), base_names) == -1){
            obs2 += ({ tempob });
        }
    }
    maxi = sizeof(obs2); 
    i = this_player()->GetScreen()[0];
    if(number = to_int(args)){
        if((number > 0) && (number <= maxi)){
            int tmp;
            while( i-- ) list[1] += "_";
            if(intp(number) && 
                    (tmp = to_int(ceil(GetCost(obs2[(number - 1)],who))))){
                list += ({ sprintf("%d      %:-35s %d", number, 
                            obs2[(number - 1)]->GetShort(), tmp) });
            }
            this_player()->eventPage(list);
            return 1;
        }
    }

    while( i-- ) list[1] += "_";
    for(ii=0; ii < maxi; ii++){
        int ok;
        int gat;

        ok = 0;
        gat = (obs2[ii]->GetArmorType());
        switch(args){
            case "all": ok = 1; break;
            case "weapon": case "weapons":
                break;
            case "armor": case "armors":
                if(obs2[ii]->GetVendorType() == 8){
                    ok = 1;
                }
            break;
            case "bag": case "bags":
                ok = obs2[ii]->GetProperty("bag");
            break;
            case "ring": case "rings":
                ok = gat & A_RING;
            break;
            case "glove": case "gloves":
                ok = gat & (A_GLOVE | A_LONG_GLOVE);
            break;
            case "boot": case "boots":
                ok = gat & (A_BOOT | A_LONG_BOOT);
            break;
            case "sock": case "socks":
                ok = gat & (A_SOCK | A_LONG_SOCK);
            break;
            case "helm": case "helmet":
                ok = gat & A_HELMET;
            break;
            case "visor": case "visors":
                ok = gat & A_VISOR;
            break;
            case "pants":
                ok = gat & A_PANTS;
            break;
            case "shirt": case "shirts":
                ok = gat & A_SHIRT;
            break;
            case "cloak": case "cloaks":
                ok = gat & A_CLOAK;
            break;
            case "belt": case "belts":
                ok = gat & A_BELT;
            break;
            case "vest": case "vests":
                ok = gat & A_VEST;
            break;
            case "shield": case "shields":
                ok = gat & A_SHIELD;
            break;
            case "body armor": case "body armors":
                ok = gat & A_BODY_ARMOR;
            break;
            case "blunt": case "knife": case "blade": case "projectile":
                case "blunts": case "knives": case "blades": case "projectiles":
                ok = (obs2[ii]->GetWeaponType() == args) ||
                (pluralize((obs2[ii]->GetWeaponType() || "")) == args);
            break;
            default:
            ok = obs2[ii]->id(args);
            break;
        }
        if( !ok ) continue;
        ok = GetCost(obs2[ii], who);
        if(!ii) ii = 0;
        list += ({ sprintf("%d      %:-35s %d", (ii+1), obs2[ii]->GetShort(), to_int(ok)) });
    }
    if( !sizeof(list) ){
        eventForce("frown");
        eventForce("say I have nothing like that to sell.");
        return 1;
    }
    this_player()->eventPage(list);
    return 1;
}

int cmdAppraise(object who, string args){
    object ob;
    int x,cost;

    if( !args || args == "" ){
        eventForce("say appraise what?");
        return 1;
    }
    if( !(ob = present(args = lower_case(args), who)) ){
        eventForce("say You have no such thing!");
        return 1;
    }
    if( !(ob->GetVendorType() & GetVendorType()) ){
        eventForce("say I have no use for " + ob->GetShort());
        return 1;
    }
    if( LocalCurrency != query_base_currency() ){
        cost=to_int(ob->GetBaseCost() / ECONOMY_D->__Query(LocalCurrency,"rate"));
    }
    else cost = ob->GetBaseCost();
    if(!cost || cost < 1){
        eventForce("say " + capitalize(who->GetKeyName()) + ", I will not buy "
                "that worthless thing from you.");
        return 1;
    }
    else x=cost/2;
    if(!x) x = 1;
    eventForce("say " + capitalize(who->GetKeyName()) + ", I will offer "
            "you " + x + " " + GetLocalCurrency() + " for " +
            ob->GetShort());
    return 1;
}

int cmdPrice(object who, string args){
    object *obs;
    string *list2;
    object *obs2;
    object ob, sroom;
    int x;
    int maxi;

    if( !args || args == "" ){
        eventForce("say price what?");
        return 1;
    }
    if( !(sroom = load_object(StorageRoom)) ){
        eventForce("say today is not really a good day for me");
        return 0;
    }
    obs = all_inventory(sroom);
    obs2 = ({});
    foreach(object tempob in obs){
        string *base_names = ({});
        foreach( object tempob2 in obs2 ) base_names += ({ base_name(tempob2) });
        if(!sizeof(obs2)){
            obs2 = ({tempob});
        }
        else if(member_array(base_name(tempob), base_names) == -1) obs2 += ({ tempob });
    }
    maxi = sizeof(obs2);
    if( x = to_int(args) ){
        list2 = ({(obs2[0]->GetKeyName())});
        for(int counter = 1;counter < maxi;++counter){
            if(member_array((obs2[counter]->GetKeyName()),list2) < 0){
                list2 += ({ ( obs2[counter]->GetKeyName()) });
            }
        }
        maxi = sizeof(list2);
        ob = obs2[x-1];
    }
    else ob = present(args = lower_case(args), sroom);
    if(!ob) foreach(object tempob in obs2) 
        if(answers_to(args,tempob)) ob = tempob;
    if( !ob ){
        eventForce("say I have no such thing!");
        return 1;
    }
    if( !(x = to_int(ceil(GetCost(ob, this_player())))) ){
        eventForce("say that thing has no value!");
        return 1;
    }
    eventForce("say " + capitalize(who->GetKeyName()) + ", I will take " +
            x + " " + GetLocalCurrency() + " for " +
            ob->GetShort());
    return 1;
}

mixed eventAsk(object who, string str){
    object *obs;
    object ob;
    string cmd, args, tmp;
    string lang, prof, orig = str;

    lang = who->GetDefaultLanguage();
    prof = who->GetLanguageLevel(lang);
    str = translate(str, prof);
    prof = this_object()->GetLanguageLevel(lang);
    str = translate(str, prof);

    if( !str || str == "" ){
        eventForce("say what do you want from me?");
        return 1;
    }
    if( sscanf(str, "%s %s", cmd, args) != 2 ){
        cmd = str;
        args = 0;
    }
    switch(cmd){
        case "appraise":
            return cmdAppraise(who, args);

        case "browse":
            return cmdBrowse(who, args);

        case "buy":
            if( str == "all" )
                obs = filter(all_inventory(who), (: $1->CanSell() :));
            else {
                if( !(ob = present(args, who)) ){
                    eventForce("say Get out of here you cheat!");
                    eventForce("bump " + this_player()->GetKeyName());
                    return 1;
                }
                obs = ({ ob });
            }
        return eventBuy(who, obs);

        case "price":
            return cmdPrice(who, args);

        case "sell":
            if( (tmp = CanSell(who, args)) == 1 ) return eventSell(who, args);
            else if( stringp(tmp) ) eventForce("say " + tmp);
            else {
                return 0;
            }

        case "show":
            return cmdShow(who, args);

        default:
        //Thx Raudhrskal
        if(!sentient::eventAsk(who,orig))
            eventForce("say I am not quite sure what you want from me");
    }
}

mixed eventSell(object who, mixed what){
    object ob, sroom;
    object *obs;
    string *list2;
    object *obs2;
    int cost,number,maxi;

    sroom = load_object(StorageRoom);
    obs = filter(all_inventory(sroom), (: !userp($1) :));
    obs2 = ({});
    foreach(object tempob in obs){
        string *base_names = ({});
        foreach( object tempob2 in obs2 ) base_names += ({ base_name(tempob2) });
        if(!sizeof(obs2)){
            obs2 = ({tempob});
        }
        else if(member_array(base_name(tempob), base_names) == -1) obs2 += ({ tempob });
    }
    maxi = sizeof(obs2);
    if(number = to_int(what)){
        number = number - 1;
        list2 = ({(obs2[0]->GetKeyName())});
        for(int counter = 1;counter < maxi;++counter){
            if(member_array((obs2[counter]->GetKeyName()),list2) < 0){
                list2 += ({ ( obs2[counter]->GetKeyName()) });
            }
        }
        maxi = sizeof(list2);
        if((number >= 0) && (number < sizeof(obs2))){
            ob = obs2[number];
        } else {
            eventForce("say I have nothing like that to sell to you.");
            return 1;
        }      
    } else {
        if( !(ob = present(what, sroom))) 
            foreach(object tempob in obs2) 
                if(answers_to(what,tempob)) ob = tempob;
        if(!ob || userp(ob) || ob->isDummy()){
            eventForce("say I have nothing like that to sell you.");
            return 1;
        }
    }
    cost=to_int(ob->GetBaseCost(GetLocalCurrency()));
    if(!cost || cost < 0){
        cost = 0; 
        cost = to_int(ob->GetValue());
    }
    if(who->GetCurrency(GetLocalCurrency()) < cost){
        eventForce("say You don't have enough "+GetLocalCurrency()+
                " to buy that.");
        return 1;
    }
    if( !(ob->eventMove(this_object())) ){
        message("error", "An error occurred moving the object, use bug -r.",
                who);
        return 1;
    }
    eventForce("say here is " + ob->GetShort() + " for " + to_int(cost) +
            " " + GetLocalCurrency() + "!");
    eventForce("give " + ob->GetUniqueId() + " to " + who->GetKeyName());
    if( environment(ob) == this_object() ){
        eventForce("say you cannot carry that!");
        eventForce("drop " + ob->GetKeyName());
    }
    if(bargain) who->AddSkillPoints("bargaining", random(to_int(floor(cost))));
    who->AddCurrency(GetLocalCurrency(), -cost);
    return 1;
}

int GetCost(object ob, object who){
    int x, mod,cost;

    cost=ob->GetBaseCost(LocalCurrency);
    if(cost && cost > 0) return cost;

    if( Costs[who] && Costs[who][ob] ) return Costs[who][ob];    
    x = ob->GetValue(GetLocalCurrency());
    mod = who->GetSkillLevel("bargaining") - GetSkillLevel("bargaining");
    if( mod < -90 ) mod = -90;
    if( mod > 90 ) mod = 90;
    x = (x * (100 - mod))/100;
    who->AddSkillPoints("bargaining", x);
    if( !Costs[who] ) Costs[who] = ([ ob : x ]);
    else Costs[who][ob] = x;
    return x;
}

int GetValue(object ob, object who){
    int x, mod;

    if( Values[who] && Values[who][ob] ) return Values[who][ob];
    x = ob->GetValue(GetLocalCurrency());
    mod = who->GetSkillLevel("bargaining") - GetSkillLevel("bargaining");
    if( mod < -90 ) mod = -90;
    if( mod > 90 ) mod = 90;
    x = (x * (100 - -mod)) / 100;
    who->AddSkillPoints("bargaining", x);
    if( !Values[who] ) Values[who] = ([ ob : x ]);
    else Values[who][ob] = x;
    return x;
}

string SetLocalCurrency(string str){ return (LocalCurrency = str); }

string GetLocalCurrency(){ return LocalCurrency; }

string SetStorageRoom(string room){ return (StorageRoom = room); }

string GetStorageRoom(){ return StorageRoom; }

int SetMaxItems(int x){ return (MaxItems = x); }

int GetMaxItems(){ return MaxItems; }

int SetVendorType(int x){ return (VendorType = x); }

int GetVendorType(){ return VendorType; }




