#ifndef ROTORS_H
#define ROTORS_H

#include <vector>
using namespace std;

class Rotor {
    private:
        vector<int> mappings;
        vector<int> notch_positions;
        int starting_pos;
        int rotor_ticks;

    public:
        Rotor* prev = nullptr;

        Rotor(vector<int> mappings, vector<int> notch_positions, int starting_pos, int rotor_ticks);
        int new_rotor(string rotors_config_file, int starting_pos, Rotor** rotor);
        int map_forwards(int letter);
        int map_backwards(int letter);
        void rotate();
        int get_notch_pos(int ith_notch);
};

#endif