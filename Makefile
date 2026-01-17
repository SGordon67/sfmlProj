CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I/home/sgo/SFML-3.0.2/include
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS = -L/home/sgo/SFML-3.0.2/lib

#Object files
OBJS = main.o globals.o VisualObject.o HealthCrate.o Ball.o Player.o Spikey.o UIHealth.o
all: main

main: $(OBJS)
	$(CXX) $(OBJS) -o main $(LDFLAGS) $(LIBS)

main.o: main.cpp globals.h enums.h BasicObject.h VisualObject.h PhysicalObject.h Ball.h HealthCrate.h Entity.h Player.h Spikey.h UIElement.h UIHealth.h Interactable.h Hazardous.h
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

UIHealth.o: UIHealth.cpp UIHealth.h UIElement.h Player.h globals.h enums.h
	$(CXX) $(CXXFLAGS) -c UIHealth.cpp

clean:
	rm -f *.o main

.PHONY: all clean
