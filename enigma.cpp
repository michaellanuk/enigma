#include <iostream>
#include <vector>
#include <fstream>

#include "plugboard.h"
#include "reflector.h"
#include "rotors.h"
#include "errors.h"
#include "enigma.h"

using namespace std;

Enigma::Enigma(Plugboard* plugboard, Reflector* reflector, vector<Rotor*> created_rotors, int number_of_rotors)
    : plugboard(plugboard), reflector(reflector), created_rotors(created_rotors), number_of_rotors(number_of_rotors) {}

int Enigma::create_enigma_machine(int argc, char** argv, Enigma** enigma) {
    int error_code, num_of_rotors;
    string rotor_pos_config_file, plugboard_config_file, reflector_config_file;
    vector<string> rotor_files;
    vector<int> rotor_starting_positions;
    vector<Rotor*> created_rotors;
    
    if (argc < 3 || argc == 4) {
        cerr << "Not enough parameters were passed to the command line" << endl;
        return INSUFFICIENT_NUMBER_OF_PARAMETERS;
    }

    plugboard_config_file = argv[1];
    reflector_config_file = argv[2];

    /* create plugboard */
    Plugboard* plugboard;
    error_code = plugboard->new_plugboard(plugboard_config_file, &plugboard);
    if (error_code != 0) {
        return error_code;
    }

    /* create reflector */
    Reflector* reflector;
    error_code = reflector->new_reflector(reflector_config_file, &reflector);
    if (error_code != 0) {
        delete plugboard;
        return error_code;
    }

    if (argc > 4) {
        num_of_rotors = argc - 4;
        rotor_pos_config_file = argv[argc - 1];
        rotor_starting_positions = get_rotor_starting_positions(rotor_pos_config_file);

        for (int i = 3; i < argc - 1; i++) {
            rotor_files.push_back(argv[i]);
        }

        for (int i = 0; i < num_of_rotors; i++) {
            Rotor* rotor;
            error_code = rotor->new_rotor(rotor_files[i], rotor_starting_positions[i], &rotor);
            if (error_code != 0) {
                delete plugboard;
                delete reflector;
                return error_code;
            }
            created_rotors.push_back(rotor);
        }
    }

    *enigma = new Enigma(plugboard, reflector, created_rotors, num_of_rotors);

    return NO_ERROR;
}

vector<int> Enigma::get_rotor_starting_positions(string starting_pos_file) {
    vector<int> starting_positions;
    int pos;
    
    ifstream input;
    input.open(starting_pos_file);

    while(!input.eof()) {
        input >> pos;
        starting_positions.push_back(pos);
    }

    input.close();

    return starting_positions;
}

/* following main being used to test functions */
int main(int argc, char** argv) {
    Enigma* enigma;

    int code = enigma->create_enigma_machine(argc, argv, &enigma);

    cout << code << endl;
}