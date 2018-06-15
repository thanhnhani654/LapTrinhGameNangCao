#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Sprite.h"

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
	eID id;
	uint32_t inGameID;
	bool deleted;
public:
	static std::vector<Object*> listObject;

	Object();

	eID GetID();
	uint32_t getInGameID();

	D3DXVECTOR2 GetPosition();
	virtual void SetPosition(float x, float y);

	virtual void Initialize();
	virtual void Update(float deltatime);
	virtual void Draw();
	virtual void onCollision(Object* object);
	virtual void destroy();

	static void DestroyProcess();
};