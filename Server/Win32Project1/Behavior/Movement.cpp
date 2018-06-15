#include "Movement.h"
#include "../GameLog/GameLog.h"

void Movement::Initialize(D3DXVECTOR2* ipos, Object* tank)
{
	this->position = ipos;
	velocity.x = 0;
	velocity.y = 0;
	speed = 0;
	this->tank = tank;
}

D3DXVECTOR2 Movement::Move(edirection idirection, float deltatime)
{
	previousDirection = direction;
	if (direction != NIL)
		previousDirectionNotNIL = direction;
	//if (idirection != NIL)
		direction = idirection;
	switch (idirection)
	{
	case UP:
		velocity.y = speed;
		velocity.x = 0;

		//Giup cho xe tank bi co dinh lai khi xe Tank doi chieu.
		if (previousDirectionNotNIL == LEFT || previousDirectionNotNIL == RIGHT && tank->GetID() != eID::ID_Bullet)
		{
			/*int normalize = round(position->x / 8.0f);
			normalize *= 8;
			position->x = normalize;*/
			position->x = Round8(position->x);
		}

		break;
	case DOWN:
		velocity.y = -speed;
		velocity.x = 0;

		if (previousDirectionNotNIL == LEFT || previousDirectionNotNIL == RIGHT)
		{
			/*int normalize = round(position->x / 8.0);
			normalize *= 8;
			position->x = normalize;*/
			position->x = Round8(position->x);
		}

		break;
	case LEFT:
		velocity.x = -speed;
		velocity.y = 0;

		if (previousDirectionNotNIL == UP || previousDirectionNotNIL == DOWN && tank->GetID() != eID::ID_Bullet)
		{
			/*long normalize = roundl(position->y / 8.0f);
			normalize *= 8;
			position->y = normalize;*/
			position->y = Round8(position->y);
		}

		break;
	case RIGHT:
		velocity.x = speed;
		velocity.y = 0;

		if (previousDirectionNotNIL == UP || previousDirectionNotNIL == DOWN && tank->GetID() != eID::ID_Bullet)
		{
			/*long normalize = roundl(position->y / 8.0f);
			normalize *= 8;
			position->y = normalize;*/
			position->y = Round8(position->y);
		}
		break;
	case NIL:
		velocity.x = 0;
		velocity.y = 0;
	}
	
	D3DXVECTOR2 temp = *position;
	return temp;
}

void Movement::projectTileMove(edirection idirection, float deltatime)
{
	previousDirection = direction;

	if (idirection != NIL)
		direction = idirection;
	switch (idirection)
	{
	case UP:
		velocity.y = speed;
		velocity.x = 0;

		break;
	case DOWN:
		velocity.y = -speed;
		velocity.x = 0;

		break;
	case LEFT:
		velocity.x = -speed;
		velocity.y = 0;

		break;
	case RIGHT:
		velocity.x = speed;
		velocity.y = 0;

		break;
	case NIL:
		velocity.x = 0;
		velocity.y = 0;
	}
}

D3DXVECTOR2 Movement::GetVelocity()
{
	if (velocity.x != 0 || velocity.y != 0)
		int a = 0;
	return velocity;
}

D3DXVECTOR2* Movement::GetPtrVelocity()
{
	return &velocity;
}

void Movement::SetSpeed(float ispeed)
{
	speed = ispeed;
	velocity.x = 0;
	velocity.y = 0;
}

float Movement::GetSpeed()
{
	return speed;
}

float* Movement::GetPtrSpeed()
{
	return &speed;
}

edirection Movement::GetDirection()
{
	return direction;
}

edirection Movement::GetPreviousDirection()
{
	return previousDirection;
}

edirection Movement::GetPreviousDirectionNotNIL()
{
	return previousDirectionNotNIL;
}

void Movement::setPosition(D3DXVECTOR2 pos)
{
	position->x = pos.x;
	position->y = pos.y;
}

D3DXVECTOR2 Movement::getPosition()
{
	return *position;
}

float Movement::Round8(float number)
{
	int temp;
	temp = (number / 8.0f) + 0.5f;
	return 8 * temp;
}