#pragma once
#include<iostream>
#include <stdlib.h> 
#include "Turret.h"
#include "GameObject.h"
#include "Bullet.h"

using namespace std;

const int TURRET4_COOLDOWN = 140;

class Turret4 : public Turret
{
private:
	bool charging = false;
	SDL_Texture* texturea = TextureManager::LoadTexture("assets/Turret_4A.png");
	SDL_Texture* textureb = TextureManager::LoadTexture("assets/Turret_4B.png");
public:
	Turret4(const char* texture, int x, int y, int w, int h, int sc, int sw) : Turret(texture, x, y, w, h, sc, sw)
	{
		bulletSound = Mix_LoadWAV("sounds/lightning.wav");
	}
	Turret4(){}
	~Turret4()
	{
		SDL_DestroyTexture(texturea);
		SDL_DestroyTexture(textureb);
	}
	static GameObject* readTurret4(ifstream& file)
	{
		Turret4* temp = new Turret4();
		GameObject* readObject = temp->readFromFile(file);
		delete temp;
		temp = nullptr;
		return readObject;
	}
	GameObject* readFromFile(ifstream& file)
	{
		GameObject* clone = Turret::readTurret(file);
		Turret4* readTurret4 = new Turret4(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth);
		readTurret4->setHealth(clone->getHealth());
		readTurret4->velocity = clone->velocity;
		readTurret4->shootingCooldown = ((Turret*)clone)->shootingCooldown;
		delete clone;
		clone = nullptr;
		return readTurret4;
	}
	void writeToFile(ofstream& file) override
	{
		file << "[Turret4]" << "\n";
		Turret::writeToFile(file);
	}
	void Update() override
	{
		Turret::Update();

		if (shootingCooldown > 0) shootingCooldown--;
		else 
		{
			Game::gameObjects.push_back(Shoot());
			setTex(texturea);
			charging = false;
		}
		if (shootingCooldown < 30 && !charging)
		{
			setTex(textureb);
			charging = true;
		}
	}

	GameObject* Shoot() override
	{
		srand(time(0));
		default_random_engine rng{ ((unsigned)rand() % 19710) };
		uniform_int_distribution<> dist{ 1, TURRET4_COOLDOWN };
		//cooldown reset
		shootingCooldown = dist(rng);
		srand(shootingCooldown);

		//hostile bullet in player direction
		Vector2D playerPos = Game::gameObjects.at(0)->position;
		Mix_PlayChannel(-1, bulletSound, 0);
		return new Bullet("assets/Lightning.png", position.x + 32, position.y + 50, 32, 32, 1, 96, false, true, getAngle(), playerPos, 2);
	}

};
