#pragma once
#include "cocos2d.h"
#include "GameScene.h"
#include "Definition.h"

class GameOverScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(unsigned score);

	virtual bool init();
    CREATE_FUNC(GameOverScene);
private:
	void GoToGameScene(cocos2d::Ref *sender);
	
};