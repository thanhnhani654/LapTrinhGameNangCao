#include "Tank.h"
#include "../GameLog/GameLog.h"

Tank::Tank()
{}

void Tank::Initialize()
{
	sprites.Initialize();
	movement = new Movement();
	movement->Initialize(&position, this);
	movement->SetSpeed(90.0f);
	state = normal;
	maxBullet = 1;
	boxCollider = new Box2D(&position, movement->GetPtrVelocity(), 14.0f, 14.0f, movement->GetPtrSpeed(), this);
	Object::Initialize();
	disable = false;
	cooldown = 0;
	alreadyDie = false;
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
	boxCollider = new Box2D(&position, movement->GetPtrVelocity(), 14.0f, 14.0f, movement->GetPtrSpeed(), this);
	Object::Initialize();
	disable = false;
	cooldown = 0;
	alreadyDie = false;
}

void Tank::UpdateInput(float deltatime)
{
	//boxCollider->rePointPosition(&position);
}

void Tank::Update(float deltatime)
{
	if (boxCollider->disable)
		this->disable = true;

	tankDied();

	if (died)
		return;
	Animator();

	/*if (movement->getPreviousDirection() != movement->GetDirection())
		GAMELOG("Before: x: %f			y:%f", position.x, position.y);*/

	position.x += movement->GetVelocity().x * deltatime / 1000;
	position.y += movement->GetVelocity().y * deltatime / 1000;

	//if (movement->getPreviousDirection() != movement->GetDirection() && movement->GetDirection() != edirection::NIL)
	//	GAMELOG("After: x: %f			y:%f		direction: %d", position.x, position.y, movement->GetDirection());

	bool destroyBullet = false;

	for (vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
	{
		(*it)->Update(deltatime);
		if ((*it)->isDestroyed())
			destroyBullet = true;
	}

	if (destroyBullet)
	{
		cooldown = 50;
		bullets.front()->boxCollider->destroy();
		delete bullets.front()->sprites;
		bullets.erase(bullets.begin());
	}
	cooldown -= deltatime;
	if (!bullets.empty())
		bCanFire = false;
	else
		bCanFire = true;
	
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
	if (died)
		return;
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

bool Tank::Fire()
{
	if (bullets.size() >= maxBullet)
		return false;

	Bullet* bullet = new Bullet();
	if (movement->GetDirection() == edirection::NIL)
		bullet->Initialize(position, movement->GetPreviousDirectionNotNIL(), inGameID);
	else
		bullet->Initialize(position, movement->GetDirection(), inGameID);

	bullets.push_back(bullet);
	return true;
}

void Tank::bulletPrediction(float ping)
{
	if (bullets.size() == 0)
		return;
	bullets.front()->GetMovementComponent()->projectTileMove(bullets.front()->direction, ping);
	//bullets.front()->Update(ping);
}

void Tank::destroy()
{
	deleted = true;
}

void Tank::onCollision(Object* object)
{
	/*if (object->GetID() == eID::ID_Bullet || object->GetID() == eID::ID_Tank)
	{
		this->disable = true;
	}*/
}

void Tank::tankDisable()
{
	disable = true;
	//boxCollider->disable = true;
}

void Tank::tankDied()
{
	if (!died)
		return;
	boxCollider->disable = true;
	static int countFrame = 0;
	if (countFrame >= 0)
		this->sprites.SetAnimation(AnimationName::effect_explosion1);
	if (countFrame > 3)
		this->sprites.SetAnimation(AnimationName::effect_explosion2);
	if (countFrame > 9)
		this->sprites.SetAnimation(AnimationName::effect_explosion3);
	if (countFrame > 12)
	{
		tankDisable();
		alreadyDie = true;
	}
	countFrame++;
}