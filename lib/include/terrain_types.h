#ifndef __TERRAIN_TYPES_H__
#define __TERRAIN_TYPES_H__

#define T_CUSTOM            (1 << 0)
#define T_OUTDOORS          (1 << 1)
#define T_INDOORS           (1 << 2)
#define T_ROAD              (1 << 3)
#define T_UNDERWATER        (1 << 4)
#define T_SURFACE           (1 << 5)
#define T_MIDAIR            (1 << 6)
#define T_SWAMP             (1 << 7)
#define T_WOODS             (1 << 8)
#define T_JUNGLE            (1 << 9)
#define T_ROUGH             (1 << 10)
#define T_UNDERGROUND       (1 << 11)
#define T_SPACE             (1 << 12)
#define T_MAGMA             (1 << 13)
#define T_PLASMA            (1 << 14)
#define T_PLANAR            (1 << 15)
#define T_SNOW              (1 << 16)
#define T_SAND              (1 << 17)
#define T_ICE               (1 << 18)
#define T_BIOLOGICAL        (1 << 19)
#define T_SEAFLOOR          (1 << 20)
#define T_ALL_OUTDOORS_LAND T_OUTDOORS | T_ROAD | T_SWAMP | T_WOODS | T_JUNGLE | T_ROUGH | T_SNOW | T_SAND | T_ICE
#define T_ALL_SEA           T_SEAFLOOR | T_UNDERWATER | T_SURFACE

#define T_MAX_TERRAIN_BIT    T_SEAFLOOR

#endif /* __TERRAIN_TYPES_H__ */
