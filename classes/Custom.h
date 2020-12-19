#ifndef _CUSTOM_
#define _CUSTOM_
#include "cocos2d.h"
#include "AudioEngine.h"

namespace  Custom
{    
    class Enemy : public cocos2d::Node
    {
    public:
        cocos2d::Sprite* character;
        cocos2d::Label* label;
        cocos2d::Size visibleSize;
        std::queue<int> movimientos;
        int numeroDeVidas = 4;
        int maxLives=4;
        bool choque = 0;
        std::vector<std::string> nombres;
        static Enemy* createEnemy(cocos2d::Scene* layer, std::string  nombre_sprite,cocos2d::Size visibleSize, std::vector<std::string> nombres) {

            auto thisEnemy = new (std::nothrow) Enemy;
            if (thisEnemy && thisEnemy->init()) {
                thisEnemy->autorelease();
                thisEnemy->visibleSize = visibleSize;
                thisEnemy->character = cocos2d::Sprite::create(nombre_sprite);
                thisEnemy->character->setScale(thisEnemy->getNewScale());
                thisEnemy->nombres = nombres;
                thisEnemy->label = cocos2d::Label::createWithSystemFont("00", "Arial Black", 30);
                thisEnemy->label->setPosition(thisEnemy->character->getPosition().x, thisEnemy->character->getPosition().y + thisEnemy->character->getBoundingBox().size.height/2 +10);
                thisEnemy->addChild(thisEnemy->character);
                thisEnemy->addChild(thisEnemy->label);
                thisEnemy->reiniciar();
                thisEnemy->randomMove(visibleSize);
                auto x = thisEnemy->character->getBoundingBox().size;
                thisEnemy->setContentSize(cocos2d::Size(x.width, x.height));
                layer->addChild(thisEnemy, 100);
                
                /*
                cocos2d::Rect aabb = thisEnemy->getBoundingBox();
                cocos2d::DrawNode* drawNode = DrawNode::create();
                thisEnemy->drawRect(aabb.origin, aabb.origin + aabb.size, Color4F(1, 0, 0, 1));
                layer->addChild(drawNode, 100);*/


                return thisEnemy;
            }
            CC_SAFE_RELEASE(thisEnemy);
            return nullptr;
        }
        void setmaxLives(int maxi) {
            this->maxLives = maxi;
        }
        void randomLives() {
            this->numeroDeVidas = rand() % maxLives + 1;
        }
        void reiniciar() {
            while (!this->movimientos.empty())
            {
                movimientos.pop();
            }
            for (int i = 0; i < numeroDeVidas; i++)
            {
                int x = rand() % 6 + 1;
                this->label->setString(getFigureLabel(x));
                this->movimientos.push(x);
                
            }
            this->setPosition(getNewPosition());

           
            this->label->setString(GetEnemyHits());
            auto x = rand() % (this->nombres.size());
            auto pinfo = cocos2d::AutoPolygon::generatePolygon("enemigos/" + nombres[x]);
            this->character->cocos2d::Sprite::create(pinfo);
            
          //  this->character->setScale(getNewScale());
            
        }
        void tryresume() {
            this->resume();
        }
        void trypause() {
            this->pause();
        }
        void randomMove(cocos2d::Size visibleSize) {

            auto randomPoint = cocos2d::Vec2(rand() % (int)visibleSize.width, rand() % (int)visibleSize.height);

            auto moveTo = cocos2d::MoveTo::create(rand() % 5 + 1, randomPoint);
            auto delayTime = cocos2d::DelayTime::create(0.5);
            auto moveAgainFunc = cocos2d::CallFunc::create([=]()
                {
                    randomMove(visibleSize);
                });

            auto actions = cocos2d::Sequence::create(moveTo, delayTime, moveAgainFunc, nullptr);

            this->runAction(actions);
        }
        float getNewScale() {
            auto j = character->getBoundingBox().size.width;            
            return((visibleSize.width*.07 / j));                      
        }
        int actualizar(int n, std::vector<std::string> nombres) {
            int x = 0;
            if (!movimientos.empty()) {
                if (movimientos.front() == n) {
                    movimientos.pop();
                    x = 1;
                }
            }
            if (movimientos.empty()) {
                reiniciar();
                x = 2;
            }
            else
            {
                this->label->setString(GetEnemyHits());
            }
            return x;
        }
        cocos2d::Vec2 getNewPosition()
        {
            int x = std::rand() % 4 + 1;
            switch (x)
            {
            case 1:
                return cocos2d::Vec2(cocos2d::rand_0_1() * visibleSize.width, -40);
                break;
            case 2:
                return cocos2d::Vec2(cocos2d::rand_0_1() * visibleSize.width, visibleSize.height + 40);
                break;
            case 3:
                return cocos2d::Vec2(visibleSize.width + 40, cocos2d::rand_0_1() * visibleSize.height);
                break;
            case 4:
                return cocos2d::Vec2(-40, cocos2d::rand_0_1() * visibleSize.height);
                break;
            default:
                break;
            }
        }

