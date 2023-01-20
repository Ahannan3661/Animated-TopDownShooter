#include "Ranger.h"
#include "Bullet.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Ranger::Ranger(const char* texture, int x, int y, int w, int h, int sc, int sw) : GameObject(texture, x, y, w, h, sc, sw, 0.0f, "Ranger")
{
	bulletSound = Mix_LoadWAV("sounds/Enemy_Bullet_3.wav");
	speed = 2;
	setHealth(3);
}

void Ranger::Update()
{
	Translate(0.f, speed);

	for (int i = 0; i < Game::gameObjects.size(); i++)
	{
		GameObject* obj2 = Game::gameObjects.at(i);
		if (obj2 != this && obj2->Alive())
		{
			if (Game::checkCollission(this, obj2))
			{
				if (obj2->hasTag("Bullet"))
				{
					if (!obj2->IsHostile())
					{
						takeHit(1);
						obj2->takeHit(1);
						return;
					}
				}
				else if(obj2->hasTag("Player") || obj2->hasTag("Nimble"))
				{
					takeHit(1);
					obj2->takeHit(1);
					return;
				}
			}
		}
	}
	//cooldown tracking
	if (shootingCooldown != 0) shootingCooldown--;
	else
	{
		Game::gameObjects.push_back(Shoot());
	}

	if (position.y <= SCREEN_HEIGHT)
	{
		position.x += static_cast<int> (velocity.x);
		position.y += static_cast<int> (velocity.y);
		velocity.x *= friction;
		velocity.y *= friction;
	}
	else
		Destroy();

	UpdateDestRect();
}

GameObject* Ranger::Shoot()
{
	//cooldown reset
	shootingCooldown = totalShootingCoolDown;

	//hostile normal bullet in player direction
	Vector2D playerPos = Game::gameObjects.at(0)->position;
	Mix_PlayChannel(-1, bulletSound, 0);
	return new Bullet("assets/EnemyBullet1.png", position.x + 16, position.y + 16, 32, 32, 1, 96, false, true, 90.0f, playerPos, 1);
}

bool Ranger::OnCoolDown()
{
	return shootingCooldown != 0;
}