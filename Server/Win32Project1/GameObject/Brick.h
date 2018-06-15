#pragma once
#include "../GameObject.h"
#include <vector>
#include "../Collision/Box2D.h"

using namespace std;

struct BrickSprite
{
	D3DXVECTOR2 position;
	Sprite sprite;
	bool disable = false;
	Box2D* box;

	D3DXVECTOR2 GetPosition() { return position; }
};

//Day la 1 class hoi bi dac biet ==> F***
class Brick : public Object
{
protected:
	vector <BrickSprite> sprites;
	Sprite* brick;
	Box2D* boxTL;
	Box2D* boxTR;
	Box2D* boxBL;
	Box2D* boxBR;


public:
	void Initialize();
	void AddBrick(int x, int y);
	void Draw();
	void onCollision(Object* object);
	void destroy();
};