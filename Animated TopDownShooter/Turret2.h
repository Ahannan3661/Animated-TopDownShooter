#pragma once
#include<iostream>
#include <stdlib.h> 
#include "Turret.h"
#include "GameObject.h"
#include "Bullet.h"

using namespace std;

const int TURRET2_COOLDOWN = 180;

class Turret2 : public Turret
{
public:
	Turret2(const char* texture, int x, int y, int w, int h, int sc, int sw) : Turret(texture, x, y, w, h, sc, sw)
	{
		bulletSound = Mix_LoadWAV("sounds/Enemy_Bullet_2.wav");
	}
	Turret2() {}
	static GameObject* readTurret2(ifstream& file)
	{
		Turret2* temp = new Turret2();
		GameObject* readObject = temp->readFromFile(file);
		delete temp;
		temp = nullptr;
		return readObject;
	}
	GameObject* readFromFile(ifstream& file)
	{
		GameObject* clone = Turret::readTurret(file);
		Turret2* readTurret2 = new Turret2(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth);
		readTurret2->setHealth(clone->getHealth());
		readTurret2->velocity = clone->velocity;
		readTurret2->shootingCooldown = ((Turret*)clone)->shootingCooldown;
		delete clone;
		clone = nullptr;
		return readTurret2;
	}
	void Update() override
	{
		Turret::Update();
		if (shootingCooldown > 0) shootingCooldown--;
		else Shoot();
	}
	void writeToFile(ofstream& file) override
	{
		file << "[Turret2]" << "\n";
		Turret::writeToFile(file);
	}
	GameObject* Shoot() override
	{
		srand(time(0));
		default_random_engine rng{ ((unsigned)rand() % 19710) };
		uniform_int_distribution<> dist{ 1, TURRET2_COOLDOWN };
		//cooldown reset
		shootingCooldown = dist(rng);
		srand(shootingCooldown);

		//hostile double bullet in player direction
		Vector2D playerPos = Game::gameObjects.at(0)->position;
		playerPos.x += 32;
		playerPos.y += 32;
		Game::gameObjects.push_back(new Bullet("assets/EnemyBullet3.png", position.x + 10, position.y + 32, 32, 32, 1, 96, false, true, getAngle(), playerPos, 1));
		Game::gameObjects.push_back(new Bullet("assets/EnemyBullet3.png", position.x + 40, position.y + 32, 32, 32, 1, 96, false, true, getAngle(), playerPos, 1));
		Mix_PlayChannel(-1, bulletSound, 0);
		return NULL;
	}

};
