#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include <SDL.h>

class Game
{
public:
   //单例
    static Game& getInstance(){
        static Game instance;
        return instance;
    };

    ~Game();
    void run();
    void init();
    void clean();
    void changeScene(Scene* scene);

    void handleEvent(SDL_Event* event);
    void update(float deltaTime);
    void render();

    SDL_Renderer* getRenderer() { return renderer; }
    SDL_Window* getWindow() { return window; }
    int getWindowWidth() { return windowWidth; }
    int getWindowHeight() { return windowHeight; }
private:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    bool isRunning = true; // C++11 新特性初始化
    Scene* currentScene = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int windowWidth = 300;
    int windowHeight = 400;
    //FPS
    int FPS = 60;
    Uint32 frameTime = 1000 / FPS;
    float deltaTime = 0;
};
#endif // GAME_H
