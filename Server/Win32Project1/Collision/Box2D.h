#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>

#include "../GameObject.h"

struct Box
{
	//Lay toa do bang cach lay dia chi bien toa do object => object thay doi toa do dan den box tu dong thay doi theo
	D3DXVECTOR2* pos;
	float w;
	float h;

	Box() 
	{
		pos = new D3DXVECTOR2();
	}
	Box(D3DXVECTOR2* pos, float w, float h)
	{
		this->pos = pos;
		this->w = w;
		this->h = h;
	}

	void destroy()
	{
		delete pos;
	}
};

enum eType : uint8_t
{
	Dynamic,
	Static
};

class Box2D
{
protected:
	Box collider;
	D3DXVECTOR2* velocity;
	bool destroyed;
	//1: Dynamic    2:Static
	eType type;  
	Object* object;

public:

	Box2D();

	//Use for Dynamic Object
	Box2D(D3DXVECTOR2* ipos, D3DXVECTOR2* iveclocity, float w, float h, float* speed, Object* object);

	//Use for Static Object
	Box2D(D3DXVECTOR2 ipos, float w, float h, Object* object);
	
	float* speed;
	//Only for Static Object;
	bool getShot;
	bool disable;

	static std::vector<Box2D*> listBox;

	void setPosition(D3DXVECTOR2 ipos);
	Box getBox();
	void setVelocity(D3DXVECTOR2 ivec);
	D3DXVECTOR2 getVelocity();
	eType getType();
	void destroy();
	Object* getObject();
	void rePointPosition(D3DXVECTOR2* pos);

	static void debugDraw(LPDIRECT3DDEVICE9 d3ddv);

	static void destroyProcess();
};