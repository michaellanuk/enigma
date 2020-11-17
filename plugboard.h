#ifndef PLUGBOARD_H
#define PLUGBOARD_H

#include <vector>
using namespace std;

class Plugboard {
    private:
        vector<int> indexes;
    
    public:
        Plugboard(vector<int> indexes);
        int new_plugboard(string plugboard_config_file, Plugboard** plugboard);
        int map_letter(int letter);
};

#endif