#include <lib.h>
#include <dirs.h>

int compare_array(mixed *arr1, mixed *arr2){
    mixed *temparr1 = copy(arr1);
    mixed *temparr2 = copy(arr2);

    if(typeof(arr1) != typeof(arr2)) return 0;

    if(sizeof(temparr1) != sizeof(temparr2)) return 0;

    foreach(mixed item in temparr1){
        if(member_array(item, temparr2) == -1) return 0;
    }

    foreach(mixed item2 in temparr2){
        if(member_array(item2, temparr1) == -1) return 0;
    }

    return 1;
}

int arrcmp(mixed *arr1, mixed *arr2){
    return compare_array(arr1, arr2);
}

