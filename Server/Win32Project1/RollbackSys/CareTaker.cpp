#include "CareTaker.h"

//std::vector<Memento*> CareTaker::mementoList;

CareTaker::CareTaker(){}
CareTaker::~CareTaker(){}

void CareTaker::addMemento(Memento* pMemento)
{
	mementoList.push_back(pMemento);
}

Memento* CareTaker::getMemento(int index)
{
	return mementoList.at(index);
}

GameScene CareTaker::reUpdate()
{
	//Neu bo nho SnapShoot nhieu hon 1200 ban thi xoa bot 500 ban
	if (mementoList.size() > 1200)
	{
		for (std::vector<Memento*>::iterator it = mementoList.begin(); it != mementoList.begin() + 500; it++)
		{
			(*it)->deleteSnapShoot();
		}
		mementoList.erase(mementoList.begin(), mementoList.begin() + 500);
	}

	//Tim ve ban SnapShoot gan voi ban update xa nhat ma client gui den
	int lastestUpdate = findPastUpdate(Server::getInstance()->updatePack[0].miliTime);

	//Neu ket qua tra ve -1 thi cho Server Update theo ban SnapShoot moi nhat hien tai
	if (lastestUpdate == -1)
	{
		return  mementoList.back()->getSnapShoot()->scene;
	}

	//Ban Update cua Client dang xet
	int currentUpdatePack = 0;

	//Dat bien dem ben ngoai de khi update cho SnapShoot tiep theo thi SnapShoot biet Update tu ban Update Client nao
	int count = 0;

	//Gan nhu vay de tranh viec goi toi ham get size cua vector nhieu lan
	int listSize = mementoList.size();
	Server* server = Server::getInstance();

	for (int i = lastestUpdate; i < listSize - 1; i++)
	{		
		for (count = currentUpdatePack; count < server->updatePackCount; count++)
		{	
			if (server->updatePack[count].miliTime < mementoList.at(i + 1)->getSnapShoot()->militime && server->updatePack[count].miliTime > 0)
			{
				//Tren ly thuyet truong hop nay se khong bao gio xay ra, nhung bo day cho chac
				if (server->updatePack[count].eventCount == 0)
				{
					GAMELOG("UpdatPack have zero event!!");
					continue;
				}
				
				for (int j = 0; j < server->updatePack[count].eventCount; j++)
				{
					GG.push_back(server->updatePack[count]._updateEvent[j]);
					//edirection* a = reinterpret_cast<edirection*>(server->updatePack[count]._updateEvent[j]._event);
					//GAMELOG("ID: %d,	Event: %d", server->updatePack[count]._updateEvent[j].inGameID, *a);
					updateEvent(mementoList.at(i), server->updatePack[count]._updateEvent[j]);
				}
			}
			else
			{		
				break;
			}
		}
		//switch to next update
		float deltatime = mementoList.at(i + 1)->getSnapShoot()->militime - mementoList.at(i)->getSnapShoot()->militime;
		mementoList.at(i + 1)->getSnapShoot()->scene = mementoList.at(i)->getSnapShoot()->scene;

		mementoList.at(i + 1)->getSnapShoot()->scene.UpdateInput(deltatime);
		CollisionManager::getInstance()->doCollision(deltatime);
		mementoList.at(i + 1)->getSnapShoot()->scene.Update(deltatime);
		currentUpdatePack = count;
	}

	server->clearUpdatePack();

	//CareTaker luu lai thong tin can thiet dung de Update cho Client
	if (snapShootServer == nullptr)
		snapShootServer = new SnapShootServer();
	//snapShootServer->position = mementoList.back()->getSnapShoot()->scene.getPlayer(1)->getTank().GetPosition();

	wrapDataToSend();
	for (int i = 0; i < 4; i++)
	{
		if (mementoList.back()->getSnapShoot()->scene.getPlayer(i) != nullptr)
		{ }
		//Server::getInstance()->CreateData(mementoList.back()->getSnapShoot()->scene.getPlayer(i)->getTank().GetPosition(), eObjectId::PlayerID, i, funcId::Pl_Position, sizeof(D3DXVECTOR2));
	}
	//server->SendDataPack();

	return mementoList.back()->getSnapShoot()->scene;
}

int CareTaker::findPastUpdate(float uPackTime)
{
	//Thoi Gian ban Update Client gui len bi sai
	if (uPackTime < 0)
	{
		//GAMELOG("Thoi gian cua ban Update cua Client < 0!!!			Call From: findPastUpdate");
		return -1;
	}

	int listSize = mementoList.size();

	//Thoi gian ban cua ClientUpdate xa nhat moi hon thoi gian cua ban SnapShoot Server moi nhat. noi chung la sai
	if (uPackTime > mementoList.back()->getSnapShoot()->militime)
	{
		GAMELOG("Oldest ClientUpdate newer than newest SnapShoot Server			Call From: findPastUpdate");
		return listSize - 1;
	}
	//(Server moi co 1 ban Update)		Co the se co loi va phai dung ben duoi de debug
	/*if (mementoList.end() - mementoList.begin() < 1)
	//{
	//	GAMELOG("Server have just one SnapShoot!			Call From: findPastUpdate");
	//	return -1;
	//}*/
	//Server moi co 1 ban Update
	if (listSize - 1 == 0)
	{
		//GAMELOG("Khong biet nhung loi nay o trong FindPastUpdate!			Call From: findPastUpdate");
		return 0;
	}

	//Ban ClientUpdate xa nhat truoc ban Snapshoot dau tien cua server
	if (uPackTime <= mementoList.front()->getSnapShoot()->militime)
		return 0;

	//Tim ban SnapShoot gan voi ban ClientUpdate xa Nhat
	for (int i = 0; i < listSize - 1; i++)
	{		
		if (uPackTime >= mementoList.at(i)->getSnapShoot()->militime && uPackTime < mementoList.at(i+1)->getSnapShoot()->militime)
		{
			return i;
		}
	}

	return -1;
}

void CareTaker::updateEvent(Memento* scene, UpdateEvent ievent)
{
	switch (ievent._objectId)
	{
	case eObjectId::PlayerID:
		switch (ievent._funcId)
		{
		case funcId::Pl_Move_Event:
			{
				edirection* moveEvent = reinterpret_cast<edirection*>(ievent._event);

				for (std::vector<Object*>::iterator it = Object::listObject.begin(); it != Object::listObject.end(); it++)
				{
					if ((*it)->getInGameID() == ievent.inGameID)
					{
						scene->getSnapShoot()->scene.getPlayer(ievent.inGameID)->Move(*moveEvent);
						break;
					}
				}
			}
			break;
		case funcId::Pl_Fire:
			{
				uint8_t* code = reinterpret_cast<uint8_t*>(ievent._event);

				scene->getSnapShoot()->scene.getPlayer(ievent.inGameID)->OnKeyDown(*code);

			}
			break;
		}
		break;
	}
}

void CareTaker::wrapDataToSend()
{
	//Player
	Server::getInstance()->CreateData(snapShootServer->position, eObjectId::PlayerID, 1, funcId::Pl_Position, sizeof(D3DXVECTOR2));
}