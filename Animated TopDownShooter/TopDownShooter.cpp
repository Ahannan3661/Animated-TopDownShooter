#include "Game.h"
#undef main
//game object to control the game
Game* game = nullptr;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Frame rate
const int FPS = 60;
//Time between each frame
const int frameDelay = 1000 / FPS;


int main(int argc, const char* args[])
{
	//Frame control variables
	Uint32 frameStart;
	int frameTime;

	//Game object initialization
	game = new Game();
	game->init("Top Down Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Core game loop
	while (game->running())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();

	return 0;
}