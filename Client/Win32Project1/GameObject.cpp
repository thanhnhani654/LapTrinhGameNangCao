#include "GameObject.h"

std::vector<Object*> Object::listObject;

Object::Object()
{
	position.x = 0;
	position.y = 0;
	deleted = false;
}

eID Object::GetID()
{
	return id;
}

D3DXVECTOR2 Object::GetPosition()
{
	return position;
}

uint32_t Object::getInGameID()
{
	return inGameID;
}

void Object::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Object::Initialize()
{
	listObject.push_back(this);
	inGameID = listObject.size();
}

void Object::Update(float deltatime)
{

}

void Object::Draw()
{

}

void Object::onCollision(Object* object)
{}

void Object::destroy()
{}

void Object::DestroyProcess()
{
	int count = 1;
	std::vector<Object*>::iterator it;
	while (count != 0)
	{
		count = 0;
		for (it = listObject.begin(); it != listObject.end(); it++)
		{
			if ((*it)->deleted)
			{
				count++;
				break;
			}
		}
		if (count != 0)
			listObject.erase((it));
	}
}