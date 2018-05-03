#pragma once
#include "Memento.h"


class Originator
{
private:
	SnapShoot state;

public:
	void setState(SnapShoot state);
	SnapShoot getState();
	Memento* createMemento();
	void restoreMemento(Memento* pMemento);
};