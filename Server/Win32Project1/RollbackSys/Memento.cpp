#include "Memento.h"

Memento::Memento(SnapShoot isnapShoot)
{
	snapShoot = new SnapShoot();
	*snapShoot = isnapShoot;
}

SnapShoot* Memento::getSnapShoot()
{
	

	return snapShoot;
}