#include <fstream>
#include <string>
#include <iostream>
#include <cctype>
#include "reflector.h"

using namespace std;

  // Adds a pair of elements to the Reflector map and returns 0 if there are no errors.
  // Returns an error code if there are errors.
int Reflector::configAddPairs(ifstream& rfConfig, string fileName) {
  int intA = 0, intB = 0;
  int count = 0;
  string inputStr;
  bool first = true;
  while (rfConfig >> inputStr) {
    for (char c : inputStr) {
      if (!isdigit(c)) return rfErrorMessage(4, fileName);
    }
    if (first == true) {
      intA = stoi(inputStr);
      if (intA < MIN_ALPHABET_INTEGER || intA > MAX_ALPHABET_INTEGER) return rfErrorMessage(3, fileName);
      first = false;
    } else {
      intB = stoi(inputStr);
      if (intB < MIN_ALPHABET_INTEGER || intB > MAX_ALPHABET_INTEGER) return rfErrorMessage(3, fileName);
      first = true;
      if (intA == intB) return rfErrorMessage(90, fileName);
      if (mapping[intA] != intA || mapping[intB] != intB) return rfErrorMessage(91, fileName);
      mapping[intA] = intB;
      mapping[intB] = intA;
    }
    rfConfig >> ws;
    count++;
  }
  if (count < NUM_OF_POSSIBLE_ALPHABETS && count % 2 == 0) return rfErrorMessage(100, fileName);
  if (count % 2 == 1) return rfErrorMessage(101, fileName);
  if (count > NUM_OF_POSSIBLE_ALPHABETS) return rfErrorMessage(102, fileName);
  return 0;
}

int Reflector::getMapValue(int value) {
  if (value >= MIN_ALPHABET_INTEGER && value <= MAX_ALPHABET_INTEGER) {
    return mapping[value];
  }
  cerr << "Input out of range for reflector mapping";
  return 3;
}

int Reflector::rfErrorMessage(int code, string fileName) {
  if (code == 0) return 0;
  switch (code) {
  case 3:
    cerr << "Invalid index between " << MIN_ALPHABET_INTEGER << " and " << "MAX_ALPHABET_INTEGER" << endl;
    break;
  case 4:
    cerr << "Non-numeric character in reflector file " << fileName << endl;
    break;
  case 90:
    cerr << "Trying to map input to itself in reflector file: " << fileName << endl;
    return 9;
  case 91:
    cerr << "Trying to pair an index with more than one other in reflector file: " << fileName << endl;
    return 9;
  case 100:
    cerr << "Insufficient number of mappings in reflector file: " << fileName << endl;
    return 10;
  case 101:
    cerr <<"Incorrect (odd) number of parameters in reflector file: " << fileName << endl;
    return 10;
  case 102:
    cerr << "More than " << NUM_OF_POSSIBLE_ALPHABETS << " parameters in reflector file: " << fileName << endl;
    return 10;
  }
  return code;
}
