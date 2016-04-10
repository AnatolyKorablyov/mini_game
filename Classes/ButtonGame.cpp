#include "ButtonGame.h"
#include "GameScene.h"

USING_NS_CC;

CActiveObject::CActiveObject(void)
{
}

CActiveObject::~CActiveObject(void)
{
}

Rect CActiveObject::getRect()
{
	auto s = getTexture()->getContentSize();
	return Rect(-s.width / 2, -s.height / 2, s.width, s.height);
}

CActiveObject* CActiveObject::createWithTexture(Texture2D* aTexture)
{
	CActiveObject* pPaddle = new (std::nothrow) CActiveObject();
	pPaddle->initWithTexture(aTexture);
	pPaddle->autorelease();

	return pPaddle;
}

bool CActiveObject::initWithTexture(Texture2D* aTexture)
{
	if (Sprite::initWithTexture(aTexture))
	{
		Sprite::setScale(0.79f);
		spriteBody = PhysicsBody::createCircle(getContentSize().width / 2, PhysicsMaterial(600, 0.5f, 600));
		spriteBody->setTag(128);
		setPhysicsBody(spriteBody);
	}

	return true;
}

void CActiveObject::onEnter()
{
	Sprite::onEnter();
}

void CActiveObject::onExit()
{
	Sprite::onExit();
}

