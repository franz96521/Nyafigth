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

#include "Level5.h"
//#include "levels/Level5.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"
//#include "levels/Level5.h"
USING_NS_CC;

Scene* Level5::createScene()
{   
    return Level5::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Level5Scene.cpp\n");
}

// on "init" you need to initialize your instance
bool Level5::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    _visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    inicializarBackground();
    inicializar();
    scheduleUpdate();
    inicializarBotones();    
    log("inicio musica");
    _sonido=cocos2d::AudioEngine::play2d("sound/pokemon.mp3",true);
    return true;
}
void Level5::update(float dt) {
    //   log("update");
    checarchoque();
    tasks();
    checarMovimiento();
}
void Level5::checarMovimiento() {
    auto x = _player->getPosition();
    /*if (x.x > _visibleSize.width) {
        _player->setPosition(0, x.y);
    }
    else if (x.x <0) {
        _player->setPosition(_visibleSize.width, x.y);
    }
    if (x.y > _visibleSize.height) {
        _player->setPosition(x.x, 0);
    }
    else if (x.y < 0) {
        _player->setPosition(x.x, _visibleSize.height);
    }
    */
    if (_mover[0] && x.x >= 0) {
        _player->setPosition(x.x - 5, x.y);
        _background->moveRight();
    }
    if (_mover[1] && x.x <= _visibleSize.width) {

        _player->setPosition(x.x + 5, x.y);


        _background->moveLeft();
    }
    if (_mover[2] && x.y <= _visibleSize.height) {
        _player->setPosition(x.x, x.y + 5);
        _background->moveUp();
    }
    if (_mover[3] && x.y >= 0) {
        _player->setPosition(x.x, x.y - 5);
        _background->moveDown();
    }

}
void Level5::inicializarUI() {
    auto inicio = cocos2d::ui::Button::create("btn/generalbtn.png");
    inicio->setScale(.2);
    inicio->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 4 * 3.45));
    inicio->setTitleText("Ir a inicio  ");
    inicio->setTitleFontSize(50);
    inicio->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        //Director::getInstance()->resume();
        saveScore();
        cocos2d::AudioEngine::stopAll();      
        Director::getInstance()->resume();
        Director::getInstance()->popToRootScene();
        });
    this->addChild(inicio);
    inicio->setLocalZOrder(50);
    
}
void Level5::inicializar() {
    auto setfile = Custom::SettingFile();

    _lives = Custom::lives::createLives(this, _visibleSize);
    _lives->setPosition(0, _visibleSize.height);
    _lives->setNumberOfLives(setfile.getl());

    _score = Custom::Scoreboard::createScores(this);
    _score->setPosition(_visibleSize);
    _score->deaths = setfile.getd();
    _score->score= setfile.gets();
    _score->deadEnemies = setfile.gete();
    _score->update();
    _taskslabel = cocos2d::Label::createWithSystemFont("tutorial", "Arial Black", 20);
    _taskslabel->setPosition(_visibleSize.width / 2, _visibleSize.height / 10 * 9);
    _taskslabel->setAlignment(cocos2d::TextHAlignment::CENTER);
    this->addChild(_taskslabel);

    _enemygroup = Custom::EnemyGroup::createGroupofEnemys(this, (int)((_nivel) *setfile.getdif() / 2)+1, _visibleSize, _nivel);
    _enemygroup->setMaxLives(setfile.getdif());

    _player = Custom::customPlayer::createPlayer(this, _visibleSize);
    _player->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);
    _player->setSkin(setfile.getsk());
    _player->setName(setfile.getname());

    _tasks = cocos2d::Sprite::create("tasks/taskboard2.png");
    _tasks->setScale(.3);
    _tasks->setPosition(_visibleSize.width / 2, _visibleSize.height / 10 * 9);
    this->addChild(_tasks);
    _tasks->setLocalZOrder(-1);

    


    inicializarEscuchador();
    initKeyboard();

}
void Level5::inicializarEscuchador() {
    auto escuchador = EventListenerTouchOneByOne::create();
    escuchador->setSwallowTouches(true);
    for (int i = 0; i < 6; i++)
    {
        _player->posiciones.push_back(std::make_pair(0, 0));
    }
    escuchador->onTouchBegan = CC_CALLBACK_2(Level5::inicioMovimiento, this);
    escuchador->onTouchMoved = CC_CALLBACK_2(Level5::enMovimiento, this);
    escuchador->onTouchEnded = CC_CALLBACK_2(Level5::finMovimiento, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(escuchador, this);
}
bool Level5::inicioMovimiento(cocos2d::Touch* toque, cocos2d::Event* Evento) {
    _player->inicioDeMovimiento(toque, Evento);
    return true;
}
bool Level5::enMovimiento(cocos2d::Touch* toque, cocos2d::Event* Evento) {
    _player->enMovimiento(toque, Evento);
    return true;
}
bool Level5::finMovimiento(cocos2d::Touch* toque, cocos2d::Event* Evento) {
    _player->finDeMovimiento(toque, Evento);
    int x = _player->evaluarMovimiento();
    auto y = _enemygroup->actualizarPuntaje(x);
    _score->increaseScore(std::get<0>(y));
    _score->increasedeadEnemies(std::get<1>(y));
    return true;
}

void Level5::initKeyboard() {
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Level5::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Level5::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _player);
}
void Level5::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    int x = _player->onKeyPressed(keyCode);
    if (x < 4) {
        _mover[x] = 1;
    }

}
void Level5::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    int x = _player->onKeyRelease(keyCode);
    if (x < 4) {
        _mover[x] = 0;
    }
}

