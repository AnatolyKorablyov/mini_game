#pragma once
#include "Definition.h"

class CActiveObject : public cocos2d::Sprite
{
private:
	cocos2d::PhysicsBody *spriteBody;
public:
	CActiveObject();
	~CActiveObject(void);
	static CActiveObject *createWithTexture(cocos2d::Texture2D* aTexture);
private:
	cocos2d::Rect getRect();
	void onEnter() override;
	void onExit() override;
	bool initWithTexture(cocos2d::Texture2D* aTexture) override;
};