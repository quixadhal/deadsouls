#ifndef s_vendor_types_h
#define s_vendor_types_h

#define VT_TREASURE    (1 << 1)
#define VT_WEAPON      (1 << 2)
#define VT_ARMOR       (1 << 3)
#define VT_LIGHT       (1 << 4)
#define VT_FOOD        (1 << 5)
#define VT_DRINK       (1 << 6)
#define VT_MAGIC       (1 << 7)
#define VT_FISHING     (1 << 8)
#define VT_PLANT       (1 << 9)
#define VT_HERB        (1 << 10)
#define VT_CONTRABAND  (1 << 11)

#define MAX_VENDOR_BIT VT_CONTRABAND

#define VT_ALL        VT_TREASURE | VT_WEAPON | VT_ARMOR | VT_LIGHT | VT_FOOD | VT_DRINK | VT_MAGIC | VT_FISHING | VT_PLANT | VT_HERB | VT_CONTRABAND

#endif /* s_vendor_types_h */
