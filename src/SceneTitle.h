#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "Scene.h"

class SceneTitle : public Scene
{
public:
    SceneTitle();
    ~SceneTitle();
    virtual void init();
    virtual void update(float deltaTime);
    virtual void render();
    virtual void clean();
    virtual void handleEvent(SDL_Event* event);

private:
    void setUI();
    SDL_Color titleColor;
    SDL_Color textColor;
};
#endif // SCENE_TITLE_H