#pragma once
#include "Sprite.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

enum eID
{
	ID_Tank,
	ID_Brick,
	ID_Steel,
	ID_Bullet,
	ID_MapBounding
};

class Object
{
protected:
	D3DXVECTOR2 position;
	D3DXVECTOR2 prevPosition;
	eID id;
	uint8_t inGameID;
	bool deleted;
public:
	static std::vector<Object*> listObject;

	Object();

	eID GetID();

	D3DXVECTOR2 GetPosition();
	D3DXVECTOR2 GetPrevPosition();
	uint32_t getInGameID();

	virtual void SetPosition(float x, float y);
	virtual void SetPosition(D3DXVECTOR2 pos);
	void SetPrevPosition();

	virtual void Initialize();
	virtual void Update(float deltatime);
	virtual void Draw();
	virtual void onCollision(Object* object);
	virtual void destroy();

	static void DestroyProcess();
};