#include "PlayerCar.h"
#include "Definition.h"

USING_NS_CC;

PlayerCar::PlayerCar(cocos2d::Layer *layer)
{
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();

	auto pinfo = AutoPolygon::generatePolygon("myCar.png");
	m_playerSprite = Sprite::create(pinfo);
	m_playerSprite->setPosition(Point(m_visibleSize.width / 2 + m_origin.x, 200));

	auto playerBody = PhysicsBody::createBox(m_playerSprite->getContentSize());
	playerBody->setDynamic(true);
	playerBody->setCollisionBitmask(1);
	playerBody->setGravityEnable(false);
	playerBody->setRotationEnable(false);
	playerBody->setContactTestBitmask(true);
	playerBody->setContactTestBitmask(PLAYER_CAR_COLLISION_BITMASK);
	playerBody->setCollisionBitmask(PLAYER_CAR_COLLISION_BITMASK);
	playerBody->setTag(128);
	playerBody->setName("Player");
	m_playerSprite->setPhysicsBody(playerBody);
	
	layer->addChild(m_playerSprite, 100);
}

void PlayerCar::ControlCarWithButton()
{
	if (m_leftMove)
	{
		m_playerSprite->setPositionX(m_playerSprite->getPositionX() - 10);
	}
	if (m_rightMove)
	{
		m_playerSprite->setPositionX(m_playerSprite->getPositionX() + 10);
	}
}

void PlayerCar::ControlCarWithTouch(float pos)
{
	m_playerSprite->setPositionX(pos);
}

