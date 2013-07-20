#ifndef __MOUTH_TYPES__
#define __MOUTH_TYPES__

#define MOUTH_NONE              (1 << 0)
#define MOUTH_LIPPED            (1 << 1)
#define MOUTH_TOOTHED           (1 << 2)
#define MOUTH_BEAKED            (1 << 3)
#define MOUTH_MANDIBLED         (1 << 4)
#define MOUTH_BALEEN            (1 << 5)
#define MOUTH_TONGUED           (1 << 6)
#define MOUTH_FANGED            (1 << 7)
#define MOUTH_VENOMOUS          (1 << 8)
#define MOUTH_DISEASED          (1 << 9)
#define MOUTH_STATIC            (1 << 10)
#define MOUTH_TUSKED            (1 << 11)

#define MOUTH_HUMANOID (MOUTH_LIPPED | MOUTH_TOOTHED | MOUTH_TONGUED )

#define MAX_MOUTH_BIT MOUTH_TUSKED

#endif /* __MOUTH_TYPES_H__ */
