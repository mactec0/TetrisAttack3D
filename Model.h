#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GL/glew.h>
#include <algorithm>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace g3d {

class Model;

class ModelBuilder {
    Model *model;

public:
    ModelBuilder();
    ~ModelBuilder();
    ModelBuilder &setTexture(g3d::Texture *tex);
    ModelBuilder &setNormalMap(g3d::Texture *tex);
    ModelBuilder &setSpecularMap(g3d::Texture *tex);
    ModelBuilder &setMesh(g3d::Mesh *newMesh);
    ModelBuilder &setPosition(const glm::vec3 &v);
    ModelBuilder &setRotation(const glm::vec3 &v);
    ModelBuilder &setScale(const glm::vec3 &v);
    ModelBuilder &setShininess(float v);
    Model *build();
};

class Model {
    g3d::Texture *texture;
    g3d::Texture *normalMap;
    g3d::Texture *specularMap;
    g3d::Mesh *mesh;
    g3d::Shader *shader;
    glm::vec3 position, scale;
    glm::quat rotation;
    glm::vec3 rotAngles;
    float shininess;

public:
    Model();
    ~Model();
    void setTexture(g3d::Texture *tex);
    void setNormalMap(g3d::Texture *tex);
    void setSpecularMap(g3d::Texture *tex);
    void setMesh(g3d::Mesh *newMesh);
    void setPosition(const glm::vec3 &v);
    glm::vec3 getPosition();
    void setRotation(const glm::vec3 &v);
    void setScale(const glm::vec3 &v);
    void setShininess(float v);
    void move(const glm::vec3 &vMove);
    void draw();
    static ModelBuilder create();
};
};
