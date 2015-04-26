#include <iostream>
#include <stdlib.h>
#include <SDL/SDL.h>
#

using namespace std;

#define WIDTH  640
#define HEIGHT 480
#define FOV    75
#define FLOOR_COLOR   0x787878 
#define PORTAL_COLOR  0X6BCBFF
#define CEILING_COLOR 0xFFA270

struct xy{
	float x,y;
};

struct Sector{
	float floor,ceil;
	xy* vertices;
	unsigned cpoints;
};

struct Scene{
	Sector* sectors;
	unsigned csectors;
};

struct Player{
	struct xyz   { float x,y,z; } position, velocity;
	struct gonio { float value,sin,cos; } angle;
	unsigned sector;
};

SDL_Event event;
SDL_Surface* screen;
Scene  scene;
Player player;
bool running;

void logDebug(const char* msg){
	cout << "DEBUG: " << msg << endl;
}

void drawLine(int x1, int y1, int x2, int y2, int color){
	logDebug("Drawing line...");
	int* pix = (int*) screen->pixels;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int y;
	cout << dx << " " << dy << endl;
	cout << x1 << " " << y1 << " | " << x2 << " " << y2 << endl;
	int s1 = min(x1,x2);
	int s2 = max(x1,x2);
 	for (int x = s1; x < s2; ++x){
		y = y1 + dy * (x - x1) / dx;
		cout << x << " " << y << endl;
		pix[x+y*WIDTH] = color;	
	}
}

void vline(int x, int y1, int y2){
	logDebug("Drawing vertical line...");
	cout << x << " " << y1 << " " << y2 << endl;
	int* pix = (int*) screen->pixels;
	for(int y = 0;  y < y1; ++y)     pix[x+y*WIDTH] = CEILING_COLOR;
	for(int y = y1; y < y2; ++y)     pix[x+y*WIDTH] = PORTAL_COLOR;
	for(int y = y2; y < HEIGHT; ++y) pix[x+y*WIDTH] = FLOOR_COLOR;
}

int createScene(){
	scene.sectors = new Sector[1];
	Sector* sector = &scene.sectors[0];
	sector->floor = 0;
	sector->ceil  = 1;
	sector->vertices = new xy[4];
	sector->vertices[0].x = 1;
	sector->vertices[0].y = 1;
	sector->vertices[1].x = -1;
	sector->vertices[1].y = 1;
	sector->vertices[2].x = -1;
	sector->vertices[2].y = -1;
	sector->vertices[3].x = 1;
	sector->vertices[3].y = -1;
	sector->cpoints = 4;
	scene.csectors = 1;
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
	createScene();
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
	//for(int i = 0; i < WIDTH; ++i) vline(i,100,300);
	for(int i = 0; i < 1; ++i){
		xy p1 = scene.sectors[0].vertices[i];
		xy p2;
		if (i == scene.sectors[0].cpoints) p2 = scene.sectors[0].vertices[0]; else p2 = scene.sectors[0].vertices[i+1];
		
		int x1  = (p1.x * FOV) / p1.y;
		int y1a = HEIGHT - (scene.sectors[i].ceil  * FOV) / p1.y;
		int y1b = HEIGHT - (scene.sectors[i].floor * FOV) / p1.y;

		int x2  = (p2.x * FOV) / p2.y;
		int y2a = HEIGHT - (scene.sectors[i].ceil  * FOV) / p2.y;
		int y2b = HEIGHT - (scene.sectors[i].floor * FOV) / p2.y;

		for (int x = x2; x < x1; ++x){
			int ya = y1a + (x - x1) * (y2a - y1a) / (x2 - x1);
			int yb = y1b + (x - x1) * (y2b - y1b) / (x2 - x1);

			vline(x+FOV,ya,yb);
		}

	};
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
