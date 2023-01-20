#include "Bullet.h"
#include "TextureManager.h"
#include <iostream>
#include <stdlib.h> 
#include <limits>
#include "Nimble.h"
#include "Smoke.h"
#include "Player.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Bullet::Bullet(const char* tex, int x, int y, int w, int h, int sc, int sw, bool home, bool hostile, float angle, int damage) : GameObject(tex, x, y, w, h, sc, sw, angle, "Bullet")
{
	speed = 3;
	this->hostile = hostile;
	this->home = home;
	this->damage = damage;
	playerDirection.x = numeric_limits<float>::max();
	playerDirection.y = numeric_limits<float>::max();
}

Bullet::Bullet(const char* tex, int x, int y, int w, int h, int sc, int sw, bool home, bool hostile, float angle, Vector2D playerPos, int damage) : GameObject(tex, x, y, w, h, sc, sw, angle, "Bullet")
{
	speed = 3;
	this->damage = damage;
	this->hostile = hostile;
	this->home = home;
	setAngle(position.getAngle(playerPos));
	playerDirection = position.unit(playerPos);
}

void Bullet::Update()
{
	for (int i = 0; i < Game::gameObjects.size(); i++)
	{
		GameObject* obj2 = Game::gameObjects.at(i);
		if (obj2 != this && obj2->Alive())
		{
			if (Game::checkCollission(this, obj2))
			{ 
				if ((obj2->hasTag("Player") && IsHostile())
					|| ((obj2->hasTag("Nimble") || obj2->hasTag("Ranger") || obj2->hasTag("Boss")) && !IsHostile()))
				{
					takeHit(1);
					obj2->takeHit(damage);
					return;
				}
			}
			else
			{
				if ((obj2->hasTag("Nimble") || obj2->hasTag("Ranger") || obj2->hasTag("Boss")) && this->Home() && homeTarget == nullptr)
				{
					if (obj2->hasTag("Boss")) homeTarget = obj2->bossDist(this);
					else
					{
						int dist = sqrt((obj2->position.x - position.x) * (obj2->position.x - position.x) + (obj2->position.y - position.y) * (obj2->position.y - position.y));

						if (dist < obj2->height + 80)
						{
							homeTarget = obj2;
						}
					}
				}
			}
		}
	}

	smokeCoolDown > 0 ? smokeCoolDown-- : smokeCoolDown = 0;

	//smoke generation
	if (home && smokeCoolDown == 0)
	{
		smokeCoolDown = totalSmokeCoolDown;
		GameObject* smoke;
		if (homeTarget != nullptr)
		{
			smoke = new Smoke("assets/smoke.png", position.x, position.y + (height / 2), 32, 32, 1, 32, getAngle() + 90.0f, homeTarget->position);
			Game::gameObjects.push_back(smoke);
		}
		else
		{
			Vector2D vector;
			vector.x = position.x - width / 2;
			vector.y = position.y - height - 10;
			smoke = new Smoke("assets/smoke.png", position.x, position.y + (height / 2), 32, 32, 1, 32, getAngle() + 90.0f, vector);
			Game::gameObjects.push_back(smoke);
		}
	}

	if(spriteStart == 0) spriteStart = SDL_GetTicks();
	//sprite change
	if (!home && spriteStart!=0 && SDL_GetTicks() - spriteStart > switchTime)
	{
		nextSprite();
		spriteStart = SDL_GetTicks();
	}

	if (hostile)
	{
		if (playerDirection.x == numeric_limits<float>::max())
		{
			if (position.y <= SCREEN_HEIGHT)
			{
				position.y += speed;
			}
			else
				Destroy();
		}
		else
		{
			position.x += playerDirection.x * speed;
			position.y += playerDirection.y * speed;
		}
	}
	//homing bullet
	else if (home && homeTarget != nullptr)
	{
		//Rocket faces the target it is going to home to
		setAngle(position.getAngle(homeTarget->position));

		Vector2D unitVector = position.unit(homeTarget->position);
		position.x += unitVector.x * speed;
		position.y += unitVector.y * speed;
	}
	else
	{
		if (position.y >= 0)
			position.y -= speed;
		else
			Destroy();
	}
	if (position.x < 0 || position.x > SCREEN_WIDTH || position.y < 0 || position.y > SCREEN_HEIGHT)
		Destroy();

	//updating destRect values
	UpdateDestRect();
}