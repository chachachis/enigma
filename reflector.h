#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <fstream>
#include <string>
#include <iostream>
#include "globalconst.h"

using namespace std;

class Reflector {
  int mapping[NUM_OF_POSSIBLE_ALPHABETS];
  int rfErrorMessage(int code, string fileName);
public:
  Reflector() {
    for (int i = 0; i < NUM_OF_POSSIBLE_ALPHABETS; i++) {
      mapping[i] = i;
    }
  }  
  int configAddPairs(ifstream& rfConfig, string fileName);
  int getMapValue(int value);
};

#endif
