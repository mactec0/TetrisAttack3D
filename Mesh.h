#pragma once
#include "Log.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <algorithm>
#include <string>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

namespace g3d {
class Mesh;

class MeshBuilder {
    std::vector<Vertex> vertices;

public:
    MeshBuilder();
    ~MeshBuilder();
    MeshBuilder &withVertices(std::vector<Vertex> &vertices);
    MeshBuilder &fromFile(const std::string &fileName);
    Mesh *build();
};

class Mesh {
    GLuint VBO, VAO;
    std::vector<Vertex> vertices;

public:
    Mesh(std::vector<Vertex> &vertices);
    Mesh();
    ~Mesh();
    void setVertices(std::vector<Vertex> &vertices);
    void setupBuffers();
    static MeshBuilder create();
    void draw();
};

};
