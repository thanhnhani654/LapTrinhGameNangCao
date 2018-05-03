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
	if (tank.GetMovement()->getPreviousDirection() != tank.GetMovement()->GetDirection())
	{
		edirection temp = tank.GetMovement()->GetDirection();
		NetWorkManage::CreateEventData(&temp, eObjectId::GameObject, Pl_Move_Event, sizeof(edirection));
	}

	static int delay = 60;
	if (delay < 0)
	{
		delay = 60;
	}
	else
		delay--;

	static float t = 0;
	t += deltatime;
	if (t > 1000)
	{
		GAMELOG("x: %f			y: %f", tank.GetPosition().x, tank.GetPosition().y);
		t = 0;
	}

	tank.Update(deltatime);
	
	if (NetWorkManage::testdata == nullptr)
		return;

	float tempfloat;
	char* tempdata = new char(sizeof(FLOAT));

	memcpy(tempdata, NetWorkManage::testdata, 4);
	tempfloat = (FLOAT)(*tempdata);
	tank.SetPosition(tempfloat, tank.GetPosition().y);
	
	memcpy(tempdata, NetWorkManage::testdata + 4, 4);
	tempfloat = (FLOAT)(*tempdata);
	tank.SetPosition(tank.GetPosition().x, tempfloat);
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

