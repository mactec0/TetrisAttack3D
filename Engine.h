#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <functional>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Light.h"
#include "ResourceManager.h"
#include "KeyManager.h"

#ifdef __APPLE__
    #include <CoreGraphics/CoreGraphics.h>
    #include <OpenGL/OpenGL.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/CGLCurrent.h>
#endif


namespace g3d {

    class Engine {
        std::function<void(float deltaTimeSec)> update;
        std::function<void()> frameRender;
        std::function<void(SDL_Event &e)> handleSDL_Events;
        bool quit;
        int width, height;

        SDL_Window *window;
        SDL_GLContext context;
        SDL_Event windowEvent;

        glm::vec3 clrColor;
        GLdouble deltaTime;

        int defaultShader;

      public:
        static Engine *Initialize();

        Engine();

        ~Engine();

        void setRenderCallback(std::function<void()> func);

        void setUpdateCallback(std::function<void(float deltaTimeSec)> func);

        void run();

        void setClearColor(glm::vec3 color);

        GLdouble getDeltaTime();

        int getWidth();
        int getHeight();

        g3d::Shader *getShader(int id);
        g3d::Shader *getDefaultShader();

        void setWindowTitle(const char *name);

        void shutdown();
    };

    extern g3d::Engine *engine;
};
