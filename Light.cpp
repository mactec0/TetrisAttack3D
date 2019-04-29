#include "Light.h"
namespace g3d{

    Light::Light(){
        Ambient={0.f, 0.f, 0.f};
        Diffuse={0.f, 0.f, 0.f};
        Specular={0.f, 0.f, 0.f};
        active=false;
        ID=-1;
    }

    Light::~Light(){

    }

    void Light::setActive(bool v){
        active=v;
    }

    void Light::setAmbient(const glm::vec3 &v){
        Ambient=v;
    }

    void Light::setDiffuse(const glm::vec3 &v){
        Diffuse=v;
    }

    void Light::setSpecular(const glm::vec3 &v){
        Specular=v;
    }

    int Light::getID() { return ID; }

    bool PointLight::PointIDs[POINT_LIGHT_MAX] = {0};
    PointLight::PointLight(){
        for(int i=0;i<POINT_LIGHT_MAX;i++)
            if(!PointIDs[i]){
                PointIDs[i]=true;
                ID=i;
                return;
            }
        ID=-1;
    }

    PointLight::~PointLight(){
        if(ID==-1)
            return;
        shaders[SHADER_PHONG_LIGHT]->setUniform(
            std::string("pointLights["+std::to_string(ID)+"].state").c_str(), 0);
        PointIDs[ID]=false;
    }

    PointLight *PointLight::create(){
        auto newLight{new PointLight()};
        if(newLight->getID()==-1){
            delete newLight;
            return NULL;
        }
        return newLight;
    }

    void PointLight::setActive(bool v){
        Light::setActive(v);
        shaders[SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("pointLights["+std::to_string(ID)+"].state").c_str(), (int)v);
    }

    void PointLight::setAmbient(const glm::vec3 &v){
        Light::setAmbient(v);
        shaders[g3d::SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("pointLights["+std::to_string(ID)+"].ambient").c_str(), Ambient);
    }

    void PointLight::setDiffuse(const glm::vec3 &v){
        Light::setDiffuse(v);
        shaders[SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("pointLights["+std::to_string(ID)+"].diffuse").c_str(), Diffuse);
    }

    void PointLight::setSpecular(const glm::vec3 &v){
        Light::setSpecular(v);
        shaders[SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("pointLights["+std::to_string(ID)+"].specular").c_str(), Specular);
    }

    void PointLight::setAttenuation(float constant, float linear, float quadratic){
        this->constant=constant;
        this->linear=linear;
        this->quadratic=quadratic;
        shaders[SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("pointLights["+std::to_string(ID)+"].constant").c_str(), constant);
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("pointLights["+std::to_string(ID)+"].linear").c_str(), linear);
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("pointLights["+std::to_string(ID)+"].quadratic").c_str(), quadratic);
    }

    void PointLight::setRange(float range){
        constant=1.f;
        linear=4.5f/range;
        quadratic=75.f/(range*range);
        shaders[SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("pointLights["+std::to_string(ID)+"].constant").c_str(), constant);
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("pointLights["+std::to_string(ID)+"].linear").c_str(), linear);
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("pointLights["+std::to_string(ID)+"].quadratic").c_str(), quadratic);
    }


    void PointLight::setPosition(const glm::vec3 &v){
        position=v;
        shaders[SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("pointLights["+std::to_string(ID)+"].position").c_str(), position);
    }

    glm::vec3 PointLight::getPosition(){ return position; }

    bool DirectionalLight::DirIDs[DIR_LIGHT_MAX] = {0};
    DirectionalLight::DirectionalLight(){
        for(int i=0;i<DIR_LIGHT_MAX;i++)
            if(!DirIDs[i]){
                DirIDs[i]=true;
                ID=i;
                return;
            }
        ID=-1;
    }

    DirectionalLight::~DirectionalLight(){
        if(ID==-1)
            return;
        shaders[SHADER_PHONG_LIGHT]->setUniform(
            std::string("dirLights["+std::to_string(ID)+"].state").c_str(), 0);
        DirIDs[ID]=false;
    }

    DirectionalLight *DirectionalLight::create(){
        auto newLight{new DirectionalLight()};
        if(newLight->getID()==-1){
            delete newLight;
            return NULL;
        }
        return newLight;
    }

    void DirectionalLight::setActive(bool v){
        Light::setActive(v);
        shaders[SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("dirLights["+std::to_string(ID)+"].state").c_str(), (int)v);
    }

    void DirectionalLight::setAmbient(const glm::vec3 &v){
        Light::setAmbient(v);
        shaders[SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("dirLights["+std::to_string(ID)+"].ambient").c_str(), Ambient);
    }

    void DirectionalLight::setDiffuse(const glm::vec3 &v){
        Light::setDiffuse(v);
        shaders[SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("dirLights["+std::to_string(ID)+"].diffuse").c_str(), Diffuse);
    }

    void DirectionalLight::setSpecular(const glm::vec3 &v){
        Light::setSpecular(v);
        shaders[SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("dirLights["+std::to_string(ID)+"].specular").c_str(), Specular);
    }

    void DirectionalLight::setDirection(const glm::vec3 &v){
        direction=glm::normalize(v);
        shaders[SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("dirLights["+std::to_string(ID)+"].direction").c_str(), direction);
    }

    void DirectionalLight::setDirection(const glm::vec3 &src, const glm::vec3 &dst){
        direction=glm::normalize(dst-src);
        shaders[SHADER_PHONG_LIGHT]->use();
        shaders[SHADER_PHONG_LIGHT]->setUniform(std::string("dirLights["+std::to_string(ID)+"].direction").c_str(), direction);
    }

};
