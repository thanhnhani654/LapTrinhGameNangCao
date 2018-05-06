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
	if (mementoList.size() > 1200)
	{
		for (std::vector<Memento*>::iterator it = mementoList.begin(); it != mementoList.begin() + 500; it++)
		{
			(*it)->deleteSnapShoot();
		}
		mementoList.erase(mementoList.begin(), mementoList.begin() + 500);
	}

	int lastestUpdate = findPastUpdate(Server::getInstance()->updatePack[0].miliTime);

	if (lastestUpdate == -1)
	{
		//Server::getInstance()->SendDataPack();
		return  mementoList.back()->getSnapShoot()->scene;
	}

	int currentUpdatePack = 0;
	int count = 0;
	int tem = Server::getInstance()->updatePackCount;
	if (count == tem)
		return  mementoList.back()->getSnapShoot()->scene;
	int listSize = mementoList.size();
	//GAMELOG("START REUPDATE!");
	for (int i = lastestUpdate; i < listSize - 1; i++)
	{		
		for (count = currentUpdatePack; count < Server::getInstance()->updatePackCount; count++)
		{	
			if (Server::getInstance()->updatePack[count].miliTime < mementoList.at(i + 1)->getSnapShoot()->militime)
			{
				if (Server::getInstance()->updatePack[count].eventCount == 0)
					continue;
				for (int j = 0; j < Server::getInstance()->updatePack[count].eventCount; j++)
				{
					if (Server::getInstance()->updatePack[count]._updateEvent->_objectId == eObjectId::GameObject)
						if (Server::getInstance()->updatePack[count]._updateEvent->_funcId == funcId::Pl_Move_Event)
						{
							edirection* moveEvent = reinterpret_cast<edirection*>(Server::getInstance()->updatePack[count]._updateEvent->_event);

							mementoList.at(i)->getSnapShoot()->scene.getPlayer()->Move(*moveEvent);	
						}
				}
			}
			else
			{		
				break;
			}
		}
		//switch to next update
		mementoList.at(i + 1)->getSnapShoot()->scene = mementoList.at(i)->getSnapShoot()->scene;
		mementoList.at(i + 1)->getSnapShoot()->scene.Update(mementoList.at(i + 1)->getSnapShoot()->militime - mementoList.at(i)->getSnapShoot()->militime);
		currentUpdatePack = count;

	}

	
	Server::getInstance()->clearUpdatePack();
	//GAMELOG("END REUPDATE!");
	//Warp Lastest Update to send to client
	if (snapShootServer == nullptr)
		snapShootServer = new SnapShootServer();
	snapShootServer->position = mementoList.back()->getSnapShoot()->scene.getPlayer()->getTank().GetPosition();

	//Viet 1 ham dong goi rieng.
	Server::getInstance()->CreateData(snapShootServer->position, eObjectId::GameObject, funcId::Pl_Position, sizeof(D3DXVECTOR2));	
	//Server::getInstance()->autoSend = 0;
	if (Server::getInstance()->SendDataPack()) {
		GAMELOG("Caretaker finish calling SendDataPack()!!!!");
	}
	return mementoList.back()->getSnapShoot()->scene;
}

int CareTaker::findPastUpdate(float uPackTime)
{
	if (uPackTime < 0)
		return -1;
	int listSize = mementoList.size();
	if (uPackTime > mementoList.back()->getSnapShoot()->militime)
		return listSize - 1;
	if (mementoList.end() - mementoList.begin() < 1)
		return -1;
	if (listSize - 1 == 0)
		return 0;
	for (int i = 0; i < listSize - 1; i++)
	{		
		if (uPackTime >= mementoList.at(i)->getSnapShoot()->militime && uPackTime < mementoList.at(i+1)->getSnapShoot()->militime)
		{
			return i;
		}
	}

	return -1;
}

void CareTaker::autoSend(float deltatime)
{
	/*if (snapShootServer == nullptr)
		snapShootServer = new SnapShootServer();
	snapShootServer->position = mementoList.back()->getSnapShoot()->scene.getPlayer()->getTank().GetPosition();
	Server::getInstance()->CreateData(snapShootServer->position, eObjectId::GameObject, funcId::Pl_Position, sizeof(D3DXVECTOR2));
	Server::getInstance()->autoSend += deltatime;
	if (Server::getInstance()->autoSend > 1000)
	{
		if (Server::getInstance()->SendDataPack())
			GAMELOG("BBB");
		else
			GAMELOG("TTT");
		Server::getInstance()->autoSend = 0;
	}*/
}