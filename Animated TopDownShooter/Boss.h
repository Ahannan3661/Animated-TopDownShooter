#pragma once
#include<iostream>
#include <stdlib.h> 
#include "GameObject.h"
#include "Turret.h"
#include "Turret1.h"
#include "Turret2.h"
#include "Turret3.h"
#include "Turret4.h"
#include <random>
#include <time.h>
#include <algorithm>

using namespace std;

class Boss : public GameObject
{
	
public:
	vector<GameObject*> turrets;
	int hitIndex = -1;
	Boss(const char* texture, int x, int y, int w, int h, int sc, int sw) : GameObject(texture, x, y, w, h, sc, sw, 0.0f, "Boss")
	{
		setHealth(5);
		speed = 2;

		turrets.push_back(new Turret1("assets/Turret_1.png", 0, 0, 64, 64, 1, 64));
		turrets.push_back(new Turret2("assets/Turret_2.png", 0, 0, 64, 64, 1, 64));
		turrets.push_back(new Turret3("assets/Turret_3.png", 0, 0, 64, 64, 1, 64));
		turrets.push_back(new Turret4("assets/Turret_4A.png", 0, 0, 64, 64, 1, 64));

		srand(unsigned(time(0)));
		random_shuffle(turrets.begin(), turrets.end());
		
		turrets[0]->position.x = this->position.x - 128;
		turrets[1]->position.x = this->position.x - 64;
		turrets[2]->position.x = this->position.x + 128;
		turrets[3]->position.x = this->position.x + 192;

	}
	Boss() {}
	~Boss()
	{
		for (int i = 0; i < turrets.size(); i++)
		{
			delete turrets.at(i);
			turrets.erase(turrets.begin() + i);
		}
	}
	static GameObject* readBoss(ifstream& file)
	{
		Boss* temp = new Boss();
		GameObject* readObject = temp->readFromFile(file);
		delete temp;
		temp = nullptr;
		return readObject;
	}

	GameObject* readFromFile(ifstream& file) override
	{
		GameObject* clone = GameObject::readGameObject(file);
		Boss* readBoss = new Boss(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth);
		readBoss->setHealth(clone->getHealth());
		readBoss->velocity = clone->velocity;
		delete clone;
		clone = nullptr;
		int noOfTurrets;
		file >> noOfTurrets;
		for (int i = 0; i < readBoss->turrets.size(); i++)
		{
			delete readBoss->turrets.at(i);
			readBoss->turrets.erase(readBoss->turrets.begin() + i);
		}
		string dataType;
		for (int i = 0; i < noOfTurrets; i++)
		{
			file >> dataType;
			if (strcmp(dataType.c_str(), "[Turret1]") == 0) readBoss->turrets.push_back(Turret1::readTurret1(file));
			else if (strcmp(dataType.c_str(), "[Turret2]") == 0) readBoss->turrets.push_back(Turret2::readTurret2(file));
			else if (strcmp(dataType.c_str(), "[Turret3]") == 0) readBoss->turrets.push_back(Turret3::readTurret3(file));
			else if (strcmp(dataType.c_str(), "[Turret4]") == 0) readBoss->turrets.push_back(Turret4::readTurret4(file));
		}
		return readBoss;
	}

	void writeToFile(ofstream& file) override
	{
		file << "[Boss]\n";
		GameObject::writeToFile(file);
		file << turrets.size() << "\n";
		for (int i = 0; i < turrets.size(); i++)
		{
			turrets[i]->writeToFile(file);
		}
	}
	void takeHit(int dmg) override
	{
		if (hitIndex == -1)
		{
			if(turrets.size() == 0)
			GameObject::takeHit(dmg);
		}
		else
		{
			turrets[hitIndex]->takeHit(dmg);
		}
	}

	GameObject* bossDist(GameObject* obj) override
	{
		int dist = sqrt((obj->position.x - position.x) * (obj->position.x - position.x) + (obj->position.y - position.y) * (obj->position.y - position.y));

		if (dist < height + 80)
		{
			return this;
		}
		else
		{
			for (int i = 0; i < turrets.size(); i++)
			{
				dist = sqrt((obj->position.x - turrets[i]->position.x) * (obj->position.x - turrets[i]->position.x) + (obj->position.y - turrets[i]->position.y) * (obj->position.y - turrets[i]->position.y));
				if (dist < turrets[i]->height + 80)
				{
					return  turrets[i];
				}
			}
			return nullptr;
		}
	}

	bool bossCollission(GameObject* obj) override
	{
		if (position.x  + width >= obj->position.x &&
			position.x  <= obj->position.x + obj->width &&
			position.y + height >= obj->position.y &&
			position.y <= obj->position.y + obj->height)
		{
			hitIndex = -1;
			return true;
		}
		else
		{
			for (int i = 0; i < turrets.size(); i++)
			{
				if (turrets[i]->position.x + turrets[i]->width >= obj->position.x &&
					turrets[i]->position.x <= obj->position.x + obj->width &&
					turrets[i]->position.y + turrets[i]->height >= obj->position.y &&
					turrets[i]->position.y <= obj->position.y + obj->height)
				{
					hitIndex = i;
					return true;
				}
			}
			return false;
		}
		
	}

	void Update() override
	{
		if (position.y < 10)
			Translate(0.f, speed);

		if (position.y <= 10)
		{
			position.x += static_cast<int> (velocity.x);
			position.y += static_cast<int> (velocity.y);
			velocity.x *= friction;
			velocity.y *= friction;
		}

		for (int i = 0; i < Game::gameObjects.size(); i++)
		{
			GameObject* obj2 = Game::gameObjects.at(i);
			if (obj2 != this && obj2->Alive())
			{
				if (obj2->hasTag("Player"))
				{
					if (Game::checkCollission(this, obj2))
					{
						takeHit(1);
						obj2->takeHit(1);
						return;
					}
				}
			}
		}

		for (int i = 0; i < turrets.size(); i++)
		{
			GameObject* obj = turrets.at(i);
			if (!obj->Alive())
			{
				((Player*)Game::gameObjects.at(0))->score += 2;
				Game::generateExplosion(obj->position);
				delete turrets.at(i);
				turrets.erase(turrets.begin() + i);
			}
		}

		UpdateDestRect();

		for (int i = 0; i < turrets.size(); i++)
		{
			turrets[i]->Update();
		}
	}

	void Render() override
	{
		TextureManager::Draw(texture, srcRect, destRect, angle);
		for (int i = 0; i < turrets.size(); i++)
		{
			TextureManager::Draw(turrets[i]->texture, turrets[i]->srcRect, turrets[i]->destRect, turrets[i]->angle);
		}
	}
};
