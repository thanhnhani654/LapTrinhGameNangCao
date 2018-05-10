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
		for (count = currentUpdatePack; count < Server::getInstance()->updatePackCount; count++)
		{	
			if (Server::getInstance()->updatePack[count].miliTime < mementoList.at(i + 1)->getSnapShoot()->militime)
			{
				//Tren ly thuyet truong hop nay se khong bao gio xay ra, nhung bo day cho chac
				if (Server::getInstance()->updatePack[count].eventCount == 0)
				{
					GAMELOG("UpdatPack have zero event!!");
					continue;
				}
				for (int j = 0; j < Server::getInstance()->updatePack[count].eventCount; j++)
				{
					//Chua viet 1 ham UpdateEvent rieng
					if (Server::getInstance()->updatePack[count]._updateEvent->_objectId == eObjectId::GameObject)
						if (Server::getInstance()->updatePack[count]._updateEvent->_funcId == funcId::Pl_Move_Event)
						{
							edirection* moveEvent = reinterpret_cast<edirection*>(server->updatePack[count]._updateEvent->_event);

							if (*moveEvent == edirection::UP)
								int a = 0;
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
	//CareTaker luu lai thong tin can thiet dung de Update cho Client
	if (snapShootServer == nullptr)
		snapShootServer = new SnapShootServer();
	snapShootServer->position = mementoList.back()->getSnapShoot()->scene.getPlayer()->getTank().GetPosition();

	//Chua viet 1 ham dong goi rieng.
	Server::getInstance()->CreateData(snapShootServer->position, eObjectId::GameObject, funcId::Pl_Position, sizeof(D3DXVECTOR2));
	Server::getInstance()->SendDataPack();

	return mementoList.back()->getSnapShoot()->scene;
}

int CareTaker::findPastUpdate(float uPackTime)
{
	//Thoi Gian ban Update Client gui len bi sai
	if (uPackTime < 0)
	{
		GAMELOG("Thoi gian cua ban Update cua Client < 0!!!			Call From: findPastUpdate");
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

