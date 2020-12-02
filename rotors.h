#ifndef ROTORS_H
#define ROTORS_H

#include <iostream>
#include <fstream>
#include <string>
#include "globalconst.h"

using namespace std;

class Rotor {
  int mapping[NUM_OF_POSSIBLE_ALPHABETS];
  bool notches[NUM_OF_POSSIBLE_ALPHABETS];
  int offset; // Refers to ring offset that changes internal wiring
  bool step;
  // Step = true if rotor was turned by rotor to its right immediately before
  // current keypress
  static int rotorCount;
  static Rotor * rotors[99];

  int configRotor(ifstream& rotorFile, string fileName);
  static void updateOffset();
  static int normalisePosition(int position);
  int getMapValue(int position);
  int getMapKey(int value);
  bool checkMapped(int position, int val);

public:
  Rotor() {
    for (int i = 0; i < NUM_OF_POSSIBLE_ALPHABETS; i++) {
      mapping[i] = i;
      notches[i] = false;
      offset = 0;
      step = false;
    }
    rotorCount++;
  }
  
  static int rotorRtoL(int input);
  static int rotorLtoR(int input);
  static int configNRotors(char **argv, int count);
  static int getRotorCount();

};

#endif
