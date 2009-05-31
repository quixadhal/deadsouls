#include SECRETS_H
#include <daemons.h>
#include <commands.h>
#include <lib.h>

inherit LIB_DAEMON;
mapping Secrets = ([]);
string secfile;

void create(){
    Secrets = ([
            "IRN_PASSWORD" : IRN_PASSWORD,
            "IRN_PASSWORD1" : IRN_PASSWORD1,
            "IRN_PASSWORD2" : IRN_PASSWORD2,
            "IRN_PASSWORD3" : IRN_PASSWORD3,
            "IMC2_CLIENT_PW" : IMC2_CLIENT_PW,
            "IMC2_SERVER_PW" : IMC2_SERVER_PW,
            "I3_SERVER_PW" : I3_SERVER_PW,
            "INSTANCE_PW" : INSTANCE_PW,
            ]);
}

static varargs void ModSecret(string secret, mixed val){
    string type;
    string *secarr, *ret = ({});

    secfile = unguarded( (: read_file("/secure/include/secrets.h") :) );
    secarr = explode(secfile, "\n");
    if(!undefinedp(Secrets[secret])){
        type = typeof(Secrets[secret]);
    }
    foreach(string line in secarr){
        if(!sizeof(line)) continue;
        if(!strsrch(line, "#define "+secret+" ")){
            if(type && type == "string"){
                line = "#define "+secret+" \""+val+"\"";
                Secrets[secret] = val;
            }
            else {
                line = "#define "+secret+" "+to_int(val);
                Secrets[secret] = to_int(val);
            }
        }
        ret += ({ trim(line) });
    }
    secfile = implode(ret, "\n") + "\n";
    unguarded( (: write_file("/secure/include/secrets.h", secfile, 1) :) );
}

mixed GetSecret(string secret){
    string prev = base_name(previous_object());
    if( (master()->valid_apply(({ "SECURE" }))) ){
        return copy(Secrets[secret]);
    }
    if(!strsrch(secret, "IRN_PASSWORD") && prev == ROUTER_D){
        return copy(Secrets[secret]);
    }
    if(!strsrch(secret, "IMC2_") && prev == IMC2_D){
        return copy(Secrets[secret]);
    }
    if(secret == "I3_SERVER_PW" && prev == INTERMUD_D){
        return copy(Secrets[secret]);
    }
    if(secret == "INSTANCE_PW" && prev == INSTANCES_D){
        return copy(Secrets[secret]);
    }
    return 0;
}

mixed SetSecret(string secret, mixed val){
    string prev = base_name(previous_object());
    if( (master()->valid_apply(({ "SECURE" }))) ){
        ModSecret(secret, val);
        return copy(Secrets[secret]);
    }
    if(!strsrch(secret, "IRN_PASSWORD") && prev == ROUTER_D){
        ModSecret(secret, val);
        return copy(Secrets[secret]);
    }
    if(!strsrch(secret, "IMC2_") && (prev == IMC2_D || prev == CMD_MUDCONFIG)){
        ModSecret(secret, val);
        return copy(Secrets[secret]);
    }
    if(secret == "I3_SERVER_PW" && prev == INTERMUD_D){
        ModSecret(secret, val);
        return copy(Secrets[secret]);
    }
    if(secret == "INSTANCE_PW" && prev == INSTANCES_D){
        ModSecret(secret, val);
        return copy(Secrets[secret]);
    }
    return 0;
}




