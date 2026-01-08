#include "Game.h"
#include "SceneMain.h"
#include <SDL.h>
#include <SDL_image.h>

Game::Game()
{
}

Game::~Game()
{
    clean();
}

void Game::run()
{
    while (isRunning)
    {
        Uint32 frameStartTime = SDL_GetTicks();
        SDL_Event event;
        handleEvent(&event);
        update(deltaTime);
        render();
        Uint32 frameEndTime = SDL_GetTicks();
        Uint32 frameDuration = frameEndTime - frameStartTime;
        if (frameDuration < frameTime)
        {
            SDL_Delay(frameTime - frameDuration);
            deltaTime = frameTime / 1000.0f;
        }else
        {
            deltaTime = frameDuration / 1000.0f;
        }
    }
}

void Game::init()
{
    // SDL初始化
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    // 创建窗口
    window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    // IMG初始化
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_ERROR, "IMG_Init Error: %s\n", IMG_GetError());
        isRunning = false;
    }
    //制定当前场景
    currentScene = new SceneMain();
    currentScene ->init();
}

void Game::clean()
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene *scene)
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->init();
}

void Game::handleEvent(SDL_Event *event)
{

    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            isRunning = false;
        }
        currentScene->handleEvent(event);
    }
}

void Game::update(float deltaTime)
{
    currentScene->update(deltaTime);
}

void Game::render()
{
    // 清空
    SDL_RenderClear(renderer);

    currentScene->render();
    // 显示更新
    SDL_RenderPresent(renderer);
}
