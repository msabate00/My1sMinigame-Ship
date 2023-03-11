#include "Game.h"
#include <stdio.h>

int main(int argc, char* args[])
{
	Game game;
	if (!game.Init()) return -1;

	bool end = false;
	while (!end)
	{
		end = game.Update();
		game.Draw();
	}

	game.Release();
	return 0;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("Error al inicializar SDL_mixer: %s\n", Mix_GetError());
		exit(1);
	}

	Mix_Music* musica = Mix_LoadMUS("Game/assets/stage1.ogg");
	if (musica == NULL) {
		printf("Error al cargar la música: %s\n", Mix_GetError());
	Mix_Chunk* efecto = Mix_LoadWAV("Game/assets/laser.wav");
	Mix_PlayMusic(musica, -1); // Reproduce la música en un loop infinito
	Mix_VolumeMusic(128);
	SDL_Delay(5000);
	Mix_PlayChannel(-1, efecto, 0); // Reproduce el efecto una vez	
	Mix_FreeMusic(musica);
	Mix_FreeChunk(efecto);
	Mix_CloseAudio();

}