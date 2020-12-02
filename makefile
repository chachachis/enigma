enigma: main.o plugboard.o reflector.o rotors.o globalconst.h
	g++ -g main.o plugboard.o reflector.o rotors.o -o enigma
main.o: main.cpp plugboard.h reflector.h rotors.h globalconst.h
	g++ -Wall -g -c main.cpp
plugboard.o: plugboard.cpp plugboard.h globalconst.h
	g++ -Wall -g -c plugboard.cpp
reflector.o: reflector.cpp reflector.h globalconst.h
	g++ -Wall -g -c reflector.cpp
rotors.o: rotors.cpp rotors.h globalconst.h
	g++ -Wall -g -c rotors.cpp
.PHONY: clean
clean:
	rm -f main.o plugboard.o reflector.o rotors.o
