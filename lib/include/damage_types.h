#ifndef s_damage_types_h
#define s_damage_types_h

#define BLUNT              (1 << 1)
#define BLADE              (1 << 2)
#define KNIFE              (1 << 3)
#define WATER              (1 << 4)
#define SHOCK              (1 << 5)
#define COLD               (1 << 6)
#define HEAT               (1 << 7)
#define GAS                (1 << 8)
#define ACID               (1 << 9)
#define MAGIC              (1 << 10)

#define MAX_DAMAGE_BIT     MAGIC

#define ALL_DAMAGE  BLUNT | BLADE | KNIFE | WATER | SHOCK | COLD | HEAT | GAS | ACID | MAGIC

#endif /* s_damage_types_h */
