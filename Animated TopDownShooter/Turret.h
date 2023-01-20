#pragma once
#include<iostream>
#include <stdlib.h> 
#include "GameObject.h"
#include <time.h>
#include <random>

using namespace std;

class Turret : public GameObject
{
protected:
	Vector2D playerPos;
	Mix_Chunk* bulletSound = NULL;
public:
	int shootingCooldown = 60;
	Turret(const char* texture, int x, int y, int w, int h, int sc, int sw) : GameObject(texture, x, y, w, h, sc, sw, 0.0f, "Turret")
	{
		playerPos = Game::gameObjects.at(0)->position;
		setAngle(position.getAngle(playerPos) - 90.0f);
		speed = 2;
		setHealth(5);
	}
	Turret() {}
	~Turret()
	{
		Mix_FreeChunk(bulletSound);
		bulletSound = NULL;
	}
	static GameObject* readTurret(ifstream& file)
	{
		Turret* temp = new Turret();
		GameObject* readObject = temp->readFromFile(file);
		delete temp;
		temp = nullptr;
		return readObject;
	}
	GameObject* readFromFile(ifstream& file)
	{
		GameObject* clone = GameObject::readGameObject(file);
		Turret* readTurret = new Turret(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth);
		readTurret->setHealth(clone->getHealth());
		readTurret->velocity = clone->velocity;
		delete clone;
		clone = nullptr;
		file >> readTurret->shootingCooldown;
		return readTurret;
	}
	virtual void writeToFile(ofstream& file) 
	{
		GameObject::writeToFile(file);
		file << shootingCooldown << "\n";
	}
	virtual void Update() 
	{
		playerPos = Game::gameObjects.at(0)->position;
		setAngle(position.getAngle(playerPos) - 90.0f);

		if (position.y < 74)
			Translate(0.f, static_cast<float>(speed));

		if (position.y <= 74)
		{
			position.x += static_cast<int> (velocity.x);
			position.y += static_cast<int> (velocity.y);
			velocity.x *= friction;
			velocity.y *= friction;
		}

		UpdateDestRect();
	}

	virtual GameObject* Shoot() 
	{
		return NULL; 
	}

};
