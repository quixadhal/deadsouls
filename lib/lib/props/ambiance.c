/*    /lib/props/ambiance.c
 *    From the Dead Souls LPC Library
 *    Ambiance is natural light in an area
 *    Created by Descartes of Borg 961221
 *    Version: @(#) ambiance.c 1.1@(#)
 *    Last modified: 96/12/22
 */

private int AmbientLight = 0;

int GetAmbientLight(){
    return AmbientLight;
}

int SetAmbientLight(int x){
    return (AmbientLight = x);
}
