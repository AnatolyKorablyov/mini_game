#pragma once
#include "cocos2d.h"

class SlowDriver
{
public:
	SlowDriver();
	void SpawnSlowDriver(cocos2d::Layer *layer);
	~SlowDriver();
private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;

	cocos2d::Sprite *m_slowDriver;
};