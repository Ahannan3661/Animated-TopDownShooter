#include "Player.h"
#include "Bullet.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Player::Player(const char* texture, int x, int y, int w, int h, int sc, int sw) : GameObject(texture, x, y, w, h, sc, sw, -180.0f, "Player")
{
	bulletSound = Mix_LoadWAV("sounds/Player_Bullet_1.wav");
	missileSound = Mix_LoadWAV("sounds/Missile.wav");
	setHealth(50);
	score = 0;
	missileCount = totalMissileCount;
	shootingCoolDown = totalShootingCoolDown;
	velocity.Zero();
}

void Player::Update()
{
	if (reloadStart != 0) { if (missileCount < totalMissileCount) reloadStart--; }
	else
	{
		if (missileCount < totalMissileCount) missileCount++;
		reloadStart = totalReloadTime;
	}

	//cooldown tracking
	if (shootingCoolDown != 0)
		shootingCoolDown--;

	//player movement
	if (position.x >= 0.0f && position.x <= (SCREEN_WIDTH - width) && position.y >= 0.0f && position.y <= (SCREEN_HEIGHT - height))
	{
		position.x += static_cast<int> (velocity.x);
		position.y += static_cast<int> (velocity.y);
		velocity.x *= friction;
		velocity.y *= friction;
	}
	else
	{
		if (position.x < 0.0f) position.x = 0.0f;
		if (position.x > (SCREEN_WIDTH - width)) position.x = (SCREEN_WIDTH - width);
		if (position.y < 0.0f) position.y = 0.0f;
		if (position.y > (SCREEN_HEIGHT - height)) position.y = (SCREEN_HEIGHT - height);
		velocity.x = 0.0f;
		velocity.y = 0.0f;
	}

	//updating destRect values
	UpdateDestRect();
}

GameObject* Player::Shoot(bool isMissile)
{
	GameObject* bullet;
	if (isMissile)
	{
		//homing missile
		bullet = new Bullet("assets/PlayerMissile.png", position.x + 16, position.y - 16, 32, 32, 1, 32, true, false, 270.0f, 1);
		missileCount--;
		Mix_PlayChannel(-1, missileSound, 0);
		reloadStart = totalReloadTime;
		
	}
	else
	{
		//normal bullet
		bullet = new Bullet("assets/PlayerBullet.png", position.x + 16, position.y - 16, 32, 32, 1, 96, false, false, 270.0f, 1);
		Mix_PlayChannel(-1, bulletSound, 0);
	}
	shootingCoolDown = totalShootingCoolDown;
	return bullet;
}

bool Player::OnCoolDown()
{
	return shootingCoolDown != 0;
}