        std::string getFigureLabel(int n) {
            switch (n)
            {
            case 1:
                return "^";
            case 2:
                return "V";
            case 3:
                return ">";
            case 4:
                return "<";
            case 5:
                return "/";
            case 6:
                return "\\";
            default:
                break;
            }
        }
        std::string GetEnemyHits() {
            std::string s = "";
            std::queue<int> movimientos2;
            while (!movimientos.empty())
            {
                s += getFigureLabel(movimientos.front())+" ";
                movimientos2.push(movimientos.front());
                movimientos.pop();
            }
            while (!movimientos2.empty())
            {
                
                movimientos.push(movimientos2.front());
                movimientos2.pop();
            }
            return s;
        }
    };
    class EnemyGroup {
    public:         
        std::vector<std::vector<std::string>>nombres = {
            {"patio/1.png","patio/2.png","patio/3.png","patio/4.png","patio/5.png" } ,
            { "mall/1.png","mall/2.png","mall/3.png"},
            { "mar/1.png","mar/2.png","mar/3.png","mar/4.png","mar/5.png","mar/6.png","mar/7.png"},
            { "ejipto/1.png","ejipto/2.png","ejipto/3.png","ejipto/4.png","ejipto/5.png","ejipto/6.png","ejipto/7.png","ejipto/8.png"},
            { "cementerio/1.png","cementerio/2.png","cementerio/3.png","cementerio/4.png","cementerio/5.png","cementerio/6.png","cementerio/7.png","cementerio/8.png","cementerio/9.png","cementerio/10.png","cementerio/11.png"},
            { "espacio/1.png","espacio/2.png","espacio/3.png","espacio/4.png","espacio/5.png","espacio/6.png","espacio/7.png"},
        };

        std::vector<Custom::Enemy*> Enemygroup;
        cocos2d::Size VisibleSize;
        int level=0; 
        int numeroDeEnemigos;
        static EnemyGroup *createGroupofEnemys(cocos2d::Scene *layer, int numeroDeEnemigos, cocos2d::Size VisibleSize, int level  ) {
            auto thisGroup = new (std::nothrow) EnemyGroup;
            thisGroup->VisibleSize = VisibleSize;
            thisGroup->numeroDeEnemigos = numeroDeEnemigos;
            thisGroup->level = level;
            for (int i = 0; i < numeroDeEnemigos; i++)
            {
                auto x = rand() % (thisGroup->nombres[level].size());
                auto e = Custom::Enemy::createEnemy(layer, "enemigos/"+ thisGroup->nombres[level][x], VisibleSize, thisGroup->nombres[level]);
                thisGroup->Enemygroup.push_back(e);
            }
            return thisGroup;
        }   
        std::tuple<int, int> actualizarPuntaje(int signo ) {
            int puntos = 0; 
            int eliminados = 0;
            for (int i = 0; i <Enemygroup.size() ; i++)
            {
                
                int x = Enemygroup[i]->actualizar(signo, nombres[level]);
                if (x == 1) {
                    puntos++;
                }else if(x == 2) {
                    puntos++;
                    eliminados++;
                }
                                              
            }
            return std::make_tuple(puntos, eliminados);
        }
        
        void setMaxLives(int n ) {
            for (int i = 0; i < Enemygroup.size(); i++)
            {
                Enemygroup[i]->setmaxLives(n);
                Enemygroup[i]->reiniciar();
            }
        }
    };
    class lives : public cocos2d::Node
    {
    private:
        int livesnum = 3;
        
