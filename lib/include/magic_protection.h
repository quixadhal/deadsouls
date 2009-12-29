/****************************************************
 
  /include/magic_protection.h
   created by Blitz@Dead Souls

passed class to AddMagicProtection()
  int bits                      Define what damage bits this magic
                                protection supports.
 
 
  int absorb                    If defined, damage is absorbed and
                                subtracted from this total, until
                                max damage is < 1, or destroyed.
  or,
 
  int protect                   This is an alternative to "absorb".
                                Instead of absorbing damage as above,
                                a random amount of this can be subtracted
                                from damage done, this lasts until "time"
                                has run out.
 
  int time                      This is decremented each heart beat.
                                Note if this is not defined and
                                "absorb damage" is defined, protection
                                is indefinite until destroyed.
 
  optional:
 
  function end                  If this is defined, it is evaluated
                                when protection is removed.
 
  function hit                  If this is defined, it is evaluated
                                anytime protection takes damage
                                or damage is reduced
 
  mixed args                    Used with the above hit function.
                                This is passed as the last argument
                                to the hit function
 
  object ob                     Pointer to the object that defined
                                the protection.

  string obname                 The file_name() of the object that
                                defined the protection.
  
  string name                   Some user-ready name of the protection,
                                such as "magical buffer"
}
 
*********************************************************/
 
class MagicProtection {
      int bits;
      int absorb;
      int protect;
      int time;
      int timestamp;
      function end;
      function hit;
      mixed args;
      object caster;
      object ob;
      string obname;
      string name;
}
