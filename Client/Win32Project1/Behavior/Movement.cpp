#include "Movement.h"

void Movement::Initialize(D3DXVECTOR2* ipos, Object* tank)
{
	position = ipos;
	velocity.x = 0;
	velocity.y = 0;
	speed = 0;
	this->tank = tank;
	/*direction = NIL;
	previousDirection = NIL;
	previousDirectionNotNIL = NIL;*/
}

void Movement::Move(edirection idirection)
{
	previousDirection = direction;

	if (direction != NIL)
		previousDirectionNotNIL = direction;

	direction = idirection;
	switch (idirection)
	{
	case UP:
		velocity.y = speed;
		velocity.x = 0;

		//Giup cho xe tank bi co dinh lai khi xe Tank doi chieu.
		if (previousDirectionNotNIL == LEFT || previousDirectionNotNIL == RIGHT)
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

		if (previousDirectionNotNIL == UP || previousDirectionNotNIL == DOWN)
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

		if (previousDirectionNotNIL == UP || previousDirectionNotNIL == DOWN)
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
	return velocity;
}

D3DXVECTOR2* Movement::GetPtrVelocity()
{
	return &velocity;
}

void Movement::setVelocity(D3DXVECTOR2 ivec)
{
	velocity = ivec;
}

void Movement::setDeltaVec(D3DXVECTOR2 ivec)
{
	velocity += ivec;
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

edirection Movement::GetPreviousDirectionNotNIL()
{
	return previousDirectionNotNIL;
}

edirection Movement::getPreviousDirection()
{
	return previousDirection;
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

bool Movement::addMoveIfNew(edirection move)
{
	moveInfo temp;
	if (moveList.size() < 1)
	{
		temp.move = move;
		temp.time = clock();
		moveList.push_back(temp);
		return true;
	}
	if (move != moveList.back().move)
	{
		temp.move = move;
		temp.time = clock();
		moveList.push_back(temp);
		return true;
	}
	return false;
}