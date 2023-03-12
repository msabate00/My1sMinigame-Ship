#include "Game.h"
#include <math.h>
#include <cmath>
#include <stdio.h>
#include "SDL_mixer/include/SDL_mixer.h"

#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )



Game::Game() {}
Game::~Game(){}

bool enemyMoveUp = true;
bool ballToEnemy = true;
float ballDirX = 1, ballDirY = 1;
int hitFrameCount = 0;
bool canShoot = true;
bool iscollide = false;
bool iaController = true;


bool Game::Init()
{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create our window: title, x, y, w, h, flags
	Window = SDL_CreateWindow("Spaceship: arrow keys + space, f1: god mode, R: Change to PvP or PVE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}
	//Create a 2D rendering context for a window: window, device index, flags
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if (Renderer == NULL)
	{
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}
	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;

	//Load images
	if (!LoadImages())
		return false;

	//Init variables
	//size: 104x82
	Player.Init(20, WINDOW_HEIGHT >> 1, 91, 105, 7);
	idx_shot = 0;
	idx_enemy_shot = 0;
	Life.Init(20, 0, 104, 82, NULL);//Lin
	int w;
	SDL_QueryTexture(img_background, NULL, NULL, &w, NULL);
	Scene.Init(0, 0, w, WINDOW_HEIGHT, 4);
	god_mode = false;

	
	//Enemigos
	Enemy.Init(WINDOW_WIDTH - 124, WINDOW_HEIGHT / 2, 91, 105, 7);

	// BACKGROUND MUSIC
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	Mix_Music* musica = Mix_LoadMUS("assets/Mozart.ogg");
	Mix_PlayMusic(musica, -1);

	return true;
}
bool Game::LoadImages()
{
	if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Log("IMG_Init, failed to init required png support: %s\n", IMG_GetError());
		return false;
	}
	img_background = SDL_CreateTextureFromSurface(Renderer, IMG_Load("background.png"));
	if (img_background == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_player = SDL_CreateTextureFromSurface(Renderer, IMG_Load("spaceship_2.png"));
	if (img_player == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_shot = SDL_CreateTextureFromSurface(Renderer, IMG_Load("shot.png"));
	if (img_shot == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	img_enemy = SDL_CreateTextureFromSurface(Renderer, IMG_Load("nave_malo_2.png"));
	if (img_enemy == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	EnemyRect = SDL_CreateTextureFromSurface(Renderer, IMG_Load("ball.png"));
	if (EnemyRect == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	/*img_life = SDL_CreateTextureFromSurface(Renderer, IMG_Load("life.png"));
	if (img_life == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}*/

	return true;
}
void Game::Release()
{
	//Release images
	SDL_DestroyTexture(img_background);
	SDL_DestroyTexture(img_player);
	SDL_DestroyTexture(img_shot);
	SDL_DestroyTexture(img_enemy);
	SDL_DestroyTexture(EnemyRect);
	//SDL_DestroyTexture(img_life);
	IMG_Quit();
	
	//Clean up all SDL initialized subsystems
	SDL_Quit();
}
bool Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	return true;
}
bool Game::Update()
{
	hitFrameCount++;
	//Read Input
	if (!Input())	return true;

	//Process Input
	float fx = 0, fy = 0;
	float ex = 0, ey = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_F1] == KEY_DOWN)		god_mode = !god_mode;
	if (keys[SDL_SCANCODE_J] == KEY_DOWN) vida -= 1;//Lin Si pulsa j resta vida
	if (keys[SDL_SCANCODE_W] == KEY_REPEAT)	fy = -1;
	if (keys[SDL_SCANCODE_S] == KEY_REPEAT)	fy = 1;
	if (keys[SDL_SCANCODE_UP] == KEY_REPEAT)	ey = -1;
	if (keys[SDL_SCANCODE_DOWN] == KEY_REPEAT)	ey = 1;
	if (keys[SDL_SCANCODE_R] == KEY_DOWN)	iaController = !iaController;
	//if (keys[SDL_SCANCODE_LEFT] == KEY_REPEAT)	fx = -1;
	//if (keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT)	fx = 1;
	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN && canShoot)
	{
		int x, y, w, h;
		Player.GetRect(&x, &y, &w, &h);
		//size: 56x20
		//offset from player: dx, dy = [(29, 3), (29, 59)]
		
		Ball.Init(x+Player.GetWidth() , y + (Player.GetHeight()/2)-15 , 30, 30, 6);
		//idx_shot++;
		//idx_shot %= MAX_SHOTS;
		canShoot = false;
		/*Shots[idx_shot].Init(x + 29, y + 59, 56, 20, 10);
		idx_shot++;
		idx_shot %= MAX_SHOTS;*/

		// EFECTO DE SONIDO AL DISPARAR
		Mix_Chunk* sonido = Mix_LoadWAV("assets/Sonido_Bola.wav");
		if (sonido == NULL)
		{
			printf("Error al cargar el sonido: %s\n", Mix_GetError());
		}
		else
		{
			if (Mix_PlayChannel(-1, sonido, 0) < 0)
			{
				printf("Error al cargar el sonido: %s\n", Mix_GetError());
			}
		}

		
	}
	if (keys[SDL_SCANCODE_K] == KEY_DOWN)
	{
		//Mix_Music* musica = Mix_LoadMUS("assets/stage1.ogg");
		Mix_Chunk* sonido = Mix_LoadWAV("assets/bip.wav");
		if (sonido == NULL)
		{
			printf("Error al cargar el sonido: %s\n", Mix_GetError());
		}
		else
		{
			if (Mix_PlayChannel(-1, sonido, 0) < 0)
			{
				printf("Error al cargar el sonido: %s\n", Mix_GetError());
			}
		}
		
	}

	//Logic
	//Scene scroll
	Scene.SetDir(-1, 0);
	Scene.Move();
	if (Scene.GetX() <= -Scene.GetWidth())	Scene.SetX(0);
	//Player update
	
	Player.SetDir(fx, fy);
	Player.Move();
	if (Player.GetY() < 0) {
		Player.SetY(0);
	}
	else if (Player.GetY() > WINDOW_HEIGHT - Player.GetHeight()) {
		Player.SetY(WINDOW_HEIGHT - Player.GetHeight());
	}

	//Enemy logic

	//Si es controlado por la IA
	if (iaController) {
		Enemy.SetSpeed(4.685);

		if (Ball.IsAlive()) {

			if (Ball.GetY() > Enemy.GetY() + Enemy.GetHeight() / 2)
			{
				Enemy.SetY(Enemy.GetY() + Enemy.GetSpeed());
			}
			if (Ball.GetY() < Enemy.GetY() + Enemy.GetHeight() / 2)
			{
				Enemy.SetY(Enemy.GetY() - Enemy.GetSpeed());
			}

		}
		else {
			Enemy.SetY(Player.GetY());
		}
	} //Sino
	else {
		Enemy.SetSpeed(7);
		Enemy.SetDir(ex, ey);
		Enemy.Move();

		//Ajuste de limites del movimiento
		if (Enemy.GetY() < 0) {
			Enemy.SetY(0);
		}
		else if (Enemy.GetY() > WINDOW_HEIGHT - Enemy.GetHeight()) {
			Enemy.SetY(WINDOW_HEIGHT - Enemy.GetHeight());
		}

	}




	


	int x, y, w, h;
	Ball.GetRect(&x, &y, &w, &h);
	SDL_Rect BallRect;
	BallRect.x = x; 
	BallRect.y = y; 
	BallRect.w = w; 
	BallRect.h = h; 


	Enemy.GetRect(&x, &y, &w, &h);
	SDL_Rect EnemyRect;
	EnemyRect.x = x;
	EnemyRect.y = y;
	EnemyRect.w = w;
	EnemyRect.h = h;

	Player.GetRect(&x, &y, &w, &h);
	SDL_Rect PlayerRect;
	PlayerRect.x = x;
	PlayerRect.y = y;
	PlayerRect.w = w;
	PlayerRect.h = h;

	


	//Ball logic

	//Colision con jugador y enemigo
	if ((SDL_HasIntersection(&BallRect, &EnemyRect) || SDL_HasIntersection(&BallRect, &PlayerRect)) && hitFrameCount > 20 ) {
		
		// EFECTO AL CHOCAR BOLA CON JUGADOR/ENEMIGO
		Mix_Chunk* sonido = Mix_LoadWAV("assets/Sonido_Bola.wav");
		if (sonido == NULL)
		{
			printf("Error al cargar el sonido: %s\n", Mix_GetError());
		}
		else
		{
			if (Mix_PlayChannel(-1, sonido, 0) < 0)
			{
				printf("Error al cargar el sonido: %s\n", Mix_GetError());
			}
		}

		//Colision enemigo
		if (SDL_HasIntersection(&BallRect, &EnemyRect)) {
			ballDirY = ((Ball.GetY() + (Ball.GetHeight() / 2)) - (Enemy.GetY() + (Enemy.GetHeight() / 2)) * 2) / (Ball.GetY() + Enemy.GetY());

			

			if (Ball.GetY() + (Ball.GetHeight() / 2) >= Enemy.GetY() + (Enemy.GetHeight() / 2)) {
				ballDirY = -ballDirY;
			}
		}
		
		//Colision jugador
		if (SDL_HasIntersection(&BallRect, &PlayerRect)) {


			if ((Ball.GetY() + (Ball.GetHeight() / 2)) < (Player.GetY() + (Player.GetHeight() / 2))) {
				ballDirY = ((Ball.GetY() + (Ball.GetHeight() / 2)) - (Player.GetY() + (Player.GetHeight() / 2)) * 2) / (Ball.GetY() + Player.GetY());
			}
			else {
				ballDirY = -((Ball.GetY() + (Ball.GetHeight() / 2)) - (Player.GetY() + (Player.GetHeight() / 2)) * 2) / (Ball.GetY() + Player.GetY());
			}

		}

		if (ballDirY > 1) ballDirY = 1;
		if (ballDirY < -1) ballDirY = -1;
		

		//Cambia la direccion de la bola
		ballToEnemy = !ballToEnemy;
		ballDirX = -ballDirX;

		//Cambia la velocidad de la bola
		ballDirX *= 1.025;
		
		
		

		//Resetea el hitFrameCount
		hitFrameCount = 0;

	}
	else {

		

		//Si se sale por arriba o por abajo, invierte la velocidad vertical
		
		if ((Ball.GetY() + Ball.GetHeight() > WINDOW_HEIGHT)) {
			Ball.SetY(WINDOW_HEIGHT - Ball.GetHeight());
			ballDirY = -abs(ballDirY);
		}
		if ((Ball.GetY() < 0)) {
			Ball.SetY(0);
			ballDirY = abs(ballDirY);
		}


		//Si se sale por los lados, resetea la bola
		if (Ball.GetX() < 0 || Ball.GetX() > WINDOW_WIDTH) {

			if (!god_mode) {
				Ball.ShutDown();
				canShoot = true;
				ballDirX = 1;
				ballDirY = 1;
			}
			else {
				ballDirX = -ballDirX;
				Mix_Chunk* sonido = Mix_LoadWAV("assets/Sonido_Bola.wav");
				if (Mix_PlayChannel(-1, sonido, 0) < 0)
				{
					printf("Error al cargar el sonido: %s\n", Mix_GetError());
				}
			}
			
		}

		//Pone su direccion
		Ball.SetDir(ballDirX, ballDirY);

		//Se mueve
		Ball.Move();

		
	}

	
		
	return false;
}
void Game::Draw()
{
	SDL_Rect rc;

	//Set the color used for drawing operations
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	//Clear rendering target
	SDL_RenderClear(Renderer);

	//God mode uses red wireframe rectangles for physical objects
	if (god_mode) SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);

	//Draw scene
	Scene.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, img_background, NULL, &rc);
	rc.x += rc.w;
	SDL_RenderCopy(Renderer, img_background, NULL, &rc);
	
	//Draw player
		//Draw player
	if (vida <= 0) {
	
	}
	else {
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_player, NULL, &rc);
		if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
	}


	Enemy.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, img_enemy, NULL, &rc);
	if (god_mode) SDL_RenderDrawRect(Renderer, &rc);


	//Draw Life
	/*Life.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	for (int i = 0; i < vida; i++)
	{
		SDL_RenderCopy(Renderer, img_life, NULL, &rc);
		rc.x += rc.w;  // Move the rect to the right by its width
	}*/
	
	//Draw shots


	if (Ball.IsAlive()) {
		Ball.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, EnemyRect, NULL, &rc);
		if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
	}
	

	if (god_mode) {
	
		SDL_RenderDrawLine(Renderer, 3, 0, 3, WINDOW_HEIGHT);
	
	}

	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(10);	// 1000/10 = 100 fps max
}



