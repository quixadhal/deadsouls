#include <lib.h>
#include <dirs.h>
#include <virtual.h>

inherit LIB_VIRT_MAP;

varargs string array BaseMap(){ //override with actual map
    return 
    ({
      //   0
      // 01234
      ({"00000",//0
	"01000",//1
	"00000",//2
	"00000",//3
	"00000",//4
      })
    });

}


varargs static void create() {
    ::create();
    //SetNoClean(1);
    //SetVirtFile(base_name(this_object()));
    //AddLocation("foo", ({2,2,0}) );
}


