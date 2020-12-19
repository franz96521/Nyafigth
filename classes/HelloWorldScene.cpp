/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "GameTutorial.h"
#include "Settings.h"
#include "levels/Level1.h"
#include "Score.h"
#include "AudioEngine.h"
#include "Custom.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    srand(time(NULL));

    _visibleSize = Director::getInstance()->getVisibleSize();
    auto menu_item_1 = MenuItemImage::create("btn/tutorial.png", "btn/tutorial.png", CC_CALLBACK_1(HelloWorld::Tutorial, this));
    auto menu_item_2 = MenuItemImage::create("btn/scores.png", "btn/scores.png", CC_CALLBACK_1(HelloWorld::HighScores, this));
    auto menu_item_3 = MenuItemImage::create("btn/settings.png", "btn/settings.png", CC_CALLBACK_1(HelloWorld::Settings, this));
    auto menu_item_4 = MenuItemImage::create("btn/close.png", "btn/close.png", CC_CALLBACK_1(HelloWorld::ImageButton, this));
    auto menu_item_5 = MenuItemImage::create("btn/play.png","btn/play.png", CC_CALLBACK_1(HelloWorld::Play, this));    

    menu_item_1->setPosition(Point(_visibleSize.width / 2, (_visibleSize.height / 6) * 5));
    menu_item_5->setPosition(Point(_visibleSize.width / 2, (_visibleSize.height / 6) * 4));
    menu_item_2->setPosition(Point(_visibleSize.width / 2, (_visibleSize.height / 6) * 3));
    menu_item_3->setPosition(Point(_visibleSize.width / 2, (_visibleSize.height / 6) * 2));
    menu_item_4->setPosition(Point(_visibleSize.width / 2, (_visibleSize.height / 6) * 1));
    

    auto* menu = Menu::create(menu_item_1, menu_item_2, menu_item_3, menu_item_4,menu_item_5,  NULL);
    menu->setPosition(Point(0, 0));
   
    this->addChild(menu);
    auto x = Custom::SettingFile();
    x.setSettings(5, 0, 0, 0, 0, "player1",1);
    _Sonido = cocos2d::AudioEngine::play2d("sound/inicio.mp3");

    inicializarfondo();
    return true;
}
void HelloWorld::Tutorial(HelloWorld::Ref* pSender) {
    log("Tutorial");
    auto scene = GameTutorial::createScene();
    cocos2d::AudioEngine::stop(_Sonido);
    Director::getInstance()->resume();
    Director::getInstance()->pushScene(scene);
}
void HelloWorld::Play(HelloWorld::Ref* pSender) {
    log("play ");
    auto scene = Level1::createScene();
    cocos2d::AudioEngine::stop(_Sonido);
    Director::getInstance()->resume();
    Director::getInstance()->pushScene(scene);
}
void HelloWorld::HighScores(HelloWorld::Ref* pSender) {
    log("scores");
    
    auto scene = Score::createScene();
    cocos2d::AudioEngine::stop(_Sonido);
    Director::getInstance()->resume();
    Director::getInstance()->pushScene(scene);
}
void HelloWorld::Settings(HelloWorld::Ref* pSender) {
    log("setting");
    log("name %s ", _name.c_str());
    log("vidas %d ", _vidas);
    log("enemigos %d ", _enemigos);
    log("skin %d ", _skin);    
    auto scene = Settings::createScene();
    cocos2d::AudioEngine::stop(_Sonido);    
    Director::getInstance()->resume();
    Director::getInstance()->pushScene(scene);

    
}
void HelloWorld::ImageButton(HelloWorld::Ref* pSender) {
    log("salir");

    Director::getInstance()->end();
}
void HelloWorld::inicializarfondo() {
    auto x = Custom::Backround::createBakground(this,_visibleSize,0);
    x->setpredefBackground(5,0);
    x->setLocalZOrder(-10);
   
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}