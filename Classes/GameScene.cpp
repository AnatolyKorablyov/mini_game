#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui\CocosGUI.h"
#include "Definition.h"
#include "GameOverScene.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto layer = GameScene::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

	scene->addChild(layer);
	return scene;
}

void GameScene::InitBackground()
{
	auto backgroundSprite = Sprite::create("road.png");
	backgroundSprite->setAnchorPoint(Vec2::ZERO);
	backgroundSprite->setPosition(0, 0);
	this->addChild(backgroundSprite, -1);

	Vector<SpriteFrame*> frames;
	Size backgroundSize = backgroundSprite->getContentSize();
	frames.pushBack(SpriteFrame::create("road1.png", Rect(0, 0, backgroundSize.width, backgroundSize.height)));
	frames.pushBack(SpriteFrame::create("road2.png", Rect(0, 0, backgroundSize.width, backgroundSize.height)));
	frames.pushBack(SpriteFrame::create("road3.png", Rect(0, 0, backgroundSize.width, backgroundSize.height)));
	auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
	auto animate = Animate::create(animation);
	backgroundSprite->runAction(RepeatForever::create(animate));
}

void GameScene::InitLabel()
{
	m_scoreLabel = Label::createWithTTF("Score: 0", "Graffiti.ttf", m_visibleSize.height * 0.05);
	m_scoreLabel->setColor(Color3B::WHITE);
	m_scoreLabel->setPosition(Point(m_visibleSize.width / 2 + m_origin.x, 100));
	this->addChild(m_scoreLabel, 100);
	m_gasolineLabel = Label::createWithTTF("Gas", "Graffiti.ttf", m_visibleSize.height * 0.05);
	m_gasolineLabel->setColor(Color3B::WHITE);
	m_gasolineLabel->setPosition(Point(m_visibleSize.width / 2 + m_origin.x, 50));
	this->addChild(m_gasolineLabel, 100);
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();

	InitBackground();

	this->schedule(schedule_selector(GameScene::SpawnSlowCar), SLOW_CAR_SPAWN_FREQUENCY);
	this->schedule(schedule_selector(GameScene::SpawnGasoline), BONUS_SPAWN_FREQUENCY);
	this->schedule(schedule_selector(GameScene::DecrementGasoline), GAS_MILEAGE);

	InitPhysics();
	InitTouch();
	InitPlayer();
	InitLabel();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Academeg.mp3", true);
	return true;
}

void GameScene::DecrementGasoline(float dt)
{
	m_gasoline -= 0.1f;
	std::stringstream ss;
	ss << m_gasoline;
	m_gasolineLabel->setString("Gas: " + ss.str());
	if (m_gasoline <= 0)
	{
		GoToGameOverScene();
	}
}


void GameScene::InitPhysics()
{
	auto contactListener = EventListenerPhysicsContact::create();

	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::OnCollision, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void GameScene::onTouchMov(Touch* touch, Event* event)
{
	(void)event;

	auto touchLocation = touch->getLocation();

	if (m_playerSpr->getBoundingBox().containsPoint(touchLocation))
	{
		MovePlayerIfPossible(touchLocation.x);
	}
}

void GameScene::RemoveBonus()
{
	for (auto bonus = m_bonusGasoline.begin(); bonus != m_bonusGasoline.end();)
	{
		if ((*bonus)->getPhysicsBody()->getName() == "empty")
		{
			(*bonus)->setVisible(false);
			this->removeChild((*bonus));
			bonus = m_bonusGasoline.erase(bonus);
		}
		else
		{
			++bonus;
		}
	}
}

bool GameScene::OnCollision(PhysicsContact& contact)
{
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	if ((a->getName() == "player" && b->getName() == "slowCar") ||
		(b->getName() == "player" && a->getName() == "slowCar"))
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		SimpleAudioEngine::getInstance()->playEffect("bomb.wav");
		GoToGameOverScene();
	}
	else if ((a->getName() == "player" && b->getName() == "spectacle") ||
		(b->getName() == "player" && a->getName() == "spectacle"))
	{
		m_score += SCORE_INCREMENT;
		std::stringstream ss;
		ss << m_score;

		m_scoreLabel->setString("Score: " + ss.str());
	}
	else if (a->getName() == "player" && b->getName() == "gas")
	{
		SimpleAudioEngine::getInstance()->playEffect("pickupBonus.wav");
		m_gasoline += 3.0f;
		b->setName("empty");
		RemoveBonus();
	}
	else if (b->getName() == "player" && a->getName() == "gas")
	{
		SimpleAudioEngine::getInstance()->playEffect("pickupBonus.wav");
		m_gasoline += 3.0f;
		a->setName("empty");
		RemoveBonus();
	}
	
	return false;
}