        std::vector<cocos2d::Sprite*> sp;
        std::vector<bool> l;
        std::string typeOfLive = "corazon.png";
        cocos2d::Size visibleSize;
        cocos2d::Size maxy ;
        void createLivesFirst() {
            this->removeAllChildren();
            sp.clear();
            for (int i = 0; i < livesnum; i++) {
                l.push_back(1);
                auto x = cocos2d::Sprite::create(typeOfLive);               
                x->setScale(getNewScale(x->getBoundingBox().size));
                auto y = x->getBoundingBox().size;
                maxy = x->getBoundingBox().size;
                x->setPosition(y.width * i+ y.width/2, y.height/2);
                this->sp.push_back(x);
                this->addChild(x);
            }
            this->setContentSize(cocos2d::Size(maxy.width * livesnum, maxy.height));
        }
    public:
        static lives* createLives(cocos2d::Scene* layer, cocos2d::Size visibleSize) {
            auto thislives = new (std::nothrow) lives;
            if (thislives && thislives->init()) {
                thislives->visibleSize = visibleSize;
                thislives->createLivesFirst();
                thislives->setContentSize(cocos2d::Size(thislives->maxy.width* thislives->livesnum, thislives->maxy.height));
                thislives->setAnchorPoint(cocos2d::Vec2(1, 0));
                layer->addChild(thislives, 100);
                thislives->setAnchorPoint(cocos2d::Vec2(0, 1));
                return thislives;
            }
            CC_SAFE_RELEASE(thislives);
            return nullptr;
        }
        int numberOfLives() {
            int c = 0;
            for (int i = 0; i < livesnum; i++) {
                if (l[i] == 1) {
                    c++;
                }
            }
            return c;
        }
        bool alive() {
            if (this->numberOfLives() > 0) {
                return true;
            }
            else {
                return false;
            }
        }
        void quitlive() {
            for (int i = 0; i < livesnum; i++) {
                if (l[i] == 1) {
                    l[i] = 0;
                    sp[i]->setOpacity(0);
                    break;
                }
            }
        }
        void restartLives() {
            for (int i = 0; i < livesnum; i++) {
                sp[i]->setOpacity(1000);
                l[i] = 1;
            }
        }
        void setNumberOfLives(int n) {
            this->livesnum = n;
            this->createLivesFirst();
            this->restartLives();
        }
        void setTypeofLives(std::string s) {
            this->typeOfLive = s;
            this->createLivesFirst();
        }
        float getNewScale(cocos2d::Size x) {
            auto j = x.width;
            return((visibleSize.width * .06 / j));
        }
    };
    class Scoreboard : public cocos2d::Node
    {
    public:
        cocos2d::Label* scoreL;
        cocos2d::Label* deadEnemiesL;
        cocos2d::Label* deathsL;
        cocos2d::Sprite* scoreboard;
        int score = 0;
        int deadEnemies = 0;
        int deaths = 0;

