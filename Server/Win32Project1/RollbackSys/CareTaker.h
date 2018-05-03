#pragma once
#include "../Server.h"
#include "../Client/Client.h"
#include "Memento.h"
#include "Originator.h"
#include <vector>
#include "../GameScene.h"
#include "../GameLog/GameLog.h"


class CareTaker
{
private:
	 std::vector<Memento*> mementoList;
public:
	CareTaker();
	~CareTaker();

	void addMemento(Memento* pMemento);
	Memento* getMemento(int index);

	GameScene reUpdate();
	int findPastUpdate(float uPackTime);
};