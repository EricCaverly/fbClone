#include "flappy.hpp"
#include <raylib.h>
#include <string>

// Title is currently unused
enum class GameState {
    Title, 
    InProg,
    GameOver
};

Bird createPlayer() {
    return Bird({Options::screenWidth/4.0, Options::screenHeight/2.0}, {255,255,0,255}, 0.3);
}

Pipes createPipes() {
    return Pipes(230, Options::basePipeSpeed, 50, Options::screenHeight-Pipe::stdPipeOpening-50);
}

int main() {
    // Game control variables
    GameState state = GameState::InProg;    
    // Setup window
    InitWindow(Options::screenWidth, Options::screenHeight, "Flappy Bird");
    SetTargetFPS(Options::targetFps);

    // Game objects
    Bird player = createPlayer();
    Pipes pipes = createPipes();

    while (!WindowShouldClose()) {
        // User Input
        if (state==GameState::InProg) {
            if(IsKeyPressed(KEY_SPACE)) {
                player.Jump(Bird::defaultJump);
            }
            pipes.BirdCollide(player);
        } else if (state == GameState::GameOver) {
            if (IsKeyPressed(KEY_R)) {
                player = createPlayer();
                pipes = createPipes();
                state = GameState::InProg;
            }
        }

        // Move pipes and player all the time
        player.Move();
        pipes.MoveAll();
        
        // Increase pipe speed with score
        pipes.m_xVel = Options::basePipeSpeed-(int)(player.GetScore()/10);

        // Check if player is now dead
        if (state==GameState::InProg && !player.m_Alive) {
            state = GameState::GameOver;
        }

        // Draw to the screen
        BeginDrawing();
        ClearBackground(Options::bgColor);

        // Draw player
        player.Draw();

        // Draw all pipes
        pipes.DrawAll();

        // Display current game state information
        switch(state) {
        case GameState::Title:
            break;
        case GameState::InProg:
            DrawText(("Score: "+std::to_string(player.GetScore())).c_str(), 20, 20, 30, BLACK);
            break;
        case GameState::GameOver:
            DrawText(("GAME OVER ["+std::to_string(player.GetScore())+"]").c_str(), 20, 20, 40, BLACK);
            DrawText("press 'r' to restart", 20, 60, 30, BLACK);
            break;
        }

        // Render
        EndDrawing();
    }

    // Clean up
    CloseWindow();
    return 0;
}