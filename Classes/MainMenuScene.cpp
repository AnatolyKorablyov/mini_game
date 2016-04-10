#include "MainMenuScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"
#include "Definition.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);
    return scene;
}


bool MainMenuScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create("mainMenu.png");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(backgroundSprite);

	/*auto titleSprite = Sprite::create("ladaLogo.png");
	titleSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height + origin.y));
	this->addChild(titleSprite);
*/
	auto playItem = MenuItemImage::create("letsGo.png", "play.png", CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));
	playItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	auto menu = Menu::create(playItem, NULL);
	menu->setPosition(Point::ZERO);

	this->addChild(menu);
    return true;
}
void MainMenuScene::GoToGameScene(cocos2d::Ref *sender)
{
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
