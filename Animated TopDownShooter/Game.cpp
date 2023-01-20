#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Player.h"
#include "Ranger.h"
#include "Nimble.h"
#include "Explosion.h"
#include <string>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include "Boss.h"
#include "Smoke.h"
#include <iostream>
#include <fstream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BOSS_ARRIVAL_TIME = 30 * 60;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Mix_Chunk* Game::explosionSound;
vector<GameObject*> Game::gameObjects;
GameObject* player;

Game::Game()
{
	//score = 0;
	isRunning = true;
	window = nullptr;
	scoreTextDest.x = 0;
	scoreTextDest.y = 0;
	missileTextDest.x = SCREEN_WIDTH - 100;
	missileTextDest.y = 0;
}

Game::~Game()
{

}

void Game::writeToFile()
{
	if (gameObjects.size() > 0)
	{
		ofstream file("data.txt");
		file << frameCount << "\n";
		file << bossWarning ? 1 : 0;
		file << "\n";
		file << bossArrived ? 1 : 0;
		file << "\n";
		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->writeToFile(file);
		}
		file << "END";
	}
}
bool Game::is_empty(ifstream& file)
{
	return file.peek() == ifstream::traits_type::eof();
}

bool Game::readFromFile()
{
	ifstream file("data.txt");
	if (!is_empty(file))
	{
		file >> frameCount;
		int temp;
		file >> temp;
		temp == 1 ? bossWarning = true : bossWarning = false;
		file >> temp;
		temp == 1 ? bossArrived = true : bossArrived = false;
		string dataType;
		do
		{
			file >> dataType;
			if (strcmp(dataType.c_str(), "[Player]") == 0) { player = Player::readPlayer(file); gameObjects.push_back(player);}
			else if (strcmp(dataType.c_str(), "[Boss]") == 0)	gameObjects.push_back(Boss::readBoss(file));
			else if (strcmp(dataType.c_str(), "[Bullet]") == 0)	gameObjects.push_back(Bullet::readBullet(file));
			else if (strcmp(dataType.c_str(), "[Explosion]") == 0)	gameObjects.push_back(Explosion::readExplosion(file));
			else if (strcmp(dataType.c_str(), "[Nimble]") == 0)	gameObjects.push_back(Nimble::readNimble(file));
			else if (strcmp(dataType.c_str(), "[Ranger]") == 0)	gameObjects.push_back(Ranger::readRanger(file));
			else if (strcmp(dataType.c_str(), "[Smoke]") == 0)	gameObjects.push_back(Smoke::readSmoke(file));
		} while (strcmp(dataType.c_str(), "END") != 0);
		return true;
	}
	return false;
}

struct MenuItem
{
	const char* label;
	SDL_Rect pos;
	SDL_Texture* texture;
	bool isSelected = false;
	SDL_Color color = { 0,0,0 };
	int rows;
	int cols;
};

