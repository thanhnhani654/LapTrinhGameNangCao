#include "GameObject.h"

Object::Object()
{
	position.x = 0;
	position.y = 0;
}

eID Object::GetID()
{
	return id;
}

D3DXVECTOR2 Object::GetPosition()
{
	return position;
}

void Object::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Object::Initialize()
{

}

void Object::Update(float deltatime)
{

}

void Object::Draw()
{

}