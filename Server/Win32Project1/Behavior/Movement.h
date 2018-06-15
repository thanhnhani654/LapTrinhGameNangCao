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
	edirection previousDirectionNotNIL;

	float speed;

	Object* tank;
public:
	void Initialize(D3DXVECTOR2* ipos, Object* tank);
	//void Initialize(D3DXVECTOR2& ipos, Object* tank);

	D3DXVECTOR2 Move(edirection idirection, float deltatime);
	void projectTileMove(edirection idirection, float deltatime);

	D3DXVECTOR2 GetVelocity();
	D3DXVECTOR2* GetPtrVelocity();

	void SetSpeed(float speed);

	float GetSpeed();
	float* GetPtrSpeed();

	edirection GetDirection();
	edirection GetPreviousDirection();
	edirection GetPreviousDirectionNotNIL();

	void setPosition(D3DXVECTOR2 pos);
	D3DXVECTOR2 getPosition();

	float Round8(float number);
};