int Game::showMenu(SDL_Window* screen, TTF_Font* font)
{
	Mix_PlayMusic(menuMusic, -1);

	const Uint32 frameDelay = 60;
	Uint32 startTime, frameTime;
	int x, y; //cursor position
	const int NUMMENU = 2;
	MenuItem menuItems[NUMMENU];
	menuItems[0].label = "Play";
	menuItems[1].label = "Exit";

	SDL_Surface* tempSurface = NULL;
	const SDL_Color colors[2] = { {0,0,0}, {0,255,0} };

	for (int i = 0; i < NUMMENU; i++)
	{
		tempSurface = TTF_RenderText_Solid(font, menuItems[i].label, menuItems[i].color);
		menuItems[i].texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		menuItems[i].pos.w = tempSurface->w;
		menuItems[i].pos.h = tempSurface->h;
		menuItems[i].pos.x = SCREEN_WIDTH / 2 - menuItems[i].pos.w / 2;
		menuItems[i].pos.y = SCREEN_HEIGHT / 2 + (menuItems[i].pos.h * i * 4);
		menuItems[i].rows = (menuItems[i].pos.h / 32) + 2;
		menuItems[i].cols = (menuItems[i].pos.w / 32) + 2;
		SDL_FreeSurface(tempSurface);
	}

	SDL_Texture* buttonEdge = TextureManager::LoadTexture("assets/Button Edge.png");
	SDL_Texture* buttonCorner = TextureManager::LoadTexture("assets/Button Corner.png");
	SDL_Texture* buttonMiddle = TextureManager::LoadTexture("assets/Button Middle.png");
	SDL_Event event;
	while (1)
	{
		startTime = SDL_GetTicks();

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				for (int i = 0; i < NUMMENU; i++)
				{
					SDL_DestroyTexture(menuItems[i].texture);
				}
				SDL_DestroyTexture(buttonEdge);
				SDL_DestroyTexture(buttonCorner);
				SDL_DestroyTexture(buttonMiddle);
				Mix_HaltMusic();
				return 1;
			case SDL_MOUSEMOTION:
				x = event.motion.x;
				y = event.motion.y;
				for (int i = 0; i < NUMMENU; i++)
				{
					if (x >= menuItems[i].pos.x && x <= menuItems[i].pos.x + (32 * menuItems[i].cols) && y >= menuItems[i].pos.y && y <= menuItems[i].pos.y + (32 * menuItems[i].rows))
					{
						if (!menuItems[i].isSelected)
						{
							menuItems[i].isSelected = true;
							menuItems[i].color = colors[1];
							tempSurface = TTF_RenderText_Solid(font, menuItems[i].label, menuItems[i].color);
							SDL_DestroyTexture(menuItems[i].texture);
							menuItems[i].texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
							SDL_FreeSurface(tempSurface);
						}
					}
					else
					{
						if (menuItems[i].isSelected)
						{
							menuItems[i].isSelected = false;
							menuItems[i].color = colors[0];
							tempSurface = TTF_RenderText_Solid(font, menuItems[i].label, menuItems[i].color);
							SDL_DestroyTexture(menuItems[i].texture);
							menuItems[i].texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
							SDL_FreeSurface(tempSurface);
						}
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				x = event.button.x;
				y = event.button.y;
				for (int i = 0; i < NUMMENU; i++)
					if (x >= menuItems[i].pos.x && x <= menuItems[i].pos.x + (32*menuItems[i].cols) && y >= menuItems[i].pos.y && y <= menuItems[i].pos.y + (32 * menuItems[i].rows))
					{
						Mix_PlayChannel(-1, buttonSound, 0);
						for (int i = 0; i < NUMMENU; i++)
						{
							SDL_DestroyTexture(menuItems[i].texture);
						}
						Mix_HaltMusic();
						SDL_DestroyTexture(buttonEdge);
						SDL_DestroyTexture(buttonCorner);
						SDL_DestroyTexture(buttonMiddle);
						return i;
					}
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					for (int i = 0; i < NUMMENU; i++)
					{
						SDL_DestroyTexture(menuItems[i].texture);
					}
					SDL_DestroyTexture(buttonEdge);
					SDL_DestroyTexture(buttonCorner);
					SDL_DestroyTexture(buttonMiddle);
					Mix_HaltMusic();
					return 1;
				}
			}
		}
		SDL_RenderClear(renderer);

		for (int i = 0; i < NUMMENU; i++)
		{
			for (int j = 0; j < menuItems[i].rows; j++)
			{
				for (int k = 0; k < menuItems[i].cols; k++)
				{
					SDL_Rect dest;
					if (j == 0 && k == 0)
					{
						dest.x = menuItems[i].pos.x;
						dest.y = menuItems[i].pos.y;
						dest.w = 32;
						dest.h = 32;
						SDL_RenderCopyEx(renderer, buttonCorner, NULL, &dest, 180.0f, NULL, SDL_FLIP_NONE);
					}
					else if (j == menuItems[i].rows-1 && k == menuItems[i].cols-1)
					{
						dest.x = menuItems[i].pos.x + 32*j;
						dest.y = menuItems[i].pos.y + 32*k;
						dest.w = 32;
						dest.h = 32;
						SDL_RenderCopyEx(renderer, buttonCorner, NULL, &dest, 0.0f, NULL, SDL_FLIP_NONE);
					}
					else if (j == menuItems[i].rows - 1 && k == 0)
					{
						dest.x = menuItems[i].pos.x;
						dest.y = menuItems[i].pos.y + 32 * j;
						dest.w = 32;
						dest.h = 32;
						SDL_RenderCopyEx(renderer, buttonCorner, NULL, &dest, 90.0f, NULL, SDL_FLIP_NONE);
					}
					else if (j == 0 && k == menuItems[i].cols-1)
					{
						dest.x = menuItems[i].pos.x + 32 * k;
						dest.y = menuItems[i].pos.y;
						dest.w = 32;
						dest.h = 32;
						SDL_RenderCopyEx(renderer, buttonCorner, NULL, &dest, -90.0f, NULL, SDL_FLIP_NONE);
					}
					else if (j == 0 && k != 0 && k != menuItems[i].cols - 1)
					{
						dest.x = menuItems[i].pos.x + 32 * k;
						dest.y = menuItems[i].pos.y;
						dest.w = 32;
						dest.h = 32;
						SDL_RenderCopyEx(renderer, buttonEdge, NULL, &dest, 180.0f, NULL, SDL_FLIP_NONE);
					}
					else if (j == menuItems[i].rows-1 && k != 0 && k != menuItems[i].cols - 1)
					{
						dest.x = menuItems[i].pos.x + 32 * k;
						dest.y = menuItems[i].pos.y + 32 * j;
						dest.w = 32;
						dest.h = 32;
						SDL_RenderCopyEx(renderer, buttonEdge, NULL, &dest, 0.0f, NULL, SDL_FLIP_NONE);
					}
					else if (j != 0 && j != menuItems[i].rows - 1 && k == 0)
					{
						dest.x = menuItems[i].pos.x;
						dest.y = menuItems[i].pos.y + 32 * j;
						dest.w = 32;
						dest.h = 32;
						SDL_RenderCopyEx(renderer, buttonEdge, NULL, &dest, 90.0f, NULL, SDL_FLIP_NONE);
					}
					else if (j != 0 && j != menuItems[i].rows - 1 && k == menuItems[i].cols-1)
					{
						dest.x = menuItems[i].pos.x + 32 * k;
						dest.y = menuItems[i].pos.y + 32 * j;
						dest.w = 32;
						dest.h = 32;
						SDL_RenderCopyEx(renderer, buttonEdge, NULL, &dest, -90.0f, NULL, SDL_FLIP_NONE);
					}
					else
					{
						dest.x = menuItems[i].pos.x + 32 * k;
						dest.y = menuItems[i].pos.y + 32 * j;
						dest.w = 32;
						dest.h = 32;
						SDL_RenderCopyEx(renderer, buttonMiddle, NULL, &dest, 0.0f, NULL, SDL_FLIP_NONE);
					}
					
				}
			}

			SDL_Rect menuItemDest = menuItems[i].pos;
			
			menuItemDest.x += (menuItems[i].rows * 32 - menuItems[i].pos.w) / 2;
			menuItemDest.y += (menuItems[i].cols * 32 - menuItems[i].pos.h) / 2;
			SDL_RenderCopy(renderer, menuItems[i].texture, NULL, &menuItemDest);
		}

		SDL_RenderPresent(renderer);

		frameTime = SDL_GetTicks() - startTime;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

}

