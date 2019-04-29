#pragma once
#include "Log.h"
#include <iostream>
#include <string>
#include <unordered_map>

namespace g3d {
struct SResource {
    std::string name;
    void *resData;
};

class CResourceManager {
    std::unordered_map<std::string, SResource *> data;

public:
    CResourceManager();
    ~CResourceManager();

    template <class T> void load(const std::string &name, const T *ptr);

    template <typename T> T *getT(const std::string &name) const;

    struct retTypeDeduce {
        retTypeDeduce(CResourceManager *manager, const std::string &name)
            : manager(manager), name(name) {}
        CResourceManager *manager;
        const std::string &name;
        template <typename T> operator T *() {
            return (T *)manager->getT<T>(name);
        }
    };
    retTypeDeduce get(const std::string &name) {
        return retTypeDeduce(this, name);
    }
};

template <class T>
void CResourceManager::load(const std::string &name, const T *ptr) {
    if (data.find(name) != data.end()) {
        g3d::ENGINE_LOG_WARN("Resource with that name already exist {"
                             + name + "}; overwriting");
        delete data.find(name)->second;
    }
    data[name] = new SResource{name, (void *)ptr};
    g3d::ENGINE_LOG_INFO("New resource: " + name);
}

template <typename T> T *CResourceManager::getT(const std::string &name) const {
    if (data.find(name) == data.end())
        g3d::ENGINE_LOG_ERROR("Resource {" + name + "} doesn't exist!");

    auto resource = data.find(name)->second;
    return (T *)resource->resData;
}

extern CResourceManager resourceManager;

};
