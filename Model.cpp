#include "Engine.h"
#include "Model.h"

namespace g3d {

ModelBuilder::ModelBuilder() { model = new Model(); }

ModelBuilder::~ModelBuilder() {}

Model *ModelBuilder::build() { return model; }

ModelBuilder &ModelBuilder::setTexture(g3d::Texture *tex) {
    model->setTexture(tex);
    return *this;
}

ModelBuilder &ModelBuilder::setNormalMap(g3d::Texture *tex) {
    model->setNormalMap(tex);
    return *this;
}

ModelBuilder &ModelBuilder::setSpecularMap(g3d::Texture *tex) {
    model->setSpecularMap(tex);
    return *this;
}

ModelBuilder &ModelBuilder::setMesh(g3d::Mesh *newMesh) {
    model->setMesh(newMesh);
    return *this;
}

ModelBuilder &ModelBuilder::setPosition(const glm::vec3 &v) {
    model->setPosition(v);
    return *this;
}

ModelBuilder &ModelBuilder::setRotation(const glm::vec3 &v) {
    model->setRotation(v);
    return *this;
}

ModelBuilder &ModelBuilder::setShininess(float v) {
    model->setShininess(v);
    return *this;
}

ModelBuilder &ModelBuilder::setScale(const glm::vec3 &v) {
    model->setScale(v);
    return *this;
}

Model::Model()
    : texture{nullptr}, normalMap{nullptr}, specularMap{nullptr}, mesh{nullptr},
      shader{nullptr}, position{glm::vec3(0.f, 0.f, 0.f)},
      scale{1.f, 1.f, 1.f}, rotation{glm::quat({0.f, 0.f, 0.f})},
      rotAngles{0.f, 0.f, 0.f}, shininess{32.f} {}

Model::~Model() {}

void Model::setTexture(g3d::Texture *tex) { texture = tex; }

void Model::setNormalMap(g3d::Texture *tex) { normalMap = tex; }

void Model::setSpecularMap(g3d::Texture *tex) { specularMap = tex; }

void Model::setMesh(g3d::Mesh *newMesh) { mesh = newMesh; }

void Model::setPosition(const glm::vec3 &v) { position = v; }

glm::vec3 Model::getPosition() { return position; }

void Model::setRotation(const glm::vec3 &v) { rotation = glm::quat(v); }

void Model::setScale(const glm::vec3 &v) { scale = v; }

void Model::setShininess(float v) { shininess = v; }

void Model::move(const glm::vec3 &vMove) { position += vMove; }

void Model::draw() {

    glm::mat4 model{glm::translate(glm::mat4(), position) * glm::toMat4(rotation) *
                    glm::scale(glm::mat4(), scale)};
    if (!shader) {
        engine->getDefaultShader()->use();
        engine->getDefaultShader()->setUniform("model", model);
        engine->getDefaultShader()->setUniform("material.shininess", shininess);
    }

    if (texture)
        texture->bind(0);
    if (specularMap)
        specularMap->bind(1);
    if (normalMap)
        normalMap->bind(2);
    if (mesh)
        mesh->draw();
    if (texture)
        texture->unbind(0);
    if (specularMap)
        specularMap->unbind(1);
    if (normalMap)
        normalMap->unbind(2);
}

ModelBuilder Model::create() { return ModelBuilder(); }

};
