#include "Bulllet.h"
#include "../GameLog/GameLog.h"

void Bullet::Initialize(D3DXVECTOR2 iposition, edirection idirection, int ownerID)
{
	id = eID::ID_Bullet;
	this->ownerID = ownerID;
	position = iposition;
	direction = idirection;

	sprites.Initialize();

	eposition = new D3DXVECTOR2();
	eposition->x = position.x + 4.5f;
	eposition->y = position.y + 4.5f ;

	boxCollider = new Box2D(eposition, move.GetPtrVelocity(), 4.0f, 4.0f, move.GetPtrSpeed(), this);

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
	move.SetSpeed(90);
	Object::Initialize();
}

void Bullet::GetDirection(edirection idirection)
{
	direction = direction;
}

void Bullet::Update(float deltatime)
{
	move.projectTileMove(direction, deltatime);
	eposition->x += move.GetVelocity().x * deltatime / 1000;
	eposition->y += move.GetVelocity().y * deltatime / 1000;
}

void Bullet::Draw()
{
	sprites.Render(eposition->x - 5.5f, eposition->y - 3.5f);
}

void Bullet::onCollision(Object* object)
{
	if (object->getInGameID() != ownerID)
	{
		deleted = true;
		boxCollider->destroy();
	}
}

void Bullet::destroy()
{
	deleted = true;
}

bool Bullet::isDestroyed()
{
	return deleted;
}