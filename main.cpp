#include <iostream>
#include <stdlib.h>
#include <SDL/SDL.h>

using namespace std;

#define WIDTH  640
#define HEIGHT 480
#define FLOOR_COLOR   0x787878 
#define PORTAL_COLOR  0X6BCBFF
#define CEILING_COLOR 0xFFA270

struct Sector{
	
};

struct Scene{
	
};

struct Player{
	struct xyz   { float x,y,z; } position, velocity;
	struct gonio { float value,sin,cos;} angle;
	unsigned sector;
};

SDL_Event event;
SDL_Surface* screen;
bool running;

void logDebug(const char* msg){
	cout << "DEBUG: " << msg << endl;
}

void vline(int x, int y1, int y2){
	int* pix = (int*) screen->pixels;
	for(int y = 0;  y < y1; ++y)     pix[x+y*WIDTH] = CEILING_COLOR;
	for(int y = y1; y < y2; ++y)     pix[x+y*WIDTH] = PORTAL_COLOR;
	for(int y = y2; y < HEIGHT; ++y) pix[x+y*WIDTH] = FLOOR_COLOR;
}

int init(){
	logDebug("Initializing SDL2...");
	if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
	logDebug("SDL2 initialized.");
	logDebug("Creating window...");
	SDL_WM_SetCaption("Trip Test","Trip Test");
	screen = SDL_SetVideoMode(WIDTH,HEIGHT,0,0);
	if (screen == NULL) return 1;
	logDebug("Window created.");
	return 0;
}

void onEvent(){
	if (SDL_PollEvent(&event)){
		switch(event.type){
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
				case SDLK_ESCAPE:
					running = false;
					break;
			}
			break;
		}
	}
}

void update(){

}

void render(){
	/*int* pix = (int*) screen->pixels;
	for(int y=0; y < HEIGHT; ++y)
		for(int x=0; x < WIDTH; ++x) pix[x+y*WIDTH] = rand() % (256*256*256);*/
	for(int i = 0; i < WIDTH; ++i) vline(i,100,300);
}

void loop(){
	running = true;
	while (running){
		onEvent();
		update();
		render();
		SDL_UpdateRect(screen,0,0,0,0);
	}
}

void cleanUp(){
	SDL_FreeSurface(screen);
	SDL_Quit();
}

int main(int argc, char* argv[]){
	logDebug("Starting program.");
	if (init() != 0) return 1;
	logDebug("Starting loop.");
	loop();
	logDebug("Cleaning up");
	cleanUp();
	return 0;
}
