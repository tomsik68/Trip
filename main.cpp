#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <SDL2/SDL.h>

using namespace std;

#define WIDTH  640
#define HEIGHT 480
#define FOV    75
#define SPEED  2
#define ASPEED 3.14/16
#define FLOOR_COLOR   0x787878 
#define PORTAL_COLOR  0X6BCBFF
#define CEILING_COLOR 0xFFA270

struct Point3D{
	float x,y,z;
};

struct Point2D{
	float x,y;
};

struct Transform{
	Point3D translate;
	Point3D rotate;
};

struct Sector{
	float floor,ceil;
	Point2D* vertices;
	unsigned cpoints;
};

struct Scene{
	Sector* sectors;
	unsigned csectors;
};

struct Player{
	Point3D position, velocity;
	struct gonio { float value,sin,cos; } angle;
	unsigned sector;
};

SDL_Event event;
SDL_Window* window;
SDL_Renderer* renderer;
Transform transform;
Scene  scene;
Player player;
Point3D p1,p2;
map<int,bool> keys;
bool running;

void logDebug(const char* msg){
	cout << "DEBUG: " << msg << endl;
}

int createScene(){
	transform.translate.x = WIDTH / 2;
	transform.translate.y = HEIGHT / 2;
	transform.translate.z = 0;
	transform.rotate.x = 0;
	transform.rotate.y = 0;
	transform.rotate.z = 0;
	player.position.x = 0;
	player.position.y = 0;
	player.position.z = 0;
	player.velocity.x = 0;
	player.velocity.y = 0;
	player.velocity.z = 0;
	player.angle.value = 0;
	player.angle.sin = 0;
	player.angle.cos = 1;
	p1.x = -100;
	p1.y = -100;
	p2.x =  100;
	p2.y = -100;
}

int init(){
	if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
	window = SDL_CreateWindow("TRIP_PRE-ALPHA",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (window == NULL) return 1;
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
				case SDLK_w:
					keys[SDLK_w] = true;
					break;
				case SDLK_s:
					keys[SDLK_s] = true;
					break;
				case SDLK_a:
					keys[SDLK_a] = true;
					break;
				case SDLK_d:
					keys[SDLK_d] = true;
					break;
				case SDLK_LEFT:
					keys[SDLK_LEFT] = true;
					break;
				case SDLK_RIGHT:
					keys[SDLK_RIGHT] = true;
					break;
			}
			break;
		case SDL_KEYUP:
			switch(event.key.keysym.sym){
				case SDLK_w:
					keys[SDLK_w] = false;
					break;
				case SDLK_s:
					keys[SDLK_s] = false;
					break;
				case SDLK_a:
					keys[SDLK_a] = false;
					break;
				case SDLK_d:
					keys[SDLK_d] = false;
					break;
				case SDLK_LEFT:
					keys[SDLK_LEFT] = false;
					break;
				case SDLK_RIGHT:
					keys[SDLK_RIGHT] = false;
					break;
			}
		}
	}
}

void update(){

	if(keys[SDLK_w]) player.velocity.y -= SPEED;
	if(keys[SDLK_s]) player.velocity.y += SPEED;
	if(keys[SDLK_a]) player.velocity.x -= SPEED;
	if(keys[SDLK_d]) player.velocity.x += SPEED;
	
	if(keys[SDLK_LEFT]){
		player.angle.value -= ASPEED;
		player.angle.sin = sin(player.angle.value);
		player.angle.cos = cos(player.angle.value);
	}
	
	if(keys[SDLK_RIGHT]){
		player.angle.value += ASPEED;
		player.angle.sin = sin(player.angle.value);
		player.angle.cos = cos(player.angle.value);
	}

	player.position.x -= player.velocity.y*player.angle.cos;
	player.position.y -= player.velocity.y*player.angle.sin;

	player.position.x -= player.velocity.x*player.angle.sin;
	player.position.y -= player.velocity.x*player.angle.cos;

	player.position.z += player.velocity.z;

	player.velocity.x = 0;
	player.velocity.y = 0;
	player.velocity.z = 0;
}

void render(){
	SDL_SetRenderDrawColor(renderer
			,255
			,255
			,255
			,255);
	SDL_RenderDrawLine(renderer
			,p1.x+transform.translate.x
			,p1.y+transform.translate.y
			,p2.x+transform.translate.x
			,p2.y+transform.translate.y);
	SDL_RenderDrawPoint(renderer
			,player.position.x+transform.translate.x
			,player.position.y+transform.translate.y);
	SDL_SetRenderDrawColor(renderer
			,255
			,0
			,0
			,255);
	SDL_RenderDrawLine(renderer
			,player.position.x+transform.translate.x
			,player.position.y+transform.translate.y
			,player.position.x+transform.translate.x+player.angle.cos*8
			,player.position.y+transform.translate.y+player.angle.sin*8);
	
}

void loop(){
	running = true;
	while (running){
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		onEvent();
		update();
		render();
		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}
}

void cleanUp(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* argv[]){
	if (init() != 0) return 1;
	loop();
	cleanUp();
	return 0;
}
