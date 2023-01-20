#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject( const char* texture, int x, int y, int w, int h, int sc, int sw, float angle, string tag)
{
	path = _strdup(texture);
	this->texture = TextureManager::LoadTexture(path);
	this->position.x = x;
	this->position.y = y;
	this->angle = angle;
	this->tag = tag;
	if(hasTag("Smoke"))
		SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
	sheetWidth = sw;
	width = w * sc;
	height = h * sc;
	scale = sc;
	srcRect.x = srcRect.y = 0;
	srcRect.w = w;
	srcRect.h = h;
	destRect.x = x;
	destRect.y = y;
	destRect.w = width;
	destRect.h = height;
	health = 1;
	velocity.Zero();
}

void GameObject::UpdateDestRect()
{
	destRect.x = static_cast<int>(position.x);
	destRect.y = static_cast<int>(position.y);
	destRect.w = width = srcRect.w * scale;
	destRect.h = height = srcRect.h * scale;
}

GameObject::~GameObject()
{
	SDL_DestroyTexture(texture);
}

void GameObject::Render()
{
	if(hasTag("Smoke"))
		TextureManager::Draw1(texture, srcRect, destRect, angle);
	else
	TextureManager::Draw(texture, srcRect, destRect, angle);
}

void GameObject::nextSprite()
{
	srcRect.x += srcRect.w;
	srcRect.x %= sheetWidth;
}