/*    /lib/virtual/virt_map.c
 *    From DeadSouls Object Library
 *    Provides a map for map based virtual rooms
 *    Created by Dvarsk 980310
 *    Version: @(#) virt_map.c 1.3@(#)
 *    Last modified: 98/08/25
 */      

#include <lib.h>

inherit LIB_DAEMON;

private mapping Location=([]);
private mixed AreaMap;
private string Virt_Room= "";

/******************* virt_map.c required overrides ************/

varargs string array BaseMap(){ //override with actual map
	return 
		({
	//   0
    	//   012
          ({"000",//0
    	    "010",//1
      	    "000",//2
      	    })});
}

/******************* virt_map.c attributes ********************/

int *GetLocations(string str){return Location[str]; }

varargs mixed GetAreaMap(int x, int y, int z){ 
	if(nullp(x) || nullp(y) || nullp(z)) return copy(AreaMap);
	if(	   x<0 
		|| y<0 
		|| z<0
		|| z>sizeof(AreaMap)
		|| y>sizeof(AreaMap[0])
		|| x>sizeof(AreaMap[0][0]))
			return "0";
	return copy(AreaMap[z][y][x..x]);

}

varargs mixed SetAreaMap(int y,int x ,int z, string str){
	if(nullp(x)||nullp(y)||nullp(z)){
		string x_line="";
	    string * y_line=({});
		AreaMap=BaseMap();
		z=sizeof(AreaMap);
		y=sizeof(AreaMap[0]);
		x=sizeof(AreaMap[0][0]);
		while (x --) x_line += "0";
		while (y --) y_line += ({x_line});
		return AreaMap = ({y_line}) +AreaMap + ({y_line});
	}
	else return AreaMap[z][y][x..x]=str;
}

int SetVirtFile( string file){
    string str;
    if( str = catch(call_other(file, "???")) ) {
		if( creatorp() ) message("error", str, previous_object());
                return 0;
	}
	Virt_Room=file+"/";
	SetAreaMap();
	return 1;
}

/******************* virt_map.c events ************************/

int * RecurseLocations(){
	mixed map=GetAreaMap();
	int *locat;
	int z=random(sizeof(map)-2)+1;
	int y=random(sizeof(map[z]));
	int x=random(sizeof(map[z][y]));

	if(map[z][y][x..x]!="1") 
        return RecurseLocations(); // Is it within the general land?
	foreach(string tmp in keys(Location)){ //Is something else here?
		locat=Location[tmp];
		if(locat[0]==x && locat[1]==y && locat[2]==z) 
            return RecurseLocations(); 
	}
	return ({x,y,z});
}

varargs void InitializeLocations(string tmp){
	if(!nullp(tmp)){
		if(!Location[tmp][0] && !Location[tmp][1] 
            && !Location[tmp][2])
			Location[tmp]=RecurseLocations(); 
		SetAreaMap(Location[tmp][1], Location[tmp][0], 
            Location[tmp][2], tmp);
		(Virt_Room+(Location[tmp][0] ) + "," 
            + (Location[tmp][1]) + ","
			+ (Location[tmp][2]))->ResetLocation();
    }
    else foreach(tmp in keys(Location)){
        Location[tmp]=RecurseLocations();
        SetAreaMap(Location[tmp][1], Location[tmp][0],
            Location[tmp][2], tmp);
		(Virt_Room+(Location[tmp][0] ) + "," + (Location[tmp][1])
            + "," +(Location[tmp][2]))->ResetLocation(); 
	}

}

// call this if all the locations randomly move around
void SwitchLocations(){
	int * old_location;
	mixed areas=BaseMap();

	foreach(string tmp in keys(Location)){
		if(Location[tmp][3]) continue;
		old_location=copy(Location[tmp]);
		Location[tmp]=({0,0,0}); 
		SetAreaMap(old_location[1], old_location[0], 
            old_location[2], areas[(old_location[2]-1)]
            [old_location[1]][old_location[0]..old_location[0]]);
		Location[tmp]=RecurseLocations();
		(Virt_Room + (old_location[0]) + ","
			+ (old_location[1]) + "," 
            + (old_location[2]))->ResetLocation();
		SetAreaMap(Location[tmp][1], Location[tmp][0],
            old_location[2], tmp);
		(Virt_Room + (Location[tmp][0] ) + ","
			+ (Location[tmp][1]) + "," 
            + (old_location[2]))->ResetLocation();

	}
}

/******************* virt_map.c driver applies ****************/

varargs static void create(string virt_file) {
    daemon::create();
    SetNoClean(1);
    if(!nullp(virt_file)) SetVirtFile(virt_file);
    else SetVirtFile(base_name(this_object()));
}

/******************* virt_map.c optional settings *************/

// AddLocation(string str, int array where)
// string = unique identifier
// where==({0,0,0}) will randomly place the location

int AddLocation(string str, int array where){ 
	if(!nullp(Location[str])) return 0;
	Location[str]=where; 
	InitializeLocations(str);
	return 1;
}

int RemoveLocation(string str){ 
	int * location=Location[str];
	mixed areas=BaseMap();
	if(nullp(location)) return 0;
	SetAreaMap(location[1],location[0],location[2],
			areas[(location[2]-1)][location[1]]
            [location[0]..location[0]]);
	(Virt_Room + (location[0] ) + ","
			+ (location[1]) + "," 
            + (location[2]))->ResetLocation();
	map_delete(Location,str);
	return 1;
}



