#ifndef s_war_h
#define s_war_h

class team {
    string name;
    string leader;
    int points;
    int war_chest;
    string array members;
}
    
class war {
    int start;
    string reason;
    class team agressor;
    class team enemy;
    string array neutrals;
}
