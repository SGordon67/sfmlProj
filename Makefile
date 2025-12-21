CXX = g++
CXXFLAGS = -std=c++17
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: main

main: main.o
	$(CXX) main.o -o main -L/home/sgo/SFML-3.0.2/lib $(LIBS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -I/home/sgo/SFML-3.0.2/include

clean:
	rm -f *.o main
