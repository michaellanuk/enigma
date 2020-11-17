#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <vector>

using namespace std;

class Reflector {
    private:
        vector<int> mappings;

    public:
        Reflector(vector<int> mappings);
        int new_reflector(string reflector_config_File, Reflector** reflector);
        int map(int letter);
};

#endif