#pragma once
#include <vector>
#include "glm/glm.hpp"
#include <string>
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
//#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "p6/tiny_obj_loader.h"

struct vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct Object3D{
    std::vector<vertex> vertices;
    Object3D(Object3D &o): vertices(o.vertices){};
    Object3D() = default;
};

Object3D load_obj(
    const char * path
);
