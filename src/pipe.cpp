#include "flappy.hpp"
#include <cstdlib>
#include <raylib.h>

/************************ Pipe ****************************/
Pipe::Pipe(float xPos, float openingTop, float openingBot)
: m_xPos(xPos), m_OpeningTop(openingTop), m_OpeningBot(openingBot){
}

Pipe::Pipe(float xPos, float openingTop) 
: m_xPos(xPos), m_OpeningTop(openingTop), m_OpeningBot(openingTop+stdPipeOpening) {
}

void Pipe::Move(float xDiff) {
    m_xPos += xDiff;
}

void Pipe::Draw() const {
    // Draw top pipe
    DrawRectangle(m_xPos, 0, pipeWidth, m_OpeningTop, GREEN);

    // Draw bottom pipe
    DrawRectangle(m_xPos, m_OpeningBot, pipeWidth, Options::screenHeight-m_OpeningBot, GREEN);
}

float Pipe::GetX() const { return m_xPos; }
float Pipe::GetOpeningTop() const { return m_OpeningTop; }
float Pipe::GetOpeningBot() const { return m_OpeningBot; }


/*********************** Pipes ********************************/

Pipes::Pipes(float xSpace, float xVel, float highestOpening, float lowestOpening) 
    : m_xVel(xVel), m_xSpace(xSpace), m_highestOpening(highestOpening), m_lowestOpening(lowestOpening) {
    m_Pipes = std::list<Pipe>();
    
    // Generate enough pipes to fill the screen while keeping m_xSpace between them
    for (int i = 0; i < Options::screenWidth; i+=m_xSpace) {
        GeneratePair(i+Options::screenWidth);
    }
}

void Pipes::DrawAll() const {
    // Draw each pipe
    for (Pipe p : m_Pipes) {
        p.Draw();
    }
}

void Pipes::MoveAll() {
    // Move pipes using an itterator
    std::list<Pipe>::iterator pi = m_Pipes.begin();
    while (pi != m_Pipes.end()) {
        pi++->Move(m_xVel);
    }

    // Check if a pipe has gone off the edge of the screen. If yes, destroy it and create a new pipe on the right
    if (m_Pipes.front().GetX() < -Pipe::pipeWidth) {
        m_Pipes.pop_front();
        GeneratePair(m_Pipes.back().GetX()+m_xSpace);
    }
}

void Pipes::GeneratePair(float startX) {

    // Randomly chose a point for the top of the opening
    int openingTop = (std::rand() % (int)(m_lowestOpening  - m_highestOpening))+m_highestOpening;

    // Create the new pipe(s)
    m_Pipes.push_back(Pipe(
        startX,
        openingTop
    ));
}

void Pipes::BirdCollide(Bird &b) const {
    // Keep track if the bird was detected to be between pipes this check
    bool betweenPipes = false;

    for (Pipe p : m_Pipes) {
        // Check if between pipes
        if (b.GetPos().x+b.GetRadius() > p.GetX() && b.GetPos().x-b.GetRadius() < p.GetX() + Pipe::pipeWidth) {

            // Check if within pipes. If so, kill the bird and change it's velocity so it looks like it hit and bounced off
            if (b.GetPos().y-b.GetRadius() < p.GetOpeningTop() || b.GetPos().y+b.GetRadius() > p.GetOpeningBot()) {
                b.m_Alive = false;
                b.SetVel({m_xVel-1, 2});
            }

            // If between pipes, keep track of that
            betweenPipes = true;
        }
    }

    // If currently between pipes
    if (betweenPipes) {
        // If not previously between pipes, set the bird's variable so it knows it's between pipes
        if (!b.m_BetweenPipes)
            b.m_BetweenPipes = true;

    // If not currently between pipes
    } else {
        // Check if the bird was previously between pipes
        if (b.m_BetweenPipes) {
            // If it was previously, then increase the score and inform the bird it's no longer between pipes
            b.IncreaseScore();
            b.m_BetweenPipes = false;
        }
    }
}