# CXX = g++
# CXXFLAGS = -std=c++17 -Wall -Wextra -I/home/sgo/SFML-3.0.2/include
# LIBS = -lsfml-graphics -lsfml-window -lsfml-system
# LDFLAGS = -L/home/sgo/SFML-3.0.2/lib

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g -MMD -MP
SFML_DIR = /home/sgo/SFML-3.0.2
# SFML_DIR = /home/bomsai/SFML-3.0.2
CXXFLAGS += -I$(SFML_DIR)/include
LDFLAGS = -L$(SFML_DIR)/lib
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

#Object files

OBJS = main.o globals.o VisualObject.o HealthCrate.o Ball.o Player.o Spikey.o Enemy1.o CircleWeapon.o Minimap.o UIHealth.o UIKillCount.o QuadTree.o
all: main

main: $(OBJS)
	$(CXX) $(OBJS) -o main $(LDFLAGS) $(LIBS)

main.o: main.cpp globals.h enums.h MainMenu.h SettingsScreen.h GameOverScreen.h Resolution.h BasicObject.h VisualObject.h PhysicalObject.h Ball.h HealthCrate.h Entity.h Player.h Spikey.h Enemy.h Enemy1.h CircleWeapon.h Minimap.h UIElement.h UIHealth.h UIKillCount.h QuadTree.h Interactable.h Hazardous.h
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

Enemy1.o: Enemy1.cpp Enemy1.h Enemy.h globals.h enums.h
	$(CXX) $(CXXFLAGS) -c Enemy1.cpp

CircleWeapon.o: CircleWeapon.cpp CircleWeapon.h Weapon.h globals.h enums.h
	$(CXX) $(CXXFLAGS) -c CircleWeapon.cpp

Minimap.o: Minimap.cpp Minimap.h UIElement.h globals.h enums.h
	$(CXX) $(CXXFLAGS) -c Minimap.cpp

UIHealth.o: UIHealth.cpp UIHealth.h UIElement.h Player.h globals.h enums.h
	$(CXX) $(CXXFLAGS) -c UIHealth.cpp

UIKillCount.o: UIKillCount.cpp UIKillCount.h UIElement.h globals.h enums.h
	$(CXX) $(CXXFLAGS) -c UIKillCount.cpp

QuadTree.o: QuadTree.cpp QuadTree.h globals.h enums.h
	$(CXX) $(CXXFLAGS) -c QuadTree.cpp

clean:
	rm -f *.o *.d main

.PHONY: all clean