        static Scoreboard* createScores(cocos2d::Scene* layer) {
            auto thisScoreBoard = new (std::nothrow) Scoreboard;
            if (thisScoreBoard && thisScoreBoard->init()) {
                thisScoreBoard->scoreL = cocos2d::Label::createWithSystemFont("Score: 0", "Arial Black", 30);
                thisScoreBoard->deadEnemiesL = cocos2d::Label::createWithSystemFont("Dead Enemies: 0", "Arial Black", 30);
                thisScoreBoard->deathsL = cocos2d::Label::createWithSystemFont("Deaths: 0", "Arial Black", 30);
                auto x = thisScoreBoard->scoreL->getBoundingBox().size;
                //scoreboard backround
                thisScoreBoard->scoreboard = cocos2d::Sprite::create("tasks/scores.png");
                thisScoreBoard->addChild(thisScoreBoard->scoreboard);
                thisScoreBoard->scoreboard->setLocalZOrder(-10);
                thisScoreBoard->scoreboard->setScale(.6);
                
                // se scores position
                thisScoreBoard->scoreL->setPosition(0, 0);
                thisScoreBoard->deadEnemiesL->setPosition(0, x.height);
                thisScoreBoard->deathsL->setPosition(0, 2 * x.height);
                // set scores aligment
                thisScoreBoard->scoreL->setAnchorPoint(cocos2d::Vec2(0, 1));
                thisScoreBoard->deadEnemiesL->setAnchorPoint(cocos2d::Vec2(0, 1));
                thisScoreBoard->deathsL->setAnchorPoint(cocos2d::Vec2(0, 1));
                thisScoreBoard->setContentSize(cocos2d::Size(x.width*2.5, 2.5 * x.height));
                thisScoreBoard->setAnchorPoint(cocos2d::Vec2(1,1));
                thisScoreBoard->scoreboard->setAnchorPoint(cocos2d::Vec2(.15, .4));
                // add scorelaves to scoreboard
                thisScoreBoard->addChild(thisScoreBoard->scoreL);
                thisScoreBoard->addChild(thisScoreBoard->deadEnemiesL);
                thisScoreBoard->addChild(thisScoreBoard->deathsL);
                layer->addChild(thisScoreBoard, 100);
                cocos2d::log("dimensionde %d %d", thisScoreBoard->getContentSize().width, thisScoreBoard->getContentSize().height);
                return thisScoreBoard;
            }
            CC_SAFE_RELEASE(thisScoreBoard);
            return nullptr;
        }
        void increaseScore(int n ) {
            this->score += n;
            this->scoreL->setString("Score: "+std::to_string(score));
        }
        void increasedeadEnemies(int n ) {
            this->deadEnemies += n;
            this->deadEnemiesL->setString("Dead Enemies: " + std::to_string(deadEnemies));
        }
        void increaseDeaths() {
            this->deaths += 1;
            this->deathsL->setString("Deaths: " + std::to_string(deaths));
        }
        void resetScores() {
            this->deadEnemiesL->setString("Dead Enemies: 00");
            this->deathsL->setString("Deaths: 00 ");
        }
        void update() {
            this->deathsL->setString("Deaths: " + std::to_string(deaths));
            this->deadEnemiesL->setString("Dead Enemies: " + std::to_string(deadEnemies));
            this->scoreL->setString("Score: " + std::to_string(score));
        }
    };
    class customPlayer : public cocos2d::Node
    {
    public:
        cocos2d::Label* Name;
        cocos2d::Sprite *Skin;
        cocos2d::Size visibleSize;
        int actualSkin = 0;
        std::vector<std::string> nombresSkins = { "default.png","sticker02.png" ,"sticker10.png","gatocris.png"};
        std::vector<std::pair<float, float>> posiciones;
        static customPlayer* createPlayer(cocos2d::Scene* layer, cocos2d::Size visibleSize) {
            auto thisPlayer = new (std::nothrow) customPlayer;
            if (thisPlayer && thisPlayer->init()) {
                thisPlayer->visibleSize = visibleSize;
                thisPlayer->Name = cocos2d::Label::createWithSystemFont("player 1", "Arial Black", 15);      
                
                auto pinfo = cocos2d::AutoPolygon::generatePolygon("player/default.png");
                thisPlayer->Skin = cocos2d::Sprite::create(pinfo);
                thisPlayer->Skin->setScale(thisPlayer->getNewScale());
                thisPlayer->addChild(thisPlayer->Name);
                thisPlayer->addChild(thisPlayer->Skin);
                layer->addChild(thisPlayer, 0);
                auto x = thisPlayer->Skin->getBoundingBox().size;    
                thisPlayer->Name->setPosition(0,x.height/2+10);
                thisPlayer->setContentSize(cocos2d::Size(x.width ,x.height));                
                return thisPlayer;
            }
            CC_SAFE_RELEASE(thisPlayer);
            return nullptr;
        }        
        // personalizar
        void setName(std::string s) {
            this->Name->setString(s);           
        }
        void setSkin(std::string s) {
            auto pinfo = cocos2d::AutoPolygon::generatePolygon(s);
            this->removeChild(this->Skin);
            this->Skin = cocos2d::Sprite::create(pinfo);
            this->Skin->setScale(this->getNewScale());
            this->addChild(this->Skin);
            auto x = this->Skin->getBoundingBox().size;
            this->setContentSize(cocos2d::Size(x.width, x.height));

            
        }
        void setSkin(int x) {
            setSkin("player/" + nombresSkins[x]);
        }
        void nextSkin() {
            actualSkin++;
            actualSkin = actualSkin % nombresSkins.size();
            setSkin("player/" + nombresSkins[actualSkin]);
        }
        // jugabilidad
        void inicioDeMovimiento(cocos2d::Touch* toque, cocos2d::Event* Evento) {

            auto posicion = toque->getStartLocation();
            cocos2d::log("inicio %f %f", posicion.x, posicion.y);
            for (int i = 0; i < 6; i++)
            {
                posiciones[i].first = posicion.x;
                posiciones[i].second = posicion.y;
            }            
        }
        void finDeMovimiento(cocos2d::Touch* toque, cocos2d::Event* Evento) {
            auto posicion = toque->getLocation();
            // cocos2d::log("fin %f %f", posicion.x, posicion.y);
            posiciones[1].first = posicion.x;
            posiciones[1].second = posicion.y;
        }
        void enMovimiento(cocos2d::Touch* toque, cocos2d::Event* Evento) {
            auto posicion = toque->getLocation();

            if (posicion.x < posiciones[2].first) {
                posiciones[2].first = posicion.x;
                posiciones[2].second = posicion.y;
                //  log("menor x ");
            }
            //maximo derecha respecto a inicio
            else if (posicion.x > posiciones[3].first) {
                posiciones[3].first = posicion.x;
                posiciones[3].second = posicion.y;
                //log("mayor x ");
            }
            //maximo arriba respecto a inicio
            if (posicion.y > posiciones[4].second) {
                posiciones[4].first = posicion.x;
                posiciones[4].second = posicion.y;
                //log("mayor y ");
            }
            //maximo abajo  respecto a inicio
            else if (posicion.y < posiciones[5].second) {
                posiciones[5].first = posicion.x;
                posiciones[5].second = posicion.y;
                //log("menor y ");
            }
        }
        int evaluarMovimiento() {
            auto inicio = posiciones[0];
            auto fin = posiciones[1];
            auto maxizquierda = posiciones[2];
            auto maxderecha = posiciones[3];
            auto maxarriba = posiciones[4];
            auto  maxabajo = posiciones[5];
            cocos2d::log("\ninicion %f %f", inicio.first, inicio.second);
            cocos2d::log("\nfin %f %f", fin.first, fin.second);
            cocos2d::log("\nmaxizquierda %f %f", maxizquierda.first, maxizquierda.second);
            cocos2d::log("\nmaxderecha %f %f", maxderecha.first, maxderecha.second);
            cocos2d::log("\nmaxarriba %f %f", maxarriba.first, maxarriba.second);
            cocos2d::log("\nmaxabajo %f %f", maxabajo.first, maxabajo.second);

            int x = 0;
            if ((inicio.first < fin.first && inicio.second < maxarriba.second && fin.second < maxarriba.second) || (inicio.first > fin.first && inicio.second < maxarriba.second && fin.second < maxarriba.second)) {
                cocos2d::log("casita ^");
                x = 1;
            }
            else if ((inicio.first < fin.first && inicio.second > maxabajo.second && fin.second > maxabajo.second) || (inicio.first > fin.first && inicio.second > maxabajo.second && fin.second > maxabajo.second)) {
                cocos2d::log("casita invertida v");
                x = 2;
            }
            else if ((inicio.first < maxderecha.first && fin.first < maxderecha.first && inicio.second>fin.second) || (inicio.first < maxderecha.first && fin.first < maxderecha.first && inicio.second < fin.second)) {
                cocos2d::log("mayorque >");
                x = 3;
            }
            else if ((inicio.first > maxizquierda.first && fin.first > maxizquierda.first && inicio.second > fin.second) || (inicio.first > maxizquierda.first && fin.first > maxizquierda.first && inicio.second < fin.second)) {
                cocos2d::log("menorque <");
                x = 4;
            }
            else if ((inicio.first > fin.first && inicio.second > fin.second) || (inicio.first < fin.first && inicio.second < fin.second)) {
                cocos2d::log("diuagonal /");
                x = 5;
            }
            else if ((inicio.first < fin.first && inicio.second > fin.second) || (inicio.first > fin.first && inicio.second < fin.second)) {
                cocos2d::log("diuagonalinvertida  \\");
                x = 6;
            }
            return x;

        }
        float getNewScale() {
            auto j = Skin->getBoundingBox().size.width;
            return((visibleSize.width * .1 / j));
        }
        int onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode) {
            auto x = this->getBoundingBox().size;
            cocos2d::log("bounding playert %f %f", x.width, x.height);
            cocos2d::Vec2 loc = this->getPosition();
            switch (keyCode) {
            case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
                //this->setPosition(loc.x - x.width / 2, loc.y);
                return 0;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
                //this->setPosition(loc.x + x.width / 2, loc.y);
                return 1;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
            case cocos2d::EventKeyboard::KeyCode::KEY_W:
                //this->setPosition(loc.x, loc.y + x.height / 2);
                return 2;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
                //this->setPosition(loc.x, loc.y - x.height / 2);
                return 3;
                break;
            }
            return 4;
        }

