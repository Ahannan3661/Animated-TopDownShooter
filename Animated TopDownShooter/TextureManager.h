#pragma once
#include "Game.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char*);
	static void Draw(SDL_Texture*, SDL_Rect, SDL_Rect, float);
	static void Draw1(SDL_Texture*, SDL_Rect, SDL_Rect, float);
};