#pragma once
#include "Game.h"
#include "Vector2D.h"
#include <iostream>
#include <fstream>
#include "TextureManager.h"


using namespace std;

class GameObject
{	
public:
	char* path;
	int health;
	float angle = 0.0f;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	bool alive = true;
	int scale;
	int sheetWidth;
	bool explodeFlag = false;
	int speed = 0;
	const float friction = 0.95f;
	Vector2D velocity;
	Vector2D position;
	int width;
	int height;
	string tag;
	bool hostile = false;

	virtual void writeToFile(ofstream& file)
	{
		file << path << "\n";
		file << health << "\n";
		file << angle << "\n";
		file << alive ? 1 : 0;
		file << "\n";
		file << scale << "\n";
		file << sheetWidth << "\n";
		file << speed << "\n";
		file << velocity.x << "\n";
		file << velocity.y << "\n";
		file << position.x << "\n";
		file << position.y << "\n";
		file << width << "\n";
		file << height << "\n";
		file << tag << "\n";
		file << hostile ? 1 : 0;
		file << "\n";
		file << srcRect.x << "\n";
		file << srcRect.y << "\n";
	}
	static GameObject* readGameObject(ifstream& file)
	{
		GameObject* temp = new GameObject();
		GameObject* readObject(temp->readFromFile(file));
		delete temp;
		temp = nullptr;
		return readObject;
	}

	virtual GameObject* readFromFile(ifstream& file)
	{
		char* path = _strdup(" ");
		file >> path;
		int health;
		file >> health;
		float angle;
		file >> angle;
		bool alive;
		int temp; file >> temp; temp == 1 ? alive = true : alive = false;
		int scale;
		file >> scale;
		int sheetWidth;
		file >> sheetWidth;
		int speed;
		file >> speed;
		Vector2D velocity;
		file >> velocity.x;
		file >> velocity.y;
		Vector2D position;
		file >> position.x;
		file >> position.y;
		int width;
		file >> width;
		int height;
		file >> height;
		string tag;
		file >> tag;
		bool hostile;
		temp; file >> temp; temp == 1 ? hostile = true : hostile = false;
		SDL_Rect srcRect;
		file >> srcRect.x;
		file >> srcRect.y;
		GameObject* clone = new GameObject(path, position.x, position.y, width, height, scale, sheetWidth, angle, tag);
		clone->velocity = velocity;
		clone->health = health;
		clone->alive = alive;
		clone->speed = speed;
		clone->hostile = hostile;
		clone->srcRect.x = srcRect.x;
		clone->srcRect.y = srcRect.y;
		return clone;
	}

	GameObject(const char*, int, int, int, int, int, int, float, string);
	GameObject() {}
	~GameObject();

	void nextSprite();

	void setAlpha(int a) { SDL_SetTextureAlphaMod(texture, a); }

	void setExplode() { explodeFlag = true; }
	bool getExplode() { return explodeFlag; }
	bool IsHostile() { return hostile; }

	bool hasTag(const char* tag)
	{
		return strcmp(this->tag.c_str(), tag) == 0;
	}

	float getAngle() { return angle; }

	virtual void takeHit(int damage) { if (health > damage) health -= damage; else { alive = false; setExplode(); } }

	int getHealth() { return health; }

	void setHealth(int h) { health = h; }

	void setAngle(float a) { angle = a; }

	virtual bool bossCollission(GameObject* obj) { return false; }
	virtual GameObject* bossDist(GameObject* obj) { return nullptr; }

	virtual void Update() {}

	virtual void Render();

	bool Alive() { return alive; }

	void Destroy() { alive = false; }

	void setTex(SDL_Texture* tex) { texture = tex; }

	void UpdateDestRect();

	void Translate(float a, float b) { strcmp(tag.c_str(), "Player") == 0 ? velocity.x = a : abs(velocity.x) - abs(a) < 0.1f ? velocity.x = a : velocity.x *= 1; velocity.y = b; }

};

