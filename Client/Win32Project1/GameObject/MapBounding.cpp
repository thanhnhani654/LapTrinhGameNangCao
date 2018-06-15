#include "MapBounding.h"

void MapBounding::Initialize(float x, float y,float w, float h)
{
	this->SetPosition(x, y);
	box = new Box2D(GetPosition(), w, h, this);
}

void MapBounding::Update(float deltatime)
{}

void MapBounding::Draw()
{}

void MapBounding::onCollision(Object* object)
{}

void MapBounding::Destroy()
{}