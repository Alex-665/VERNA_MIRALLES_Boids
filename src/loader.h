#pragma once
#include <vector>
#include "glm/glm.hpp"
#include <string>
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "p6/tiny_obj_loader.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct Object3D{
    std::vector<Vertex> vertices;
    Object3D(Object3D &o): vertices(o.vertices){};
    Object3D() = default;
};

Object3D load_obj(
    const char * path
);
