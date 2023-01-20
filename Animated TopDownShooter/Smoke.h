#pragma once
#include<iostream>
#include <stdlib.h> 
#include "GameObject.h"

using namespace std;

class Smoke : public GameObject
{
private:
	int life = 60;
	int alpha = 255;
	Vector2D moveDirection;
	const int speed = 1;
public:
	Smoke(const char* tex, int x, int y, int w, int h, int sc, int sw, float angle, Vector2D moveDir) : GameObject(tex, x, y, w, h, sc, sw, angle, "Smoke")
	{
		moveDirection = position.unit(moveDir);
	}
	Smoke(){}
	static GameObject* readSmoke(ifstream& file)
	{
		Smoke* temp = new Smoke();
		GameObject* readObject = temp->readFromFile(file);
		delete temp;
		temp = nullptr;
		return readObject;
	}
	GameObject* readFromFile(ifstream& file) override
	{
		GameObject* clone = GameObject::readGameObject(file);
		Vector2D temp;
		temp.Zero();
		Smoke* readSmoke = new Smoke(clone->path, clone->position.x, clone->position.y, clone->width, clone->health, clone->scale, clone->sheetWidth, clone->angle, temp);
		readSmoke->velocity = clone->velocity;
		delete clone;
		clone = nullptr;
		file >> readSmoke->life;
		file >> readSmoke->alpha;
		file >> readSmoke->moveDirection.x;
		file >> readSmoke->moveDirection.y;
		return readSmoke;
	}
	void writeToFile(ofstream& file) override
	{
		file << "[Smoke]\n";
		GameObject::writeToFile(file);
		file << life << "\n";
		file << alpha << "\n";
		file << moveDirection.x << "\n";
		file << moveDirection.y << "\n";
	}
	void Update() override
	{
		if (life > 1)
		{
			life--;
			setAlpha(alpha);
			alpha -= 4;
			position.x += moveDirection.x * speed;
			position.y += moveDirection.y * speed;

			UpdateDestRect();
		}
		else
		{
			Destroy();
		}

	}
};
