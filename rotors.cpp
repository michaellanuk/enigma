#include <iostream>
#include <vector>
#include <fstream>

#include "rotors.h"
#include "errors.h"
#include "constants.h"

using namespace std;

Rotor::Rotor(vector<int> mappings, vector<int> notch_positions, int starting_pos, int rotor_ticks)
    : mappings(mappings), notch_positions(notch_positions), starting_pos(starting_pos), rotor_ticks(rotor_ticks) {}

int Rotor::new_rotor(string rotors_config_file, int starting_pos, Rotor** rotor) {
    int num;
    vector<int> notch_positions;
    vector<int> mappings;
    
    ifstream input;
    input.open(rotors_config_file);

    if (input.fail()) {
        cerr << "Could not open rotor configuration file" << endl;
        return ERROR_OPENING_CONFIGURATION_FILE;
    }

    while (!input.eof()) {       
        input >> num >> ws;
        
        if (input.fail()) {
            cerr << "Non-numeric character encountered in rotor configuration file" << endl;
            return NON_NUMERIC_CHARACTER;
        }
        
        if (num < 0 || num > 25) {
            cerr << "Out-of-range number encountered in rotor configuration file" << endl;
            return INVALID_INDEX;
        }

        if (mappings.size() < ALPHABET_SIZE) {
            mappings.push_back(num); 
        } else {
            notch_positions.push_back(num);
        }

        /* the following check needs to be after inserting num to vector
        in the case that a repeated number appears as the last one */
        for (int i = 0; i < mappings.size() - 1; i++) {
            for (int j = i + 1; j < mappings.size(); j++) {
                if (mappings[i] == mappings[j]) {
                    cerr << "One or more letters has not been mapped or has been multiple times in rotor configuration file" << endl;
                    return INVALID_ROTOR_MAPPING;
                }
            }
        }        
    }
    
    input.close();

    *rotor = new Rotor(mappings, notch_positions, starting_pos, 0);
    
    return NO_ERROR;
}

int Rotor::map_forwards(int letter) {
    int mapped = (mappings[(letter + rotor_ticks) % ALPHABET_SIZE] - rotor_ticks + ALPHABET_SIZE) % ALPHABET_SIZE;

    return mapped;
}

int Rotor::map_backwards(int letter) {
    for (int i = 0; i < mappings.size(); i++) {
        if (mappings[i] == (letter + rotor_ticks) % ALPHABET_SIZE) {
            return (i - rotor_ticks + ALPHABET_SIZE) % ALPHABET_SIZE;
        }
    }

    return 0;
}

void Rotor::rotate() {
    rotor_ticks = (rotor_ticks + 1) % ALPHABET_SIZE;

    int turnover_point;

    /* 
        turnover_point is the difference in distance between the 
        letter at the top of each rotor and the letter that,
        when it reaches the top, rotates the next rotor.
    */

    for (int i = 0; i < notch_positions.size(); i++) {
        if (notch_positions[i] <= starting_pos) {
            turnover_point = starting_pos - notch_positions[i];
        } else {
            turnover_point = ALPHABET_SIZE - (notch_positions[i] - starting_pos);
        }
    }

    if (rotor_ticks == turnover_point && prev != nullptr) {    
        prev->rotate();
    }
}

int Rotor::get_notch_pos(int ith_notch) {
    return notch_positions[ith_notch];
}