bool Level5::checarchoque() {
    for (int i = 0; i < _enemygroup->Enemygroup.size(); i++)
    {
        if (_player->getBoundingBox().intersectsRect(_enemygroup->Enemygroup[i]->getBoundingBox())) {
            if (!_enemygroup->Enemygroup[i]->choque) {
                log("vida -1");
                _lives->quitlive();
                if (!_lives->alive()) {
                    log("mueto ");
                    _score->increaseDeaths();
                    _lives->restartLives();
                }
            }
            _enemygroup->Enemygroup[i]->choque = 1;
            return true;
        }
        else
        {
            _enemygroup->Enemygroup[i]->choque = 0;
        }
    }
    return false;
}
void Level5::tasks() {     
    if (!_botonsalida) {
        _botonsalida = true;
        _taskslabel->setString("Infinite mode");
        //Director::getInstance()->pause();
        inicializarUI();
    }

}
void Level5::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
void Level5::inicializarBackground() {
    _background = Custom::Backround::createBakground(this, _visibleSize,1);
    _background->setLocalZOrder(-10);
    _background->setNewBackground(_nivel,1);
    
}

void Level5::inicializarBotones() {
    auto pausa = cocos2d::ui::Button::create("btn/pause.png");
    pausa->setPosition(Vec2(_visibleSize.width-30, _visibleSize.height / 4 * 3));    
    pausa->setScale(.3);
    pausa->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {        
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                pausaBottom();
                break;
            case ui::Widget::TouchEventType::ENDED:               
                break;
            default:
                break;
            }
        });
    this->addChild(pausa);

}
void Level5::pausaBottom() {
    if (_pausa) {
        Director::getInstance()->resume();
        _pausa = false;
    }
    else {
        Director::getInstance()->pause();
        _pausa = true;
    }
}
void Level5::saveScore() {
    Custom::ScoreFile scores = Custom::ScoreFile::ScoreFile();    
    log("main muertes %d enemigos %d score %d", _score->deaths, _score->deadEnemies, _score->score);
    scores.addscore(_score->deaths, _score->deadEnemies, _score->score,_player->Name->getString());
    auto setfile = Custom::SettingFile();
    setfile.setSettings(setfile.getl(), 0, 0, 0, setfile.getsk(), setfile.getname(), setfile.getdif());
}

