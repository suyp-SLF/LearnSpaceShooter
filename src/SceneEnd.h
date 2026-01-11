#ifndef SCENEEND_H
#define SCENEEND_H

#include "Scene.h"

#include <string>

class SceneEnd : public Scene {
public:
    virtual void init();
    virtual void update(float deltaTime);
    virtual void render();
    virtual void clean();
    virtual void handleEvent(SDL_Event* event);
private:
    std::string name = "";
    bool typing = true;
    void renderPhase1();
    void renderPhase2();

    void removeLastUTF8Char(std::string& str);
};
#endif // SCENEEND_H