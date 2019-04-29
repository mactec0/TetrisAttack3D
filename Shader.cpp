#include "Shader.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

namespace g3d {

Shader::Shader() : program{0} {}

Shader::Shader(const std::string &vertexFileName,
               const std::string &fragmentFileName) {
    vertexShaderData = readFile(vertexFileName);
    fragmentShaderData = readFile(fragmentFileName);
    this->compile();
}

Shader::~Shader() { glDeleteProgram(program); }

bool Shader::loadFromFile(const std::string &vertexFileName,
                          const std::string &fragmentFileName) {
    vertexShaderData = readFile(vertexFileName);
    fragmentShaderData = readFile(fragmentFileName);
    return true;
}

void Shader::fromMemory(const std::string &vertexData,
                        const std::string &fragmentData) {
    vertexShaderData = vertexData;
    fragmentShaderData = fragmentData;
}

std::string Shader::readFile(const std::string &fileName) {
    std::stringstream ss;
    std::ifstream f(fileName);
    if (!f) {
        std::cerr << "[ERROR] Cannot load {" << fileName << "}\n";
        exit(0);
    }
    ss << f.rdbuf();
    f.close();
    return ss.str();
}

GLint Shader::getUniformLocation(const GLchar *name) {
    std::map<std::string, GLint>::iterator it = uniformMap.find(name);
    if (it == uniformMap.end())
        uniformMap[name] = glGetUniformLocation(program, name);
    return uniformMap[name];
}

template <class T> void Shader::setUniform(const GLchar *name, const T &value) {
    GLint location = getUniformLocation(name);
    glUniform1i(location, value);
}

template <>
void Shader::setUniform<GLint>(const GLchar *name, const GLint &value) {
    GLint location = getUniformLocation(name);
    glUniform1i(location, value);
}

template <>
void Shader::setUniform<GLuint>(const GLchar *name, const GLuint &value) {
    GLint location = getUniformLocation(name);
    glUniform1ui(location, value);
}

template <>
void Shader::setUniform<GLfloat>(const GLchar *name, const GLfloat &value) {
    GLint location = getUniformLocation(name);
    glUniform1f(location, value);
}

template <>
void Shader::setUniform<glm::vec2>(const GLchar *name, const glm::vec2 &value) {
    GLint location = getUniformLocation(name);
    glUniform2f(location, value.x, value.y);
}

template <>
void Shader::setUniform<glm::vec3>(const GLchar *name, const glm::vec3 &value) {
    GLint location = getUniformLocation(name);
    glUniform3f(location, value.x, value.y, value.z);
}

template <>
void Shader::setUniform<glm::vec4>(const GLchar *name, const glm::vec4 &value) {
    GLint location = getUniformLocation(name);
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

template <>
void Shader::setUniform<glm::mat3>(const GLchar *name, const glm::mat3 &value) {
    GLint location = getUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

template <>
void Shader::setUniform<glm::mat4>(const GLchar *name, const glm::mat4 &value) {
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setTextureSlots(const GLchar *name, const GLint &slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    GLint location = getUniformLocation(name);
    glUniform1i(location, slot);
}

GLuint Shader::compile() {
    const GLchar *vsData{vertexShaderData.c_str()};
    const GLchar *fsData{fragmentShaderData.c_str()};

    GLuint vs{glCreateShader(GL_VERTEX_SHADER)};
    GLuint fs{glCreateShader(GL_FRAGMENT_SHADER)};

    glShaderSource(vs, 1, &vsData, NULL);
    glShaderSource(fs, 1, &fsData, NULL);

    glCompileShader(vs);
    checkShaderErrors(vs);
    glCompileShader(fs);
    checkShaderErrors(fs);

    program = glCreateProgram();
    if (program == 0) {
        std::cerr << "[ERROR] glCreateProgram() failed!" << std::endl;
        return false;
    }
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    checkProgramErrors();

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

void Shader::checkProgramErrors() {
    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, ' ');
        glGetProgramInfoLog(program, length, &length, &log[0]);
        std::cerr << "[Error] glLinkProgram() failed! " << log << std::endl;
    }
}

void Shader::checkShaderErrors(GLuint shader) {
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, ' ');
        glGetShaderInfoLog(shader, length, &length, &log[0]);
        std::cerr << "[Error] glCompileShader() failed!" << log << std::endl;
    }
}

Shader *Shader::defaultPhongLightingShader() {
    auto new_shader{new Shader("shaders/default.vs", "shaders/default.fs")};
    new_shader->use();
    for (int i = 0; i < 16; i++)
        new_shader->setUniform(
            std::string("pointLights[" + std::to_string(i) + "].state").c_str(),
            0);
    for (int i = 0; i < 4; i++)
        new_shader->setUniform(
            std::string("dirLights[" + std::to_string(i) + "].state").c_str(),
            0);

    new_shader->setTextureSlots("specularMap", 1);
    new_shader->setTextureSlots("diffuseMap", 0);
    new_shader->setUniform("material.shininess", 32.f);
    new_shader->setUniform("brightness", 1.f);
    new_shader->setUniform("colorModulation", glm::vec3(1.f, 1.f, 1.f));
    return new_shader;
}

void Shader::use() {
    if (program > 0)
        glUseProgram(program);
}

};
