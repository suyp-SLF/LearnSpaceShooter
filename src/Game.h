#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifndef GAME_H
#define GAME_H

#include "Object.h"
#include "Scene.h"

#include <string>
#include <map>
#include <SDL.h>
#include <SDL_ttf.h>

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
    static void mainLoopWrapper(void* arg);
    void init();
    void clean();
    void changeScene(Scene* scene);

    void handleEvent(SDL_Event* event);
    void update(float deltaTime);
    void render();
    // 其他
    // 中心设置text
    SDL_Point setCenterText(TTF_Font *font, std::string text, float PosY, SDL_Color color);;
    SDL_Point setText(TTF_Font *font, std::string text, float PosX, float PosY, SDL_Color color);

    //getter
    SDL_Renderer* getRenderer() { return renderer; }
    SDL_Window* getWindow() { return window; }
    int getWindowWidth() { return windowWidth; }
    int getWindowHeight() { return windowHeight; }

    void setScore(int score) { this->score = score; };
    //获取分数
    int getScore() {return score; };

    //设置字体
    TTF_Font* fontTitle;
    TTF_Font* fontText;

    //排名
    std::map<int, std::string, std::greater<int>> rank;
private:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    bool isRunning = true; // C++11 新特性
    Scene* currentScene = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int windowWidth = 600;
    int windowHeight = 800;

    //远处背景
    Background farBackground;
    Background nearBackground;
    //FPS
    int FPS = 60;
    Uint32 frameTime = 1000 / FPS;
    float deltaTime = 0;
    //分数,以及getset方法
    int score = 0; //设置分数


    void updateBackground(float deltaTime);
    void renderBackground();
};
#endif // GAME_H