void GameScene::InitTouch()
{
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBeg, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMov, this);
	listener->onTouchEnded = [=](Touch* touch, Event* event) {};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


void GameScene::MovePlayerIfPossible(float newX)
{
	float sprHalfWidth = m_playerSpr->getBoundingBox().size.width / 2;

	if (newX >= sprHalfWidth && newX < m_visibleSize.width - sprHalfWidth)
	{
		m_playerSpr->setPositionX(newX);
	}
}

bool GameScene::onTouchBeg(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}

void GameScene::SpawnSlowCar(float dt)
{
	for (int i = 0; i < C_BOMBS_COUNT; i++)														
	{
		Sprite* carSprite = Sprite::create("slow.png");
		Sprite* spectacleSprite = Sprite::create("spectacles.png");
		auto posX = CCRANDOM_0_1() * m_visibleSize.width;
		auto posY = m_visibleSize.height + carSprite->getContentSize().height / 2;

		carSprite->setPosition(posX, posY);
		spectacleSprite->setPosition(posX, posY);
		this->addChild(carSprite, 1);
		this->addChild(spectacleSprite, 1);

		SetPhysicsBodyBox(carSprite);
		SetPhysicsBodyCircle(spectacleSprite);

		auto speed = (CCRANDOM_0_1() + 0.8f) * -250;

		carSprite->getPhysicsBody()->setVelocity(Vect(0, speed));
		carSprite->getPhysicsBody()->setName("slowCar");

		spectacleSprite->getPhysicsBody()->setVelocity(Vect(0, speed));
		spectacleSprite->getPhysicsBody()->setName("spectacle");
		m_slowCar.pushBack(carSprite);
	}
}

void GameScene::SpawnGasoline(float dt)
{
	Sprite* gas = Sprite::create("benzin.png");

	gas->setPosition(CCRANDOM_0_1() * m_visibleSize.width, m_visibleSize.height + gas->getContentSize().height / 2);
	this->addChild(gas, 1);

	SetPhysicsBodyBox(gas);

	gas->getPhysicsBody()->setVelocity(Vect(0, ((CCRANDOM_0_1() + 0.8f) * -250)));
	gas->getPhysicsBody()->setName("gas");
	m_bonusGasoline.pushBack(gas);

}

void GameScene::SetPhysicsBodyCircle(cocos2d::Sprite* sprite)
{
	auto body = PhysicsBody::createCircle(sprite->getContentSize().width);

	body->setContactTestBitmask(true);
	body->setDynamic(true);

	sprite->setPhysicsBody(body);
}

void GameScene::SetPhysicsBodyBox(cocos2d::Sprite* sprite)
{
	auto body = PhysicsBody::createBox(cocos2d::Size(sprite->getContentSize().width, sprite->getContentSize().height));

	body->setContactTestBitmask(true);
	body->setDynamic(true);

	sprite->setPhysicsBody(body);
}

void GameScene::InitPlayer()
{
	m_playerSpr = Sprite::create("myCar.png");
	m_playerSpr->setPosition(m_visibleSize.width / 2, 200);
	this->addChild(m_playerSpr, 0);
	SetPhysicsBodyBox(m_playerSpr);

	m_playerSpr->getPhysicsBody()->setGravityEnable(false);
	m_playerSpr->getPhysicsBody()->setName("player");
}

void GameScene::GoToGameOverScene()
{
	auto scene = GameOverScene::createScene(m_score);
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}