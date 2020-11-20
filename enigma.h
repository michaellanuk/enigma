#ifndef ENIGMA_H
#define ENIGMA_H

#include <vector>

#include "plugboard.h"
#include "reflector.h"
#include "rotors.h"

class Enigma {
    private:
        Plugboard* plugboard;
        Reflector* reflector;
        vector<Rotor*> created_rotors;
        int number_of_rotors;

    public:
        Enigma(Plugboard* plugboard, Reflector* reflector, vector<Rotor*> created_rotors, int number_of_rotors);
        ~Enigma();
        int create_enigma_machine(int argc, char** argv, Enigma** enigma);
        vector<int> get_rotor_starting_positions(string starting_pos_file);
        void connect_rotors(vector<Rotor*> &rotors);
        int encrypt_input_message(string &message);
        int convert_char_to_int(char c);
        char convert_int_to_char(int n);
        void encrypt_character(char &c);
};

#endif