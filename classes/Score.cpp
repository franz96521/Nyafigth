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

#include "Score.h"

USING_NS_CC;


Scene* Score::createScene()
{  
    return Score::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in ScoreScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Score::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    srand(time(NULL));
    _visibleSize = Director::getInstance()->getVisibleSize();
    
    
    inicializarUI();
    inicializarScoreboard();
    inicializarEscuchador();
    inicializarfondo();
    _sonido = cocos2d::AudioEngine::play2d("sound/roky.mp3");
    return true;
}


void Score::inicializarUI() {
    auto button = cocos2d::ui::Button::create("btn/cerrar.png");
    button->setPosition(Vec2(_visibleSize.width -35, _visibleSize.height / 4 * 3.7));
    button->setScale(.7);
    //button->setTitleText("atras ");
    button->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {      
        cocos2d::AudioEngine::stop(_sonido);
        Director::getInstance()->popScene();
        });
    this->addChild(button);
}
void Score::inicializarScoreboard() {
    auto  sc = Custom::ScoreFile::ScoreFile();
    log("inicializando ");
    _scoreboard = cocos2d::Label::createWithSystemFont(sc.getString(), "Arial Black", 30);
    _scoreboard->setColor(cocos2d::Color3B(116, 181, 4));
    _scoreboard->setAlignment(TextHAlignment::CENTER);
    _scoreboard->setAnchorPoint(Vec2(.5, 1));
    
    _scoreboard->setPosition(_visibleSize.width / 2, _visibleSize.height -20);
    this->addChild(_scoreboard);
}

void Score::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
void Score::inicializarEscuchador() {
    auto escuchador = EventListenerTouchOneByOne::create();
    escuchador->setSwallowTouches(true);    
    escuchador->onTouchBegan = CC_CALLBACK_2(Score::inicioMovimiento, this);
    escuchador->onTouchMoved = CC_CALLBACK_2(Score::enMovimiento, this);
    escuchador->onTouchEnded = CC_CALLBACK_2(Score::finMovimiento, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(escuchador, this);
}
bool Score::inicioMovimiento(cocos2d::Touch* toque, cocos2d::Event* Evento) {
    _inicio = toque->getStartLocation();
    return true;
}
bool Score::enMovimiento(cocos2d::Touch* toque, cocos2d::Event* Evento) {
    _fin = toque->getLocation();
    float mov = _fin.y - _inicio.y;
    auto x = _scoreboard->getPosition();
    if (mov > 0) {
        _scoreboard->setPosition(x.x, x.y + 10);
    }
    else {
        _scoreboard->setPosition(x.x, x.y - 10);
    }
    
    return true;
}
bool Score::finMovimiento(cocos2d::Touch* toque, cocos2d::Event* Evento) {
   
    return true;
}
void Score::inicializarfondo() {
    auto x = Custom::Backround::createBakground(this, _visibleSize, 0);
    x->setpredefBackground(6,0);
    x->setLocalZOrder(-10);
    x->image->setOpacity(70);

}
