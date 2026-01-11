#include "SceneEnd.h"
#include "Game.h"

#include <SDL_ttf.h>

void SceneEnd::init()
{
    // 判断是否打开文字输入模式
    if (!SDL_IsTextInputActive())
    {
        // 打开
        SDL_StartTextInput();
    }
    if (!SDL_IsTextInputActive())
    {
        // 出错
        SDL_Log("Text input mode started");
    }
}

void SceneEnd::update(float deltaTime)
{
    blinkTime -= deltaTime;
    if (blinkTime < 0){
        blinkTime = 0.5;
    }
}

void SceneEnd::render()
{
    if (typing)
    {
        renderPhase1();
    }
    else
    {
        renderPhase2();
    }
}

void SceneEnd::clean()
{
}

void SceneEnd::handleEvent(SDL_Event *event)
{
    if (typing)
    {
        // 获得输入的字符
        if (event->type == SDL_TEXTINPUT)
        {
            name += event->text.text;
        }
        // 检查输入的字符是否是回车键
        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_RETURN)
        {
            typing = false;
        }
        // 检查是否是退格键
        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_BACKSPACE)
        {
            removeLastUTF8Char(name);
        }
    }
}

void SceneEnd::renderPhase1()
{
    SDL_Color color = {255, 255, 255, 255}; // 白色
    game.setCenterText(game.fontText, "你的得分是" + std::to_string(game.getScore()), 0.1, color);
    game.setCenterText(game.fontTitle, "GAMEOVER", 0.3, color);
    game.setCenterText(game.fontText, "请输入你的名字,按回车键提交", 0.5, color);
    SDL_Point tailPos = game.setCenterText(game.fontText, name, 0.7, color);
    if(blinkTime < 0.25){
        game.setText(game.fontText, "_", tailPos.x + 1, tailPos.y, color);
    }
}

void SceneEnd::renderPhase2()
{
}

void SceneEnd::removeLastUTF8Char(std::string &str)
{
   if (str.empty()) return;

    // 从字符串末尾向前探测
    size_t lastPos = str.length() - 1;

    // UTF-8 的后续字节（Trailing bytes）都以 10 开头 (即 0x80 到 0xBF)
    // 我们一直向前找，直到找到该字符的起始字节
    while (lastPos > 0 && (str[lastPos] & 0xC0) == 0x80) {
        lastPos--;
    }

    // 此时 lastPos 指向最后一个字符的起始位置
    str.erase(lastPos);
}
