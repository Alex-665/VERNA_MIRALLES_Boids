#pragma once

class TrackballCamera {
    private:
        glm::vec3 m_position;
        float m_distance;
        float m_angleX;
        float m_angleY;
    public:
        TrackballCamera(): m_distance(5.f), m_angleX(0.f), m_angleY(0.f) {}; 
        TrackballCamera(float dist): m_distance(dist), m_angleX(0.f), m_angleY(0.f) {};
        TrackballCamera(float dist, float angleX, angleY): m_distance(dist), m_angleX(angleX), m_angleY(angleY) {};
        inline void moveFront(float delta) {m_distance -= delta;}
        inline void rotateLeft(float degrees) {m_angleY += degrees;}
        inline void rotateUp(float degrees) {m_angleX += degrees;}
        glm::mat4 getViewMatrix() const;
};
