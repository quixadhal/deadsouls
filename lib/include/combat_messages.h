/* compiled by Blitz@Dead Souls and Mikla@Dead Souls */

// The SOBER_COMBAT define is intended to make combat messages a
// bit less emo. The problem with the original combat messages is
// that if you have, say, two robots fighting, it's weird and 
// jarring to see them foaming at the mouth or grunting angrily.
// Just remove the define for the old-style behavior, if you want
// the bellowing and ranting back. -Crat 05Nov2007

#define SOBER_COMBAT

#ifndef SOBER_COMBAT

#define MOVE_TYPES ({\
({ "feint deceptively", "feints deceptively" }),\
({ "dance around", "dances around" }),\
({ "lunge quickly", "lunges quickly" }),\
({ "duck low", "ducks low" }),\
({ "move fast", "moves fast" }),\
({ "strike rapidly", "strikes rapidly" }),\
({ "sidestep nimbly", "sidesteps nimbly" }),\
({ "growl menacingly", "growls menacingly" }),\
({ "grin sadistically", "grins sadistically" }),\
({ "grunt angrily", "grunts angrily" }),\
({ "bellow loudly", "bellows loudly" }),\
({ "swing blindly", "swings blindly" }),\
({ "shout profanities", "shouts profanities" }),\
({ "swell with ferocity", "swells with ferocity" }),\
({ "rally with determination", "rallies with determination" }),\
({ "spit derisively", "spits derisively" }),\
({ "glare contemptuously", "glares contemptuously" }),\
({ "stumble fortuitously", "stumbles fortuitously" }),\
({ "scream, \"Die!\"", "screams, \"Die!\"" }),\
({ "foam at the mouth", "foams at the mouth" }),\
({ "rant, \"Aahrrg!\"", "rants, \"Aahrrg!\"" }),\
({ "execute a diving roll maneuver", "executes a diving roll maneuver" }),\
({ "attack with renewed frenzy", "attacks with renewed frenzy" }),\
({ "yell, \"Look!! Behind you!\"", "yells, \"Look!! Behind you!\"" }),\
({ "go completely berserk", "goes completely berserk" }),\
({ "rage violently", "rages violently" }),\
({ "gyrate energetically", "gyrates energetically" }),\
({ "whirl around quickly", "whirls around quickly" }),\
({ "whirl blurringly", "whirls blurringly" }),\
({ "see an opening", "sees an opening" }),\
({ "do a cunning move", "does a cunning move" }),\
({ "twirl with finesse", "twirls with finesse" }),\
})

#else

#define MOVE_TYPES ({\
({ "feint", "feints" }),\
({ "twist swiftly", "twists swiftly" }),\
({ "lunge quickly", "lunges quickly" }),\
({ "duck", "ducks" }),\
({ "move fast", "moves fast" }),\
({ "strike rapidly", "strikes rapidly" }),\
({ "sidestep", "sidesteps" }),\
({ "gyrate briskly", "gyrates briskly" }),\
({ "whirl around quickly", "whirls around quickly" }),\
({ "whirl blurringly", "whirls blurringly" }),\
})

#endif

 
#define BLADE_DEGREES ({\
({\
({ "prick", "pricks" }),\
({ "lightly", "superficially", "just barely" })\
}),\
({\
({ "scratch", "scratches" }),\
({ "mildly", "barely", })\
}),\
({\
({ "jab", "jabs" }),\
({ "quickly", "meanly", })\
}),\
({\
({ "cut", "cuts" }),\
({ "painfully" }),\
}),\
({\
({ "slice", "slices" }),\
({ "deeply" }),\
}),\
({\
({ "pierce", "pierces" }),\
({ "wickedly" }),\
}),\
({\
({ "slash", "slashes" }),\
({ "expertly" }),\
}),\
({\
({ "stab", "stabs" }),\
({ "fiercely" }),\
}),\
({\
({ "carve", "carves" }),\
({ "to pieces" }),\
}),\
({\
({ "cleave", "cleaves" }),\
({ "cruelly" }),\
}),\
({\
({ "wound", "wounds" }),\
({ "greviously" }),\
}),\
({\
({ "devastate", "devastates" }),\
({ "completely" }),\
}),\
({\
({ "destroy", "destroys" }),\
({ "utterly", }),\
}),\
})
 
#define PROJECTILE_DEGREES ({\
({\
({ "prick", "pricks" }),\
({ "lightly", "superficially", "just barely" })\
}),\
({\
({ "scratch", "scratches" }),\
({ "mildly", "barely", }),\
}),\
({\
({ "graze", "grazes" }),\
({ "sharply" }),\
}),\
({\
({ "gash", "gashes" }),\
({ "deeply" }),\
}),\
({\
({ "tear", "tears" }),\
({ "painfully" }),\
}),\
({\
({ "pierce", "pierces" }),\
({ "wickedly" }),\
}),\
({\
({ "shear", "shears" }),\
({ "to ribbons" }),\
}),\
({\
({ "puncture", "punctures" }),\
({ "deeply", "with a quick thrust" }),\
}),\
({\
({ "rip", "rips" }),\
({ "apart", "to pieces" }),\
}),\
({\
({ "impale", "impales" }),\
({ "without mercy" }),\
}),\
({\
({ "wound", "wounds" }),\
({ "greviously" }),\
}),\
({\
({ "devastate", "devastates" }),\
({ "completely" }),\
}),\
({\
({ "destroy", "destroys" }),\
({ "utterly" }),\
}),\
})
 
#define BLUNT_DEGREES ({\
({\
({ "brush", "brushes" }),\
({ "lightly", "softly", "with little force" }),\
}),\
({\
({ "bruise", "bruises" }),\
({ "mildly" }),\
}),\
({\
({ "hit", "hits" }),\
({ "hard" }),\
}),\
({\
({ "pound", "pounds" }),\
({ "solidly" }),\
}),\
({\
({ "hammer", "hammers" }),\
({ "painfully" }),\
}),\
({\
({ "pummel", "pummels" }),\
({ "soundly" }),\
}),\
({\
({ "thrash", "thrashes" }),\
({ "viciously" }),\
}),\
({\
({ "smash", "smashes" }),\
({ "forcefully" }),\
}),\
({\
({ "crush", "crushes" }),\
({ "violently" }),\
}),\
({\
({ "slam", "slams" }),\
({ "powerfully" }),\
}),\
({\
({ "wound", "wounds" }),\
({ "greviously" }),\
}),\
({\
({ "devastate", "devastates", }),\
({ "completely" }),\
}),\
({\
({ "destroy", "destroys" }),\
({ "utterly" }),\
}),\
})
