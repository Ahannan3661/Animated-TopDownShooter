#pragma once
#include<iostream>
#include <stdlib.h> 
#include "GameObject.h"

using namespace std;

class Bullet : public GameObject
{
private:
	bool home; //should home or not
	Vector2D playerDirection;
	GameObject* homeTarget = nullptr; //target enemy
	const Uint32 switchTime = 100;
	Uint32 spriteStart = 0;
	const int totalSmokeCoolDown = 5;
	int smokeCoolDown = totalSmokeCoolDown;
	int damage = 1;
public:
	Bullet(const char*, int, int , int, int, int, int, bool, bool, float, int);
	Bullet(const char*, int, int, int, int, int, int, bool, bool, float, Vector2D, int);
	Bullet() { home = false; }
	void Update() override;
	bool Home() { return home; }
	void writeToFile(ofstream& file) override
	{
		file << "[Bullet]\n";
		GameObject::writeToFile(file);
		int temp;
		playerDirection.x == numeric_limits<float>::max() ? temp = 0 : temp = 1;
		file << temp << "\n";
		file << home ? 0 : 1;
		file << "\n";
		file << damage << "\n";
		file << spriteStart << "\n";
		file << smokeCoolDown << "\n";
	}
	static GameObject* readBullet(ifstream& file)
	{
		Bullet* temp = new Bullet();
		GameObject* readObject = temp->readFromFile(file);
		delete temp;
		temp = nullptr;
		return readObject;
	}
	GameObject* readFromFile(ifstream& file) override
	{
		GameObject* clone = GameObject::readGameObject(file);
		bool targeted;
		int temp; file >> temp; temp == 1 ? targeted = true : targeted = false;
		bool home;
		temp; file >> temp; temp == 1 ? home = true : home = false;
		int damage;
		file >> damage;
		Bullet* readBullet;
		if (targeted)
		{
			readBullet = new Bullet(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth, home, clone->hostile, clone->angle, Game::gameObjects.at(0)->position - 32, damage);
		}
		else
		{
			readBullet = new Bullet(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth, home, clone->hostile, clone->angle, damage);
		}
		readBullet->velocity = clone->velocity;
		delete clone;
		clone = nullptr;
		file >> readBullet->spriteStart;
		file >> readBullet->smokeCoolDown;
		return readBullet;
	}
	void SetTarget(GameObject* t) { homeTarget = t; }
	GameObject* GetTarget() { return homeTarget; }
	void setPlayerDirection(Vector2D d) { playerDirection = d; }
	Vector2D getPlayerDirection() { return playerDirection; }
};
