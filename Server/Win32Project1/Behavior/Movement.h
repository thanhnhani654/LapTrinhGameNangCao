#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include "../GameObject.h"

using namespace std;

enum edirection : uint8_t
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NIL
};

class Movement
{
protected:
	D3DXVECTOR2* position;

	D3DXVECTOR2 velocity;

	edirection direction;
	edirection previousDirection;

	float virtualX;
	float virtualY;

	float speed;

	Object* tank;
public:
	void Initialize(D3DXVECTOR2* ipos, Object* tank);

	void Move(edirection idirection, float deltatime);

	D3DXVECTOR2 GetVelocity();

	void SetSpeed(float speed);

	float GetSpeed();

	edirection GetDirection();

	void setPosition(D3DXVECTOR2 pos);
	D3DXVECTOR2 getPosition();
};