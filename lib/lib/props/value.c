/*    /lib/props/value.c
 *    From the Dead Souls Mud Library
 *    Basic value attributes
 *    Created by Descartes of Borg 970101
 *    Version: @(#) value.c 1.1@(#)
 *    Last modified: 97/01/01
 */

#include <vendor_types.h>
#include <daemons.h>

private int DestroyOnSell = 0;
private int Value         = 0;
private int Cost          = 0;
private int VendorType    = VT_TREASURE;

int GetDestroyOnSell(){
    return DestroyOnSell;
}

int SetDestroyOnSell(int x){
    if( x ){
        DestroyOnSell = 1;
    }
    else {
        DestroyOnSell = 0;
    }
    return DestroyOnSell;
}

string array GetSave(){
    return ({ "Value","Cost" });
}

varargs int GetValue(string str){
    if(estatep(this_object())) return 0;
    if(Cost && str && valid_currency(str)) return query_value(Cost,query_base_currency(),str);
    return Value;
}

int SetValue(mixed y){
    mixed x;
    if(arrayp(y)) return this_object()->SetBaseCost(y[1],y[0]); 
    else x = y;
    if( !intp(x) ){
        error("Bad argument 1 to SetValue().\n\tExpected: int, Got: " +
                typeof(x) + "\n");
    }
    else {
        return (Value = x);
    }
}

int GetVendorType(){
    return VendorType;
}

int SetVendorType(int x){
    if( !intp(x) ){
        error("Bad argument 1 to SetVendorType().\n\tExpected: int, Got: " +
                typeof(x) + "\n");
    }
    if( !(x & VT_ALL) ){
        error("Invalid vendor type.\n");
    }
    return (VendorType = x);
}

varargs mixed SetBaseCost(mixed arg, int i){
    float base, rate;

    if(intp(arg)) Cost = arg;
    else if(stringp(arg)){
        if(member_array(arg,ECONOMY_D->__QueryCurrencies()) == -1){
            return "Bad currency value to SetBaseCost.";
        }
        if(!i) return "Bad amount value to SetBaseCost.";

        base = query_base_rate();
        rate = ECONOMY_D->__Query(arg,"rate");

        Cost = i * rate;

    }
    return Cost;
}  

int GetBaseCost(string str){
    if(estatep(this_object())) return 0;
    if(!str)return Cost;

    if(member_array(str,ECONOMY_D->__QueryCurrencies()) == -1){
        return 0;
    }

    return query_value(Cost,query_base_currency(),str);
}

int SetDollarCost(int x){
    SetBaseCost("dollars", x);
    return Cost;
}
