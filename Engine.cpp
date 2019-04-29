#include "Engine.h"

#define CALL_CALLBACK(func, ...)                                               \
    if (func)                                                                  \
        func(__VA_ARGS__);

namespace g3d {

Engine *Engine::Initialize() { return new Engine(); }

Engine::Engine() {
    quit = false;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "SDL Error :" << std::string(SDL_GetError());
        exit(0);
    }

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    width = 1280;
    height = 720;
    window = SDL_CreateWindow(
        "G3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        std::cout << "SDL Error :" << std::string(SDL_GetError());
        exit(0);
    }

    context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;

    GLenum glewError{glewInit()};
    if (GLEW_OK != glewError) {
        std::cout << "Failed to initialize GLEW: "
                  << glewGetErrorString(glewError) << std::endl;
        exit(0);
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->setClearColor(glm::vec3(0.05f, 0.105f, 0.19f));

    shaders[SHADER_PHONG_LIGHT] = g3d::Shader::defaultPhongLightingShader();

    defaultShader = SHADER_PHONG_LIGHT;
}

Engine::~Engine() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::run() {
    deltaTime = 0.0f;
    Uint64 lastFrame{0L};
    Uint64 currentFrame{SDL_GetPerformanceCounter()};

    while (!quit) {
        lastFrame = currentFrame;
        currentFrame = SDL_GetPerformanceCounter();
        deltaTime = (currentFrame - lastFrame) /
                    (GLdouble)SDL_GetPerformanceFrequency();

        g3d::camera.updateAnimation(deltaTime);

        CALL_CALLBACK(update, deltaTime)

        keyManager.resetClicked();
        while (SDL_PollEvent(&windowEvent)) {
            if (windowEvent.type == SDL_QUIT) {
                exit(0);
            }
            if (windowEvent.type == SDL_KEYDOWN) {
                keyManager.setClicked(windowEvent.key.keysym.sym, true);
                keyManager.setPressed(windowEvent.key.keysym.sym, true);
            } else if (windowEvent.type == SDL_KEYUP) {
                keyManager.setPressed(windowEvent.key.keysym.sym, false);
            }

            CALL_CALLBACK(handleSDL_Events, windowEvent)
        }

        glClearColor(clrColor.r, clrColor.g, clrColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view{g3d::camera.getViewMatrix()};
        auto projection{glm::perspective(
            glm::radians(g3d::camera.getFov()),
            (float)this->getWidth() / (float)this->getHeight(), 0.1f, 250.0f)};
        getDefaultShader()->setUniform("view", view);
        getDefaultShader()->setUniform("projection", projection);

        getDefaultShader()->use();
        getDefaultShader()->setUniform("viewPos", g3d::camera.getPosition());
        getDefaultShader()->setUniform("view", view);
        getDefaultShader()->setUniform("projection", projection);

        CALL_CALLBACK(frameRender)
        SDL_GL_SwapWindow(window);
    }
}

void Engine::setRenderCallback(std::function<void()> func) {
    frameRender = func;
}

void Engine::setUpdateCallback(std::function<void(float deltaTimeSec)> func) {
    update = func;
}

void Engine::setClearColor(glm::vec3 color) { clrColor = color; }

GLdouble Engine::getDeltaTime() { return deltaTime; }

int Engine::getWidth() { return width; }

int Engine::getHeight() { return height; }

g3d::Shader *Engine::getShader(int id) { return shaders[id]; }

g3d::Shader *Engine::getDefaultShader() { return shaders[defaultShader]; }

void Engine::setWindowTitle(const char *name) { SDL_SetWindowTitle(window, name); }

void Engine::shutdown() { quit = true; }

g3d::CResourceManager resourceManager;
g3d::Engine *engine;
g3d::Shader *shaders[g3d::SHADER_MAX];
g3d::CKeyManager keyManager;
Camera camera;
};
