#include "Player.h"

void Player::Initialize()
{
	tank.Initialize(eTankType::Tank_Yellow_1);
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
	_ProcessKeyBoard();
	
}

void Player::Update(float deltatime)
{
	Move(deltatime);
	static int multilsend = 0;
	//edirection temp = tank.GetMovement()->GetDirection();
	//NetWorkManage::getInstance()->CreateEventData(&temp, eObjectId::GameObject, Pl_Move_Event, sizeof(edirection));
	//Gui nhieu lan de dam bao Server nhan duoc. Do loi Server khong nhan duoc event khong biet do Server hay Client
	if (multilsend > 0)
	{
		multilsend -= 1;
		edirection temp = tank.GetMovement()->GetDirection();
		NetWorkManage::getInstance()->CreateEventData(&temp, eObjectId::GameObject, Pl_Move_Event, sizeof(edirection));
	}
	if (tank.GetMovement()->getPreviousDirection() != tank.GetMovement()->GetDirection())
	{
		edirection temp = tank.GetMovement()->GetDirection();
		NetWorkManage::getInstance()->CreateEventData(&temp, eObjectId::GameObject, Pl_Move_Event, sizeof(edirection));
		multilsend = 5;
	}

	static int delay = 60;
	if (delay < 0)
	{
		GAMELOG("X = %f			Y = %f", tank.GetPosition().x, tank.GetPosition().y);
		delay= 60;
	}
	else
		delay--;

	tank.Update(deltatime);


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
		tank.GetMovement()->Move(UP, deltatime);
		moveEvent = UP;
	}
	else if (IsKeyDown(DIK_S))
	{
		tank.GetMovement()->Move(DOWN, deltatime);
		moveEvent = DOWN;
	}
	else if (IsKeyDown(DIK_A))
	{
		tank.GetMovement()->Move(LEFT, deltatime);
		moveEvent = LEFT;
	}
	else if (IsKeyDown(DIK_D))
	{
		tank.GetMovement()->Move(RIGHT, deltatime);
		moveEvent = RIGHT;
	}
	else
	{
		tank.GetMovement()->Move(NIL, deltatime);
	}

	//NetWorkManage::CreateEventData(&moveEvent,eObjectId::GameObject, Pl_Move_Event, sizeof(moveEvent));

}

Tank* Player::getTank()
{
	return &tank;
}
