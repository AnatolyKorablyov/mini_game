#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

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
	void SpawnSlowCar(float dt);
	void SpawnGasoline(float dt);

	void InitBackground();
	void InitPhysics();
	void InitTouch();
	void InitPlayer();
	void InitLabel();

	void DecrementGasoline(float dt);
	void GoToGameOverScene();
	void RemoveBonus();

	bool OnCollision(cocos2d::PhysicsContact& contact);
	bool onTouchBeg(cocos2d::Touch* touch, cocos2d::Event* event);
	void SetPhysicsBodyBox(cocos2d::Sprite* sprite);
	void SetPhysicsBodyCircle(cocos2d::Sprite* sprite);
	void onTouchMov(cocos2d::Touch* touch, cocos2d::Event* event);
	void MovePlayerIfPossible(float newX);
private:
	cocos2d::PhysicsWorld *sceneWorld;
	cocos2d::Sprite* m_playerSpr;
	cocos2d::Size m_visibleSize;
	cocos2d::Vec2 m_origin;
	cocos2d::Vector<cocos2d::Sprite*> m_slowCar;
	cocos2d::Vector<cocos2d::Sprite*> m_bonusGasoline;
	cocos2d::Label * m_scoreLabel;
	cocos2d::Label * m_gasolineLabel;
	float m_gasoline = 10.0f;
	unsigned m_score = 0;
};