#pragma once
#include<iostream>
#include <stdlib.h> 
#include "Turret.h"
#include "GameObject.h"
#include "Bullet.h"

using namespace std;

const int TURRET3_COOLDOWN = 240;

class Turret3 : public Turret
{
public:
	Turret3(const char* texture, int x, int y, int w, int h, int sc, int sw) : Turret(texture, x, y, w, h, sc, sw)
	{
		bulletSound = Mix_LoadWAV("sounds/Missile.wav");
	}
	Turret3() {}
	void Update() override
	{
		Turret::Update();

		if (shootingCooldown > 0) shootingCooldown--;
		else Game::gameObjects.push_back(Shoot());
	}
	static GameObject* readTurret3(ifstream& file)
	{
		Turret3* temp = new Turret3();
		GameObject* readObject = temp->readFromFile(file);
		delete temp;
		temp = nullptr;
		return readObject;
	}
	GameObject* readFromFile(ifstream& file)
	{
		GameObject* clone = Turret::readTurret(file);
		Turret3* readTurret3 = new Turret3(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth);
		readTurret3->setHealth(clone->getHealth());
		readTurret3->velocity = clone->velocity;
		readTurret3->shootingCooldown = ((Turret*)clone)->shootingCooldown;
		delete clone;
		clone = nullptr;
		return readTurret3;
	}
	void writeToFile(ofstream& file) override
	{
		file << "[Turret3]" << "\n";
		Turret::writeToFile(file);
	}
	GameObject* Shoot() override
	{
		srand(time(0));
		default_random_engine rng{ ((unsigned)rand() % 19710) };
		uniform_int_distribution<> dist{ 1, TURRET3_COOLDOWN };
		//cooldown reset
		shootingCooldown = dist(rng);
		srand(shootingCooldown);

		//hostile bullet in player direction
		Vector2D playerPos = Game::gameObjects.at(0)->position;
		Mix_PlayChannel(-1, bulletSound, 0);
		return new Bullet("assets/EnemyMissile.png", position.x + 32, position.y + 40, 32, 32, 1, 32, false, true, getAngle(), playerPos, 3);
	}

};
