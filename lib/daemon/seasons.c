/*    /daemon/seasons.c
 *    from the Dead Souls LPC Library
 *    handles game-based time
 *    created by Descartes of Borg 950508
 *    Version: @(#) seasons.c 1.7@(#)
 *    Last modified: 96/10/27
 */

#include <lib.h>
#include <cfg.h>
#include <config.h>
#include "include/seasons.h"

inherit LIB_DAEMON;

private static int CurrentDay, CurrentYear, Dawn, Morning, Twilight, Night;
private static string CurrentSeason, TimeOfDay;
private static mapping Moons;
private static class month CurrentMonth;
private static string *Days;
private static function *DawnCalls, *MorningCalls, *TwilightCalls;
private static function *NightCalls, *MidnightCalls;
private static class month *Months;

static void create() {
    string *lines;
    int i, maxi;

    daemon::create();
    MorningCalls = ({});
    DawnCalls = ({});
    TwilightCalls = ({});
    NightCalls = ({});
    MidnightCalls = ({});
    maxi = sizeof(lines = filter(explode(read_file(CFG_MONTHS), "\n"),
                   (: $1 && $1 != "" && $1[0] != '#' :)));
    Months = allocate(maxi);
    for(i=0; i<maxi; i++) {
        Months[i] = new(class month);
        sscanf(lines[i], "%s:%s:%d:%d",((class month)Months[i])->Name,
                                      ((class month)Months[i])->Season,
                                      ((class month)Months[i])->Days,
                                      ((class month)Months[i])->DaylightHours);
    }
    Days = filter(explode(read_file(CFG_DAYS), "\n"),
                  (: $1 && $1 != "" && $1[0] != '#' :));
    maxi = sizeof(lines = filter(explode(read_file(CFG_MOONS), "\n"),
                   (: $1 && $1 != "" && $1[0] != '#' :)));
    Moons = allocate_mapping(maxi);
    for(i=0; i<maxi; i++) {
        string nom, id, desc, lnom;
        int phase;

        sscanf(lines[i], "%s:%d:%s:%s", nom, phase, id, desc);
        lnom = convert_name(nom);
        Moons[lnom] = new(class moon);
        ((class moon)Moons[lnom])->Name = nom;
        ((class moon)Moons[lnom])->Phase = phase;
        ((class moon)Moons[lnom])->Id = id;
        ((class moon)Moons[lnom])->Description = desc;
    }
    eventConfigure();
}

static void eventConfigure() {
    int i, x, days, tot, maxi;

    days = (time() - DAY_ONE) / (DAY_LENGTH * HOUR_LENGTH);
    for(tot=0, i=0, maxi = sizeof(Months); i<maxi; i++)
      tot += ((class month)Months[i])->Days;
    CurrentYear = days / tot + 1;
    days = (days % tot) + 1;
    for(i=0, maxi = sizeof(Months); i<maxi; i++) {
        if( days <= ((class month)Months[i])->Days ) {
            CurrentMonth = (class month)Months[i];
            CurrentSeason = ((class month)Months[i])->Season;
            CurrentDay = days;
            break;
        }
        else days -= ((class month)Months[i])->Days;
    }
    x = CurrentMonth->DaylightHours * HOUR_LENGTH;
    Morning = ((DAY_LENGTH * HOUR_LENGTH) - x) / 2;
    Twilight = Morning + x;
    if( Morning < HOUR_LENGTH ) {
        Dawn = Morning/2;
        Night = Twilight + Morning/2;
    }
    else {
        Dawn = Morning - HOUR_LENGTH;
        Night = Twilight + HOUR_LENGTH;
    }
    x = GetCurrentTime();
    if( x < Dawn ) {
        TimeOfDay = "night";
        call_out( (: eventDawn :), Dawn - x);
    }
    else if( x < Morning ) {
        TimeOfDay = "dawn";
        call_out( (: eventMorning :), Morning - x);
    }
    else if( x < Twilight ) {
        TimeOfDay = "day";
        call_out( (: eventTwilight :), Twilight - x);
    }
    else if( x < Night ) {
        TimeOfDay = "twilight";
        call_out( (: eventNight :), Night - x );
    }
    else {
        TimeOfDay = "night";
        call_out( (: eventMidnight :), (DAY_LENGTH * HOUR_LENGTH) - x);
    }
}

