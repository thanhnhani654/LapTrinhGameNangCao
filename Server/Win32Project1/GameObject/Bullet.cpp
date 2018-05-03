#include "Bulllet.h"

void Bullet::Initialize(D3DXVECTOR2 iposition, edirection idirection)
{
	position = iposition;
	direction = idirection;

	sprites.Initialize();

	eposition = new D3DXVECTOR2();
	eposition->x = position.x -1;
	eposition->y = position.y - 1;

	switch (direction)
	{
	case UP:
		sprites.SetAnimation(AnimationName::Bullet_up);
		break;
	case DOWN:
		sprites.SetAnimation(AnimationName::Bullet_down);
		break;
	case LEFT:
		sprites.SetAnimation(AnimationName::Bullet_left);
		break;
	case RIGHT:
		sprites.SetAnimation(AnimationName::Bullet_right);
		break;
	default:
		break;
	}

	//move = new Movement();
	move.Initialize(eposition, this);
	move.SetSpeed(1);
}

void Bullet::GetDirection(edirection idirection)
{
	direction = direction;
}

void Bullet::Update(float deltatime)
{
	move.Move(direction);
}

void Bullet::Draw()
{
	sprites.Render(eposition->x, eposition->y);
}