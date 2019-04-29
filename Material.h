#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include "glm/glm.hpp"

struct Material{
    glm::vec3 ambient;
    GLint diffuseMapSlot;
    GLint specularMapSlot;
    GLint NormalMapSlot;
};
