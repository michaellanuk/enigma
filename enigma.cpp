#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

#include "plugboard.h"
#include "reflector.h"
#include "rotors.h"
#include "errors.h"
#include "enigma.h"
#include "constants.h"

using namespace std;

Enigma::Enigma(Plugboard* plugboard, Reflector* reflector, vector<Rotor*> created_rotors, int number_of_rotors)
    : plugboard(plugboard), reflector(reflector), created_rotors(created_rotors), number_of_rotors(number_of_rotors) {}

Enigma::~Enigma() {
    delete plugboard;
    delete reflector;
    for (int i = 0; i < created_rotors.size(); i++) {
        delete created_rotors[i];
    }
}

int Enigma::create_enigma_machine(int argc, char** argv, Enigma** enigma) {
    int error_code, num_of_rotors = 0;
    string rotor_pos_config_file, plugboard_config_file, reflector_config_file;
    vector<string> rotor_files;
    vector<int> rotor_starting_positions;
    vector<Rotor*> created_rotors;
    
    if (argc < 3 || argc == 4) {
        cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>)* rotor-positions" << endl;
        return INSUFFICIENT_NUMBER_OF_PARAMETERS;
    }

    plugboard_config_file = argv[1];
    reflector_config_file = argv[2];

    /* create plugboard */
    Plugboard* plugboard = nullptr;
    error_code = plugboard->new_plugboard(plugboard_config_file, &plugboard);
    if (error_code != 0) {
        return error_code;
    }

    /* create reflector */
    Reflector* reflector = nullptr;
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
            Rotor* rotor = nullptr;
            error_code = rotor->new_rotor(rotor_files[i], rotor_starting_positions[i], &rotor);
            if (error_code != 0) {
                delete plugboard;
                delete reflector;
                return error_code;
            }
            created_rotors.push_back(rotor);
        }
    }

    if (num_of_rotors > 0) {
        connect_rotors(created_rotors);
    }

    *enigma = new Enigma(plugboard, reflector, created_rotors, num_of_rotors);

    return NO_ERROR;
}

vector<int> Enigma::get_rotor_starting_positions(string starting_pos_file) {
    vector<int> starting_positions;
    int pos;
    
    ifstream input;
    input.open(starting_pos_file);

    if (input.fail()) {
        cerr << "Could not open rotor.pos file" << endl;
        exit(ERROR_OPENING_CONFIGURATION_FILE);
    }

    while(!input.eof()) {
        input >> pos >> ws;

        if (input.fail()) {
            cerr << "Non-numeric character in rotor positions file rotor.pos" << endl;
            exit(NON_NUMERIC_CHARACTER);
        }

        if (pos < 0 || pos > 25) {
            cerr << "Out-of-range number encountered in rotor positions file rotor.pos" << endl;
            exit(INVALID_INDEX);
        }

        starting_positions.push_back(pos);
    }

    input.close();

    return starting_positions;
}

void Enigma::connect_rotors(vector<Rotor*> &rotors) {
    int n = rotors.size();

    if (n > 1) {
        for (int i = 1; i < n; i++) {
            rotors[i]->prev = rotors[i - 1];
        }
    } else if (n == 1) {
        rotors[0]->prev = nullptr;
    } else {
        return;
    }
}

int Enigma::encrypt_input_message(string& message) {
    string output_message;

    for (int i = 0; i < message.length(); i++) {
        if (message[i] < ASCII_A || message[i] > ASCII_Z) {
            cerr << "Invalid character encountered. Please enter only upper-case letters A-Z" << endl;
            return INVALID_INPUT_CHARACTER;
        }
        encrypt_character(message[i]);
        output_message += message[i];
    }
    
    cout << output_message << endl;

    return NO_ERROR;
}

int Enigma::convert_char_to_int(char c) {
    return (c - ASCII_A) % ALPHABET_SIZE;
}

char Enigma::convert_int_to_char(int n) {
    return n + ASCII_A;
}

/* encrypt characters through each enigma component */
void Enigma::encrypt_character(char &c) { 
    int letter_as_num = convert_char_to_int(c);

    letter_as_num = plugboard->map_letter(letter_as_num);

    // rotate the right-most rotor
    if (number_of_rotors > 0) {
        created_rotors[number_of_rotors - 1]->rotate();
    }

    for (int i = number_of_rotors - 1; i >= 0; i--) { // map from right to left (last in array to first)
        letter_as_num = (created_rotors[i])->map_forwards(letter_as_num);
    }

    letter_as_num = reflector->map(letter_as_num);

    for (int i = 0; i < number_of_rotors; i++) { // map from left to right (first in array to last)
        letter_as_num = (created_rotors[i])->map_backwards(letter_as_num);
    }

    letter_as_num = plugboard->map_letter(letter_as_num); // goes through plugboard again

    c = convert_int_to_char(letter_as_num);
}