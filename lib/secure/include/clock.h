#define THE_BEGINNING 720561600
#define NOW(x) x-THE_BEGINNING

#define SECOND 1
#define MINUTE 20
#define HOUR 1200
#define DAY 24000
#define WEEK 120000
#define MONTH 480000
#define YEAR 4800000

#define CURRENT_YEAR(x) (1+((NOW(x))/(YEAR)))
#define CURRENT_MONTH(x) ((NOW(x))%YEAR)/MONTH
#define CURRENT_WEEK(x) ((NOW(x))%MONTH)/(MONTH/4)
#define CURRENT_DAY(x) ((NOW(x))%(MONTH/4))/DAY
#define CURRENT_HOUR(x) ((NOW(x))%DAY)/HOUR
#define CURRENT_MINUTE(x) ((NOW(x))%HOUR)/MINUTE
#define CURRENT_SECOND(x) ((NOW(x))%20)

#define LONGEST_DAY 101
#define SHORTEST_DAY 1

#define DAYS ({ "Shadowday", "Lockday", "Flameday", "Darkday", "Vaigday" })
#define MONTHS ({ "Roki", "Praxi", "Altki", "Ketralki", "Aenterki",\
    "Kepki", "Kortki", "Kantki", "Sartki", "Denki" })
