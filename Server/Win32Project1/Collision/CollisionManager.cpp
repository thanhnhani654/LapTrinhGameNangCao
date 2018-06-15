#include "CollisionManager.h"
#include "../GameLog/GameLog.h"

CollisionManager* CollisionManager::instance;

CollisionManager* CollisionManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new CollisionManager();
	}
	return instance;
}

float CollisionManager::CollisionDetection(Box2D object1, Box2D object2, int* normalx, int* normaly, float deltatime)
{
	float delta = deltatime / 1000;
	D3DXVECTOR2 vec1 = object1.getVelocity();
	vec1.x *= delta;
	vec1.y *= delta;
	D3DXVECTOR2 vec2 = object2.getVelocity();
	Box box1 = object1.getBox();
	Box box2 = object2.getBox();

	/*Box* BroadphaseBox = getBroadphaseBox(object1, delta);
	if (!IsIntersection(*BroadphaseBox, box2))
	{
		BroadphaseBox->destroy();
		delete BroadphaseBox;
		return delta;
	}*/

	float dxEntry, dxExit;
	float dyEntry, dyExit;

	float txEntry, tyEntry, tEntry;
	float txExit, tyExit, tExit;

	if (vec1.x > 0)
	{
		dxEntry =	box2.pos->x					-	(box1.pos->x + box1.w);
		dxExit =	(box2.pos->x + box2.w)		-	 box1.pos->x;
	}
	else
	{
		dxEntry =	(box2.pos->x + box2.w)		-	 box1.pos->x;
		dxExit =	box2.pos->x					-	(box1.pos->x + box1.w);
	}

	if (vec1.y > 0)
	{
		dyEntry =	box2.pos->y					-	(box1.pos->y + box1.h);
		dyExit =	(box2.pos->y + box2.h)		-	 box1.pos->y;
	}
	else
	{
		dyEntry =	(box2.pos->y + box2.h)		-	 box1.pos->y;
		dyExit =	box2.pos->y					-	(box1.pos->y + box1.h);
	}

	if (vec1.x == 0)
	{
		txEntry = -std::numeric_limits<float>::infinity();
		txExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		txEntry = dxEntry / vec1.x;
		txExit = dxExit / vec1.x;
	}

	if (vec1.y == 0)
	{
		tyEntry = -std::numeric_limits<float>::infinity();
		tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		tyEntry = dyEntry / vec1.y;
		tyExit = dyExit / vec1.y;
	}

	tEntry = max(txEntry, tyEntry);
	tExit = min(txExit, tyExit);
	tEntry *= delta;
	tExit *= delta;

	if (tEntry			>			tExit		||
		txEntry < 0.0f && tyEntry < 0.0f		||
		txEntry > delta && tyEntry > delta)
		return delta;

	if (txEntry > tyEntry)
	{
		*normalx = dxEntry > 0.0f ? 1 : -1;
	}
	else
	{
		*normaly = dyEntry > 0.0f ? 1 : -1;
	}

	return tEntry;
}

bool CollisionManager::IsIntersection(Box object1, Box object2)
{
	if (object1.pos->x							< object2.pos->x + object2.w		&&
		object1.pos->x + object1.w				> object2.pos->x					&&
		object1.pos->y							< object2.pos->y + object2.h		&&
		object1.pos->y + object1.h				> object2.pos->y
		)		
		return true;
	return false;
}

Box* CollisionManager::getBroadphaseBox(Box2D &object, float delta)
{
	Box temp;
	temp.pos->x = object.getVelocity().x == 0 ? object.getBox().pos->x : object.getBox().pos->x + object.getVelocity().x * delta;
	temp.pos->y = object.getVelocity().y == 0 ? object.getBox().pos->y : object.getBox().pos->y + object.getVelocity().y * delta;
	temp.w = object.getBox().w;
	temp.h = object.getBox().h;

	return new Box(temp.pos, temp.w, temp.h);
}

Box CollisionManager::getBroadphaseBo(Box2D &object, float delta)
{
	Box temp;
	temp.pos->x = object.getVelocity().x == 0 ? object.getBox().pos->x : object.getBox().pos->x + object.getVelocity().x * delta;
	temp.pos->y = object.getVelocity().y == 0 ? object.getBox().pos->y : object.getBox().pos->y + object.getVelocity().y * delta;
	temp.w = object.getBox().w;
	temp.h = object.getBox().h;

	return temp;
}

