#include <iostream>
#include <fstream>
#include <vector>

#include "plugboard.h"
#include "errors.h"

using namespace std;

Plugboard::Plugboard(vector<int> indexes) 
    : indexes(indexes) {}

int Plugboard::new_plugboard(string plugboard_config_file, Plugboard** plugboard) {
    int num;
    vector<int> indexes;
    
    ifstream input;
    input.open(plugboard_config_file);

    if (input.fail()) {
        cerr << "Could not open plugboard configuration file" << endl;
        return ERROR_OPENING_CONFIGURATION_FILE;
    }

    while (!input.eof()) {       
        input >> num;
        
        if (input.fail()) {
            cerr << "Non-numeric character encountered in plugboard file" << endl;
            return NON_NUMERIC_CHARACTER;
        }
        
        if (num < 0 || num > 25) {
            cerr << "Out-of-range number encountered in plugboard file" << endl;
            return INVALID_INDEX;
        }

        indexes.push_back(num);

        /* the following check needs to be after inserting num to vector
        in the case that a repeated index appears as the last number */
        for (int i = 0; i < indexes.size() - 1; i++) {
            for (int j = i + 1; j < indexes.size(); j++) {
                if (indexes[i] == indexes[j]) {
                    cerr << "One or more contacts is connected multiple times or with itself in plugboard file" << endl;
                    return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
                }
            }
        }
    }

    if (indexes.size() % 2 == 1) {
        cerr << "Odd number of contacts in plugboard file" << endl;
        return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    }

    *plugboard = new Plugboard(indexes);
    
    input.close();
    
    return NO_ERROR;
}

int Plugboard::map_letter(int letter) {
    for (int i = 0; i < indexes.size(); i++) {
        if (indexes[i] == letter) {
            if (i % 2 == 0) {
                return indexes[i + 1];
            } else {
                return indexes[i - 1];
            }
        }
    }
    return letter; // if there is no configuration for letter passed
}