static void eventDawn() {
    object *obs;
    int i;

    call_out( (: eventMorning :), Morning - GetCurrentTime() );
    TimeOfDay = "dawn";
    obs = filter(users(), (: environment($1) &&
                           (string)environment($1)->GetClimate()!="indoors" &&
                           !((int)environment($1)->GetProperty("no time")) :));
    message("environment", "%^YELLOW%^The sun appears just over the horizon.",
           obs );
    i = sizeof(DawnCalls);
    while(i--) catch(evaluate(DawnCalls[i]));
}

static void eventMorning() {
    object *obs;
    int i;

    call_out( (: eventTwilight :), Twilight - GetCurrentTime());
    TimeOfDay = "day";
    obs = filter(users(), (: environment($1) &&
                           (string)environment($1)->GetClimate()!="indoors" &&
                           !((int)environment($1)->GetProperty("no time")) :));
    message("environment", "%^BOLD%^YELLOW%^The sun now shines completely "
            "on a new day.", obs);
    i = sizeof(MorningCalls);
    while(i--) catch(evaluate(MorningCalls[i]));
}

static void eventTwilight() {
    object *obs;
    int i;

    call_out( (: eventNight :), Night - GetCurrentTime() );
    TimeOfDay = "twilight";
    obs = filter(users(), (: environment($1) &&
                           (string)environment($1)->GetClimate()!="indoors" &&
                           !((int)environment($1)->GetProperty("no time")) :));
    message("environment", "%^CYAN%^The sun begins to fall away into "
            "twilight.", obs);
    i = sizeof(TwilightCalls);
    while(i--) catch(evaluate(TwilightCalls[i]));
}

static void eventNight() {
    object *obs;
    int i;

    call_out( (: eventMidnight :), DAY_LENGTH - GetCurrentTime() );
    TimeOfDay = "night";
    obs = filter(users(), (: environment($1) &&
                           (string)environment($1)->GetClimate()!="indoors" &&
                           !((int)environment($1)->GetProperty("no time")) :));
    message("environment", "%^BOLD%^BLUE%^Night darkens all that is real.", obs);
    i = sizeof(NightCalls);
    while(i--) catch(evaluate(NightCalls[i]));
}

static void eventMidnight() {
    int i;

    CurrentDay++;
    i = CurrentMonth->Days;
    if( CurrentDay > i ) {
        int y;

        y = CurrentYear;
        eventConfigure();
        if( y != CurrentYear )
          message("shout", "Happy New Year!!!\nIt is now the year " +
                  GetYearString(CurrentYear) + "!!!!!", users());
        return;
    }
    call_out( (: eventDawn :), Dawn);
    TimeOfDay = "night";
    i = sizeof(MidnightCalls);
    while(i--) catch(evaluate(MidnightCalls[i]));
}

int GetCurrentDay() { return CurrentDay; }

string GetCurrentDayName() { return GetDayName(time()); }

string GetCurrentMonth() { return CurrentMonth->Name; }

string GetCurrentSeason() { return CurrentSeason; }

int GetCurrentTime() { return (time()- DAY_ONE) % (DAY_LENGTH * HOUR_LENGTH); }

int GetCurrentYear() { return CurrentYear; }

int GetDay(int x) {
    int tot, days, i, maxi;

    days = absolute_value((x - DAY_ONE) / (DAY_LENGTH * HOUR_LENGTH));
    for(tot=0, i=0, maxi = sizeof(Months); i<maxi; i++)
      tot += ((class month)Months[i])->Days;
    days = (days % tot) + ( (x < DAY_ONE) ? 0 : 1 );
    if( x < DAY_ONE ) {
        i = sizeof(Months);
        while(i--) {
            if( days < ((class month)Months[i])->Days )
              return ((class month)Months[i])->Days - days;
            else days -= ((class month)Months[i])->Days;
        }
        return 0;
    }
    for(i=0, maxi = sizeof(Months); i<maxi; i++) {
        if( days <= ((class month)Months[i])->Days ) return days;
        else days -= ((class month)Months[i])->Days;
    }
    return 0;
}

