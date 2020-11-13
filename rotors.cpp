#include <iostream>
#include <vector>
#include <fstream>

#include "rotors.h"
#include "errors.h"

using namespace std;

Rotor::Rotor(vector<int> mappings, vector<int> rotor_start_pos):
	mappings(mappings), rotor_start_pos(rotor_start_pos) {}

int Rotor::new_rotor(string rotors_config_file, Rotor** rotor) {
    int num;
    vector<int> mappings;
    vector<int> rotor_start_pos;
    
    ifstream input;
    input.open(rotors_config_file);

    if (input.fail()) {
        cerr << "Could not open configuration file" << endl;
        return ERROR_OPENING_CONFIGURATION_FILE;
    }

    while (!input.eof()) {       
        input >> num;
        
        if (input.fail()) {
            cerr << "Non-numeric character encountered" << endl;
            return NON_NUMERIC_CHARACTER;
        }
        
        if (num < 0 || num > 25) {
            cerr << "One or more numbers is out of range" << endl;
            return INVALID_INDEX;
        }

        if (mappings.size() < 26) {
            mappings.push_back(num);
            
        } else {
            rotor_start_pos.push_back(num);
        }

        /* the following check needs to be after inserting num to vector
        in the case that a repeated number appears as the last one */
        for (int i = 0; i < mappings.size(); i++) {
            for (int j = i + 1; j < mappings.size(); j++) {
                if (mappings[i] == mappings[j]) {
                    cerr << "One or more letters has not been mapped or has been multiple times" << endl;
                    return INVALID_ROTOR_MAPPING;
                }
            }
        }        
    }
    
    input.close();

    *rotor = new Rotor(mappings, rotor_start_pos);
    
    return NO_ERROR;
}

int Rotor::map_forwards(int letter, int rotor_ticks) {
    int mapped = (mappings[(letter + rotor_ticks) % 26] + 26 - rotor_ticks) % 26;

    return mapped;
}

int Rotor::map_backwards(int letter, int rotor_ticks) {
    for (int i = 0; i < mappings.size(); i++) {
        if (mappings[i] == (letter + rotor_ticks) % 26) {
            return (i - rotor_ticks + 26) % 26;
        }
    }
    return 0;
}

int main() {}