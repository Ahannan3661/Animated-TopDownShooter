#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <vector>
#include "GameObject.h"
#include "Vector2D.h"
#include <SDL_ttf.h>
#include <string>
#include <SDL_mixer.h>
#include <iostream>
#include <fstream>

class GameObject;

using namespace std;

class Game
{
public:
	Game();
	~Game();

	void init(const char*, int, int, int, int);
	void update();
	void render();
	void clean();
	void handleEvents();

	bool running() { return isRunning; }


	static bool checkCollission(GameObject*, GameObject*);
	static void generateExplosion(Vector2D&);
	int showMenu(SDL_Window*, TTF_Font*);
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static Mix_Chunk* explosionSound;
	static vector<GameObject*> gameObjects;
	void writeToFile();
	bool readFromFile();
	bool is_empty(ifstream&);

private:
	bool dontsave = false;
	Mix_Music* menuMusic = NULL;
	Mix_Music* gameMusic = NULL;
	Mix_Chunk* buttonSound = NULL;
	int userChoice = -1;
	bool bossArrived = false;
	bool bossWarning = false;
	string scoreText = "Score: ";
	string missileText = "Missiles: ";
	SDL_Surface* scoreSurface = NULL;
	SDL_Surface* missileSurface = NULL;
	SDL_Texture* scoreTexture = NULL;
	SDL_Rect scoreTextDest;
	SDL_Texture* missileTexture = NULL;
	SDL_Rect missileTextDest;

	Uint32 frameCount = 0;
	bool isRunning;
	SDL_Window* window = NULL;
	TTF_Font* font = NULL;
};