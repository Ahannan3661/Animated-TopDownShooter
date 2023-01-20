#pragma once
#include<iostream>
#include <stdlib.h> 
#include "GameObject.h"

using namespace std;

class Nimble : public GameObject
{
private:
	const int totalShootingCoolDown = 90;
	int shootingCooldown = 20;
	const int totalDodgingCoolDown = 60;
	int dodgingCoolDown = 0;
	Mix_Chunk* bulletSound = NULL;
public:
	Nimble(const char*, int, int, int, int, int, int);
	Nimble() {}
	~Nimble()
	{
		Mix_FreeChunk(bulletSound);
		bulletSound = NULL;
	}
	static GameObject* readNimble(ifstream& file)
	{
		Nimble* temp = new Nimble();
		GameObject* readObject = temp->readFromFile(file);
		delete temp;
		temp = nullptr;
		return readObject;
	}
	GameObject* readFromFile(ifstream& file) override
	{
		GameObject* clone = GameObject::readGameObject(file);
		Nimble* readNimble = new Nimble(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth);
		readNimble->velocity = clone->velocity;
		delete clone;
		clone = nullptr;
		file >> readNimble->shootingCooldown;
		file >> readNimble->dodgingCoolDown;
		return readNimble;
	}
	void writeToFile(ofstream& file) override
	{
		file << "[Nimble]\n";
		GameObject::writeToFile(file);
		file << shootingCooldown << "\n";
		file << dodgingCoolDown << "\n";
	}
	void Update() override;
	GameObject* Shoot();
	bool OnCoolDown();
};
