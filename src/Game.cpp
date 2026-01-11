#include "Game.h"
#include "SceneTitle.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

Game::Game()
{
}

void Game::updateBackground(float deltaTime)
{
    // 远处的背景
    farBackground.offset.y += farBackground.speed * deltaTime;
    if (farBackground.offset.y >= farBackground.height)
    {
        farBackground.offset.y -= farBackground.height;
    }
    // 近处的背景
    nearBackground.offset.y += nearBackground.speed * deltaTime;
    if (nearBackground.offset.y >= nearBackground.height)
    {
        nearBackground.offset.y -= nearBackground.height;
    }
}

void Game::renderBackground()
{
 // 渲染远处背景,循环
    for (int i = 0; i <= getWindowHeight(); i += farBackground.height)
    {
        SDL_Rect farBackgroundRect = {
            0,
            farBackground.offset.y + i,
            farBackground.width,
            farBackground.height};
        SDL_RenderCopy(getRenderer(), farBackground.texture, NULL, &farBackgroundRect);
        for (int i = -getWindowHeight(); i <= getWindowWidth(); i += farBackground.width)
        {
            SDL_Rect farBackgroundRect = {
                i,
                farBackground.offset.x + i,
                farBackground.width,
                farBackground.height};
            SDL_RenderCopy(getRenderer(), farBackground.texture, NULL, &farBackgroundRect);
        }
    }
    // 渲染近处背景,循环
    for (int i = -getWindowHeight(); i <= getWindowHeight(); i += nearBackground.height)
    {
        SDL_Rect nearBackgroundRect = {
            0,
            nearBackground.offset.y + i,
            nearBackground.width,
            nearBackground.height};
        SDL_RenderCopy(getRenderer(), nearBackground.texture, NULL, &nearBackgroundRect);
        for (int i = 0; i <= getWindowWidth(); i += nearBackground.width)
        {
            SDL_Rect nearBackgroundRect = {
                i,
                nearBackground.offset.x + i,
                nearBackground.width,
                nearBackground.height};
            SDL_RenderCopy(getRenderer(), nearBackground.texture, NULL, &nearBackgroundRect);
        }
    }
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
    //ttf初始化
    if (TTF_Init() != 0)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_ERROR, "TTF_Init Error: %s\n", TTF_GetError());
        isRunning = false;
    }
    // mixer初始化
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_ERROR, "Mix_OpenAudio Error: %s\n", Mix_GetError());
        isRunning = false;
    }
    // 远处背景
    farBackground.texture = IMG_LoadTexture(getRenderer(), "assets/image/Stars-B.png");
    SDL_QueryTexture(farBackground.texture, NULL, NULL, &farBackground.width, &farBackground.height);
    farBackground.speed = 50;
    nearBackground.texture = IMG_LoadTexture(getRenderer(), "assets/image/Stars-A.png");
    SDL_QueryTexture(nearBackground.texture, NULL, NULL, &nearBackground.width, &nearBackground.height);
    nearBackground.speed = 100;
    //初始化字体
    fontTitle = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 64);
    fontText = TTF_OpenFont("assets/font/VonwaonBitmap-12px.ttf", 24);
    //制定当前场景
    currentScene = new SceneTitle();
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
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    if (farBackground.texture != nullptr)
    {
        SDL_DestroyTexture(farBackground.texture); // 远景
    }
    if (nearBackground.texture != nullptr)
    {
        SDL_DestroyTexture(nearBackground.texture); // 近景
    }
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
    updateBackground(deltaTime);      // 更新背景
    
}

void Game::render()
{
    // 清空
    SDL_RenderClear(renderer);
    renderBackground();      // 渲染背景
    currentScene->render();
    // 显示更新
    SDL_RenderPresent(renderer);
}

SDL_Point Game::setCenterText(TTF_Font *font, std::string text, float PosY, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect;
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    rect.x = (windowWidth - rect.w) / 2;
    rect.y = PosY * windowHeight;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    return {rect.x + rect.w, rect.y};
}

SDL_Point Game::setText(TTF_Font *font, std::string text, float PosX, float PosY, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect;
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    rect.x = PosX;
    rect.y = PosY;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    return {rect.x, rect.y};
}
