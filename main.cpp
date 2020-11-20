#include <iostream>

#include "enigma.h"
#include "plugboard.h"
#include "reflector.h"
#include "rotors.h"
#include "errors.h"
#include "constants.h"

int main(int argc, char** argv) {
    Enigma* enigma = nullptr;
    string message, message_wo_ws;
    int code = NO_ERROR;
    
    int code = enigma->create_enigma_machine(argc, argv, &enigma);
    if (code) {
        return code;
    }

    cout << "Enter message:" << endl;
    getline(cin, message);

    for (long unsigned int i = 0; i < message.length(); i++) {
        if (message[i] != ASCII_SPACE) {
            message_wo_ws += message[i];
        }
    }

    code = enigma->encrypt_input_message(message_wo_ws);

    return code;
}