#pragma once
#include <stdlib.h> 
#include "GameObject.h"
#include <SDL_mixer.h>

using namespace std;

class Player : public GameObject
{
private:
	const int totalMissileCount = 10;
	const int totalShootingCoolDown = 15;
	const int totalReloadTime = 120;
	int missileCount;
	int shootingCoolDown;
	int reloadStart = totalReloadTime;
	Mix_Chunk* bulletSound = NULL;
	Mix_Chunk* missileSound = NULL;
public:
	int score;
	Player(const char*, int, int, int, int, int, int);
	Player() { missileCount = 0; shootingCoolDown = 0; score = 0; }
	~Player()
	{
		Mix_FreeChunk(bulletSound);
		bulletSound = NULL;
		Mix_FreeChunk(missileSound);
		missileSound = NULL;
	}
	static GameObject* readPlayer(ifstream& file)
	{
		Player* temp = new Player();
		GameObject* readObject = temp->readFromFile(file);
		delete temp;
		temp = nullptr;
		return readObject;
	}
	GameObject* readFromFile(ifstream& file) override
	{
		GameObject* clone = GameObject::readGameObject(file);
		Player* readPlayer = new Player(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth);
		readPlayer->setHealth(clone->getHealth());
		readPlayer->velocity = clone->velocity;
		delete clone;
		clone = nullptr;
		file >> readPlayer->missileCount;
		file >> readPlayer->shootingCoolDown;
		file >> readPlayer->reloadStart;
		file >> readPlayer->score;
		return readPlayer;
	}
	void writeToFile(ofstream& file) override
	{
		file << "[Player]\n";
		GameObject::writeToFile(file);
		file << missileCount << "\n";
		file << shootingCoolDown << "\n";
		file << reloadStart << "\n";
		file << score << "\n";
	}
	void Update() override;
	bool OnCoolDown();
	int getMissiles() { return missileCount; }
	GameObject* Shoot(bool);
	bool hasMissile() { return missileCount > 0; }
};
