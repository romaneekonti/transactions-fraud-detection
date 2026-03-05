CXX = g++
LIBS = -lncurses

program: main.cpp
	$(CXX) main.cpp -o program $(LIBS)