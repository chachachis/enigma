#include <fstream>
#include <string>
#include <iostream>
#include <cctype>
#include "plugboard.h"

using namespace std;

// Adds a pair of elements to the Reflector map and returns 0 if there are no errors.
// Returns the error code if there are errors.
int Plugboard::configAddPlugs(ifstream& pbConfig, string fileName) {
  int intA = 0, intB = 0;
  int count = 0;
  string inputStr;
  bool first = true;
  while (count < NUM_OF_POSSIBLE_ALPHABETS && pbConfig >> inputStr) {
    for (char c : inputStr) {
      if (!isdigit(c)) return pbErrorMessage(4, fileName);
    }
    if (first == true) {
      intA = stoi(inputStr);
      if (intA < MIN_ALPHABET_INTEGER || intA > MAX_ALPHABET_INTEGER) return pbErrorMessage(3, fileName);
      first = false;
    } else {
      intB = stoi(inputStr);
      if (intB < MIN_ALPHABET_INTEGER || intB > MAX_ALPHABET_INTEGER) return pbErrorMessage(3, fileName);
      first = true;
      if (intA == intB || mapping[intA] != intA || mapping[intB] != intB) return pbErrorMessage(5, fileName);
      mapping[intA] = intB;
      mapping[intB] = intA;
    }
    pbConfig >> ws;
    count++;
  }
  pbConfig >> ws >> intA;
  if (!pbConfig.eof()) return pbErrorMessage(60, fileName);
  if (count < NUM_OF_POSSIBLE_ALPHABETS && count % 2 != 0) return pbErrorMessage(61, fileName);
  return 0;
}

int Plugboard::getMapValue(int val) {
  if (val >= MIN_ALPHABET_INTEGER && val <= MAX_ALPHABET_INTEGER) {
    return mapping[val];
  }
  cerr << "Input out of bounds for plugboard mapping";
  return 3;  
}

// Prints message to standard error stream when called with non-zero error code
int Plugboard::pbErrorMessage(int code, string fileName) {
  if (code == 0) return 0;
  switch (code) {
  case 3:
    cerr << "A number in plugboard file " << fileName << " is not between 0 and 25" << endl;
    break;
  case 4:
    cerr << "Non-numeric character in plugboard file " << fileName << endl;
    break;
  case 5:
    cerr << "Impossible plugboard configuration from file " << fileName << endl;
    break;
  case 60:
    cerr << "Incorrect number (more than " << NUM_OF_POSSIBLE_ALPHABETS << ") of parameters in plugboard file " << fileName << endl;
    return 6;
    break;
  case 61:
    cerr << "Incorrect (odd) number of parameters in plugboard file " << fileName << endl;
    return 6;
    break;
  }
  return code;
}
