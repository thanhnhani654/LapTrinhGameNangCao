#pragma once
#include "../GameObject.h"
#include "../Behavior/Movement.h"

class Bullet : public Object
{
protected:
	Sprite sprites;
	edirection direction;
	Movement move;

	//Su dung con tro tempposition thay cho position cua Object
	//vi co 1 cai loi nao do xay ra lam cho Bullet khong lay duoc &position
	LPD3DXVECTOR2 eposition;
public:
	void Initialize(D3DXVECTOR2 iposition, edirection idirection);
	void GetDirection(edirection idirection);

	void Update(float deltatime);
	void Draw();

};
