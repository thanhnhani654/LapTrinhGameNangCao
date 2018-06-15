#include "Tank.h"
#include "../GameLog/GameLog.h"
Tank::Tank()
{}

void Tank::Initialize()
{
	id = eID::ID_Tank;
	sprites.Initialize();
	movement = new Movement();
	
	movement->Initialize(&position, this);
	movement->SetSpeed(90.0f);
	state = normal;
	maxBullet = 1;
	test = 0;
	boxCollider = new Box2D(&position, movement->GetPtrVelocity(), 14.0f, 14.0f, movement->GetPtrSpeed(), this);
	Object::Initialize();
	disable = false;
}

void Tank::Initialize(eTankType itype)
{
	id = eID::ID_Tank;
	sprites.Initialize();
	type = itype;
	movement = new Movement();
	movement->Initialize(&position, this);
	movement->SetSpeed(90.0f);
	state = normal;
	maxBullet = 1;
	test = 0;
	boxCollider = new Box2D(&position, movement->GetPtrVelocity(), 14.0f, 14.0f, movement->GetPtrSpeed(), this);
	Object::Initialize();
	disable = false;
}

void Tank::UpdateInput(float deltatime)
{
	boxCollider->rePointPosition(&position);
}

void Tank::Update(float deltatime)
{
	Animator();

	SetPrevPosition();

	position.x += movement->GetVelocity().x * deltatime / 1000;
	position.y += movement->GetVelocity().y * deltatime / 1000;

	bool destroyBullet = false;

	for (vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
	{
		(*it)->Update(deltatime);
		if ((*it)->isDestroyed())
			destroyBullet = true;
	}

	if (destroyBullet)
	{
		bullets.erase(bullets.begin());
	}
}

void Tank::Draw()
{
	sprites.Render(position.x, position.y);
	sprites.Next();

	for (vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
	{
		(*it)->Draw();
	}
}

void Tank::onCollision(Object* object)
{
	if (object->GetID() == eID::ID_Bullet || object->GetID() == eID::ID_Tank)
	{
		this->disable = true;
	}
}

void Tank::SetType(eTankType itype)
{
	type = itype;
	if (state == normal);
	switch (movement->GetDirection())
	{
	case UP:
		sprites.SetAnimation(type * 4);
		break;
	case LEFT:
		sprites.SetAnimation(type * 4 + 1);
		break;
	case DOWN:
		sprites.SetAnimation(type * 4 + 2);
		break;
	case RIGHT:
		sprites.SetAnimation(type * 4 + 3);
		break;
	}
}

Movement* Tank::GetMovement()
{
	return movement;
}

void Tank::Animator()
{
	switch (state)
	{
	case normal:
		//Movement Animation
		if (movement->GetVelocity().y > 0)
			sprites.SetAnimation(type * 4);
		else if (movement->GetVelocity().x < 0)
			sprites.SetAnimation(type * 4 + 1);
		else if (movement->GetVelocity().y < 0)
			sprites.SetAnimation(type * 4 + 2);
		else if (movement->GetVelocity().x > 0)
			sprites.SetAnimation(type * 4 + 3);
		break;
	default:
		break;
	}
}

void Tank::Fire()
{
	if (bullets.size() >= maxBullet)
		return;

	Bullet* bullet = new Bullet();// = new Bullet();
	if (movement->GetDirection() == edirection::NIL)
		bullet->Initialize(position, movement->GetPreviousDirectionNotNIL(), inGameID);
	else	
		bullet->Initialize(position, movement->GetDirection(), inGameID);

	bullets.push_back(bullet);
}

void Tank::destroy()
{
	deleted = true;
}