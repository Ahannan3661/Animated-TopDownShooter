#pragma once
#include<iostream>
#include <stdlib.h> 
#include "Turret.h"
#include "GameObject.h"
#include "Bullet.h"

using namespace std;

const int TURRET1_COOLDOWN = 120;

class Turret1 : public Turret
{
public:
	Turret1(const char* texture, int x, int y, int w, int h, int sc, int sw) : Turret(texture, x, y, w, h, sc, sw)
	{
		bulletSound = Mix_LoadWAV("sounds/Enemy_Bullet_3.wav");
	}
	Turret1() {}

	static GameObject* readTurret1(ifstream& file)
	{
		Turret1* temp = new Turret1();
		GameObject* readObject = temp->readFromFile(file);
		delete temp;
		temp = nullptr;
		return readObject;
	}
	GameObject* readFromFile(ifstream& file)
	{
		GameObject* clone = Turret::readTurret(file);
		Turret1* readTurret1 = new Turret1(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth);
		readTurret1->setHealth(clone->getHealth());
		readTurret1->velocity = clone->velocity;
		readTurret1->shootingCooldown = ((Turret*)clone)->shootingCooldown;
		delete clone;
		clone = nullptr;
		return readTurret1;
	}

	void Update() override
	{
		Turret::Update();

		if (shootingCooldown > 0) shootingCooldown--;
		else Game::gameObjects.push_back(Shoot());
	}
	void writeToFile(ofstream& file) override
	{
		file << "[Turret1]" << "\n";
		Turret::writeToFile(file);
	}
	GameObject* Shoot() override
	{
		srand(static_cast<unsigned int> (time(0)));
		default_random_engine rng{ ((unsigned)rand() % 19710) };
		uniform_int_distribution<> dist{ 1, TURRET1_COOLDOWN };
		//cooldown reset
		shootingCooldown = dist(rng);
		srand(shootingCooldown);

		//hostile bullet in player direction
		Vector2D playerPos = Game::gameObjects.at(0)->position;
		Mix_PlayChannel(-1, bulletSound, 0);
		return new Bullet("assets/EnemyBullet3.png", static_cast<int>(position.x) + 32, static_cast<int>(position.y) + 32, 32, 32, 1, 96, false, true, getAngle(), playerPos, 1);
	}

};
