#pragma once
#include "../GameObject.h"
#include <vector>
#include "Brick.h"

using namespace std;

//Day la 1 class hoi bi dac biet ==> F***
class Steel : public Object
{
protected:
	vector <BrickSprite> sprites;
public:
	void Initialize();
	void AddSteel(int x, int y);
	void Draw();

};
