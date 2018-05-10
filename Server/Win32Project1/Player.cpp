#include "Player.h"

Player* Player::inst;

void Player::Initialize()
{
	//tank = new Tank();
	tank.Initialize(eTankType::Tank_Yellow_1);
	inst = this;
	moveEvent = NIL;
}

void Player::OnKeyDown(int Keycode)
{
	//Dung de test cac loai tank. dung xoa
	static int i = 1;
	switch (Keycode)
	{
	case DIK_U:
		tank.SetType((eTankType)i);
		i++;
	case DIK_I:
		tank.sprites.SetAnimation(i);
		i++;
		break;
	case DIK_O:
		tank.sprites.SetAnimation(i);
		i--;
		break;
	case DIK_J:
		tank.Fire();
		break;
	default:
		break;
	}
}

void Player::UpdateInput()
{
}

void Player::Update(float deltatime)
{
	tank.GetMovement().setPosition(tank.GetPosition());
	tank.GetMovement().Move(moveEvent, deltatime);

	tank.Update(deltatime);

	static int delay = 60;
	if (delay < 0)
	{
		//GAMELOG("X = %f			Y = %f", tank.GetPosition().x, tank.GetPosition().y);
		delay = 60;
	}
	else
		delay--;
}

void Player::Draw()
{
	tank.Draw();
}

void Player::Move(float deltatime)
{
	edirection moveEvent = NIL;
	if (IsKeyDown(DIK_W))
	{
		tank.GetMovement().Move(UP, deltatime);
		moveEvent = UP;
	}
	else if (IsKeyDown(DIK_S))
	{
		tank.GetMovement().Move(DOWN, deltatime);
		moveEvent = DOWN;
	}
	else if (IsKeyDown(DIK_A))
	{
		tank.GetMovement().Move(LEFT, deltatime);
		moveEvent = LEFT;
	}
	else if (IsKeyDown(DIK_D))
	{
		tank.GetMovement().Move(RIGHT, deltatime);
		moveEvent = RIGHT;
	}
	else
	{
		tank.GetMovement().Move(NIL, deltatime);
	}

}

void Player::Move(edirection moveEvent)
{
	this->moveEvent = moveEvent;
	
}

Player* Player::getInstance()
{
	if (inst == NULL)
		return NULL;
	return inst;
}

Tank Player::getTank()
{
	return tank;
}

edirection Player::getDirection()
{
	return moveEvent;
}