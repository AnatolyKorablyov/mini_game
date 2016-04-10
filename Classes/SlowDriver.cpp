#include "SlowDriver.h"
#include "Definition.h"

USING_NS_CC;

SlowDriver::SlowDriver()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
}

void SlowDriver::SpawnSlowDriver(cocos2d::Layer *layer)
{	
	auto SlowDriver = Sprite::create("slow.png");
	auto SlowDriverBody = PhysicsBody::createBox(SlowDriver->getContentSize());

	auto random = CCRANDOM_0_1();

	if (random < LOWER_SCREEN_PIPE_THRESHOLD)
	{
		random = float(LOWER_SCREEN_PIPE_THRESHOLD);
	}
	else if (random > UPPER_SCREEN_PIPE_THRESHOLD)
	{																					
		random = float(UPPER_SCREEN_PIPE_THRESHOLD);
	}

	auto topPipePosition = random * visibleSize.width;
	SlowDriverBody->setDynamic(true);
	SlowDriverBody->setContactTestBitmask(true);
	SlowDriverBody->setContactTestBitmask(SLOW_CAR_CONTACT_BITMASK);
	SlowDriverBody->setCollisionBitmask(SLOW_CAR_CONTACT_BITMASK);

	SlowDriver->setPosition(topPipePosition + origin.x, visibleSize.height - 20);
	SlowDriver->setPhysicsBody(SlowDriverBody);

	layer->addChild(SlowDriver);
	
	auto topPipeAction = MoveBy::create(PIPE_MOVEMENT_SPEED * visibleSize.width, Point(0, -visibleSize.width * 2));
	SlowDriver->runAction(topPipeAction);
	
}

SlowDriver::~SlowDriver()
{

}