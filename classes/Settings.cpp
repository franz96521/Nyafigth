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

#include "Settings.h"

USING_NS_CC;

Scene* Settings::createScene()
{ 
    return Settings::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in SettingsScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Settings::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    srand(time(NULL));
    _visibleSize = Director::getInstance()->getVisibleSize();
    auto x = Custom::SettingFile();
    x.setSettings(5, 0, 0, 0,0, name, 5);

    inicializarMenu();
    inicializarcontadores();
    inicializarpersonaje();
    inicializarfondo();
    inicializarUI();
    _sonido2 = cocos2d::AudioEngine::play2d("sound/zelda.mp3");
    return true;
}


void Settings::inicializarpersonaje() {
    _textField = cocos2d::ui::TextField::create("Enter player name", "Arial", 30);
    _textField->setPosition(Vec2(_visibleSize.width / 3 * 2.10, _visibleSize.height / 10 * 5.5));
    this->addChild(_textField);

    _player = Custom::customPlayer::createPlayer(this, _visibleSize);
    _player->setPosition(_visibleSize.width / 8 * 5.5, _visibleSize.height / 4);
    auto x = Custom::SettingFile();

    _player->setName(x.getname());
    skinNum = x.getsk();
    name = x.getname();
    _player->setSkin(x.getsk());
    _player->setScale(2);    

    vidasnum = x.getl();
    enemynum = x.gete();
    
}
void Settings::inicializarMenu() {
    auto menu_item_1 = MenuItemFont::create("change skin", CC_CALLBACK_1(Settings::skin, this));
    auto menu_item_2 = MenuItemFont::create("change lives", CC_CALLBACK_1(Settings::lives, this));
    auto menu_item_3 = MenuItemFont::create("change difficulty", CC_CALLBACK_1(Settings::maxenemys, this));
    auto menu_item_4 = MenuItemFont::create("change name", CC_CALLBACK_1(Settings::changename, this));
    

    menu_item_1->setPosition(Point(_visibleSize.width / 4, (_visibleSize.height / 6) * 5));
    menu_item_2->setPosition(Point(_visibleSize.width / 4, (_visibleSize.height / 6) * 4));
    menu_item_3->setPosition(Point(_visibleSize.width / 4, (_visibleSize.height / 6) * 3));
    menu_item_4->setPosition(Point(_visibleSize.width / 4, (_visibleSize.height / 6) * 2));
    

    auto* menu = Menu::create(menu_item_1, menu_item_2, menu_item_3, menu_item_4, NULL);
    menu->setPosition(Point(0, 0));
    this->addChild(menu);
}
void Settings::inicializarcontadores() {
    auto x = Custom::SettingFile();
    _lives = Custom::lives::createLives(this, _visibleSize);
    _lives->setPosition(_visibleSize.width / 2, _visibleSize.height / 10 * 9);
    _lives->setNumberOfLives(5);

    _enemys = Custom::lives::createLives(this, _visibleSize);
    _enemys->setTypeofLives("dificultad.png");
    _enemys->setNumberOfLives(5);
    _enemys->setPosition(_visibleSize.width / 2, _visibleSize.height / 10 * 7.5);

    vidasnum = x.getl();
    enemynum = x.gete();

}
void Settings::skin(Settings::Ref* pSender) {
    
    _player->nextSkin();
    skinNum = _player->actualSkin;
    log("skin %d", skinNum);
}
void Settings::lives(Settings::Ref* pSender) {
    
    if (_lives->alive()) {
        _lives->quitlive();
        if (!_lives->alive()) {
            _lives->restartLives();
        }
    }    
    vidasnum = _lives->numberOfLives();
    log("lives %d", vidasnum);
}
void Settings::maxenemys(Settings::Ref* pSender) {
    
    if (_enemys->alive()) {
        _enemys->quitlive();
        if (!_enemys->alive()) {
            _enemys->restartLives();
        }
    }
    enemynum= _enemys->numberOfLives();
    log("maxenemys %d", enemynum);
}
void Settings::changename(Ref* pSender) {
    _player->setName(_textField->getString());
    name = _textField->getString();
    log("name  %s", (name).c_str());
}
void Settings::inicializarfondo() {
    auto x = Custom::Backround::createBakground(this, _visibleSize, 0);
    x->setpredefBackground(7, 0);
    x->setLocalZOrder(-10);
    x->image->setOpacity(50);
    
}
void Settings::inicializarUI() {
    auto button = cocos2d::ui::Button::create("btn/cerrar.png");
    button->setPosition(Vec2(_visibleSize.width - 35, _visibleSize.height / 4 * 3.7));
    button->setScale(.7);
    //button->setTitleText("atras ");
    button->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        try
        {
            cocos2d::AudioEngine::stopAll();
        }
        catch (const std::exception&)
        {

        }
        auto x = Custom::SettingFile();
        x.setSettings(vidasnum, 0, 0, 0, skinNum, name, enemynum);
        Director::getInstance()->resume();
        Director::getInstance()->popScene();
        });
    this->addChild(button);
}



void Settings::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}