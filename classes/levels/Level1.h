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

#ifndef __Level1_SCENE_H__
#define __Level1_SCENE_H__

#include "cocos2d.h"
#include "Custom.h"
#include "ui/CocosGUI.h"

class Level1 : public cocos2d::Scene
{
private:
    Custom::customPlayer* _player;
    Custom::Scoreboard* _score;
    Custom::Enemy* _enemy;
    Custom::EnemyGroup* _enemygroup;
    Custom::lives* _lives;
    cocos2d::Size _visibleSize;
    cocos2d::Label* _taskslabel;
    Custom::Backround* _background;
    cocos2d::Sprite* _tasks;    
    bool _botonsalida = false;
    bool _mover[4];
    bool _pausa;
    int _sonido;
    int _task1 = 1;
    int _nivel=1;
    void inicializar();
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Level1);

    bool inicioMovimiento(cocos2d::Touch* toque, cocos2d::Event* Evento);
    bool finMovimiento(cocos2d::Touch* toque, cocos2d::Event* Evento);
    bool enMovimiento(cocos2d::Touch* toque, cocos2d::Event* Evento);
    void inicializarEscuchador();
    void initKeyboard();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    bool checarchoque();
    void update(float dt) override;
    void tasks();
    void inicializarUI();
    void inicializarBackground();
    void checarMovimiento();
    void pausaBottom();
    void inicializarBotones();
    void saveScore();
};

#endif // __Level1_SCENE_H__
