#pragma once

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )


#include "Entity.h"

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768
#define MAX_KEYS		256
#define MAX_SHOTS		1

class Game
{
public:
	Game();
	~Game();

	bool Init();
	bool LoadImages();
	void Release();
	
	bool Input();
	bool Update();
	void Draw();


private:
	SDL_Window *Window;
	SDL_Renderer *Renderer;
<<<<<<< Updated upstream
	SDL_Texture *img_background = nullptr, *img_player = nullptr, *img_shot = nullptr, *img_ball = nullptr, *img_enemy = nullptr, *img_life = nullptr;
=======
	SDL_Texture *img_background = nullptr, *img_player = nullptr, *img_shot = nullptr, *EnemyRect = nullptr, *img_enemy = nullptr, 
		* img_puntuacion5 = nullptr,
		* img_puntuacion4 = nullptr,
		* img_puntuacion3 = nullptr,
		* img_puntuacion2 = nullptr,
		* img_puntuacion1 = nullptr,
		* img_puntuacion0 = nullptr;
>>>>>>> Stashed changes

	Entity Player, Shots[MAX_SHOTS], Scene, Enemy, Ball, Puntuacion1, Puntuacion2, EnemyShoot[MAX_SHOTS];
	int idx_shot, idx_enemy_shot;

	bool god_mode;

	unsigned int punt1 = 5;
	unsigned int punt2 = 5;
	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP	};
	KEY_STATE keys[MAX_KEYS]; 
};
