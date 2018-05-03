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
		mementoList.erase(mementoList.begin(), mementoList.begin() + 500);
	int lastestUpdate = findPastUpdate(Server::updatePack[0].miliTime);

	if (lastestUpdate == -1)
		return  mementoList.back()->getSnapShoot()->scene;

	int currentUpdatePack = 0;
	static int coo = 0;
	static int coo2 = 0;
	int count = 0;
	if (count == Server::updatePackCount)
		return  mementoList.back()->getSnapShoot()->scene;
	int listSize = mementoList.size();

	GAMELOG("START REUPDATE!");
	for (int i = lastestUpdate; i < listSize - 1; i++)
	{
		coo2++;
		
		for (count = currentUpdatePack; count < Server::updatePackCount; count++)
		{	
			if (Server::updatePack[count].miliTime < mementoList.at(i + 1)->getSnapShoot()->militime)
			{
				if (Server::updatePack[count].eventCount == 0)
					continue;
				for (int j = 0; j < Server::updatePack[count].eventCount; j++)
				{
					if (Server::updatePack[count]._updateEvent->_objectId == eObjectId::GameObject)
						if (Server::updatePack[count]._updateEvent->_funcId == funcId::Pl_Move_Event)
						{
							edirection* moveEvent = reinterpret_cast<edirection*>(Server::updatePack[count]._updateEvent->_event);
							mementoList.at(i)->getSnapShoot()->scene.getPlayer()->Move(*moveEvent);
							GAMELOG("AT: %d => X: = %f", listSize - 1, mementoList.at(listSize - 1)->getSnapShoot()->scene.getPlayer()->getTank().GetPosition().x);
							GAMELOG("AT: %d => X: = %f",i,mementoList.at(i)->getSnapShoot()->scene.getPlayer()->getTank().GetPosition().x);

							/*GAMELOG("client Event Time: %f\n this Update Time: %f", Server::updatePack[count].miliTime, mementoList.at(i)->getSnapShoot()->militime);
							GAMELOG("next Update Time: %f", mementoList.at(i+1)->getSnapShoot()->militime);
							GAMELOG("Sever Time: %f", mementoList.at(mementoList.size() - 1)->getSnapShoot()->militime);
							GAMELOG("Index: %d\n", i);*/

							//delete moveEvent;
							
						}
				}
				/*if (count == 0)
					mementoList.at(i)->getSnapShoot().scene.Update(mementoList.at(i+1)->getSnapShoot().militime - mementoList.at(i)->getSnapShoot().militime);
				else
					mementoList.at(i)->getSnapShoot().scene.Update(Server::updatePack[count].miliTime - Server::updatePack[count - 1].miliTime);*/
				//coo++;
				
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
		Memento* b = mementoList.at(i + 1);
		GAMELOG("X: %f	COUNT: %d\n", mementoList.at(i)->getSnapShoot()->scene.getPlayer()->getTank().GetPosition().x, coo);
		coo++;

	}
	//GAMELOG("reUpdate Count: %d", coo);
	

	if (coo != 0)
	{
		int a = 0;
		GAMELOG("Update Count: %d", coo);
		GAMELOG("Update Count2: %d", coo2);
		GAMELOG("Update Count3: %d", listSize - 1 - lastestUpdate);
	}
	coo = 0;
	coo2 = 0;
	Server::clearUpdatePack();

	GAMELOG("END REUPDATE!");
	return mementoList.back()->getSnapShoot()->scene;
}

int CareTaker::findPastUpdate(float uPackTime)
{
	if (uPackTime == 0)
		return -1;
	int listSize = mementoList.size();
	if (uPackTime > mementoList.back()->getSnapShoot()->militime)
		return listSize - 1;
	if (mementoList.end() - mementoList.begin() < 5)
		return -1;
	for (int i = 0; i < listSize - 1; i++)
	{
		if (uPackTime >= mementoList.at(i)->getSnapShoot()->militime && uPackTime < mementoList.at(i+1)->getSnapShoot()->militime)
		{
			return i;
		}
	}

	return -1;
}