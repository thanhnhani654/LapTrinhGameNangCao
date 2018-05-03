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
	//_ProcessKeyBoard();
	//Move();
}

void Player::Update(float deltatime)
{
	tank.GetMovement().setPosition(tank.GetPosition());
	tank.GetMovement().Move(moveEvent);
	tank.SetPosition(tank.GetMovement().getPosition().x, tank.GetMovement().getPosition().y);

	tank.Update(deltatime);



	//static float t = 0;
	//if (deltatime > 0)
	//	t += deltatime;
	//if (t > 1000)
	//{
	//	GAMELOG("x: %f			y: %f", tank.GetPosition().x, tank.GetPosition().y);
	//	t = 0;
	//}
}

void Player::Draw()
{
	tank.Draw();
}

void Player::Move()
{
	edirection moveEvent = NIL;
	if (IsKeyDown(DIK_W))
	{
		tank.GetMovement().Move(UP);
		moveEvent = UP;
	}
	else if (IsKeyDown(DIK_S))
	{
		tank.GetMovement().Move(DOWN);
		moveEvent = DOWN;
	}
	else if (IsKeyDown(DIK_A))
	{
		tank.GetMovement().Move(LEFT);
		moveEvent = LEFT;
	}
	else if (IsKeyDown(DIK_D))
	{
		tank.GetMovement().Move(RIGHT);
		moveEvent = RIGHT;
	}
	else
	{
		tank.GetMovement().Move(NIL);
	}

	//NetWorkManage::CreateEventData(&moveEvent,eObjectId::GameObject, Pl_Move_Event, sizeof(moveEvent));

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
