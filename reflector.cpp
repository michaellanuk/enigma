#include <vector>
#include <fstream>
#include <iostream>

#include "reflector.h"
#include "errors.h"

using namespace std;

Reflector::Reflector(vector<int> mappings)
    : mappings(mappings) {}

int Reflector::new_reflector(string reflector_config_file, Reflector** reflector) {
    int num;
    vector<int> mappings;
    
    ifstream input;
    input.open(reflector_config_file);

    if (input.fail()) {
        cerr << "Could not open reflector configuration file" << endl;
        return ERROR_OPENING_CONFIGURATION_FILE;
    }

    while (!input.eof()) {       
        input >> num;
        
        if (input.fail()) {
            cerr << "Non-numeric character encountered in reflector file" << endl;
            return NON_NUMERIC_CHARACTER;
        }
        
        if (num < 0 || num > 25) {
            cerr << "Out-of-range number encountered in reflector file" << endl;
            return INVALID_INDEX;
        }

        mappings.push_back(num);

        /* the following check needs to be after inserting num to vector
        in the case that a repeated index appears as the last number */
        for (int i = 0; i < mappings.size() - 1; i++) {
            for (int j = i + 1; j < mappings.size(); j++) {
                if (mappings[i] == mappings[j]) {
                    cerr << "One or more numbers is mapped multiple times or with itself in reflector file" << endl;
                    return INVALID_REFLECTOR_MAPPING;
                }
            }
        }
    }

    if (mappings.size() != 26) {
        cerr << "Reflector file does not contain 13 pairs of numbers" << endl;
        return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    }

    *reflector = new Reflector(mappings);
    
    input.close();
    
    return NO_ERROR;
}

int Reflector::map(int letter) {
    for (int i = 0; i < mappings.size(); i++) {
        if (mappings[i] == letter) {
            if (i % 2 == 0) {
                return mappings[i + 1];
            } else {
                return mappings[i - 1];
            }
        }
    }
    return 0;
}