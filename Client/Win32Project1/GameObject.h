#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Sprite.h"

enum eID
{
	ID_Tank,
	ID_Brick,
	ID_Steel,
	ID_Bullet
};

class Object
{
protected:
	D3DXVECTOR2 position;
	eID id;
public:

	Object();

	eID GetID();

	D3DXVECTOR2 GetPosition();
	virtual void SetPosition(float x, float y);

	virtual void Initialize();
	virtual void Update(float deltatime);
	virtual void Draw();
};