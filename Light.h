#pragma once
#include "Shader.h"

namespace g3d {
#define POINT_LIGHT_MAX 16
#define DIR_LIGHT_MAX 4

class Light {
protected:
    glm::vec3 Ambient, Diffuse, Specular;
    bool active;
    int ID;
    Light();
    virtual ~Light();

public:
    virtual void setActive(bool v);
    virtual void setAmbient(const glm::vec3 &v);
    virtual void setDiffuse(const glm::vec3 &v);
    virtual void setSpecular(const glm::vec3 &v);

    int getID();
};


class PointLight : public Light {
    float constant, linear, quadratic;
    glm::vec3 position;
    static bool PointIDs[POINT_LIGHT_MAX];
public:
    PointLight();
    ~PointLight();
    static PointLight *create();

    virtual void setActive(bool v);
    virtual void setAmbient(const glm::vec3 &v);
    virtual void setDiffuse(const glm::vec3 &v);
    virtual void setSpecular(const glm::vec3 &v);

    void setAttenuation(float constant, float linear, float quadratic);
    void setRange(float range);
    void setPosition(const glm::vec3 &v);
    glm::vec3 getPosition();
};


class DirectionalLight : public Light {
    glm::vec3 direction;
    static bool DirIDs[DIR_LIGHT_MAX];
public:
    DirectionalLight();
    ~DirectionalLight();
    static DirectionalLight *create();

    virtual void setActive(bool v);
    virtual void setAmbient(const glm::vec3 &v);
    virtual void setDiffuse(const glm::vec3 &v);
    virtual void setSpecular(const glm::vec3 &v);

    void setDirection(const glm::vec3 &v);
    void setDirection(const glm::vec3 &src, const glm::vec3 &dst);
};
};
