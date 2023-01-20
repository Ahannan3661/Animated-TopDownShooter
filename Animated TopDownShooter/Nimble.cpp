#include "Nimble.h"
#include "Bullet.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Nimble::Nimble(const char* texture, int x, int y, int w, int h, int sc, int sw) : GameObject(texture, x, y, w, h, sc, sw, 180.0f, "Nimble")
{
	speed = 2;
	bulletSound = Mix_LoadWAV("sounds/Enemy_Bullet_1.wav");
}

void Nimble::Update()
{
	Translate(0.f, speed);

	dodgingCoolDown > 0 ? dodgingCoolDown-- : dodgingCoolDown = 0;

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
				else if (obj2->hasTag("Player") || obj2->hasTag("Nimble") || obj2->hasTag("Ranger"))
				{
					takeHit(1);
					obj2->takeHit(1);
					return;
				}
			}
			else
			{
				if (obj2->hasTag("Bullet"))
				{
					if (!obj2->IsHostile())
					{
						int dist = sqrt((obj2->position.x - position.x) * (obj2->position.x - position.x) + (obj2->position.y - position.y) * (obj2->position.y - position.y));

						if (dist < height+30 && !(velocity.x > 0.1f) && dodgingCoolDown == 0)
						{
							dodgingCoolDown = totalDodgingCoolDown;
							int random = rand() % 2;
							if (random == 0)
								Translate(-speed * 4, 0);
							else
								Translate(speed * 4, 0);
						}
					}
				}
			}
		}
	}

	if (position.x < 0.0f) position.x = 0.0f;
	if (position.x > (SCREEN_WIDTH - width)) position.x = (SCREEN_WIDTH - width);
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

GameObject* Nimble::Shoot()
{
	shootingCooldown = totalShootingCoolDown;
	//straight hostile normal bullet
	GameObject* bullet = new Bullet("assets/EnemyBullet2.png", position.x + 16, position.y + 16, 32, 32, 1, 96, false, true, 90.0f, 1);
	Mix_PlayChannel(-1, bulletSound, 0);
	return bullet;
}

bool Nimble::OnCoolDown()
{
	return dodgingCoolDown != 0;
}