#include "Player.h"

Player* Player::inst;

void Player::Initialize()
{
	//tank = new Tank();
	
	tank.Initialize(eTankType::Tank_Yellow_1);
	inst = this;
	moveEvent = NIL;
	disable = true;
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

void Player::UpdateInput(float deltatime)
{
	//Server Debug
	//_ProcessKeyBoard();
	//Move(deltatime);

	//Update From Client
    tank.UpdateInput(deltatime);
	tank.SetPosition(tank.GetMovement()->Move(moveEvent, deltatime));
}

void Player::Update(float deltatime)
{
	edirection temp = tank.GetMovement()->GetDirection();
	edirection temp2 = tank.GetMovement()->GetPreviousDirection();

	static int tankdata = 0;

	tank.Update(deltatime);
	static int frame = 0;
	//if (frame > 00)
	{
		if (tank.GetPrevPosition() != tank.GetPosition() || frame > 60)
		{
			Server::getInstance()->CreateEventData(&tank.GetPosition(), eObjectId::PlayerID, tank.getInGameID(), Pl_Position, sizeof(D3DXVECTOR2));
		}
		if (tank.GetMovement()->GetDirection() != tank.GetMovement()->GetPreviousDirection() || frame > 60)
		{
			Server::getInstance()->CreateEventData(&temp, eObjectId::PlayerID, tank.getInGameID(), Pl_Move_Event, sizeof(edirection));
			frame = 0;
		}
		
		tankdata++;
	}
	frame++;
	
	if (tank.disable == true)
		disable = true;

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

Tank* Player::getTankPtr()
{
	return &tank;
}

edirection Player::getDirection()
{
	return moveEvent;
}