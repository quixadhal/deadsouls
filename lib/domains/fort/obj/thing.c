#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
string *TestArray = ({"one"});

void create(){
    ::create();
    TestArray += ({ "two" });

    SetKeyName("generic thing");
    SetId( ({"thing","item","thang","dingus"}) );
    SetAdjectives( ({"generic","sample","template"}) );
    SetShort("a generic thing");
    SetLong("This is an object of indeterminate nature and proportions.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
}

string *SetFun(mixed foo){
    if(!foo || !sizeof(foo)) return TestArray;
    if(stringp(foo)) foo = ({ foo });
    TestArray += foo;
    return TestArray;
}

void init(){
    ::init();
    add_action("do_it","sscan");
}

int test_array(string str){
    if(member_array(str, this_player()->GetSkills()) ) return 1;
    else return 0;
}

int do_it(string str){
    float foo;
    float bar;
    float baz;

    bar = -7;
    baz =  -10;
    foo = (bar / baz);
    write("foo: "+foo);

}

