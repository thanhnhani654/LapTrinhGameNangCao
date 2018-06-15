#include "Player.h"

void Player::Initialize()
{
	playerID = 0;
	tank.Initialize(eTankType::Tank_Yellow_1);
	moveEvent = NIL;
	disable = true;
	bStartGame = false;
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
	case DIK_RETURN:
		if (this->tank.getInGameID() == NetWorkManage::getInstance()->playerID && !bStartGame)
		{
			char temp = 's';
			NetWorkManage::getInstance()->CreateEventData(&temp, eObjectId::PlayerID, this->getTank()->getInGameID(), funcId::Pl_StartGame, 1);
			LoadingScreen::getInstance()->DisableAll();
			NetWorkManage::getInstance()->bStartGame = true;
			bStartGame = true;
		}
	case DIK_J:
		{
		//if (tank.Fire())
		//{
		
		uint8_t code = Keycode;
		if (tank.bCanFire && tank.cooldown < 0 && !tankFired)
		{
			tank.bCanFire = false;
			NetWorkManage::getInstance()->CreateEventData(&code, eObjectId::PlayerID, tank.getInGameID(), Pl_Fire, sizeof(uint8_t));
			tankFired = true;
			tankFiredRunTime = 0;
		}
		}
		break;
	default:
		break;
	}
}

void Player::UpdateInput(float deltatime)
{
	_ProcessKeyBoard();

	if (playerID == tank.getInGameID())
	{
		
		Move2(deltatime);
		Move3(deltatime);
		tank.GetMovement()->Move(moveEvent);
		
	}
	else
	{
		tank.GetMovement()->Move(moveEvent);
	}
}

void Player::Update(float deltatime)
{
	TankFire(deltatime);

	tank.Update(deltatime);
	if (tank.disable)
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
		tank.GetMovement()->Move(UP);
		moveEvent = UP;
	}
	else if (IsKeyDown(DIK_S))
	{
		tank.GetMovement()->Move(DOWN);
		moveEvent = DOWN;
	}
	else if (IsKeyDown(DIK_A))
	{
		tank.GetMovement()->Move(LEFT);
		moveEvent = LEFT;
	}
	else if (IsKeyDown(DIK_D))
	{
		tank.GetMovement()->Move(RIGHT);
		moveEvent = RIGHT;
	}
	else
	{
		tank.GetMovement()->Move(NIL);
	}
}

void Player::Move2(float deltatime)
{
	edirection temp = UP;
	//edirection moveEvent = NIL;
	if (IsKeyDown(DIK_W))
	{
		temp = UP;
	}
	else if (IsKeyDown(DIK_S))
	{
		temp = DOWN;
	}
	else if (IsKeyDown(DIK_A))
	{
		temp = LEFT;
	}
	else if (IsKeyDown(DIK_D))
	{
		temp = RIGHT;
	}
	else
	{
		temp = NIL;
	}

	if (tank.GetMovement()->addMoveIfNew(temp))
		NetWorkManage::getInstance()->CreateEventData(&temp, eObjectId::PlayerID, tank.getInGameID(), Pl_Move_Event, sizeof(edirection));
}

void Player::Move3(float deltatime)
{
	if (tank.GetMovement()->moveList.empty())
		return;

	for (int i = 0; i < tank.GetMovement()->moveList.size(); i++)
	{
		tank.GetMovement()->moveList[i].runTime += deltatime;
	}
	moveInfo temp = tank.GetMovement()->moveList.front();

	if (temp.runTime > NetWorkManage::getInstance()->RTT / 2)
	{
		moveEvent = temp.move;
		tank.GetMovement()->moveList.erase(tank.GetMovement()->moveList.begin());
	}
	
}

void Player::TankFire(float deltatime)
{
	if (!tankFired)
		return;

	if (tankFiredRunTime > NetWorkManage::getInstance()->RTT / 2)
	{
		tank.Fire();
		tankFired = false;
		return;
	}

	tankFiredRunTime += deltatime;


}

Tank* Player::getTank()
{
	return &tank;
}

bool Player::StartGame()
{
	if (bStartGame)
	{
		return true;
	}
	_ProcessKeyBoard();
	return false;
}
