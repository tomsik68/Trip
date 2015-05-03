all: Trip

Trip: main.cpp
	g++ main.cpp -o Trip -lmingw32 -lsdl2main -lsdl2 -mwindows
