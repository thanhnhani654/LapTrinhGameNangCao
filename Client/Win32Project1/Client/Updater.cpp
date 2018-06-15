#include "Updater.h"

Updater* Updater::inst;

Updater* Updater::getInstance()
{
	if (inst == nullptr)
		return nullptr;
	return inst;
}

Updater::Updater(GameScene* scene)
{
	this->scene = scene;
	client = NetWorkManage::getInstance();
	inst = this;
	newestEvent = 0;
	OverFlowed = false;
	len = 0;
}

void Updater::analysis()
{
	bool startread = false;
	int readlen = 0;

	CompareUpdate comUp;
	comUp.data = nullptr;

	char tempflag[2];
	char tempObjectId[4];
	char tempfuncID[4];

	int findingCount = 0;

	while (true)
	{
		tempflag[0] = client->szBuffer[readlen];
		tempflag[1] = client->szBuffer[readlen+1];
		
		//Check StartRead
		if (!startread)
			if (tempflag[0] == 's' && tempflag[1] == 'p')
			{
				//GAMELOG("GetPack   %f", waitingTime);
				client->getRtt();
				storepack t;
				memcpy(t.t, client->szBuffer, 1024);
				F10EQ.push_back(t);
				startread = true;
				readlen += 2;
				bUpdateGame = true;
				waitingTime = 0;
				findingCount = 0;
			}
			else if (tempflag[0] == 'u' && tempflag[1] == 'i')
			{
				//GAMELOG("GetPack   %f", waitingTime);
				updatePlayer(readlen);
				startread = true;
				readlen += 3;
				tempflag[0] = client->szBuffer[readlen];
				tempflag[1] = client->szBuffer[readlen + 1];
				findingCount = 0;
			}
			else if (tempflag[0] == 's' && tempflag[1] == 't')
			{
				client->SyncTime(readlen);
				//GAMELOG("GetPack   %f", waitingTime);
				startread = true;
				readlen += 6;
				tempflag[0] = client->szBuffer[readlen];
				tempflag[1] = client->szBuffer[readlen + 1];
				findingCount = 0;
			}
			else
			{
				if (OverFlowed && readlen < 10)
				{
					startread = true;
				}

				if (readlen > 2000)
				{
					updateFSList.clear();
					client->brequestSyncTime = false;
					OverFlowed = true;
					memcpy(buffer + len, client->szBuffer, client->ret);
					memset(client->szBuffer, 0, 2047);
					len += client->ret;
					GAMELOG("OVERFLOW				Call From:Updater::analysis()");
					break;
				}
				if (findingCount > 18)
				{
					if (OverFlowed)
					{
						memcpy(buffer + len, client->szBuffer, client->ret);
						memset(client->szBuffer, 0, 2047);
						len += client->ret;
						OverFlowed = false;
						GAMELOG("ReAnalysis");
						reAnalysis();
					}
					break;
				}
				readlen++;
				findingCount++;
				continue;
				//break;
			}

		//check end Update Pack
		if (startread)
			if (tempflag[0] == 'e' && tempflag[1] == 'p')
			{
				startread = false;
				readlen += 2;
				continue;
			}

		if (readlen > 2000)
		{
			updateFSList.clear();
			client->brequestSyncTime = false;
			OverFlowed = true;
			memcpy(buffer + len, client->szBuffer, client->ret);
			memset(client->szBuffer, 0, 2047);
			len += client->ret;
			GAMELOG("OVERFLOW				Call From:Updater::analysis()");
			break;
		}


		//Get ID
		memcpy(tempObjectId, client->szBuffer + readlen, sizeof(uint32_t));	readlen += sizeof(uint32_t);
		comUp.id = *reinterpret_cast<uint32_t*>((tempObjectId));

		//Get Object ID
		memcpy(tempObjectId, client->szBuffer + readlen, 1);	readlen += 1;
		comUp.objectid = static_cast<eObjectId>((*tempObjectId));

		//Get InGame ID
		memcpy(tempObjectId, client->szBuffer + readlen, 1);	readlen += 1;
		comUp.inGameID = static_cast<uint8_t>((*tempObjectId));

		//Get FuncId
		memcpy(tempfuncID, client->szBuffer + readlen, 1);	     readlen += 1;
		comUp.funcid = static_cast<funcId>((*tempfuncID));

		//Get Data
		//Can viet 1 ham rieng
		switch (comUp.objectid)
		{
		case eObjectId::PlayerID:
			switch (comUp.funcid)
			{
			case funcId::Pl_Disable:
				comUp.data = nullptr;
				break;
			case funcId::Pl_Fire:
				comUp.data = nullptr;
				break;
			case funcId::Pl_Move_Event:
				comUp.data = new char(sizeof(edirection));
				memcpy(comUp.data, client->szBuffer + readlen, sizeof(edirection));
				readlen += sizeof(edirection);
				break;
			case funcId::Pl_Position:
				comUp.data = new char(sizeof(D3DXVECTOR2));
				memcpy(comUp.data, client->szBuffer + readlen, sizeof(D3DXVECTOR2));
				readlen += sizeof(D3DXVECTOR2);
				break;
			case funcId::Pl_StartGame:
				comUp.data = nullptr;
				break;
			case funcId::Pl_EndGame:
				comUp.data = nullptr;
				break;
			}
			break;
		case eObjectId::BulletID:
		{
			switch (comUp.funcid)
			{
			case funcId::Pl_Position:
				comUp.data = new char(sizeof(D3DXVECTOR2));
				memcpy(comUp.data, client->szBuffer + readlen, sizeof(D3DXVECTOR2));
				readlen += sizeof(D3DXVECTOR2);
				break;
			case funcId::Pl_Disable:
				comUp.data = nullptr;
				break;
			}
			break;
		}
		case eObjectId::BrickID:
			switch (comUp.funcid)
			{
			case funcId::Pl_Disable:
				comUp.data = new char(sizeof(D3DXVECTOR2));
				memcpy(comUp.data, client->szBuffer + readlen, sizeof(D3DXVECTOR2));
				readlen += sizeof(D3DXVECTOR2);
				D3DXVECTOR2* vec;
				vec = reinterpret_cast<D3DXVECTOR2*>(comUp.data);
				break;
			}
			break;
		case eObjectId::BotID:
			switch (comUp.funcid)
			{
			case Pl_Move_Event:
				comUp.data = new char(sizeof(edirection));
				memcpy(comUp.data, client->szBuffer + readlen, sizeof(edirection));
				readlen += sizeof(edirection);
				break;
			case funcId::Pl_Position:
				comUp.data = new char(sizeof(D3DXVECTOR2));
				memcpy(comUp.data, client->szBuffer + readlen, sizeof(D3DXVECTOR2));
				readlen += sizeof(D3DXVECTOR2);
				break;
			case funcId::Pl_Fire:
				comUp.data = nullptr;
				break;
			case funcId::Pl_Disable:
				comUp.data = nullptr;
				break;
			}
		default:
			break;
		}

		updateFSList.push_back(comUp);
		if (comUp.funcid == funcId::Pl_Disable)
		GG.push_back(comUp);
		GG2.push_back(comUp);
	}

	memset(client->szBuffer, 0, 2047);
}

