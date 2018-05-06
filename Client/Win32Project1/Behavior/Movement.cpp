#include "Movement.h"

void Movement::Initialize(D3DXVECTOR2* ipos)
{
	position = ipos;
	velocity.x = 0;
	velocity.y = 0;
	speed = 0;
}

void Movement::Move(edirection idirection, float deltatime)
{
	previousDirection = direction;

	//if (idirection != NIL)
		direction = idirection;
	switch (idirection)
	{
	case UP:
		velocity.y = speed;
		velocity.x = 0;

		//Giup cho xe tank bi co dinh lai khi xe Tank doi chieu.
		if (previousDirection == LEFT || previousDirection == RIGHT)
		{
			int normalize = round(position->x / 8.0f);
			normalize *= 8;
			position->x = normalize;
		}

		break;
	case DOWN:
		velocity.y = -speed;
		velocity.x = 0;

		if (previousDirection == LEFT || previousDirection == RIGHT)
		{
			int normalize = round(position->x / 8.0);
			normalize *= 8;
			position->x = normalize;
		}

		break;
	case LEFT:
		velocity.x = -speed;
		velocity.y = 0;

		if (previousDirection == UP || previousDirection == DOWN)
		{
			long normalize = roundl(position->y / 8.0f);
			normalize *= 8;
			position->y = normalize;
		}

		break;
	case RIGHT:
		velocity.x = speed;
		velocity.y = 0;

		if (previousDirection == UP || previousDirection == DOWN)
		{
			long normalize = roundl(position->y / 8.0f);
			normalize *= 8;
			position->y = normalize;
		}


		break;
	case NIL:
		velocity.x = 0;
		velocity.y = 0;
	}
	float del = deltatime / 1000.0f;
	position->x += velocity.x * del;// *(deltatime / (1000.0f / 60));
	position->y += velocity.y * del;// *(deltatime / (1000.0f / 60));
}

D3DXVECTOR2 Movement::GetVelocity()
{
	return velocity;
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

edirection Movement::GetDirection()
{
	return direction;
}

edirection Movement::getPreviousDirection()
{
	return previousDirection;
}