#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace g3d {
MeshBuilder::MeshBuilder() {}

MeshBuilder::~MeshBuilder() {}

MeshBuilder &MeshBuilder::withVertices(std::vector<Vertex> &vertices) {
    this->vertices = vertices;
    return *this;
}

MeshBuilder &MeshBuilder::fromFile(const std::string &fileName) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::ifstream f(fileName);
    if (!f)
        ENGINE_LOG_ERROR("Cannot load {" + fileName + "}");

    ENGINE_LOG_INFO("Loading model {" + fileName + "}");

    std::string line;
    while (std::getline(f, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") {
            glm::vec3 vec;
            ss >> vec.x >> vec.y >> vec.z;
            vertices.push_back(vec);
        } else if (type == "vt") {
            glm::vec2 vec;
            ss >> vec.x >> vec.y;
            uvs.push_back(vec);
        } else if (type == "vn") {
            glm::vec3 vec;
            ss >> vec.x >> vec.y >> vec.z;
            normals.push_back(glm::normalize(vec));
        } else if (type == "f") {
            int vertexIndex, uvIndex, normalIndex;
            std::string face;
            while (ss >> face) {
                while (face.find("//") != std::string::npos)
                    face.replace(face.find("//"), 2, " -1 ");
                std::replace(face.begin(), face.end(), '/', ' ');

                std::stringstream ss2(face);
                ss2 >> vertexIndex >> uvIndex >> normalIndex;
                if (vertexIndex != -1)
                    vertexIndices.push_back(vertexIndex);
                if (uvIndex != -1)
                    uvIndices.push_back(uvIndex);
                if (normalIndex != -1)
                    normalIndices.push_back(normalIndex);
            }
        }
    }
    f.close();

    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        Vertex vertex;
        vertex.position = (vertices.size() > 0) ? vertices[vertexIndices[i] - 1]
                                                : glm::vec3(0.f, 0.f, 0.f);
        vertex.normal = (normals.size() > 0) ? normals[normalIndices[i] - 1]
                                             : glm::vec3(0.f, 0.f, 0.f);
        vertex.texCoords = (uvs.size() > 0) ? uvs[uvIndices[i] - 1] : glm::vec2(0.f, 0.f);
        this->vertices.push_back(vertex);
    }

    return *this;
}

Mesh *MeshBuilder::build() {
    Mesh *newMesh{new Mesh(vertices)};
    newMesh->setupBuffers();
    return newMesh;
}

Mesh::Mesh(std::vector<Vertex> &vertices) { this->vertices = vertices; }

Mesh::Mesh() {}

Mesh::~Mesh() {}

void Mesh::setVertices(std::vector<Vertex> &vertices) {
    this->vertices = vertices;
}

void Mesh::setupBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)(11 * sizeof(GLfloat)));
    glBindVertexArray(0);
}

MeshBuilder Mesh::create() { return MeshBuilder(); }

void Mesh::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

};
