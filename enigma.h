#ifndef ENIGMA_H
#define ENIGMA_H

#include <vector>

#include "plugboard.h"
#include "reflector.h"
#include "rotors.h"

class Enigma {
    private:
        int number_of_rotors;
        Plugboard* plugboard;
        Reflector* reflector;
        vector<Rotor*> created_rotors;

    public:
        Enigma(Plugboard* plugboard, Reflector* reflector, vector<Rotor*> created_rotors, int number_of_rotors);
        int create_enigma_machine(int argc, char** argv, Enigma** enigma);
        vector<int> get_rotor_starting_positions(string starting_pos_file);
};

#endif