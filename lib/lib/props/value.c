/*    /lib/props/value.c
 *    From the Dead Souls V Object Library
 *    Basic value attributes
 *    Created by Descartes of Borg 970101
 *    Version: @(#) value.c 1.1@(#)
 *    Last modified: 97/01/01
 */

#include <vendor_types.h>

private int DestroyOnSell = 0;
private int Value         = 0;
private int VendorType    = VT_TREASURE;

int GetDestroyOnSell() {
    return DestroyOnSell;
}

int SetDestroyOnSell(int x) {
    if( x ) {
	DestroyOnSell = 1;
    }
    else {
	DestroyOnSell = 0;
    }
    return DestroyOnSell;
}

string array GetSave() {
    return ({ "Value" });
}

int GetValue() {
    return Value;
}
 
int SetValue(int x) {
    if( !intp(x) ) {
	error("Bad argument 1 to SetValue().\n\tExpected: int, Got: " +
	      typeof(x) + "\n");
    }
    else {
	return (Value = x);
    }
}

int GetVendorType() {
    return VendorType;
}

int SetVendorType(int x) {
    if( !intp(x) ) {
	error("Bad argument 1 to SetVendorType().\n\tExpected: int, Got: " +
	      typeof(x) + "\n");
    }
    if( !(x & VT_ALL) ) {
	error("Invalid vendor type.\n");
    }
    return (VendorType = x);
}

int CanSell() {
    return 1;
}