string GetDayName(int x) {
    int days;

    days = absolute_value((x - DAY_ONE) / (DAY_LENGTH * HOUR_LENGTH));
    return Days[days % sizeof(Days)];
}

string *GetDays() { return Days + ({}); }

int GetDaylightHours(string mon) {
    int i;

    i = sizeof(Months);
    while(i--) {
        if( ((class month)Months[i])->Name == mon )
          return ((class month)Months[i])->DaylightHours;
    }
    return 0;
}

int GetHour(int x) {
    int y;

    y = absolute_value(GetTime(x));
    y = y % (DAY_LENGTH * HOUR_LENGTH);
    if( x < DAY_ONE ) y = (DAY_LENGTH * HOUR_LENGTH) - y;
    return (y / HOUR_LENGTH);
}

int GetMinutes(int x) {
    int y;

    y = absolute_value(GetTime(x));
    y = y % (DAY_LENGTH * HOUR_LENGTH);
    if( x < DAY_ONE ) y = (DAY_LENGTH * HOUR_LENGTH) - y;
    return (y % HOUR_LENGTH) / (HOUR_LENGTH/60);
}

string GetMonth(int x) {
    int monthIndex;
    
    monthIndex = GetMonthIndex(x);
    return Months[monthIndex]->Name;
}

private int GetMonthIndex(int x) {
    int tot, days, i, maxi;

    if( x < DAY_ONE ) days = (DAY_ONE - x) / (DAY_LENGTH * HOUR_LENGTH);
    else days = (x - DAY_ONE) / (DAY_LENGTH * HOUR_LENGTH);
    for(tot=0, i=0, maxi = sizeof(Months); i<maxi; i++)
      tot += ((class month)Months[i])->Days;
    days = (days % tot) + ((x >= DAY_ONE) ? 1 : 0);
    if( x < DAY_ONE ) {
        i = sizeof(Months);
        while(i--) {
            if( days < ((class month)Months[i])->Days )
              return i;
            else days -= ((class month)Months[i])->Days;
        }
        return 0;
    }
    for(i=0, maxi = sizeof(Months); i<maxi; i++) {
        if( days <= ((class month)Months[i])->Days )
          return i;
        else days -= ((class month)Months[i])->Days;
    }
    return 0;
}

string *GetMonths() {
    string *ret = ({});
    int i, maxi;

    for(i=0, maxi = sizeof(Months); i<maxi; i++)
      ret += ({ ((class month)Months[i])->Name });
    return ret;
}

string GetSeason(int x) {
    int monthIndex;
    
    monthIndex = GetMonthIndex(x);
    return Months[monthIndex]->Season;
}

int GetTime(int x) { return (x - DAY_ONE); }

string GetTimeOfDay() { return TimeOfDay; }

function AddTimeEvent(string tod, function f) {
    switch(tod) {
        case "dawn": DawnCalls += ({ f }); break;
        case "morning": MorningCalls += ({ f }); break;
        case "twilight": TwilightCalls += ({ f }); break;
        case "night": NightCalls += ({ f }); break;
        case "midnight": MidnightCalls += ({ f }); break;
        default: return 0;
    }
    return f;
}

mapping GetTimeEvents() {
    return ([ "dawn" : DawnCalls + ({}), "morning" : MorningCalls + ({}),
             "twilight" : TwilightCalls + ({}), "night" : NightCalls + ({}),
             "midnight" : MidnightCalls + ({}) ]);
}

int GetYear(int x) {
    int i, tot;

    i = sizeof(Months);
    while(i--) tot += ((class month)Months[i])->Days;
    i = ((x - DAY_ONE) / (DAY_LENGTH * HOUR_LENGTH)) / tot;
    if( x < DAY_ONE ) return i;
    else return i + 1;
}

string GetYearString(int x) {
    if( x < 0 ) return (x * (-1)) + " BN";
    else return x + " NM";
}

string GetPhaseName(mixed val) {
    if( stringp(val) ) {
	val = GetPhase(val);
    }
    switch(val) {
        case 0: return "new";
        case 1: return "waxing";
        case 2: return "waning";
        case 3: return "full";
        default: return "error";
    }
}

