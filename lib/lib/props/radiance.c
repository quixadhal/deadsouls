/*    /lib/props/radiance.c
 *    From the Dead Souls LPC Library
 *    Radiance is the capacity of one object to radiate light
 *    Created by Descartes of Borg 961221
 *    Version: @(#) radiance.c 1.1@(#)
 *    Last modified: 96/12/22
 */

private static int RadiantLight = 0;

int GetRadiantLight(int ambient){
    return RadiantLight;
}

int SetRadiantLight(int x){
    return (RadiantLight = x);
}

int GetBaseRadiance(int foo){
    return RadiantLight;
}
