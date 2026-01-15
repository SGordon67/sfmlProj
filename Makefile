CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I/home/sgo/SFML-3.0.2/include
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS = -L/home/sgo/SFML-3.0.2/lib

#Object files
OBJS = main.o VisualObject.o HealthCrate.o Ball.o Player.o Spikey.o globals.o
all: main

main: $(OBJS)
	$(CXX) $(OBJS) -o main $(LDFLAGS) $(LIBS)

main.o: main.cpp globals.h enums.h BasicObject.h VisualObject.h HealthCrate.h Ball.h Entity.h Player.h Spikey.h Interactable.h
	$(CXX) $(CXXFLAGS) -c main.cpp

globals.o: globals.cpp globals.h enums.h
	$(CXX) $(CXXFLAGS) -c globals.cpp

VisualObject.o: VisualObject.cpp VisualObject.h BasicObject.h globals.h enums.h
	$(CXX) $(CXXFLAGS) -c VisualObject.cpp

HealthCrate.o: HealthCrate.cpp HealthCrate.h Crate.h VisualObject.h Interactable.h globals.h enums.h
	$(CXX) $(CXXFLAGS) -c HealthCrate.cpp

Ball.o: Ball.cpp Ball.h PhysicalObject.h globals.h enums.h
	$(CXX) $(CXXFLAGS) -c Ball.cpp

Player.o: Player.cpp Player.h Entity.h globals.h enums.h
	$(CXX) $(CXXFLAGS) -c Player.cpp

Spikey.o: Spikey.cpp Spikey.h Entity.h globals.h enums.h
	$(CXX) $(CXXFLAGS) -c Spikey.cpp

clean:
	rm -f *.o main

.PHONY: all clean