int GetPhase(string m) {
    int x, y, z;

    m = convert_name(m);
    x = ((class moon)Moons[m])->Phase;
    y = (time() - DAY_ONE) % x;
    z = x / 4;
    return y / z;
}

int GetRadiantLight() {
    switch( TimeOfDay ) {
    case "night": return (GetMoonLight() * 2);
    case "day": return 60;
    default: return 30;
    }
}

int GetMoonLight() {
    string *moons;
    int i, y = 0;

    i = sizeof(moons = keys(Moons));
    while(i--) {
        int z;

        z = GetPhase(moons[i]);
        if( z == 4) z = 2;
        y += z;
    }
    return y;
}

string GetLong(string arg) {
    string array arr, mn;
    object env;
    string tmp;
    int i;

    if( !(env = environment(this_player())) )
      return "You are in serious trouble.";
    switch(arg) {
        case "sun":
          switch(GetTimeOfDay()) {
              case "dawn":
              return "The sun is hanging low in the eastern sky.";
              case "day":
              return "The sun is shining brightly in the daytime sky.";
              case "twilight":
              return "The sun is sinking into the western sky.";
              case "night":
              return "There is no sun to be seen.";
          }
        case "moon": case "moons":
          if( GetTimeOfDay() != "night" ) return "During the day?";
          else {
              string *moons;
              int x = 0;

              i = sizeof(moons = keys(Moons));
              while(i--) {
                  int y;

                  if( y = GetPhase(moons[i]) ) continue;
                  else if( tmp )
                    tmp += capitalize(((class moon)Moons[moons[i]])->Id) +
                       " is " + GetPhaseName(y) + ". ";
                  else tmp = "\n" +
                    capitalize(((class moon)Moons[moons[i]])->Id) + " is " +
                      GetPhaseName(y) + ".  ";
                  x = 1;
              }
              if( !x ) return 0;
              else return tmp;
          }
        case "sky":
          if( GetTimeOfDay() == "night" ) {
	      tmp =  GetLong("moon");
	      if( !tmp ) {
		  return "The sky is filled only with the glitter of stars.";
	      }
	      else {
		  return tmp;
	      }
	  }
          else {
	      string sky;
	      
	      tmp = GetLong("sun");
	      if( this_player() ) {
		  env = environment(this_player());
	      }
	      if( sky = env->GetSky() ) {
		  env = find_object(sky);
		  if( env ) {
		      object array obs = filter(all_inventory(env),
						function(object ob) {
						    if( ob->GetInvis(this_player()) ) {
							return 0;
						    }
			                            if( living(ob) ) {
							return 1;
						    }
						    return 0;
		                                });

		      if( sizeof(obs) ) {
			  int maxi = sizeof(obs);

			  sky = obs[i]->GetName();
			  if( maxi == 1 ) {
			      sky += " is flying in the sky.";
			  }
			  else {
			      for(i=1; i<maxi; i++) {
				  if( i == maxi-1 ) {
				      if( maxi == 2 ) {
					  sky += " and ";
				      }
				      else {
					  sky += ", and ";
				      }
				  }
				  else {
				      sky += ", ";
				  }
				  sky += obs[i]->GetName();
			      }
			      sky += " are flying in the sky.";
			  }
			  tmp = sky + "\n" + tmp;
		      }
		  }
	      }
	      return tmp;
	  }
        default:
          if( Moons[arg] ) return ((class moon)Moons[arg])->Description;
          arr = map(mn = keys(Moons), (: ((class moon)Moons[$1])->Id :));
          if( (i = member_array(arg, arr)) != -1 )
            return ((class moon)Moons[mn[i]])->Description;
          else return 0;
    }
}

string array GetMoons() {
    return map(keys(Moons), (: ((class moon)Moons[$1])->Name :));
}

int eventShow(object who, string args) {
    string str;
    if( !who || !sizeof(args) ) return 0;
    if( !str = GetLong(args) ) return 0;
    if( (string)environment(who)->GetClimate() == "indoors" ) {
        who->eventPrint("You are indoors!");
        return 1;
    }
    who->eventPrint(str);
    environment(who)->eventPrint((string)who->GetName() + " gazes toward the sky.", who);
    return 1;
}
