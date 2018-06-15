#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include "../GameObject.h"
#include <time.h>

using namespace std;

enum edirection : uint8_t
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NIL
};

struct moveInfo
{
	edirection move;
	clock_t time;
	float runTime;
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
	vector<moveInfo> moveList;

	void Initialize(D3DXVECTOR2* ipos, Object* tank);

	void Move(edirection idirection);
	void projectTileMove(edirection idirection, float deltatime);

	D3DXVECTOR2 GetVelocity();
	D3DXVECTOR2* GetPtrVelocity();

	void SetSpeed(float speed);
	void setVelocity(D3DXVECTOR2 ivec);
	void setDeltaVec(D3DXVECTOR2 ivec);

	float GetSpeed();
	float* GetPtrSpeed();

	edirection GetDirection();
	edirection GetPreviousDirectionNotNIL();

	void setPosition(D3DXVECTOR2 pos);
	D3DXVECTOR2 getPosition();
	edirection getPreviousDirection();
	float Round8(float number);

	bool addMoveIfNew(edirection move);
};