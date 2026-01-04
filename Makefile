CXX = g++
CXXFLAGS = -std=c++17 -I/home/sgo/SFML-3.0.2/include
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS = -L/home/sgo/SFML-3.0.2/lib

all: main

main: main.o BasicObject.o VisualObject.o StaticPhysicalObject.o PhysicalObject.o Player.o globals.o
	$(CXX) main.o BasicObject.o VisualObject.o StaticPhysicalObject.o PhysicalObject.o Player.o globals.o -o main $(LDFLAGS) $(LIBS)

main.o: main.cpp BasicObject.h VisualObject.h StaticPhysicalObject.h PhysicalObject.h enums.h globals.h
	$(CXX) $(CXXFLAGS) -c main.cpp

BasicObject.o: BasicObject.cpp BasicObject.h
	$(CXX) $(CXXFLAGS) -c BasicObject.cpp

VisualObject.o: VisualObject.cpp VisualObject.h enums.h
	$(CXX) $(CXXFLAGS) -c VisualObject.cpp

StaticPhysicalObject.o: StaticPhysicalObject.cpp StaticPhysicalObject.h
	$(CXX) $(CXXFLAGS) -c StaticPhysicalObject.cpp

PhyscialObject.o: PhysicalObject.cpp PhysicalObject.h enums.h
	$(CXX) $(CXXFLAGS) -c PhysicalObject.cpp

Player.o: Player.cpp Player.h globals.h
	$(CXX) $(CXXFLAGS) -c Player.cpp

globals.o: globals.cpp globals.h
	$(CXX) $(CXXFLAGS) -c globals.cpp

clean:
	rm -f *.o main
