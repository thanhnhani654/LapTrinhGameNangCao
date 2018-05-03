#include "Originator.h"

void Originator::setState(SnapShoot state)
{
	this->state = state;
}

SnapShoot Originator::getState()
{
	return this->state;
}

Memento* Originator::createMemento()
{
	return new Memento(this->state);
}

void Originator::restoreMemento(Memento* pMemento)
{
	this->state = *pMemento->getSnapShoot();
}