#ifndef l_races_h
#define l_races_h

class Stat {
    int Class;
    int Average;
}

class Race {
    mixed array Limbs;
    mapping Resistance;
    mapping Stats;
    mapping Fingers;
    int array Sensitivity;
    int Complete;
    int PlayerFlag;
    string Language;
    mapping Skills;
    int Mass;
    int Size;
    int Btype;
    int Rtype;
}

int GetArmor(string str);
int GetSize(string str);
int GetSize(string str);
int GetBodyType(string str);
int GetRespirationType(string str);
int GetResistance(string str);

#endif /* l_races_h */
