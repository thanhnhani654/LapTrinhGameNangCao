#pragma once
#include "../Server.h"
#include "../Client/Client.h"
#include "Memento.h"
#include "Originator.h"
#include <vector>
#include "../GameScene.h"
#include "../GameLog/GameLog.h"
#include "../GameObject.h"
#include "../Behavior/Movement.h"
#include <exception>

struct SnapShootServer
{
	//Player: position, direction
	D3DXVECTOR2 position;
};

//Memento Pattern de biet them chi tiet
class CareTaker
{
private:
	 std::vector<Memento*> mementoList;
	 SnapShootServer* snapShootServer;
public:
	CareTaker();
	~CareTaker();

	void addMemento(Memento* pMemento);
	Memento* getMemento(int index);

	GameScene reUpdate();
	int findPastUpdate(float uPackTime);
};