        int onKeyRelease(cocos2d::EventKeyboard::KeyCode keyCode) {
            auto x = this->getBoundingBox().size;
            cocos2d::log("bounding playert %f %f", x.width, x.height);
            cocos2d::Vec2 loc = this->getPosition();
            switch (keyCode) {
            case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
                //this->setPosition(loc.x - x.width / 2, loc.y);
                return 0;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
                //this->setPosition(loc.x + x.width / 2, loc.y);
                return 1;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
            case cocos2d::EventKeyboard::KeyCode::KEY_W:
                //this->setPosition(loc.x, loc.y + x.height / 2);
                return 2;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
                //this->setPosition(loc.x, loc.y - x.height / 2);
                return 3;
                break;
            }
            return 4;
        }
    };
    class Backround : public cocos2d::Node
    {
    
        
    public:
        cocos2d::Sprite* image;
        cocos2d::Size visibleSize;
        std::vector<std::string> nombrespred = { "1.jpg","mall.jpg","playa.jpg","piramide.jpg","espacio.jpg","menuinicio.jpg","score.jpg","settings.jpg" };
        std::vector<std::vector<std::string>> nombres = {
            {"patio/1.jpg","patio/2.jpg","patio/3.jpg","patio/4.jpg","patio/5.jpg","patio/6.jpg"},
            {"mall/1.jpg","mall/2.jpg","mall/3.jpg","mall/4.jpg","mall/5.jpg","mall/6.jpg","mall/7.jpg","mall/8.jpg","mall/9.jpg"},
            {"mar/1.jpg","mar/2.jpg","mar/3.jpg","mar/4.jpg"},            
            {"ejipto/1.jpg","ejipto/2.jpg","ejipto/3.jpg","ejipto/4.jpg","ejipto/5.jpg","ejipto/6.jpg"},             
            {"cementerio/1.jpg","cementerio/2.jpg","cementerio/3.jpg","cementerio/4.jpg","cementerio/5.jpg","cementerio/6.jpg","cementerio/7.png","cementerio/8.jpg","cementerio/9.jpg","cementerio/10.jpg","cementerio/11.jpg"},
            {"espacio/1.jpg","espacio/2.jpg","espacio/3.jpg","espacio/4.jpg","espacio/5.jpg","espacio/6.jpg"}
        };
        int aumento = 0;
        static Backround* createBakground(cocos2d::Scene* layer, cocos2d::Size visibleSize,int incre) {
            auto thisBackground = new (std::nothrow) Backround;
            if (thisBackground && thisBackground->init()) {
                thisBackground->image = cocos2d::Sprite::create("fondos/1.jpg");
                thisBackground->addChild(thisBackground->image);
                thisBackground->visibleSize = visibleSize;
                thisBackground->setPosition(thisBackground->visibleSize.width / 2, thisBackground->visibleSize.height / 2);
                thisBackground->image->setScale(thisBackground->getNewScale(incre));
                layer->addChild(thisBackground, 100);
                return thisBackground;
            }
            CC_SAFE_RELEASE(thisBackground);
            return nullptr;
        }       
        std::string getbackgroundName(int i) {
            int x = nombres[i].size();
            int r = rand() % x;
            return nombres[i][r];
        }
        void setNewBackground(int x,int scale) {            
            this->removeChild(this->image);
            this->image= cocos2d::Sprite::create("fondos/"+ getbackgroundName(x));
            this->image->setScale(this->getNewScale(scale));
            this->addChild(this->image);            
        }
        void setpredefBackground(int x,int scale) {
            this->removeChild(this->image);
            this->image = cocos2d::Sprite::create("fondos/" + nombrespred[x]);
            this->image->setScale(this->getNewScale(scale));
            this->addChild(this->image);
        }
        void moveUp() {
            auto x = this->image->getScale();
            if (this->aumento < 1) {
                this->image->setScale(x+.01);
                this->aumento += .01;
            }
        }
        void moveDown() {
            auto x = this->image->getScale();
            if (this->aumento >=0 ) {
                this->image->setScale(x - .01);
                this->aumento -= .01;
            }
        }
        void moveLeft() {
            auto x = this->getPosition();
            if (x.x >= 0) {
                this->setPositionX(x.x - 1);
            }
        }
        void moveRight() {
            auto x = this->getPosition();
            if (x.x >= 0) {
                this->setPositionX(x.x +1);
            }
        }
        float getNewScale(int incre) {
            float j = visibleSize.width;            
            auto tam = image->getContentSize().width;
            
            if (tam <=  j) {
                return j / tam+incre;
            }
            else {
                cocos2d::log("tamalos %f %f", j, tam);
                cocos2d::log("tamalos %f %f", tam / j + incre, tam / j );
                return j/tam+ incre;
            }            
        }

    };
    class ScoreFile {
    public:
        std::string filename = "score.txt";
        std::string path;
        cocos2d::FileUtils *fileUtils;
        ScoreFile() {
            fileUtils = cocos2d::FileUtils::getInstance();            
            path = fileUtils->getWritablePath() + filename;
            cocos2d::log("%s", path.c_str());
        }
        void addscore(int d, int e, int s,std::string nombre) {
            std::string text = fileUtils->getStringFromFile(path);            
            std::string newText = nombre + " deaths: "+std::to_string(d)+"______kills: "+ std::to_string(e)+"______score: " +std::to_string(s)+"\n";
            fileUtils->writeStringToFile(newText+ text , path);
        }
        std::string getString() {
            std::string text = fileUtils->getStringFromFile(path);
            return text;
        }
    };
    class SettingFile {
    public:
        std::string filename = "settings.txt";
        std::string path;
        cocos2d::FileUtils* fileUtils;
        SettingFile() {
            fileUtils = cocos2d::FileUtils::getInstance();
            path = fileUtils->getWritablePath() + filename;
        }
        void setSettings(int l, int d, int e, int s,int sk,  std::string nombre, int dif) {
            std::string text = nombre + " " + std::to_string(l)+" "+std::to_string(d) +" "+ std::to_string(e) +" " +std::to_string(s)  +" " + std::to_string(sk) + " " + std::to_string(dif);
            fileUtils->writeStringToFile(text, path);
        }
        std::vector <std::string> GetSettings() {
            auto x = getString();
            std::stringstream check1(x);
            std::vector <std::string> tokens;            
            std::string intermediate;
            while (getline(check1, intermediate, ' '))
            {
                tokens.push_back(intermediate);
            }
            return tokens;
        }
        std::string getname() {
            auto x = GetSettings();
            return x[0];
        }
        int getl() {
            auto x = GetSettings();
            return stoi(x[1]);
        }
        int getd() {
            auto x = GetSettings();
            return stoi(x[2]);
        }
        int gete() {
            auto x = GetSettings();
            return stoi(x[3]);
        }
        int gets() {
            auto x = GetSettings();
            return stoi(x[4]);
        }
        int getsk() {
            auto x = GetSettings();
            return stoi(x[5]);
        }
        int getdif() {
            auto x = GetSettings();
            return stoi(x[6]);
        }
        std::string getString() {
            std::string text = fileUtils->getStringFromFile(path);
            return text;
        }
    };
    class Sound {
    public:
        std::vector<std::string> sonidos = {"inicio.mp3", "pokemon.mp3","roky.mp3","pokemon2.mp3","zelda.mp3" };
        int  audio = cocos2d::AudioEngine::INVALID_AUDIO_ID;
        Sound() {}
        Sound(int x ) {
            cocos2d::log("sonido creadop %d", x);
            audio = cocos2d::AudioEngine::play2d("sound/"+sonidos[x]);
        }
        void stopSound() {
            cocos2d::AudioEngine::pause(audio);
        }
        void playSound(int x ) {
            
            cocos2d::AudioEngine::play2d("sound/" + sonidos[x],true);            
        }
        void playone(int x) {
            cocos2d::AudioEngine::play2d("sound/" + sonidos[x]);
        }
        void stopall() {
            cocos2d::AudioEngine::stopAll();
            //cocos2d::AudioEngine::end();
            cocos2d::AudioEngine::uncacheAll();
            cocos2d::AudioEngine::end();
        }
        ~Sound() {}
    };
    
};
#endif 