bool Game::checkCollission(GameObject* obj1, GameObject* obj2)
{
	if (obj1->hasTag("Boss")) return obj1->bossCollission(obj2);
	if (obj2->hasTag("Boss")) return obj2->bossCollission(obj1);
	if (obj1->position.x  + obj1->width >= obj2->position.x &&
		obj1->position.x <= obj2->position.x + obj2->width &&
		obj1->position.y + obj1->height >= obj2->position.y &&
		obj1->position.y <= obj2->position.y + obj2->height)
		return true;
	return false;
}

void Game::generateExplosion(Vector2D& pos)
{
	Mix_PlayChannel(-1, explosionSound, 0);
	for (int i = 0 ; i < 100 ; i++)
	{
		int speed = rand() % 5 + 1;
		int angle = rand() % 360;
		int scale = 1 / ((rand() % 5)+1);
		int life = (rand() % 60) + 1;
		gameObjects.push_back(new Explosion("assets/explosion.png", pos.x + 32, pos.y + 32, 16, 16, scale, 16, angle, speed, life));
	}
}

void Game::init(const char* title, int xpos, int ypos, int width, int height)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		window = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
			}
		}
	}
	font = TTF_OpenFont("assets/times new roman.ttf", 28);
	if (font == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	if (success)
	{
		
		//if (!readFromFile())
		//{
			menuMusic = Mix_LoadMUS("sounds/Menu.mp3");
			buttonSound = Mix_LoadWAV("sounds/Button.wav");

			userChoice = showMenu(window, font);

			gameMusic = Mix_LoadMUS("sounds/Game.mp3");
			explosionSound = Mix_LoadWAV("sounds/Explosion.wav");

			if (userChoice == 1) isRunning = false;
			else
			{
				isRunning = true;
				Mix_PlayMusic(gameMusic, -1);
				player = new Player("assets/Player.png", 320, 400, 64, 64, 1, 64);
				gameObjects.push_back(player);
			}
		//}
	}
	else isRunning = false;
}

void Game::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_w:
			((Player*)player)->Translate(0.0f, -6);
			break;
		case SDLK_s:
			((Player*)player)->Translate(0.0f, 6);
			break;
		case SDLK_a:
			((Player*)player)->Translate(-6, 0.0f);
			break;
		case SDLK_d:
			((Player*)player)->Translate(6, 0.0f);
			break;
		case SDLK_SPACE:
			if(!((Player*)player)->OnCoolDown()) gameObjects.push_back(((Player*)player)->Shoot(false));
			break;
		case SDLK_b:
			if (!((Player*)player)->OnCoolDown() && ((Player*)player)->hasMissile()) gameObjects.push_back(((Player*)player)->Shoot(true));
			break;
		default:
			break;
		}
	default:
		break;
	}
}

