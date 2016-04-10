#pragma once
#include "cocos2d.h"
#include "SlowDriver.h"
#include "PlayerCar.h"
#include "ButtonGame.h"

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	CREATE_FUNC(GameScene);
private:
	void SetPhysicsWorld(cocos2d::PhysicsWorld *world)
	{
		sceneWorld = world;
	};
	void SpawnPipe(float dt);

	//void ControlPlayer(const std::string & nameObject);
	bool onContactBegin(cocos2d::PhysicsContact &contact);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void CreateStaticElements(cocos2d::Vec2 const &origin, cocos2d::Size const &visibleSize);
	void CreateActiveElements(cocos2d::Vec2 const &origin, cocos2d::Size const &visibleSize);
	void GoToGameOverScene();
private:

	cocos2d::Sprite *m_backgroundSprite;
	cocos2d::PhysicsWorld *sceneWorld;
	SlowDriver m_slowDriver;
	PlayerCar* m_playerCar;
	CActiveObject *m_leftButton;
	CActiveObject *m_rightButton;
	std::unordered_map<int, cocos2d::Node*> m_mouses;
	cocos2d::Label *m_pScoresLabel;
};