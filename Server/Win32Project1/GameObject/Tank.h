#pragma once
#include "../GameObject.h"
#include "../Sprite.h"
#include "../Behavior/Movement.h"
#include "Bulllet.h"
#include <vector>
#include "../Collision/Box2D.h"

enum eTankType
{
	Tank_Yellow_1 = 0,
	Tank_Yellow_2 = 1,
	Tank_Yellow_3,
	Tank_Yellow_4,
	Tank_Yellow_5,
	Tank_Yellow_6,
	Tank_Yellow_7,
	Tank_Yellow_8,
	Tank_Green_1,
	Tank_Green_2,
	Tank_Green_3,
	Tank_Green_4,
	Tank_Green_5,
	Tank_Green_6,
	Tank_Green_7,
	Tank_Green_8,
	Tank_White_1,
	Tank_White_2,
	Tank_White_3,
	Tank_White_4,
	Tank_White_5,
	Tank_White_6,
	Tank_White_7,
	Tank_White_8,
	Tank_Red_1,
	Tank_Red_2,
	Tank_Red_3,
	Tank_Red_4,
	Tank_Red_5,
	Tank_Red_6,
	Tank_Red_7,
	Tank_Red_8,
};

enum eTankState
{
	normal
};

class Tank : public Object
{
protected:
	eTankType	type;
	eTankState	state;
	Movement*	movement;
	vector <Bullet*> bullets;
	int maxBullet;

	Box2D* boxCollider;

	int test;

public:
	bool disable;
	Sprite sprites;
	Tank();
	void Initialize();
	void Initialize(eTankType itype);
	void UpdateInput(float deltatime);
	void Update(float deltatime);
	void Draw();
	void onCollision(Object* object);
	void Animator();

	void SetType(eTankType itype);
	Movement* GetMovement();

	void Fire();
	
	void destroy();
};