void Game::update()
{
	frameCount++;

	//scoreText += Score;
	scoreSurface = TTF_RenderText_Solid(font, ("Score: " + to_string(((Player*)gameObjects.at(0))->score)).c_str(), {0,0,0});
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	scoreTextDest.w = scoreSurface->clip_rect.w;
	scoreTextDest.h = scoreSurface->clip_rect.h;
	SDL_FreeSurface(scoreSurface);

	missileSurface = TTF_RenderText_Solid(font, ("Missiles: " + to_string(((Player*)gameObjects.at(0))->getMissiles())).c_str(), { 0,0,0 });
	missileTexture = SDL_CreateTextureFromSurface(renderer, missileSurface);
	missileTextDest.w = missileSurface->clip_rect.w;
	missileTextDest.h = missileSurface->clip_rect.h;
	missileTextDest.x = SCREEN_WIDTH - missileTextDest.w;
	SDL_FreeSurface(missileSurface);

	if (bossWarning && !bossArrived)
	{
		SDL_Surface* warnSurface = TTF_RenderText_Solid(font, "!!!!!", {255, 0, 0});
		SDL_Texture* warntexture = SDL_CreateTextureFromSurface(renderer, warnSurface);
		SDL_Rect warnPos;
		warnPos.w = warnSurface->w;
		warnPos.h = warnSurface->h;
		SDL_FreeSurface(warnSurface);
		warnPos.x = SCREEN_WIDTH / 2 - warnPos.w / 2;
		warnPos.y = SCREEN_HEIGHT / 2 - warnPos.h / 2;

		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, warntexture, NULL, &warnPos);

		SDL_RenderPresent(renderer);

		SDL_DestroyTexture(warntexture);

		this_thread::sleep_for(2000ms);

		gameObjects.push_back(new Boss("assets/Boss.png", SCREEN_WIDTH / 2 - 64, 0, 128, 128, 1, 128));

		bossArrived = true;
	}
	
	if (frameCount < BOSS_ARRIVAL_TIME)
	{
		if (frameCount % 120 == 0)
		{
			int random = rand() % (SCREEN_WIDTH - 64);
			int random1 = rand() % 2;
			if (random1 == 0)
			{
				gameObjects.push_back(new Ranger("assets/Ranger.png", random, 0, 64, 64, 1, 64));
			}
			else
			{
				gameObjects.push_back(new Nimble("assets/Nimble.png", random, 0, 64, 64, 1, 64));
			}
		}
	}
	else
	{
		if (!bossArrived)
		{
			for (int i = 1; i < gameObjects.size(); i++)
			{
				if (gameObjects.at(i)->Alive()) gameObjects.at(i)->Destroy();
			}
		}
		bossWarning = true;
	}
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects.at(i)->Alive()) gameObjects.at(i)->Update();
	}

	for (int i = 0; i < gameObjects.size(); i++)
	{
		GameObject* obj = gameObjects.at(i);
		if (obj->Alive() == false)
		{
			if ((obj->hasTag("Nimble") || obj->hasTag("Ranger") || obj->hasTag("Boss")) && obj->getExplode())
			{
				((Player*)player)->score++;
				Game::generateExplosion(gameObjects.at(i)->position);

				if (obj->hasTag("Boss")) 
				{
					Mix_HaltMusic();
					userChoice = showMenu(window, font);
					if (userChoice == 0)
					{
						gameObjects.clear();
						player = new Player("assets/Player.png", 320, 400, 64, 64, 1, 64);
						gameObjects.push_back(player);
						bossWarning = false;
						bossArrived = false;
						frameCount = 0;
						Mix_PlayMusic(gameMusic, -1);
						continue;
					}
					else isRunning = false;
				} 
			}
			if (i == 0) { isRunning = false; dontsave = true; }
			delete gameObjects.at(i);
			gameObjects.erase(gameObjects.begin() + i);
		}
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreTextDest);

	SDL_RenderCopy(renderer, missileTexture, NULL, &missileTextDest);

	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects.at(i)->Alive()) gameObjects.at(i)->Render();
	}

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	if(!dontsave)
		writeToFile();

	for (int i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects.at(i);
		gameObjects.erase(gameObjects.begin() + i);
	}
	SDL_DestroyTexture(scoreTexture);
	SDL_DestroyTexture(missileTexture);
	Mix_FreeMusic(menuMusic);
	menuMusic = NULL;
	Mix_FreeMusic(gameMusic);
	gameMusic = NULL;
	Mix_FreeChunk(buttonSound);
	buttonSound = NULL;
	Mix_FreeChunk(explosionSound);
	explosionSound = NULL;
	TTF_CloseFont(font);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
