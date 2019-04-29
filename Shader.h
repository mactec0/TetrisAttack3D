#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <fstream>
#include <map>
#include <sstream>
#include <string>

namespace g3d {
enum {
    SHADER_PHONG_LIGHT,
    SHADER_MAX,
};

class Shader {
    std::map<std::string, GLint> uniformMap;
    GLuint program;
    std::string vertexShaderData, fragmentShaderData;

    std::string readFile(const std::string &fileName);
    void checkProgramErrors();
    void checkShaderErrors(GLuint shader);

  public:
    Shader();
    Shader(const std::string &vertexFileName,
           const std::string &fragmentFileName);
    ~Shader();
    GLint getUniformLocation(const GLchar *name);
    bool loadFromFile(const std::string &vertexFileName,
                      const std::string &fragmentFileName);
    void fromMemory(const std::string &vertexData,
                    const std::string &fragmentData);

    template <class T> void setUniform(const GLchar *name, const T &value);
    void setTextureSlots(const GLchar *name, const GLint &slot);

    GLuint compile();

    static Shader *defaultPhongLightingShader();

    void use();
};

extern g3d::Shader *shaders[g3d::SHADER_MAX];
};
