CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I/home/sgo/SFML-3.0.2/include
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS = -L/home/sgo/SFML-3.0.2/lib

#Object files
OBJS = main.o VisualObject.o Crate.o StaticPhysicalObject.o PhysicalObject.o Player.o globals.o
all: main

main: $(OBJS)
	$(CXX) $(OBJS) -o main $(LDFLAGS) $(LIBS)

main.o: main.cpp globals.h enums.h BasicObject.h VisualObject.h Crate.h StaticPhysicalObject.h PhysicalObject.h Entity.h Player.h Interactable.h
	$(CXX) $(CXXFLAGS) -c main.cpp

globals.o: globals.cpp globals.h
	$(CXX) $(CXXFLAGS) -c globals.cpp

VisualObject.o: VisualObject.cpp VisualObject.h BasicObject.h
	$(CXX) $(CXXFLAGS) -c VisualObject.cpp

Crate.o: Crate.cpp Crate.h VisualObject.h Interactable.h
	$(CXX) $(CXXFLAGS) -c Crate.cpp

StaticPhysicalObject.o: StaticPhysicalObject.cpp StaticPhysicalObject.h BasicObject.h
	$(CXX) $(CXXFLAGS) -c StaticPhysicalObject.cpp

PhysicalObject.o: PhysicalObject.cpp PhysicalObject.h BasicObject.h
	$(CXX) $(CXXFLAGS) -c PhysicalObject.cpp

Player.o: Player.cpp Player.h Entity.h
	$(CXX) $(CXXFLAGS) -c Player.cpp

clean:
	rm -f *.o main

.PHONY: all clean
