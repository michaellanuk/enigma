#ifndef ROTORS_H
#define ROTORS_H

#include <vector>
using namespace std;

class Rotor {
    private:
        int notch_pos;
        int starting_pos;
        vector<int> mappings;
        int rotor_ticks = 0;

    public:
        Rotor* next;

        Rotor(vector<int> mappings, int notch_pos, int starting_pos);
        int new_rotor(string rotors_config_file, int starting_pos, Rotor** rotor);
        int map_forwards(int letter, int rotor_ticks);
        int map_backwards(int letter, int rotor_ticks);
};

#endif