void CollisionManager::doCollision(float deltatime)
{
	double delta = deltatime / 1000.0f;
	for (std::vector<Box2D*>::iterator dy = Box2D::listBox.begin(); dy != Box2D::listBox.end(); dy++)
	{
		if ((*dy)->disable)
			continue;
		if ((*dy)->getType() == eType::Static)
			continue;
		while (true)
		{
			int countCollid = 0;
			for (std::vector<Box2D*>::iterator st = Box2D::listBox.begin(); st != Box2D::listBox.end(); st++)
			{
				if ((*st)->getObject()->GetID() == eID::ID_MapBounding)
					int a = 0;
				Box BroadphaseBox = getBroadphaseBo(*(*dy), delta);
				if (!IsIntersection(BroadphaseBox, (*st)->getBox()))
				{
					BroadphaseBox.destroy();
					continue;
				}
				BroadphaseBox.destroy();
				if ((*st)->getObject()->GetID() == eID::ID_MapBounding)
					int a = 0;

				if ((*dy) == (*st) || (*st)->disable)
					continue;
				int normalx = 0;
				int normaly = 0;
				//float collidTime = 50;
				float collidTime = this->CollisionDetection(*(*dy), *(*st), &normalx, &normaly, deltatime);
				if (collidTime >= delta)
					continue;

				float remainTime = delta - collidTime;

				if ((*dy)->getObject()->GetID() == eID::ID_Bullet)
					(*st)->getShot = true;

				(*dy)->getObject()->onCollision((*st)->getObject());
				(*st)->getObject()->onCollision((*dy)->getObject());

				//De tam de dung mat dan thoi
				if ((*dy)->getObject()->GetID() == eID::ID_Bullet && (*st)->getObject()->GetID() == eID::ID_Tank ||
					(*st)->getObject()->GetID() == eID::ID_Bullet && (*dy)->getObject()->GetID() == eID::ID_Tank)
					continue;

				D3DXVECTOR2 pos = *(*dy)->getBox().pos;
				D3DXVECTOR2 velocity = (*dy)->getVelocity();
				if (normalx != 0)
				{
					pos.x -= (velocity.x * remainTime) * 1.1f;
				}
				if (normaly != 0)
				{
					pos.y -= (velocity.y * remainTime) * 1.1f;
				}

				(*dy)->setPosition(pos);
				
			}

			
			if (countCollid == 0)
				break;
		}
	}
}

void CollisionManager::doCollision(float deltatime, std::vector<Box2D*> saved)
{
	double delta = deltatime / 1000.0f;
	for (std::vector<Box2D*>::iterator dy = saved.begin(); dy != saved.end(); dy++)
	{
		if ((*dy)->disable)
			continue;
		if ((*dy)->getType() == eType::Static)
			continue;
		while (true)
		{
			int countCollid = 0;
			for (std::vector<Box2D*>::iterator st = saved.begin(); st != saved.end(); st++)
			{
				if ((*dy) == (*st) || (*st)->disable)
					continue;
				int normalx;
				int normaly;

				float collidTime = this->CollisionDetection(*(*dy), *(*st), &normalx, &normaly, deltatime);
				if (collidTime >= delta)
					continue;

				//float remainTime = collidTime > 0.1f * delta ? delta - collidTime : delta;
				float remainTime = delta - collidTime;
				(*st)->getShot = true;

				(*dy)->getObject()->onCollision((*st)->getObject());
				(*st)->getObject()->onCollision((*dy)->getObject());

				if ((*dy)->getObject()->GetID() == eID::ID_Bullet && (*st)->getObject()->GetID() == eID::ID_Tank ||
					(*st)->getObject()->GetID() == eID::ID_Bullet && (*dy)->getObject()->GetID() == eID::ID_Tank)
					continue;

				D3DXVECTOR2 pos = *(*dy)->getBox().pos;
				D3DXVECTOR2 velocity = (*dy)->getVelocity();
				if (normalx != 0)
				{
					pos.x -= (velocity.x * remainTime) * 1.1f;
				}
				if (normaly != 0)
				{
					pos.y -= (velocity.y * remainTime) * 1.1f;
				}

				(*dy)->setPosition(pos);
				GAMELOG("reUpdate Collided!");
			}

			if (countCollid == 0)
				break;
		}
	}
}