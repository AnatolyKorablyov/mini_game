#pragma once
#include "cocos2d.h"
#include <string>

class PlayerCar
{
public:
	PlayerCar(cocos2d::Layer *layer);
	void ControlCarWithButton();
	void ControlCarWithTouch(float pos);
	bool m_leftMove;
	bool m_rightMove;
private:
	cocos2d::Size m_visibleSize;
	cocos2d::Vec2 m_origin;
	cocos2d::Sprite *m_playerSprite;
	
};

