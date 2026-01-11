#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
//物品类型枚举
enum ItemType
{
     SPEED,
     HEALTH,
     DAMAGE
};
struct Player
{
     SDL_Texture* texture = nullptr;
     SDL_FPoint position = {0, 0};
     int width = 0;
     int height = 0;
     int speed = 200;
     int currentHealth = 0;
     int maxHealth = 8;
     Uint32 coolDown = 200;
     Uint32 lastShot = 0;
     bool isDead = false;
};
struct Enemy
{
     SDL_Texture* texture = nullptr;
     SDL_FPoint position = {0, 0};
     int width = 0;
     int height = 0;
     int speed = 100;
     int health = 1;
     Uint32 coolDown = 3000;
     Uint32 lastShot = 0;
};
struct ProjectilePlayer
{
     SDL_Texture* texture = nullptr;
     SDL_FPoint position = {0, 0};
     int width = 0;
     int height = 0;
     int speed = 300;
     int damage = 1;
};
struct ProjectileEnemy
{   
     SDL_Texture* texture = nullptr;
     SDL_FPoint position = {0, 0};
     SDL_FPoint velocity = {0, 0};
     int width = 0;
     int height = 0;
     int speed = 300;
     int damage = 1;
};
//爆炸帧动画
struct Explosion
{
     SDL_Texture* texture = nullptr;
     SDL_FPoint position = {0, 0};
     int width = 0;
     int height = 0;
     int currentFrame = 0;
     int frameCount = 0;
     Uint32 lastFrame = 0;
     Uint32 frameDelay = 100;
};
struct Item
{
     SDL_Texture* texture = nullptr;
     SDL_FPoint position = {0, 0};
     //运动方向
     SDL_FPoint velocity = {0, 0};
     //运动速度
     int speed = 300;
     //存在时间
     Uint32 lifeTime = 10000;
     int width = 0;
     int height = 0;
     ItemType type = HEALTH; // 0: health, 1: speed, 2: damage
};
//背景
struct Background
{
     SDL_Texture* texture = nullptr;
     SDL_Point offset = {0, 0};
     int speed = 1;
     int width = 0;
     int height = 0;
};

#endif // OBJECT_H