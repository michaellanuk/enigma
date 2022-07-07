# Enigma
Implementation of an Enigma machine in C++.

Enigma is the common name for the coding machine used by German forces in the Second World War. Two machines set up in the same way allowed the sending of a message securely between their users.

The program utilises an invertible function, thus to decrypt a message one only needs to type the encrypted ciphertext into an identically configured Enigma machine and then read the plaintext off the output board.

## Usage:

Compile with `make`.

Provide command line with configuration files for the plugboard, reflector, (optional) rotor(s), and (optional) rotor starting positions.

e.g. for a machine with 2 rotors:

```
./enigma plugboards/I.pb reflectors/I.rf rotors/I.rot rotors/II.rot rotors/I.pos
Enter message:
HELLO WORLD
```
