#ifndef s_boobytrap_h
#define s_boobytrap_h

#define BOOBYTRAP_OPEN              (1 << 1)
#define BOOBYTRAP_CLOSE             (1 << 2)
#define BOOBYTRAP_GET               (1 << 3)
#define BOOBYTRAP_DROP              (1 << 4)
#define BOOBYTRAP_LOCK              (1 << 5)
#define BOOBYTRAP_UNLOCK            (1 << 6)
#define BOOBYTRAP_STEAL             (1 << 7)
#define BOOBYTRAP_PICK              (1 << 8)
#define BOOBYTRAP_WEAR              (1 << 9)
#define BOOBYTRAP_REMOVE            (1 << 10)
#define BOOBYTRAP_WIELD             (1 << 11)
#define BOOBYTRAP_UNWIELD           (1 << 12)

#define BOOBYTRAP_ALL               BOOBYTRAP_OPEN | BOOBYTRAP_CLOSE | BOOBYTRAP_GET | BOOBYTRAP_DROP | BOOBYTRAP_LOCK | BOOBYTRAP_UNLOCK | BOOBYTRAP_STEAL | BOOBYTRAP_PICK | BOOBYTRAP_WEAR | BOOBYTRAP_REMOVE | BOOBYTRAP_WIELD | BOOBYTRAP_UNWIELD

#endif /* s_boobytrap_h */
