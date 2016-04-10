#pragma once
#include "cocos2d.h"
#include "GameScene.h"

class GameOverScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
    CREATE_FUNC(GameOverScene);

private:
	void GoToGameScene(cocos2d::Ref *sender);
};