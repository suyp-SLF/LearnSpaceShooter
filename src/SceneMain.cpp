#include <SDL_image.h>
// 随机数
#include <random>

#include "SceneMain.h"
#include "Game.h"

SceneMain::SceneMain() : game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime);
    updateProjectiles(deltaTime);     // 更新玩家子弹
    updateEnemies(deltaTime);         // 更新敌方
    updateProjectileEnemy(deltaTime); // 更新敌方子弹
    generateEnemies();                // 随机生成敌方
}

void SceneMain::render()
{
    renderProjectiles();     // 渲染玩家子弹
    renderProjectileEnemy(); // 渲染敌方子弹
    renderEnemies();         // 渲染敌机
    // 渲染玩家
    SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
}

void SceneMain::handleEvent(SDL_Event *event)
{
    return;
}

void SceneMain::init()
{
    // 随机数
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<>(0, 1);

    // 初始化玩家
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/PNG/SpaceShip.png");
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height - 10;

    // 初始化子弹模版
    projectilePlayerTemp.texture = IMG_LoadTexture(game.getRenderer(), "assets/PNG/laser-2.png");
    SDL_QueryTexture(projectilePlayerTemp.texture, NULL, NULL, &projectilePlayerTemp.width, &projectilePlayerTemp.height);
    projectilePlayerTemp.width /= 4;
    projectilePlayerTemp.height /= 4;

    // 初始化敌人模版
    enemyTemp.texture = IMG_LoadTexture(game.getRenderer(), "assets/PNG/insect-1.png");
    SDL_QueryTexture(enemyTemp.texture, NULL, NULL, &enemyTemp.width, &enemyTemp.height);
    enemyTemp.width /= 4;
    enemyTemp.height /= 4;

    // 初始化敌方子弹模版
    projectileEnemyTemp.texture = IMG_LoadTexture(game.getRenderer(), "assets/PNG/bullet-2.png");
    SDL_QueryTexture(projectileEnemyTemp.texture, NULL, NULL, &projectileEnemyTemp.width, &projectileEnemyTemp.height);
    projectileEnemyTemp.width /= 4;
    projectileEnemyTemp.height /= 4;
}

void SceneMain::clean()
{
    // 清理容器
    for (auto projectile : projectilesPlayer)
    {
        if (projectile != nullptr)
        {
            delete projectile;
        }
    }
    // 清理模版
    if (projectilePlayerTemp.texture != nullptr)
    {
        SDL_DestroyTexture(projectilePlayerTemp.texture);
    }
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
    }
}

void SceneMain::keyboardControl(float deltaTime)
{
    // SDL_GetKeyboardState(NULL)获得键盘状态
    // keyboardState是一个数组，每个元素对应一个键
    // 如果该键被按下，则对应的元素为1，否则为0

    auto keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState)
    {
        if (keyboardState[SDL_SCANCODE_W])
        {
            player.position.y -= player.speed * deltaTime;
        }
        if (keyboardState[SDL_SCANCODE_S])
        {
            player.position.y += player.speed * deltaTime;
        }
        if (keyboardState[SDL_SCANCODE_A])
        {
            player.position.x -= player.speed * deltaTime;
        }
        if (keyboardState[SDL_SCANCODE_D])
        {
            player.position.x += player.speed * deltaTime;
        }

        // 限制移动范围
        if (player.position.x < 0)
        {
            player.position.x = 0;
        }
        if (player.position.x > game.getWindowWidth() - player.width)
        {
            player.position.x = game.getWindowWidth() - player.width;
        }
        if (player.position.y < 0)
        {
            player.position.y = 0;
        }
        if (player.position.y > game.getWindowHeight() - player.height)
        {
            player.position.y = game.getWindowHeight() - player.height;
        }
        // 子弹发射
        if (keyboardState[SDL_SCANCODE_J])
        {
            // 发射子弹
            auto currentTime = SDL_GetTicks();
            if (currentTime - player.lastShot > player.coolDown)
            {
                shoot();
                player.lastShot = currentTime;
            }
        }
    }
}

