#ifndef PLUGBOARD_H
#define PLUGBOARD_H

#include <fstream>
#include <string>
#include <iostream>
#include "globalconst.h"

using namespace std;

class Plugboard {
  int mapping[NUM_OF_POSSIBLE_ALPHABETS];
  int pbErrorMessage(int code, string fileName);
public:
  Plugboard() {
    for (int i = 0; i < NUM_OF_POSSIBLE_ALPHABETS; i++){
      mapping[i] = i;
    }
  }  
  int configAddPlugs(ifstream& pbConfig, string fileName);
  int getMapValue(int intA);
};

#endif
