#ifndef l_party_h
#define l_party_h

class party {
    object Leader;
    object *Members;
    object *Invited;
}

static void create();
mixed CanChangeLeader(object who, object targ);
mixed CanCreateParty(object who, string name);
mixed CanInviteMember(object who, object member);
mixed CanJoinParty(object who, string pname);
mixed CanRemoveMember(object who, object targ);
mixed CanRemoveParty(object who);
mixed eventChangeLeader(object who, object targ);
mixed eventCreateParty(object who, string name);
mixed eventInviteMember(object who, object member);
mixed eventJoinParty(object who, string name);
mixed eventRemoveMember(object who, object targ);
mixed eventRemoveParty(object who);
object GetPartyLeader(string name);
object *GetPartyMembers(string name);
static void RemoveInvitiation(string name, object who);

#endif /* l_party_h */
