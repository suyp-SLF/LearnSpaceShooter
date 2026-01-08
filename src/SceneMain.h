#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"

#include <list>
#include <random>

class Game; // forward declaration

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

    void keyboardControl(float deltaTime);
    void shoot();
    void shootEnemy(Enemy *enemy);
    void updateProjectiles(float deltaTime);
    void updateEnemies(float deltaTime);
    void updateProjectileEnemy(float deltaTime);
    void renderProjectiles();
    void renderEnemies();
    void renderProjectileEnemy();
    void generateEnemies();
private:
    Game &game;
    Player player;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    ProjectilePlayer projectilePlayerTemp;
    std::list<ProjectilePlayer*> projectilesPlayer;

    Enemy enemyTemp;
    std::list<Enemy*> enemies;

    ProjectileEnemy projectileEnemyTemp;
    std::list<ProjectileEnemy*> projectilesEnemy;


};
#endif // SCENE_MAIN_H