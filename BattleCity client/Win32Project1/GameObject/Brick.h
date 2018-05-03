#pragma once
#include "../GameObject.h"
#include <vector>

using namespace std;

struct BrickSprite
{
	D3DXVECTOR2 position;
	Sprite sprite;

	D3DXVECTOR2 GetPosition() { return position; }
};

//Day la 1 class hoi bi dac biet ==> F***
class Brick : public Object
{
protected:
	vector <BrickSprite> sprites;
public:
	void Initialize();
	void AddBrick(int x, int y);
	void Draw();
	
};