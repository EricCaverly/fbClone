#include <list>
#include <raylib.h>

/*

Takeaway Questions:
    - I am not 100% sure what the best practice is for using Getgers / Setters vs just making the member variable public. 
        - In C# it's always better to use properties but the language support is much nicer
    - Is having an Options namespace like this a good idea? It may be more difficult to load this from a File but as I have yet to experiement with "good" file parsing who knows
        - Along with that, where should consts like pipe width go? options, or under pipe?
    - Originally I tried to use inheritence with Birds and Pipes under an entity class but reaslized later on that they were too dissimilar. I know realize that was silly in the first place. Ensure that inheritance is only used when it makes sense
    - My BirdCollide function does 2 things, checks if the bird died or if it should score. This goes against the single function per function principle, but it is efficient. Should this become multiple functions?
    - Is the game reset logic in main.cpp acceptable? It is easier than implementing a Reset function but is that best practice?
*/

/*
Future plans:
    - Switch to 2D graphics via sprites instead of basic shapes
    - Implement a main menu and more gameplay hints
    - Sounds?
*/

namespace Options {
    const Color bgColor = {100, 200, 200};
    const int screenWidth = 600;
    const int screenHeight = 800;
    const int targetFps = 60;
    const int basePipeSpeed = -2;
}

class Bird {
public:
    static constexpr float defaultJump = -8;    // Sets the yVel to this when jump is pressed
    bool m_Alive;                               // Keeps track of if the bird is alive
    bool m_BetweenPipes;                        // Keeps track of if the bird is currently between pipes (used for score keeping)
private:    
    int m_Score;        // Track the bird's current score
    Vector2 m_Pos;      // Bird's position
    Vector2 m_Vel;      // Bird's velocity
    float m_Gravity;    // Acceleration due to gravity on the Bird's vertical velocity (added)

    // Remove these when switching to images
    Color m_Color;      // Color of the bird
    float m_Radius;     // Radius of the bird
public: 
    Bird(Vector2 startPos, Color c, float gravity);
    void Draw() const;          // Display the bird
    void Move();                // Move the bird and check if it goes outside window
    void Jump(float upVel);     // Set vertical velocity to simulate flapping
    Vector2 GetPos() const;     // Get the position of the bird
    float GetRadius() const;    // Get the radius of the bird
    void SetVel(Vector2 vel);   // Set the velocity, used when the bird dies

    void IncreaseScore();       // Increase the score by one if bird is alive
    int GetScore() const;       // Obtain the value of the current score
};

class Pipe  {
public:
    static constexpr float pipeWidth = 35;
    static constexpr float stdPipeOpening = 200;
private:
    float m_xPos;           // X position of the pipe pair
    float m_OpeningTop;     // Y position of the bottom of the top pipe
    float m_OpeningBot;     // Y position of the top of the bottom pipe
public:
    Pipe(float xPos, float openningTop);
    Pipe(float xPos, float openningTop, float openningBot);

    void Move(float xDiff);     // Moves the pipe pair by xDiff
    void Draw() const;          // Draws the pipe pair

    float GetX() const;             // Gets the current x position of this pipe pair
    float GetOpeningTop() const;    // Gets the y pos of the bottom of the top pipe
    float GetOpeningBot() const;    // Gets the y pos of the top of the bottom pipe
};

class Pipes {
public:
    float m_xVel;           // Stores the x velocity of all pipes
private:    
    float m_xSpace;             // Stores the distance between front edges of new pipes
    float m_highestOpening;     // Stores the highest (on the screen, lowest numerically) that the top pipe can end
    float m_lowestOpening;      // Stores the lowest (on the screen, highest numerically) that the top pipe can end
    std::list<Pipe> m_Pipes;    // Doubly linked list of all pipes
public:
    Pipes(float xSpace, float xVel, float highestOpening, float lowestOpening);

    void MoveAll();             // Move all the pipes within m_Pipes using Move() with m_xVel
    void DrawAll() const;       // Display all pipes within m_Pipes

    void BirdCollide(Bird &b) const;    // Check if the bird passed has hit (should die) or passed through (should increment score)
private:
    void GeneratePair(float startX);    // Calculates a random pipe pair and adds it to m_Pipes
};
