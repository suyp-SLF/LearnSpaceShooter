#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneEnd.h"
#include "Game.h"

#include <SDL_image.h>
#include <random>

SceneMain::SceneMain()
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
    updateExplosions();      // 更新爆炸动画
    updateItems(deltaTime);           // 更新道具

    generateEnemies();                // 随机生成敌方
    if(player.isDead){
        changeSeceneDelayed(deltaTime);
    }
}

void SceneMain::render()
{
    renderProjectiles();     // 渲染玩家子弹
    renderProjectileEnemy(); // 渲染敌方子弹
    renderExplosions();      // 渲染爆炸动画
    renderItems();           // 渲染道具
    renderEnemies();         // 渲染敌机
    renderUI();              // 渲染血条
    // 渲染玩家
    SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
}

void SceneMain::handleEvent(SDL_Event *event)
{
    //按esc切到title场景
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE)
    {
        game.changeScene(new SceneTitle());
    }
}

void SceneMain::init()
{
    // 随机数
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<>(0, 1);

    // 载入字体
    scoreFont = TTF_OpenFont("assets/font/VonwaonBitmap-12px.ttf", 24);

    // 初始化玩家
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    player.currentHealth = player.maxHealth;
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height - 10;

    // 初始化子弹模版
    projectilePlayerTemp.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-2.png");
    SDL_QueryTexture(projectilePlayerTemp.texture, NULL, NULL, &projectilePlayerTemp.width, &projectilePlayerTemp.height);
    projectilePlayerTemp.width /= 4;
    projectilePlayerTemp.height /= 4;

    // 初始化敌人模版
    enemyTemp.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-1.png");
    SDL_QueryTexture(enemyTemp.texture, NULL, NULL, &enemyTemp.width, &enemyTemp.height);
    enemyTemp.width /= 4;
    enemyTemp.height /= 4;

    // 初始化敌方子弹模版
    projectileEnemyTemp.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-2.png");
    SDL_QueryTexture(projectileEnemyTemp.texture, NULL, NULL, &projectileEnemyTemp.width, &projectileEnemyTemp.height);
    projectileEnemyTemp.width /= 4;
    projectileEnemyTemp.height /= 4;

    // 初始化爆炸动画模版
    explosionTemp.texture = IMG_LoadTexture(game.getRenderer(), "assets/effect/explosion.png");
    SDL_QueryTexture(explosionTemp.texture, NULL, NULL, &explosionTemp.width, &explosionTemp.height);
    explosionTemp.currentFrame = 0;
    explosionTemp.frameCount = 9;

    // 初始化道具模版
    healthItemTemp.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bonus_life.png");
    SDL_QueryTexture(healthItemTemp.texture, NULL, NULL, &healthItemTemp.width, &healthItemTemp.height);
    healthItemTemp.width /= 4;
    healthItemTemp.height /= 4;
    healthItemTemp.lifeTime = 10000;
    healthItemTemp.type = HEALTH;
    // 初始化声音map
    soundMap.insert(std::make_pair("playerShoot", Mix_LoadWAV("assets/sound/laser_shoot4.wav")));
    soundMap.insert(std::make_pair("enemyShoot", Mix_LoadWAV("assets/sound/xs_laser.wav")));
    soundMap.insert(std::make_pair("explosion", Mix_LoadWAV("assets/sound/explosion1.wav")));
    soundMap.insert(std::make_pair("item", Mix_LoadWAV("assets/sound/eff5.wav")));
    soundMap.insert(std::make_pair("hurt", Mix_LoadWAV("assets/sound/eff11.wav")));
    // 血量UI
    healthBar = IMG_LoadTexture(game.getRenderer(), "assets/image/Health UI Black.png");
    // SDL_QueryTexture(healthBar, NULL, NULL, NULL, NULL);
}

