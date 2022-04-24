#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "Scene.h"

class MainScene : public Scene {
public:
    MainScene (GLuint width, GLuint height, std::string window_name)
        : Scene (width, height, window_name)
    {
    }

    void update () override;
    void setupScene () override;
};

#endif
