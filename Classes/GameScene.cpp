#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui\CocosGUI.h"
#include "Definition.h"
#include "MainMenuScene.h"
#include "GameOverScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto layer = GameScene::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

	scene->addChild(layer);
	return scene;
}


bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	
	this->schedule(schedule_selector(GameScene::SpawnPipe), PIPE_SPAWN_FREQUENCY);

	
	CreateStaticElements(origin, visibleSize);
	CreateActiveElements(origin, visibleSize);
	

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	this->scheduleUpdate();

	return true;
}

void GameScene::SpawnPipe(float dt)
{
	m_slowDriver.SpawnSlowDriver(this);
}

void GameScene::CreateStaticElements(Vec2 const &origin, Size const &visibleSize)
{
	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);
	this->addChild(edgeNode);

	auto backgroundSprite = Sprite::create("road.png");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(backgroundSprite);

	m_pScoresLabel = Label::createWithTTF("Helolo", "Graffiti.ttf", visibleSize.height * 0.15);
	m_pScoresLabel->setColor(Color3B::WHITE);
	m_pScoresLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height - origin.y - 50));

	this->addChild(m_pScoresLabel, 10000);
}

void GameScene::CreateActiveElements(Vec2 const &origin, Size const &visibleSize)
{
	m_playerCar = new PlayerCar(this);
	
	/*auto FinishForSlowCarBody = PhysicsBody::createBox(Size(visibleSize.width, 2));
	FinishForSlowCarBody->setDynamic(true);
	FinishForSlowCarBody->setGravityEnable(false);
	FinishForSlowCarBody->setContactTestBitmask(true);
	FinishForSlowCarBody->setContactTestBitmask(OBSTACLE_COLLISION_BITMASK);
	FinishForSlowCarBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);

	auto FinishForSlowCarNode = Node::create();
	FinishForSlowCarNode->setPosition(Point(visibleSize.width / 2, 200));
	FinishForSlowCarNode->setPhysicsBody(FinishForSlowCarBody);
	this->addChild(FinishForSlowCarNode);*/
/*
	Texture2D *pLeftButtonTexture = Director::getInstance()->getTextureCache()->addImage("left.png");
	m_leftButton = CActiveObject::createWithTexture(pLeftButtonTexture);
	m_leftButton->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100, origin.y + 50));
	m_leftButton->setTag(128);
	m_leftButton->getPhysicsBody()->setDynamic(false);
	m_leftButton->getPhysicsBody()->setName("leftButton");
	addChild(m_leftButton);


	Texture2D *pRightButtonTexture = Director::getInstance()->getTextureCache()->addImage("right.png");
	m_rightButton = CActiveObject::createWithTexture(pRightButtonTexture);
	m_rightButton->setPosition(Vec2(origin.x + visibleSize.width / 2 + 100, origin.y + 50));
	m_rightButton->setTag(128);
	m_rightButton->getPhysicsBody()->setDynamic(false);
	m_rightButton->setName("rightButton");
	addChild(m_rightButton);*/
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact)
{
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	if (((PLAYER_CAR_COLLISION_BITMASK == a->getCollisionBitmask()) &&
		(SLOW_CAR_CONTACT_BITMASK == b->getCollisionBitmask())) ||
		((PLAYER_CAR_COLLISION_BITMASK == b->getCollisionBitmask()) &&
			(SLOW_CAR_CONTACT_BITMASK == a->getCollisionBitmask())))
	{
		delete m_playerCar;
		GoToGameOverScene();
	}
	else if (((SLOW_CAR_CONTACT_BITMASK == a->getCollisionBitmask()) &&
		(OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask())) ||
		((SLOW_CAR_CONTACT_BITMASK == b->getCollisionBitmask()) &&
			(OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask())))
	{
		m_pScoresLabel->setString("NOT GOOD");
	}
	return false;
}

//void GameScene::ControlPlayer(const std::string & nameObject)
//{
//	if (nameObject == "leftButton")
//	{
//		m_playerCar->m_leftMove = true;
//		m_playerCar->ControlCarWithButton();
//		m_playerCar->m_leftMove = false;
//	}
//	else if (nameObject == "rightButton")
//	{
//		m_playerCar->m_rightMove = true;
//		m_playerCar->ControlCarWithButton();
//		m_playerCar->m_rightMove = false;
//	}
//}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	auto location = touch->getLocation();
	auto arr = sceneWorld->getShapes(location);

	PhysicsBody* body = nullptr;
	for (auto& obj : arr)
	{
		if ((obj->getBody()->getTag() & 128) != 0)
		{
			body = obj->getBody();
		}
	}

	if (body != nullptr)
	{
		Node* pMouse = Node::create();
		auto physicsBody = PhysicsBody::create(PHYSICS_INFINITY, PHYSICS_INFINITY);
		physicsBody->setDynamic(false);
		pMouse->addComponent(physicsBody);
		pMouse->setPosition(location);
		pMouse->setName(body->getName());
		this->addChild(pMouse);
		PhysicsJointPin* joint = PhysicsJointPin::construct(physicsBody, body, location);
		joint->setMaxForce(5000.0f * body->getMass());
		sceneWorld->addJoint(joint);
		m_mouses.insert(std::make_pair(touch->getID(), pMouse));
//		ControlPlayer(pMouse->getName());

		return true;
	}

	return false;
}



void GameScene::onTouchMoved(Touch* touch, Event* event)
{
	auto it = m_mouses.find(touch->getID());

	if (it != m_mouses.end())
	{
		if (it->second->getName() == "Player")
		{
			m_playerCar->ControlCarWithTouch(touch->getLocation().x);

		}
		//else
		//{
		//	ControlPlayer(it->second->getName());
		//}

	}
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{
	auto it = m_mouses.find(touch->getID());

	if (it != m_mouses.end())
	{
		this->removeChild(it->second);
		m_mouses.erase(it);
	}
}


void GameScene::GoToGameOverScene()
{
	auto scene = GameOverScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
