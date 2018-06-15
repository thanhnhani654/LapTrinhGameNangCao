#pragma once
#include "Box2D.h"
#include "../GameObject.h"


class CollisionManager
{
public:
	static CollisionManager* instance;
	static CollisionManager* getInstance();

	
	float CollisionDetection(Box2D object1, Box2D object2, int* normalx, int* normaly, float deltatime);

	bool IsIntersection(Box object1, Box object2);
	Box* getBroadphaseBox(Box2D &object, float delta);
	Box getBroadphaseBo(Box2D &object, float delta);

	void doCollision(float deltatime);
};