void SceneMain::shoot()
{
    // 发射子弹
    auto projectile = new ProjectilePlayer(projectilePlayerTemp);
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y = player.position.y;
    projectilesPlayer.push_back(projectile);
}
void SceneMain::shootEnemy(Enemy *enemy)
{
    // 向玩家方向发射子弹
    auto projectile = new ProjectileEnemy(projectileEnemyTemp);
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y = enemy->position.y + enemy->height;
    //根据xy位置获得敌方到玩家的归一向量
    projectile->velocity.x = player.position.x - enemy->position.x;
    projectile->velocity.y = player.position.y - enemy->position.y;
    auto length = sqrt(projectile->velocity.x * projectile->velocity.x + projectile->velocity.y * projectile->velocity.y);
    projectile->velocity.x /= length;
    projectile->velocity.y /= length;

    projectilesEnemy.push_back(projectile);
}
void SceneMain::updateProjectiles(float deltaTime)
{
    // 移动子弹
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();)
    {
        auto projectile = *it;
        projectile->position.y -= projectile->speed * deltaTime;
        // 检查超过屏幕
        if (projectile->position.y < -projectile->height)
        {
            delete projectile;                // 删除子弹
            it = projectilesPlayer.erase(it); // 删除
            // 输出删除子弹
            SDL_Log("子弹移除！");
        }
        else
        {
            //子弹碰撞到敌机
            for (auto e_it = enemies.begin(); e_it != enemies.end();)
            {
                auto enemy = *e_it;
                //子弹矩形
                SDL_Rect projectileRect = {static_cast<int>(projectile->position.x), 
                                            static_cast<int>(projectile->position.y), 
                                            projectile->width, 
                                            projectile->height};
                //敌机矩形
                SDL_Rect enemyRect = {static_cast<int>(enemy->position.x),
                                    static_cast<int>(enemy->position.y),
                                    enemy->width,
                                    enemy->height};
                // 检查碰撞
                if (SDL_HasIntersection(&projectileRect, &enemyRect))
                {
                    enemy->health -= projectile->damage;
                    delete projectile;                // 删除子弹
                    it = projectilesPlayer.erase(it); // 删除
                    if (enemy->health <= 0){
                        delete enemy;                     // 删除敌机
                        e_it = enemies.erase(e_it);         // 删除
                    }else{
                        ++e_it;
                    }
                    break;
                }else{
                    ++it;
                }
            }
        }
    }
}
void SceneMain::updateEnemies(float deltaTime)
{
    // 移动敌机
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        auto projectile = *it;
        projectile->position.y += projectile->speed * deltaTime;
        // 检查超过屏幕
        if (projectile->position.y > game.getWindowHeight() + projectile->height)
        {
            delete projectile;
            it = enemies.erase(it);
            // 输出删除敌机
            SDL_Log("敌机移除！");
        }
        else
        {
            ++it;
        }
        // 发射子弹
        if (SDL_GetTicks() - projectile->lastShot > projectile->coolDown)
        {
            shootEnemy(projectile);
            projectile->lastShot = SDL_GetTicks();
        }
    }
}
/**
 * 更新敌机子弹的位置并检查是否超出屏幕边界
 * @param deltaTime 增量时间，用于控制移动速度
 */
void SceneMain::updateProjectileEnemy(float deltaTime)
{
    // 移动敌机
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();)
    {
        int margin = 40;
        auto projectile = *it;
        projectile->position.x += projectile->velocity.x * projectile->speed * deltaTime;
        projectile->position.y += projectile->velocity.y * projectile->speed * deltaTime;
        // 检查超过屏幕向外margin大小,四个方向全部校验
        if (projectile->position.y > game.getWindowHeight() + projectile->height + margin ||
            projectile->position.x > game.getWindowWidth() + projectile->width + margin ||
            projectile->position.x < -projectile->width - margin ||
            projectile->position.y < -projectile->height - margin)
        {
            delete projectile;
            it = projectilesEnemy.erase(it); // 删除
            // 输出删除敌机
        }
        ++it;
    }
}
void SceneMain::renderProjectiles()
{
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();)
    {
        auto projectile = *it;
        SDL_Rect projectileRect = {static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);

        ++it;
    }
}
void SceneMain::renderEnemies()
{
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        auto projectile = *it;
        SDL_Rect projectileRect = {static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);

        ++it;
    }
}
void SceneMain::renderProjectileEnemy()
{
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();)
    {
        auto projectile = *it;
        SDL_Rect projectileRect = {static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);

        ++it;
    }
}
// 生成敌人
void SceneMain::generateEnemies()
{
    // 随机在地图上方生成敌人
    if (dis(gen) > (1 / 60.0f))
    {
        return;
    }
    auto enemy = new Enemy(enemyTemp);
    // 在屏幕上方随机位置生成

    enemy->position.x = dis(gen) * game.getWindowWidth();
    enemy->position.y = -enemy->height;
    enemies.push_back(enemy);
}
