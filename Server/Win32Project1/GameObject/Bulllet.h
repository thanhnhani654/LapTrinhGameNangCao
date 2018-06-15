#pragma once
#include "../GameObject.h"
#include "../Behavior/Movement.h"
#include "../Collision/Box2D.h"
#include <vector>

class Bullet : public Object
{
protected:
	Sprite sprites;
	edirection direction;
	Movement move;
	Box2D* boxCollider;
	int ownerID;

	//Su dung con tro tempposition thay cho position cua Object
	//vi co 1 cai loi nao do xay ra lam cho Bullet khong lay duoc &position
	LPD3DXVECTOR2 eposition;
public:

	void Initialize(D3DXVECTOR2 iposition, edirection idirection, int ownerID);
	void GetDirection(edirection idirection);

	void Update(float deltatime);
	void Draw();
	void onCollision(Object* object);
	void destroy();
	bool isDestroyed();
	//void destroyBulletProcess();

};
