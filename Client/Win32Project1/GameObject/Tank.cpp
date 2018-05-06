#include "Tank.h"

Tank::Tank()
{}

void Tank::Initialize()
{
	sprites.Initialize();
	movement = new Movement();
	movement->Initialize(&position);
	movement->SetSpeed(90.0f);
	state = normal;
	maxBullet = 1;
}

void Tank::Initialize(eTankType itype)
{
	sprites.Initialize();
	type = itype;
	movement = new Movement();
	movement->Initialize(&position);
	movement->SetSpeed(90.0f);
	state = normal;
	maxBullet = 100;
}

void Tank::Update(float deltatime)
{
	Animator();

	for (vector<Bullet>::iterator it = bullets.begin(); it != bullets.end(); ++it)
	{
		(*it).Update(deltatime);
	}
}

void Tank::Draw()
{
	sprites.Render(position.x, position.y);
	sprites.Next();

	for (vector<Bullet>::iterator it = bullets.begin(); it != bullets.end(); ++it)
	{
		(*it).Draw();
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

	Bullet bullet;// = new Bullet();
	bullet.Initialize(position, movement->GetDirection());

	bullets.push_back(bullet);
}