void SceneMain::clean()
{
    // 清理容器
    for (auto projectile : projectilesPlayer)
    {
        if (projectile != nullptr)
        {
            delete projectile; // 玩家子弹
        }
    }
    for (auto enemy : enemies)
    {
        if (enemy != nullptr)
        {
            delete enemy; // 敌人
        }
    }
    for (auto projectile : projectilesEnemy)
    {
        if (projectile != nullptr)
        {
            delete projectile; // 敌方子弹
        }
    }
    for (auto explosion : explosions)
    {
        if (explosion != nullptr)
        {
            delete explosion; // 爆炸动画
        }
    }
    for (auto item : items)
    {
        if (item != nullptr)
        {
            delete item; // 道具
        }
    }
    // 清理声音
    for (auto sound : soundMap)
    {
        Mix_FreeChunk(sound.second);
    }
    soundMap.clear();
    // 清理背景

    // 清理模版
    if (projectilePlayerTemp.texture != nullptr)
    {
        SDL_DestroyTexture(projectilePlayerTemp.texture); // 玩家子弹
    }
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture); // 玩家
    }
    if (enemyTemp.texture != nullptr)
    {
        SDL_DestroyTexture(enemyTemp.texture); // 敌人
    }
    if (projectileEnemyTemp.texture != nullptr)
    {
        SDL_DestroyTexture(projectileEnemyTemp.texture); // 敌方子弹
    }
    if (explosionTemp.texture != nullptr)
    {
        SDL_DestroyTexture(explosionTemp.texture); // 爆炸动画
    }
    if (healthItemTemp.texture != nullptr)
    {
        SDL_DestroyTexture(healthItemTemp.texture); // 道具
    }
    // 清理healthBar
    if (healthBar != nullptr)
    {
        SDL_DestroyTexture(healthBar);
    }
    // 清理字体
    TTF_CloseFont(scoreFont);
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
    // 播放玩家射击音效
    Mix_PlayChannel(0, soundMap["playerShoot"], 0);
    // 发射子弹
    auto projectile = new ProjectilePlayer(projectilePlayerTemp);
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y = player.position.y;
    projectilesPlayer.push_back(projectile);
}
void SceneMain::shootEnemy(Enemy *enemy)
{
    // 播放敌机射击音效
    Mix_PlayChannel(-1, soundMap["enemyShoot"], 0);
    // 向玩家方向发射子弹
    auto projectile = new ProjectileEnemy(projectileEnemyTemp);
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y = enemy->position.y + enemy->height;
    // 根据xy位置获得敌方到玩家的归一向量
    projectile->velocity.x = player.position.x - enemy->position.x;
    projectile->velocity.y = player.position.y - enemy->position.y;
    auto length = sqrt(projectile->velocity.x * projectile->velocity.x + projectile->velocity.y * projectile->velocity.y);
    projectile->velocity.x /= length;
    projectile->velocity.y /= length;

    projectilesEnemy.push_back(projectile);
}
void SceneMain::changeSeceneDelayed(float deltaTime)
{
    timerEnd += deltaTime;
    //超过3秒调用changeScene
    if (timerEnd > 3)
    {
        game.setScore(score);
        game.changeScene(new SceneEnd());
    }
}
void SceneMain::updateProjectiles(float deltaTime)
{
    if (player.isDead)
    {
        return;
    }
    // 移动子弹
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();)
    {
        auto projectile = *it;
        projectile->position.y -= projectile->speed * deltaTime;

        bool projectileRemoved = false;

        // 检查超过屏幕
        if (projectile->position.y < -projectile->height)
        {
            delete projectile;
            it = projectilesPlayer.erase(it);
            SDL_Log("子弹移除！");
            continue; // 已经处理删除，继续下一个
        }

        // 子弹碰撞到敌机
        for (auto e_it = enemies.begin(); e_it != enemies.end();)
        {
            auto enemy = *e_it;

            // 创建矩形用于碰撞检测
            SDL_Rect projectileRect = {
                static_cast<int>(projectile->position.x),
                static_cast<int>(projectile->position.y),
                projectile->width,
                projectile->height};

            SDL_Rect enemyRect = {
                static_cast<int>(enemy->position.x),
                static_cast<int>(enemy->position.y),
                enemy->width,
                enemy->height};

            // 检查碰撞
            if (SDL_HasIntersection(&projectileRect, &enemyRect))
            {
                enemy->health -= projectile->damage;

                // 删除子弹
                delete projectile;
                it = projectilesPlayer.erase(it);
                projectileRemoved = true;

                // 检查敌机生命值
                if (enemy->health <= 0)
                {
                    // 得分
                    score += 10;
                    // 播放敌机爆炸音效
                    Mix_PlayChannel(-1, soundMap["explosion"], 0);
                    // 输出敌机死亡
                    SDL_Log("敌机死亡！");
                    // 播放爆炸
                    Explosion *explosion = new Explosion(explosionTemp);
                    explosion->position = enemy->position;
                    explosions.push_back(explosion);
                    // 50%概率掉落道具
                    if (dis(rd) > 0.5)
                    {
                        auto item = new Item(healthItemTemp);
                        // 获得随机的归一向量，
                        item->velocity.x = dis(rd) - 0.5;
                        item->velocity.y = dis(rd) - 0.5;
                        item->position = enemy->position;
                        items.push_back(item);
                    }
                    delete enemy;
                    e_it = enemies.erase(e_it);
                }
                else
                {
                    ++e_it;
                }

                break; // 子弹已销毁，跳出敌机循环
            }
            else
            {
                ++e_it;
            }
        }

        if (!projectileRemoved)
        {
            ++it;
        }
    }
}
void SceneMain::updateEnemies(float deltaTime)
{
    if (player.isDead)
    {
        return;
    }
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
    if (player.isDead)
    {
        return;
    }

    // 移动敌机子弹
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();)
    {
        int margin = 40;
        auto projectile = *it;
        projectile->position.x += projectile->velocity.x * projectile->speed * deltaTime;
        projectile->position.y += projectile->velocity.y * projectile->speed * deltaTime;

        bool projectileRemoved = false;

        // 检查超过屏幕边界（包含margin）
        if (projectile->position.y > game.getWindowHeight() + projectile->height + margin ||
            projectile->position.x > game.getWindowWidth() + projectile->width + margin ||
            projectile->position.x < -projectile->width - margin ||
            projectile->position.y < -projectile->height - margin)
        {
            delete projectile;
            it = projectilesEnemy.erase(it);
            SDL_Log("敌机子弹移除！");
            continue; // 已经删除，继续下一个
        }

        // 获取子弹矩形
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height};

        // 获取玩家矩形
        SDL_Rect playerRect = {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height};

        // 检测玩家是否被击中
        if (SDL_HasIntersection(&projectileRect, &playerRect))
        {
            // 玩家受伤音效
            Mix_PlayChannel(-1, soundMap["hurt"], 0);
            player.currentHealth -= projectile->damage;

            // 删除子弹
            delete projectile;
            it = projectilesEnemy.erase(it);
            projectileRemoved = true;

            SDL_Log("玩家被击中！剩余血量为%d", player.currentHealth);

            // 检查玩家是否死亡
            if (player.currentHealth <= 0)
            {
                player.isDead = true;
                SDL_Log("玩家死亡！");
                Explosion *explosion = new Explosion(explosionTemp);
                explosion->position = player.position;
                explosions.push_back(explosion);
            }
        }

        // 如果子弹没有被移除，递增迭代器
        if (!projectileRemoved)
        {
            ++it;
        }
    }
}
void SceneMain::updateExplosions()
{
    uint32_t ticks = SDL_GetTicks();
    for (auto it = explosions.begin(); it != explosions.end();)
    {
        // 增加一个简单的计时器在结构体中，或者使用 deltaTime
        if (ticks - (*it)->lastFrame > 100)
        {
            (*it)->currentFrame++;
            (*it)->lastFrame = ticks;

            if ((*it)->currentFrame >= (*it)->frameCount)
            {
                delete *it; // 如果不使用对象池，至少保证这里没有繁重的逻辑
                it = explosions.erase(it);
                continue;
            }
        }
        ++it;
    }
}
void SceneMain::updateItems(float deltaTime)
{
    // 移动道具
    for (auto it = items.begin(); it != items.end();)
    {
        auto item = *it;
        // 玩家矩形
        SDL_Rect playerRect = {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height};
        // 物品矩形
        SDL_Rect itemRect = {
            static_cast<int>(item->position.x),
            static_cast<int>(item->position.y),
            item->width,
            item->height};
        // 判断是否与玩家重合
        if (SDL_HasIntersection(&itemRect, &playerRect))
        {
            // 处理道具被拾取的逻辑
            // SDL播放物品音效
            Mix_PlayChannel(-1, soundMap["item"], 0);
            // 玩家回复一点血量，最多不超过最大血量
            if (player.currentHealth < player.maxHealth)
            {
                player.currentHealth += 1;
                if (player.currentHealth > player.maxHealth)
                {
                    player.currentHealth = player.maxHealth;
                }
            }
            else
            {
                score += 20; // 满血拾取道具增加分数
            }

            SDL_Log("玩家拾取道具！当前血量为%d", player.currentHealth);
            delete item;
            it = items.erase(it);
            continue;
        }

        // 更新位置
        item->position.x += item->speed * deltaTime * item->velocity.x;
        item->position.y += item->speed * deltaTime * item->velocity.y;

        // --- 处理反弹逻辑 ---

        // 左右边界反弹
        if (item->position.x <= 0)
        {
            item->position.x = 0; // 防止卡在边界外
            item->velocity.x *= -1;
        }
        else if (item->position.x >= game.getWindowWidth() - item->width)
        {
            item->position.x = game.getWindowWidth() - item->width;
            item->velocity.x *= -1;
        }

        // 顶部反弹
        if (item->position.y <= 0)
        {
            item->position.y = 0;
            item->velocity.y *= -1;
        }

        // --- 处理消失逻辑 ---

        // 超过生存时间（假设你在结构体里定义了 spawnTime 和 lifetime）
        // 或者你可以保留原来的底部移除逻辑，但如果要反弹，底部就不该移除
        if (item->position.y > game.getWindowHeight() + item->height)
        {
            delete item;
            it = items.erase(it);
            SDL_Log("道具因超出底部移除！");
        }
        else
        {
            // 检查生命周期（示例）
            // if (SDL_GetTicks() - item->spawnTime > 5000) { /* 删除逻辑 */ }

            ++it;
        }
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
void SceneMain::renderExplosions()
{
    // 根据帧数渲染一部分图片
    for (auto it = explosions.begin(); it != explosions.end(); ++it)
    {
        auto explosion = *it;

        // 1. 计算单帧的宽度
        int singleFrameWidth = explosion->width / explosion->frameCount;

        // 2. srcRect: 决定从纹理的哪个位置“裁剪”
        // x 坐标 = 当前帧(0,1,2..) * 单帧宽度
        SDL_Rect srcRect = {
            explosion->currentFrame * singleFrameWidth,
            0,
            singleFrameWidth,
            explosion->height};

        // 3. dstRect: 决定显示在屏幕的什么位置
        // 宽度也应该是单帧宽度，否则图片会被拉伸
        SDL_Rect dstRect = {
            static_cast<int>(explosion->position.x),
            static_cast<int>(explosion->position.y),
            singleFrameWidth,
            explosion->height};

        SDL_RenderCopy(game.getRenderer(), explosion->texture, &srcRect, &dstRect);
    }
}
void SceneMain::renderItems()
{
    for (auto it = items.begin(); it != items.end();)
    {
        auto item = *it;
        SDL_Rect itemRect = {
            static_cast<int>(item->position.x),
            static_cast<int>(item->position.y),
            item->width,
            item->height};
        SDL_RenderCopy(game.getRenderer(), item->texture, NULL, &itemRect);
        ++it;
    }
}
void SceneMain::renderUI()
{

    // 红心之间间隔
    int heartSpacing = 10;
    // 红心大小
    int heartSize = 20;
    // 红心数量为玩家血量
    for (int i = 0; i < player.maxHealth; i++)
    {
        // 失去的血量用灰色表示
        if (i >= player.currentHealth)
        {
            SDL_Rect heartRect = {heartSpacing + i * heartSize, 10, heartSize, heartSize};
            SDL_SetTextureColorMod(healthBar, 100, 100, 100);
            SDL_RenderCopy(game.getRenderer(), healthBar, NULL, &heartRect);
        }
        else
        {
            SDL_SetTextureColorMod(healthBar, 255, 255, 255);
            SDL_Rect heartRect = {heartSpacing + i * heartSize, 10, heartSize, heartSize};
            SDL_RenderCopy(game.getRenderer(), healthBar, NULL, &heartRect);
        }
    }

    // 渲染ttf得分
    auto text = "得分：" + std::to_string(score);
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderUTF8_Solid(scoreFont, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(game.getRenderer(), surface);
    SDL_Rect textRect = {game.getWindowWidth() - surface->w, 10, surface->w, surface->h};
    SDL_RenderCopy(game.getRenderer(), texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
// 生成敌人
void SceneMain::generateEnemies()
{
    if (player.isDead)
    {
        return;
    }
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
