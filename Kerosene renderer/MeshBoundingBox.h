#pragma once
#include <glm/glm.hpp>
struct MeshBoundingBox {
    glm::vec2 minMaxX = glm::vec2(INFINITY, 0.0f);
    glm::vec2 minMaxY = glm::vec2(INFINITY, 0.0f);
    glm::vec2 minMaxZ = glm::vec2(INFINITY, 0.0f);

    MeshBoundingBox(const MeshBoundingBox& instance) {
        this->minMaxX = instance.minMaxX;
        this->minMaxY = instance.minMaxY;
        this->minMaxZ = instance.minMaxZ;
    }

    MeshBoundingBox() {
    }

    MeshBoundingBox operator +(const glm::vec3& other) {
        MeshBoundingBox returnBox(*this);
        returnBox.minMaxX.x += other.x;
        returnBox.minMaxX.y += other.x;

        returnBox.minMaxY.x += other.y;
        returnBox.minMaxY.y += other.y;

        returnBox.minMaxZ.x += other.z;
        returnBox.minMaxZ.y += other.z;

        return returnBox;
    }
};