void Updater::reAnalysis()
{
	bool startread = false;
	int readlen = 0;

	CompareUpdate comUp;
	comUp.data = nullptr;

	char tempflag[2];
	char tempObjectId[4];
	char tempfuncID[4];

	int findingCount = 0;

	while (true)
	{
		tempflag[0] = buffer[readlen];
		tempflag[1] = buffer[readlen + 1];

		//Check StartRead
		if (!startread)
			if (tempflag[0] == 's' && tempflag[1] == 'p')
			{
				client->getRtt();
				startread = true;
				readlen += 2;
				bUpdateGame = true;
				waitingTime = 0;
			}
			else if (tempflag[0] == 'u' && tempflag[1] == 'i')
			{
				updatePlayer(readlen);
				startread = true;
				readlen += 3;
				tempflag[0] = buffer[readlen];
				tempflag[1] = buffer[readlen + 1];
			}
			else if (tempflag[0] == 's' && tempflag[1] == 't')
			{
				client->SyncTime(readlen);

				startread = true;
				readlen += 6;
				tempflag[0] = buffer[readlen];
				tempflag[1] = buffer[readlen + 1];
			}
			else
			{
				findingCount++;
				if (readlen > 20000)
				{
					updateFSList.clear();
					client->brequestSyncTime = false;
					GAMELOG("OVERFLOW 20kBuffer				Call From:Updater::analysis()");
					memset(buffer, 0, 20479);
					len = 0;
					break;
				}
				if (findingCount > 18)
					break;
				readlen++;
				continue;
			}

		//check end Update Pack
		if (startread)
			if (tempflag[0] == 'e' && tempflag[1] == 'p')
			{
				startread = false;
				readlen += 2;
				continue;
			}

		if (readlen > 20000)
		{
			updateFSList.clear();
			client->brequestSyncTime = false;
			GAMELOG("OVERFLOW	20kBuffer			Call From:Updater::analysis()");
			break;
		}

		//Get ID
		memcpy(tempObjectId, buffer + readlen, sizeof(uint32_t));	readlen += sizeof(uint32_t);
		comUp.id = *reinterpret_cast<uint32_t*>((tempObjectId));

		//Get Object ID
		memcpy(tempObjectId, buffer + readlen, 1);	readlen += 1;
		comUp.objectid = static_cast<eObjectId>((*tempObjectId));

		//Get InGame ID
		memcpy(tempObjectId, buffer + readlen, 1);	readlen += 1;
		comUp.inGameID = static_cast<uint8_t>((*tempObjectId));

		//Get FuncId
		memcpy(tempfuncID, buffer + readlen, 1);	     readlen += 1;
		comUp.funcid = static_cast<funcId>((*tempfuncID));

		//Get Data
		//Can viet 1 ham rieng
		switch (comUp.objectid)
		{
		case eObjectId::PlayerID:
			switch (comUp.funcid)
			{
			case funcId::Pl_Disable:
				comUp.data = nullptr;
				break;
			case funcId::Pl_Fire:
				comUp.data = nullptr;
				break;
			case funcId::Pl_Move_Event:
				comUp.data = new char(sizeof(edirection));
				memcpy(comUp.data, buffer + readlen, sizeof(edirection));
				readlen += sizeof(edirection);
				break;
			case funcId::Pl_Position:
				comUp.data = new char(sizeof(D3DXVECTOR2));
				memcpy(comUp.data, buffer + readlen, sizeof(D3DXVECTOR2));
				readlen += sizeof(D3DXVECTOR2);
				break;
			case funcId::Pl_StartGame:
				comUp.data = nullptr;
				break;
			case funcId::Pl_EndGame:
				comUp.data = nullptr;
				break;
			}
			break;
		case eObjectId::BulletID:
		{
			switch (comUp.funcid)
			{
			case funcId::Pl_Position:
				comUp.data = new char(sizeof(D3DXVECTOR2));
				memcpy(comUp.data, buffer + readlen, sizeof(D3DXVECTOR2));
				readlen += sizeof(D3DXVECTOR2);
				break;
			case funcId::Pl_Disable:
				comUp.data = nullptr;
				break;
			}
			break;
		}
		case eObjectId::BrickID:
			switch (comUp.funcid)
			{
			case funcId::Pl_Disable:
				comUp.data = new char(sizeof(D3DXVECTOR2));
				memcpy(comUp.data, buffer + readlen, sizeof(D3DXVECTOR2));
				readlen += sizeof(D3DXVECTOR2);
				D3DXVECTOR2* vec;
				vec = reinterpret_cast<D3DXVECTOR2*>(comUp.data);
				break;
			}
			break;
		case eObjectId::BotID:
			switch (comUp.funcid)
			{
			case Pl_Move_Event:
				comUp.data = new char(sizeof(edirection));
				memcpy(comUp.data, client->szBuffer + readlen, sizeof(edirection));
				readlen += sizeof(edirection);
				break;
			case funcId::Pl_Position:
				comUp.data = new char(sizeof(D3DXVECTOR2));
				memcpy(comUp.data, client->szBuffer + readlen, sizeof(D3DXVECTOR2));
				readlen += sizeof(D3DXVECTOR2);
				break;
			case funcId::Pl_Fire:
				comUp.data = nullptr;
				break;
			case funcId::Pl_Disable:
				comUp.data = nullptr;
				break;
			}
		default:
			break;
		}

		updateFSList.push_back(comUp);
	}

	memset(buffer, 0, 20479);
	len = 0;
}

