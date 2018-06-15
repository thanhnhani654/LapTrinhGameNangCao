#include "Box2D.h"
#include "../GameLog/GameLog.h"

std::vector<Box2D*> Box2D::listBox;

Box2D::Box2D(D3DXVECTOR2* ipos, D3DXVECTOR2* ivelocity, float w, float h, float* speed, Object* object)
{
	this->collider.pos = ipos;
	this->collider.w = w;
	this->collider.h = h;
	this->velocity = ivelocity;
	this->destroyed = false;
	this->speed = speed;
	this->object = object;
	this->type = eType::Dynamic;
	this->getShot = false;

	listBox.push_back(this);
}

Box2D::Box2D(D3DXVECTOR2 ipos, float w, float h, Object* object)
{
	this->collider.pos = new D3DXVECTOR2();
	this->collider.pos->x = ipos.x;
	this->collider.pos->y = ipos.y;
	this->collider.w = w;
	this->collider.h = h;
	this->velocity = new D3DXVECTOR2();
	this->velocity->x = 0;
	this->velocity->y = 0;
	this->type = eType::Static;
	this->destroyed = false;
	this->object = object;
	this->getShot = false;

	listBox.push_back(this);
}

void Box2D::setPosition(D3DXVECTOR2 ipos)
{
	*collider.pos = ipos;
}

Box Box2D::getBox()
{
	return this->collider;
}

void Box2D::setVelocity(D3DXVECTOR2 ivec)
{
	*velocity = ivec;
}

D3DXVECTOR2 Box2D::getVelocity()
{
	return *this->velocity;
}

eType Box2D::getType()
{
	return this->type;
}

void Box2D::destroy()
{
	destroyed = true;
	if (type == eType::Static)
	{
		delete this->velocity;
	}
}

Object* Box2D::getObject()
{
	return object;
}

void Box2D::debugDraw(LPDIRECT3DDEVICE9 d3ddv)
{
	static LPD3DXLINE line;
	int listSize = listBox.size();
	if (listSize == 0)
		return;
	for (int i = 0; i < listSize; i++)
	{
		D3DXCreateLine(d3ddv, &line);
		int ratio = 217;
		D3DXVECTOR2 lines[] = { D3DXVECTOR2(listBox[i]->collider.pos->x, (listBox[i]->collider.pos->y)*-1 + ratio),
								D3DXVECTOR2(listBox[i]->collider.pos->x, (listBox[i]->collider.pos->y)*-1 + listBox[i]->collider.h + ratio) ,
								D3DXVECTOR2(listBox[i]->collider.pos->x + listBox[i]->collider.w, (listBox[i]->collider.pos->y)*-1 + listBox[i]->collider.h + ratio) ,
								D3DXVECTOR2(listBox[i]->collider.pos->x + listBox[i]->collider.w, (listBox[i]->collider.pos->y)*-1 + ratio),
								D3DXVECTOR2(listBox[i]->collider.pos->x, (listBox[i]->collider.pos->y)*-1 + ratio) };

		line->Begin();
		line->Draw(lines, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
		line->End();
		line->Release();
	}
}

void Box2D::destroyProcess()
{
	int count = 1;
	std::vector<Box2D*>::iterator it;
	while (count != 0)
	{
		count = 0;
		for (it = listBox.begin(); it != listBox.end(); it++)
		{
			if ((*it)->destroyed)
			{
				count++;
				break;
			}
		}
		
		if (count != 0)
		{
			if ((*it)->getType() == eType::Static)
				(*it)->collider.destroy();
			listBox.erase(it);
		}
	}

}