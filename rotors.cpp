#include <iostream>
#include <fstream>
#include <string>
#include "rotors.h"

using namespace std;

// Declaration of static rotor class members:
// rotorCount records number of rotors in total
// rotors[99] stores a pointer to each rotor in an array of maximum size 99
int Rotor::rotorCount = 0;
Rotor * Rotor::rotors[99];


// Creates array of pointers to rotors in left to right order 
// based on array of filenames
int Rotor::configNRotors(char **argv, int count) {
  if (count == 0) {
    return 0;
  }

  ifstream rotorFile;
  string rotFileString;
  int errorcode = 0;
    
  for (int i = 0; i < count; i++) {
    rotFileString = "./";
    rotFileString.append(argv[3 + i]);
    rotorFile.open(rotFileString);
    if (rotorFile.fail()) {
      cout << "File " << rotFileString << " cannot be opened" << endl;
      exit(1);
    }
    rotors[i] = new Rotor;

    errorcode = rotors[i]->configRotor(rotorFile, argv[3 + i]);
    if (errorcode != 0) {
      return errorcode;
    }
    rotorFile.close();
  }

  //Open position file and populate offset attribute for each rotor in array of rotor pointers.
  ifstream rotorPositionStream;
  string posFileString = "./";
  posFileString.append(argv[3 + count]);
  
  rotorPositionStream.open(posFileString);
  if (rotorPositionStream.fail()) {
    cerr << "Error opening file " << posFileString;
    return 11;
  }
  int val = 0;
  int i = 0;
  string inputStr;
  while(rotorPositionStream >> inputStr && i < rotorCount) {
    for (char c : inputStr) {
      if (!isdigit(c)) {
	cerr << "Non-numeric character in rotor positions file " << argv[3 + count];
	return 4;
      }
    }
    val = stoi(inputStr);
    if (val < MIN_ALPHABET_INTEGER || val > MAX_ALPHABET_INTEGER) return 3;
    rotors[i]->offset = val;
    i++;
    rotorPositionStream >> ws;
  }
  if (i < rotorCount) {
    cerr << "No starting position for rotor " << i << " in rotor position file: " << argv[3 + count];
    return 8;
  }
  rotorPositionStream.close();
  return 0;
}


// Creates rotor object based on rotor input file stream
// and returns a pointer to that rotor object.
int Rotor::configRotor(ifstream& rotorStream, string fileName) {
  string inputStr;
  int next;
  int i = 0;

  // Position mapping for first 26 integers
  while (i < NUM_OF_POSSIBLE_ALPHABETS && rotorStream >> inputStr) {
    for (char c : inputStr) {
      if (!isdigit(c)) {
	cerr << "Non-numeric character for mapping in rotor file " << fileName;
	return 4;
      }
    }
    next = stoi(inputStr);
    if (next < MIN_ALPHABET_INTEGER || next > MAX_ALPHABET_INTEGER) return 3;
    if (checkMapped(i, next)) {
      cerr << "Invalid mapping of input " << i << " to output "		\
	   << next << " (output " << next << " is already mapped "	\
	   << "to from input " << getMapKey(next) << ")";
      return 7;
    }
    mapping[i] = next;
    i++;
  }

  // No mapping for some of 26 inputs
  if (rotorStream.eof() && i < NUM_OF_POSSIBLE_ALPHABETS) {
    cerr << "Not all inputs mapped in rotor file: " << fileName;
    return 7;
  }

  // Sets the ith position in the notches array to TRUE based on input from
  // rotorFile ifstream.
  while(rotorStream >> inputStr) {
    for (char c : inputStr) {
      if (!isdigit(c)) {
	cerr << "Non-numeric character indicated as a notch in rotor file " << fileName;
      }
    }
    next = stoi(inputStr);
    if (next < MIN_ALPHABET_INTEGER || next > MAX_ALPHABET_INTEGER) return 3;
    if (notches[next] == true) {
      cerr << "Notch position specified more than once in rotor file " << fileName;
      return 7;
    }
    notches[next] = true;
    rotorStream >> ws;
  }
  return 0;
}


// Called whenever rotorRtoL is called, to add one and update the offset positions of each rotor.
void Rotor::updateOffset() {
  rotors[rotorCount - 1]->offset++;
  rotors[rotorCount - 1]->offset = normalisePosition(rotors[rotorCount - 1]->offset);
  if (rotorCount >=2 && rotors[rotorCount - 1]->notches[rotors[rotorCount - 1]->offset]) {
    rotors[rotorCount - 2]->offset++;
    rotors[rotorCount - 2]->step = true;
    return;
  }
  // Stepping mechanism excludes right-most rotor
  int i = rotorCount - 2;
  while (i >= 0) {
    if (rotors[i]->step && rotors[i]->notches[rotors[i]->offset + 1]) {
      rotors[i]->offset++;
      if (i - 1 >= 0) rotors[i - 1]->offset++;
      rotors[i]->step = false;
      return;
    }
    i--;
  }
}

// Called to adjust an offset value when it is incremented above 25 or below 0
int Rotor::normalisePosition(int position) {
  while (position > MAX_ALPHABET_INTEGER) {
    position -= NUM_OF_POSSIBLE_ALPHABETS;
  }
  while (position < MIN_ALPHABET_INTEGER) {
    position += NUM_OF_POSSIBLE_ALPHABETS;
  }
  
  return position;
}

int Rotor::getMapValue(int position) {
  return mapping[position];
}

int Rotor::getMapKey(int value) {
  for (int i = MIN_ALPHABET_INTEGER; i < NUM_OF_POSSIBLE_ALPHABETS; i++) {
    if (mapping[i] == value) {
      return i;
    }
  }
  cerr << "Trying to get rotor mapping input value of invalid output value" << endl;
  return -1;
}

// Helper function of configRotor() to check if a specific output value has
// already been observed and entered into the rotor mapping array.
bool Rotor::checkMapped(int position, int val) {
  for (int i = 0; i < position; i++) {
    if (mapping[i] == val) {
      return true;
    }
  }
  return false;
}

// Functions rotorRtoL and rotorLtoR are called to encode each character.
int Rotor::rotorRtoL(int input) {
  if (rotorCount == 0) return input;
  updateOffset();
  for (int i = rotorCount; i > 0; i--) {
    input = normalisePosition(input + rotors[i-1]->offset);
    input = rotors[i-1]->getMapValue(input);
    input = normalisePosition(input - rotors[i-1]->offset);
  }
  return input;
}

int Rotor::rotorLtoR(int input) {
  if (rotorCount == 0) return input;
  for (int i = 0; i < rotorCount; i++) {
    input = normalisePosition(input + rotors[i]->offset);
    input = rotors[i]->getMapKey(input);
    input = normalisePosition(input - rotors[i]->offset);
  }
  return input;
}
