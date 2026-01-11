#include "SceneTitle.h"
#include "SceneMain.h"
#include "Game.h"

#include <SDL_mixer.h>

SceneTitle::SceneTitle()
{
}
SceneTitle::~SceneTitle()
{
}

void SceneTitle::init()
{
    titleColor = {255, 255, 255, 255};
    textColor = {255, 255, 255, 255};
    //播放背景音乐
    Mix_Music *music = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    Mix_PlayMusic(music, -1);
}

void SceneTitle::update(float deltaTime)
{
    if (textColor.a >= 255)
    {
        textColor.a -= deltaTime * 255;;
    }else{
        textColor.a += deltaTime * 255;;
    }
    
}

void SceneTitle::render()
{
    setUI();
}

void SceneTitle::clean()
{
}

void SceneTitle::handleEvent(SDL_Event *event)
{
    //按J切到主场景
    if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.sym == SDLK_j)
        {
            game.changeScene(new SceneMain());
        }
    }
}

void SceneTitle::setUI()
{
    // game.getRenderer()->clear();
    game.setCenterText(game.fontTitle, "SDL太空战机", 0.1, titleColor);
    game.setCenterText(game.fontText, "按J键开始", 0.7, textColor);
}
