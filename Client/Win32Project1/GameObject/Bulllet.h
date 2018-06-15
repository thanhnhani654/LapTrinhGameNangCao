#pragma once
#include "../GameObject.h"
#include "../Behavior/Movement.h"
#include "../Collision/Box2D.h"
#include <vector>

class Bullet : public Object
{
protected:
	
	
	Movement move;
	
	int ownerID;

	//Su dung con tro tempposition thay cho position cua Object
	//vi co 1 cai loi nao do xay ra lam cho Bullet khong lay duoc &position
	LPD3DXVECTOR2 eposition;
public:
	Sprite* sprites;
	Box2D* boxCollider;
	edirection direction;
	void Initialize(D3DXVECTOR2 iposition, edirection idirection, int ownerID);
	void GetDirection(edirection idirection);
	Movement* GetMovementComponent();

	void UpdateInput(float deltatime);
	void Update(float deltatime);
	void Draw();
	void onCollision(Object* object);
	void destroy();
	bool isDestroyed();
	D3DXVECTOR2 Bullet::getEPosition();
	void setEPosition(float x, float y);
	//void destroyBulletProcess();

};
