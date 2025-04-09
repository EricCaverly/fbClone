#include "flappy.hpp"
#include <raylib.h>


Bird::Bird(Vector2 startPos, Color c, float gravity) 
    : m_Alive(true), m_BetweenPipes(false), m_Score(0), m_Gravity(gravity), m_Color(c), m_Radius(18) {
    m_Pos = startPos;
    m_Vel = {0, 0};
}

void Bird::Draw() const {
    DrawCircleV(m_Pos, m_Radius, m_Color);
}

void Bird::Move() {
    // Move the bird, increasing velocity with accel
    m_Vel.y += m_Gravity;
    m_Pos.y += m_Vel.y;
    m_Pos.x += m_Vel.x;

    // Check if the bird hit the top of the screen
    if (m_Pos.y <= m_Radius) {
        m_Pos.y = m_Radius;
        m_Vel.y = 0;
    }

    // Check if the bird fell off the screen
    if (m_Pos.y >= Options::screenHeight) {
        m_Alive = false;
    }
}

void Bird::Jump(float upVel) {
    m_Vel.y = upVel;
}

Vector2 Bird::GetPos() const { return m_Pos; }

float Bird::GetRadius() const { return m_Radius; }

void Bird::SetVel(Vector2 vel) {
    m_Vel = vel;
}

void Bird::IncreaseScore() {
    // Only increase the score if the bird is still alive
    if (m_Alive)
        ++m_Score;
}

int Bird::GetScore() const {
    return m_Score;
}