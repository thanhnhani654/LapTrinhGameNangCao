#pragma once
#include "../GameObject.h"
#include "../Collision/Box2D.h"

class MapBounding : public Object
{
private:
	Box2D* box;
public:
	void Initialize(float x, float y,float w, float h);
	void Update(float deltatime);
	void Draw();
	void onCollision(Object* object);
	void Destroy();
};