void Updater::ChecknUpdate()
{
	if (updateFSList.empty())
		return;
	ping = client->ping;
	for (std::vector<CompareUpdate>::iterator it = updateFSList.begin(); it != updateFSList.end(); it++)
	{
		//if ((*it).funcid != funcId::Pl_Disable)
		//{
		//	//Kiem tra Pack toi lung tung thi bo
		//	if (newestEvent >= (*it).id)
		//	{
		//		continue;
		//	}
		//	newestEvent = (*it).id;
		//}
		//reUpdate co the de o day sau nay
		updateState((*it));
	}
	updateFSList.clear();
}

void Updater::updateState(CompareUpdate pack)
{
	switch (pack.objectid)
	{
	case eObjectId::PlayerID:
		switch (pack.funcid)
		{
		case funcId::Pl_Disable:
		{
			if (scene->getPlayer(pack.inGameID) != nullptr)
			{
				//scene->getPlayer(pack.inGameID)->disable = true;
				scene->getPlayer(pack.inGameID)->getTank()->died = true;
			}
			break;
		}
		case funcId::Pl_Fire:
		{
			if (scene->getPlayer(pack.inGameID) != nullptr && pack.inGameID != client->playerID)
			{
				scene->getPlayer(pack.inGameID)->getTank()->Fire();

				scene->getPlayer(pack.inGameID)->getTank()->bulletPrediction(client->ping + 2* (float)(1000 / 60));
			}

			break;
		}
		case funcId::Pl_Move_Event:
		{
			edirection* moveEvent = reinterpret_cast<edirection*>(pack.data);
			if (scene->getPlayer(pack.inGameID) != nullptr)
			{
				scene->getPlayer(pack.inGameID)->moveEvent = *moveEvent;
				scene->getPlayer(pack.inGameID)->getTank()->Animator();
				scene->getPlayer(pack.inGameID)->getTank()->GetMovement()->Move(*moveEvent);
			}

			break;
		}
		case funcId::Pl_Position:
		{
		D3DXVECTOR2* position = reinterpret_cast<D3DXVECTOR2*>(pack.data);
		D3DXVECTOR2 currentPos = scene->getPlayer(pack.inGameID)->getTank()->GetPosition();
		D3DXVECTOR2 newPos = currentPos;
		int guess;

		float distance = abs(currentPos.x - (*position).x) + abs(currentPos.y - (*position).y);

		if (ping < 0)
			guess = 0;
		else
			guess = ping / (float)(1000 / 60);
		float f2dis = (4 / (float)60) * (float)scene->getPlayer(pack.inGameID)->getTank()->GetMovement()->GetSpeed();

		//Chinh State bat chap
		static int frame = 0;
		frame++;
		//if (frame < 30)
		{
			if (distance < f2dis)
			{
				break;
			}
		}
		//else
		frame = 0;
		////GAMELOG("distance = %f, 2 frame distance = %f", distance, f2dis);

		if (currentPos.x != position->x && currentPos.y != position->y)
			newPos = *position;
		else if (currentPos.y != position->y)
		{
			newPos.y = (position->y - currentPos.y) / 2 + currentPos.y;
		}
		else
		{
			newPos.x = (position->x - currentPos.x) / 2 + currentPos.x;
		}

		for (int i = 0; i < Object::listObject.size(); i++)
		{
			if (Object::listObject.at(i)->getInGameID() == pack.inGameID)
			{
				((Tank*)Object::listObject.at(i))->SetPosition(newPos.x, newPos.y);
				GAMELOG("X = %f     Y = %f", position->x, position->y);
				break;
			}
		}
		//scene->getPlayer()->getTank()->SetPosition(position->x, position->y);
		break;
		}
		case funcId::Pl_StartGame:
		{
			LoadingScreen::getInstance()->DisableAll();
			client->bStartGame = true;
			scene->getPlayer(pack.inGameID)->bStartGame = true;
			break;
		}
		case funcId::Pl_EndGame:
		{
			if (scene->getPlayer(client->playerID)->getTank()->getInGameID() == pack.inGameID)
			{
				LoadingScreen::getInstance()->EnableEndGameScreen(true);
			}
			else
			{
				LoadingScreen::getInstance()->EnableEndGameScreen(false);
			}
			break;
		}
		}
		break;
	case eObjectId::BulletID:
	{
		switch (pack.funcid)
		{
		case funcId::Pl_Position:
		{
			D3DXVECTOR2* position = reinterpret_cast<D3DXVECTOR2*>(pack.data);
			for (int i = 0; i < Object::listObject.size(); i++)
		{
			if (Object::listObject.at(i)->getInGameID() == pack.inGameID)
			{
				((Bullet*)Object::listObject.at(i))->setEPosition(position->x, position->y);
				break;
			}
		}
		}
			break;
		case funcId::Pl_Disable:
		{
			for (int i = 0; i < Object::listObject.size(); i++)
		{
			if (Object::listObject.at(i)->getInGameID() == pack.inGameID)
			{
				((Bullet*)Object::listObject.at(i))->destroy();
				break;
			}
		}
		}
			break;
		}
	}
		break;
	case eObjectId::BrickID:
		switch (pack.funcid)
		{
		case funcId::Pl_Disable:
			for (int i = 0; i < Object::listObject.size(); i++)
			{
				if (Object::listObject.at(i)->getInGameID() == pack.inGameID)
				{
					D3DXVECTOR2* position = reinterpret_cast<D3DXVECTOR2*>(pack.data);
					((Brick*)Object::listObject.at(i))->checkDisable(*position);
					break;
				}
			}
			break;
		}
		break;
	case eObjectId::BotID:
		switch (pack.funcid)
		{
		case Pl_Move_Event:
		{
			edirection* moveEvent = reinterpret_cast<edirection*>(pack.data);
			for (int i = 0; i < Object::listObject.size(); i++)
			{				
				if (Object::listObject.at(i)->getInGameID() == pack.inGameID)
				{
					((Tank*)Object::listObject.at(i))->GetMovement()->Move(*moveEvent);
					((Tank*)Object::listObject.at(i))->Animator();
					break;
				}
			}
			break;
		}
		case Pl_Position:
		{
			D3DXVECTOR2* position = reinterpret_cast<D3DXVECTOR2*>(pack.data);
			for (int i = 0; i < Object::listObject.size(); i++)
			{
				if (Object::listObject.at(i)->getInGameID() == pack.inGameID)
				{
					((Tank*)Object::listObject.at(i))->SetPosition(position->x, position->y);
					break;
				}
			}
			break;
		}
		case Pl_Fire:
		{
			for (int i = 0; i < Object::listObject.size(); i++)
			{
				if (Object::listObject.at(i)->getInGameID() == pack.inGameID)
				{
					((Tank*)Object::listObject.at(i))->Fire();
					break;
				}
			}
			break;
		}
		case Pl_Disable:
		{
			for (int i = 0; i < Object::listObject.size(); i++)
			{
				if (Object::listObject.at(i)->getInGameID() == pack.inGameID)
				{
					((Tank*)Object::listObject.at(i))->died = true;
					break;
				}
			}
			break;
		}
		}
		break;
	}
}


