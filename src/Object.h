#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>

struct Player
{
     SDL_Texture* texture = nullptr;
     SDL_FPoint position = {0, 0};
     int width = 0;
     int height = 0;
     int speed = 200;
     Uint32 coolDown = 500;
     Uint32 lastShot = 0;
};
struct Enemy
{
     SDL_Texture* texture = nullptr;
     SDL_FPoint position = {0, 0};
     int width = 0;
     int height = 0;
     int speed = 100;
     int health = 2;
     Uint32 coolDown = 1000;
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
};


#endif // OBJECT_H