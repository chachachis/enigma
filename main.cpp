#include <iostream>
#include <string>
#include "plugboard.h"
#include "reflector.h"
#include "rotors.h"
#include "globalconst.h"

using namespace std;

// Function that configures reflector, rotor(s) and plugboard based on
// command line arguments.
int config(int argc, char** argv);

// Function that outputs error messages to command line based on error code.
int configError(int code, string fileName = "BLANK");

// Function translates character input into encoded output by calling
// methods outlined in plugboard, reflector, rotor class objects.
char encoder(char input);

// Static plugboard and reflector objects created in main and used to call
// class methods from respective .h and .cpp files
static Plugboard pb;
static Reflector rf;


// Main programme calls config function to configure Enigma,
// then takes a message input and translates messages by character.
int main(int argc, char** argv) {
  
  int configError = config(argc, argv);
  if (configError !=0) exit(configError);

  string message;
  getline(cin, message);

  int i = 0;
  for (char c : message) {
    if (!isspace(c)){
      if (c < MIN_ALPHABET || c > MAX_ALPHABET) {
	cerr << c << " is not a valid input character (input characters must be upper case letters A-Z)!";
	return 2;
      }
      cout << encoder(c);
      i++; 
    }
  }
  return 0;
}

int config(int argc, char** argv) {

  // if there are fewer than 4 arguments, exit with INSUFFICIENT_NUMBER_OF_PARAMETERS code
  if (argc < 4) return configError(1);

  // Check if command line argument is a plugboard file
  ifstream pbConfigStream;
  string pbName = argv[1];
  if (pbName.substr(pbName.length() - 3, 3) != ".pb") return configError(1);

  // Opens plugboard file and calls plugboard configuration method.
  // If there is an error configuring plugboard object, programme exits with
  // error code and message defined in plugboard.cpp.
  string pbPathName  = "./";
  pbPathName.append(pbName);
  pbConfigStream.open(pbPathName);
  if (pbConfigStream.fail()) return configError(11, pbPathName);;
  int pbResult = pb.configAddPlugs(pbConfigStream, pbName);
  if (pbResult != 0) return pbResult;

  // Check if command line argument is a reflector file  
  ifstream rfConfigStream;
  string rfName = argv[2];
  if (rfName.substr(rfName.length() - 3, 3) != ".rf") return configError(1);

  // Opens reflector file and calls reflector configuration method.
  // If there is an error configuring reflector object, programme exits with
  // error code and message defined in reflector.cpp.
  string rfPathName = "./";
  rfPathName.append(rfName);
  rfConfigStream.open(rfPathName);
  if (rfConfigStream.fail()) return configError(11, rfName);;
  int rfResult = rf.configAddPairs(rfConfigStream, rfName);
  if (rfResult != 0) return rfResult;

  // Determine number of rotor arguments passed in command line
  int countRotors = 0;
  string rotorName = argv[argc - 2];
  while (rotorName.substr(rotorName.length() - 4, 4) == ".rot") {
    countRotors++;
    rotorName = argv[argc - 2 - countRotors];
  }

  // Checks if rotor position file is defined in command line
  if (countRotors + 3 > argc) return configError(1);
  string posName = argv[countRotors + 3];
  if (posName.substr(posName.length() - 4, 4) != ".pos") return configError(1); 

  // Calls rotor configuration method by passing in command line arguments
  // and number of rotors. If there is an error configuring rotor objects,
  // programme exits with error code and messages defined in rotor.cpp.
  int rotorConfigResult;
  rotorConfigResult = Rotor::configNRotors(argv, countRotors);
  if (rotorConfigResult !=0) return rotorConfigResult;
  
  return 0;
}

int configError(int code, string fileName) {
  if (code == 0) return 0;
  switch (code) {
  case 1:
    cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>)* rotor-positions" << endl;
    break;
  case 11:
    cerr << "Error opening configuration file " << fileName << endl;
    break;
  }
  return code;
}

char encoder(char input) {
  int temp;
  temp = static_cast<int>(input) - MIN_ALPHABET;
  temp = pb.getMapValue(temp);
  temp = Rotor::rotorRtoL(temp);
  temp = rf.getMapValue(temp);
  temp = Rotor::rotorLtoR(temp);
  temp = pb.getMapValue(temp);
  temp = temp + MIN_ALPHABET;
  return static_cast<char>(temp);
}
