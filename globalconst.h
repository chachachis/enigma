#ifndef GLOBALCONST_H
#define GLOBALCONST_H


// The range between MIN_ALPHABET_INTEGER and MAX_ALPHABET_INTEGER
// used in Enigma must be continuous
static const int MIN_ALPHABET_INTEGER = 0;
static const int MAX_ALPHABET_INTEGER = 25;
static const int NUM_OF_POSSIBLE_ALPHABETS = MAX_ALPHABET_INTEGER - MIN_ALPHABET_INTEGER + 1;
static const char MIN_ALPHABET = 'A';
static const char MAX_ALPHABET = 'Z';

#endif
