#ifndef __INC_VOTING
#define __INC_VOTING

#include <dirs.h>

#define VOTE_ERROR                  0
#define VOTE_SUCCESS                1
#define VOTE_ALREADY_RUNNING        2
#define VOTE_NOT_CANDIDATE          3
#define VOTE_NOT_RUNNING            4
#define VOTE_NOT_PRIMARY            5
#define VOTE_NOT_CLASS_MEMBER       6
#define VOTE_ALREADY_VOTED          7
#define VOTE_MODE_CANDIDATES        8
#define VOTE_MODE_VOTING            9
#define VOTE_RUNNING                10

#define VOTE_DAY_COUNT              7

#define SAVE_VOTES                  DIR_VOTES "/voting"
#define VOTE_ROOM                   "/domains/Ylsrim/room/vote_hall.c"

mixed eventSave();
mixed eventStartVoting();
mixed eventEndVoting();
mixed eventAddCandidate( string sClass, string sWho );
mixed eventRemoveCandidate( string sClass, string sWho );
mixed eventCastVote( string sClass, string sVoter, string sVotee );
mixed eventNextDay();
mixed eventTallyVotes();
int GetStatus();
int GetMode();
string *GetCandidates( string sClass );
string *GetCouncil();
string GetCouncilMember( string sClass );
int GetVoteStatus( object ob );
string GetVoteRoom();
int IsCouncilMember( object ob );

#endif /* __INC_VOTING */
