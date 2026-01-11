#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"

#include <list>
#include <map>
#include <random>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class SceneMain : public Scene
{
public:
    SceneMain();
    ~SceneMain();

    void update(float deltaTime) override;
    void render() override;
    void handleEvent(SDL_Event *event) override;
    void init() override;
    void clean() override;



    
private:
    Player player;
    TTF_Font *scoreFont;
    int score = 0;
    float timerEnd = 0;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    ProjectilePlayer projectilePlayerTemp;
    std::list<ProjectilePlayer*> projectilesPlayer;

    Enemy enemyTemp;
    std::list<Enemy*> enemies;

    ProjectileEnemy projectileEnemyTemp;
    std::list<ProjectileEnemy*> projectilesEnemy;

    //爆炸帧动画
    Explosion explosionTemp;
    std::list<Explosion*> explosions;
    //道具
    Item healthItemTemp;
    std::list<Item*> items;
    //声音容器
    std::map<std::string, Mix_Chunk*> soundMap;

    //血量
    SDL_Texture* healthBar;
    //渲染
    void renderProjectiles();
    void renderEnemies();
    void renderProjectileEnemy();
    void renderExplosions();
    void renderItems();
    
    void renderUI();
    //更新
    void generateEnemies();
    void updateProjectiles(float deltaTime);
    void updateEnemies(float deltaTime);
    void updateProjectileEnemy(float deltaTime);
    void updateExplosions();
    void updateItems(float deltaTime);

    void keyboardControl(float deltaTime);
    void shoot();
    void shootEnemy(Enemy *enemy);
    
    void changeSeceneDelayed(float deltaTime);

};
#endif // SCENE_MAIN_H