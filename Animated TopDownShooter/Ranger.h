#pragma once
#include<iostream>
#include <stdlib.h> 
#include "GameObject.h"

using namespace std;

class Ranger : public GameObject
{
private:
	const int totalShootingCoolDown = 120;
	int shootingCooldown = 20;
	Mix_Chunk* bulletSound = NULL;
public:
	Ranger(const char*, int, int, int, int, int, int);
	Ranger() {}
	~Ranger()
	{
		Mix_FreeChunk(bulletSound);
		bulletSound = NULL;
	}
	static GameObject* readRanger(ifstream& file)
	{
		Ranger* temp = new Ranger();
		GameObject* readObject = temp->readFromFile(file);
		delete temp;
		temp = nullptr;
		return readObject;
	}
	GameObject* readFromFile(ifstream& file) override
	{
		GameObject* clone = GameObject::readGameObject(file);
		Ranger* readRanger = new Ranger(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth);
		readRanger->setHealth(clone->getHealth());
		readRanger->velocity = clone->velocity;
		delete clone;
		clone = nullptr;
		file >> readRanger->shootingCooldown;
		return readRanger;
	}
	void writeToFile(ofstream& file) override
	{
		file << "[Ranger]\n";
		GameObject::writeToFile(file);
		file << shootingCooldown << "\n";
	}
	void Update() override;
	GameObject* Shoot();
	bool OnCoolDown();
};
