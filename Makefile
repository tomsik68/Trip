all: Trip

Trip: main.cpp
	g++ main.cpp -o Trip -lmingw32 -lsdlmain -lsdl -mwindows