void Updater::updateID()
{
	bool notgetPack = true;
	while (notgetPack)
	{
		NetWorkManage::getInstance()->cRecv();
		int i = 0;
		for (i = 0; i < 1024; i++)
		{
			if (i == 1020)
				break;
			if (client->szBuffer[i] == 'u' &&
				client->szBuffer[i + 1] == 'i' &&
				client->szBuffer[i + 3] == 'e' &&
				client->szBuffer[i + 4] == 'p')
			{
				notgetPack = false;
				break;
			}
		}
		if (i == 1020)
			continue;

		uint8_t id = static_cast<uint8_t>(client->szBuffer[i + 2]);

		switch (id)
		{
		case 4:
			LoadingScreen::getInstance()->EnableRect4();
		case 3:
			LoadingScreen::getInstance()->EnableRect3();
		case 2:
			LoadingScreen::getInstance()->EnableRect2();
		case 1:
			LoadingScreen::getInstance()->EnableRect1();
		}

		if (NetWorkManage::getInstance()->playerID == 0)
		{
			NetWorkManage::getInstance()->playerID = id;
			for (int i = 1; i <= id; i++)
			{
				scene->getPlayer(i)->playerID = id;
			}
		}
		for (int i = 1; i <= id; i++)
		{
			scene->getPlayer(i)->disable = false;
		}
	}
}

