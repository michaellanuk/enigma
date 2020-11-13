#ifndef ROTORS_H
#define ROTORS_H

#include <vector>
using namespace std;

class Rotor {
    private:
        vector<int> rotor_start_pos;
        vector<int> mappings;
        int rotor_ticks = 0;

    public:
        Rotor(vector<int> mappings, vector<int> rotor_start_pos);
        int new_rotor(string rotors_config_file, Rotor** rotor);
        int map_forwards(int letter, int rotor_ticks);
        int map_backwards(int letter, int rotor_ticks);
};

#endif