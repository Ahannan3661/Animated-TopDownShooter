#pragma once
#include<iostream>
#include <stdlib.h> 
#include "GameObject.h"

using namespace std;

class Explosion : public GameObject
{
private:
	int life;
public:
	Explosion(const char* tex, int x, int y, int w, int h, int sc, int sw, float angle, int speed, int life) : GameObject(tex, x, y, w, h, sc, sw, angle, "Explosion")
	{
		this->life = life;
		this->speed = speed;
	}
	Explosion() { life = 0; }
	static GameObject* readExplosion(ifstream& file)
	{
		Explosion* temp = new Explosion();
		GameObject* readObject = temp->readFromFile(file);
		delete temp;
		temp = nullptr;
		return readObject;
	}
	GameObject* readFromFile(ifstream& file) override
	{
		GameObject* clone = GameObject::readGameObject(file);
		int life;
		file >> life;
		Explosion* readExplosion = new Explosion(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth, clone->angle, clone->speed, life);
		delete clone;
		clone = nullptr;
		return readExplosion;
	}
	void writeToFile(ofstream& file) override
	{
		file << "[Explosion]\n";
		GameObject::writeToFile(file);
		file << life << "\n";
	}
	void Update() override
	{
		if (life > 0)
		{
			life--;
			position.x += cos(getAngle()) * speed;
			position.y += sin(getAngle()) * speed;

			UpdateDestRect();
		}
		else
		{
			Destroy();
		}
	}
};