void Updater::updatePlayer(int len)
{
	if (NetWorkManage::getInstance()->bStartGame)
		return;
	uint8_t id = static_cast<uint8_t>(client->szBuffer[len + 2]);

	switch (id)
	{
	case 4:
		LoadingScreen::getInstance()->EnableRect4();
	case 3:
		LoadingScreen::getInstance()->EnableRect3();
	case 2:
		LoadingScreen::getInstance()->EnableRect2();
	case 1:
		LoadingScreen::getInstance()->EnableRect1();
	}

	if (NetWorkManage::getInstance()->playerID == 0)
	{
		NetWorkManage::getInstance()->playerID = id;
		for (int i = 1; i <= id; i++)
		{
			scene->getPlayer(i)->playerID = id;
		}
	}	

	for (int i = 1; i <= id; i++)
	{
		
		scene->getPlayer(i)->disable = false;		
	}
}

void Updater::isUpdateGame(float deltatime)
{
	//GAMELOG("waiting Time: %f",waitingTime);
	waitingTime += deltatime;
	//GAMELOG("waiting time %f", waitingTime);
	if (waitingTime > 5000 && !bUpdateGame)
	{
		MessageBox(NULL, L"Disconected!", L"Conection Error", NULL);
		client->disconected = true;
	}
	if (waitingTime > client->RTT)
		bUpdateGame = false;

}