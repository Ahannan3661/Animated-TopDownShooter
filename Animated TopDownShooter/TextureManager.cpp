#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, float angle)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, NULL, SDL_FLIP_NONE);
}

void TextureManager::Draw1(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, float angle)
{
	SDL_Point* c = new SDL_Point();
	c->x = dest.w/2;
	c->y = 0;
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, c, SDL_FLIP_NONE);
}
