#include "Game.h"
#include <math.h>

Game::Game() {}
Game::~Game(){}

bool enemyMoveUp = true;
bool ballToRight = true;
int framecount = 0;
bool canShoot = true;

bool Game::Init()
{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create our window: title, x, y, w, h, flags
	Window = SDL_CreateWindow("Spaceship: arrow keys + space, f1: god mode", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
	Player.Init(20, WINDOW_HEIGHT >> 1, 104, 82, 5);
	idx_shot = 0;
	idx_enemy_shot = 0;
	Life.Init(20, 0, 104, 82, NULL);//Lin
	int w;
	SDL_QueryTexture(img_background, NULL, NULL, &w, NULL);
	Scene.Init(0, 0, w, WINDOW_HEIGHT, 4);
	god_mode = false;

	
	//Enemigos
	Enemy.Init(WINDOW_WIDTH - 124, WINDOW_HEIGHT / 2, 104, 104, 6);

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
	img_player = SDL_CreateTextureFromSurface(Renderer, IMG_Load("spaceship.png"));
	if (img_player == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_shot = SDL_CreateTextureFromSurface(Renderer, IMG_Load("shot.png"));
	if (img_shot == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	img_enemy = SDL_CreateTextureFromSurface(Renderer, IMG_Load("nave_malo.png"));
	if (img_enemy == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_ball = SDL_CreateTextureFromSurface(Renderer, IMG_Load("ball.png"));
	if (img_ball == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	img_life = SDL_CreateTextureFromSurface(Renderer, IMG_Load("life.png"));
	if (img_life == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	return true;
}
void Game::Release()
{
	//Release images
	SDL_DestroyTexture(img_background);
	SDL_DestroyTexture(img_player);
	SDL_DestroyTexture(img_shot);
	SDL_DestroyTexture(img_enemy);
	SDL_DestroyTexture(img_ball);
	SDL_DestroyTexture(img_life);
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
	framecount++;
	//Read Input
	if (!Input())	return true;

	//Process Input
	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_F1] == KEY_DOWN)		god_mode = !god_mode;
	if (keys[SDL_SCANCODE_J] == KEY_DOWN) vida -= 1;//Lin Si pulsa j resta vida
	if (keys[SDL_SCANCODE_UP] == KEY_REPEAT)	fy = -1;
	if (keys[SDL_SCANCODE_DOWN] == KEY_REPEAT)	fy = 1;
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

		
	}

	//Logic
	//Scene scroll
	Scene.Move(-1, 0);
	if (Scene.GetX() <= -Scene.GetWidth())	Scene.SetX(0);
	//Player update
	Player.Move(fx, fy);
	//Shots update
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].Move(1, 0);
			if (Shots[i].GetX() > WINDOW_WIDTH || Shots[i].GetX() < 0) {
				canShoot = true;
				Shots[i].ShutDown();
			}
		}
	}

	//Enemy logic

	if (Enemy.GetY() + Enemy.GetHeight() >= WINDOW_HEIGHT && enemyMoveUp == true ) {
		enemyMoveUp = false;
	}
	else if(Enemy.GetY() <= 0 && enemyMoveUp == false){
		enemyMoveUp = true;
	}
	if (enemyMoveUp) {
		//Enemy.Move(0, 1);
	}
	else {
		//Enemy.Move(0, -1);
	}
	Enemy.Move(0, 0);


	//Ball logic
	Ball.Move(1,1);






	//ESTO HACE QUE EL ENEMIGO DISPARE, PARA EL PONG NO HACE FALTA
	/*for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (EnemyShoot[i].IsAlive())
		{
			EnemyShoot[i].Move(-1, 0);
			if (EnemyShoot[i].GetX() > WINDOW_WIDTH)	Shots[i].ShutDown();
		}
	}

	if (framecount % 100 == 0) {
		int x, y, w, h;
		Enemy.GetRect(&x, &y, &w, &h);
		EnemyShoot[idx_enemy_shot].Init(x - 29, y, 56, 20, 10);
		idx_enemy_shot++;
		idx_enemy_shot %= MAX_SHOTS;
	}
	idx_enemy_shot = 0;
	*/
	/*for (int i = 0; i < MAX_SHOTS; i++) {
		if (Player.Collide(EnemyShoot[idx_enemy_shot])) {
			
		}
		idx_enemy_shot++;
	}*/
	
	

	

	
		
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
	if (vida == 0) {}
	else {
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_player, NULL, &rc);
		if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
	}


	Enemy.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, img_enemy, NULL, &rc);


	//Draw Life
	Life.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	for (int i = 0; i < vida; i++)
	{
		SDL_RenderCopy(Renderer, img_life, NULL, &rc);
		rc.x += rc.w;  // Move the rect to the right by its width
	}
	
	//Draw shots

	if (Ball.IsAlive()) {
		Ball.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_ball, NULL, &rc);
		if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
	}


	/*for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_shot, NULL, &rc);
			if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
		}
	}*/

	/*//Draw enemy shots, no creo que nos haga falta
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (EnemyShoot[i].IsAlive())
		{
			EnemyShoot[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_shot, NULL, &rc);
			//if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
		}
	}*/

	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(10);	// 1000/10 = 100 fps max
}