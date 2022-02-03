/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"



// Game-related State data
SpriteRenderer  *Renderer;
GameObject      *Player;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    delete Renderer;
    delete Player;
}
void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("/Users/khushpatel/Documents/Computer Graphics/CG-Assignment1/shaders/sprite.vs", "/Users/khushpatel/Documents/Computer Graphics/CG-Assignment1/shaders/sprite.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader myShader;
    myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);
    // load textures
    ResourceManager::LoadTexture("/Users/khushpatel/Documents/Computer Graphics/CG-Assignment1/textures/awesomeface.png", true, "face");
    
    ResourceManager::LoadTexture("/Users/khushpatel/Documents/Computer Graphics/CG-Assignment1/textures/white1.png",false, "background");

    ResourceManager::LoadTexture("/Users/khushpatel/Documents/Computer Graphics/CG-Assignment1/textures/icyterrain.jpeg", false, "block_solid");
    ResourceManager::LoadTexture("/Users/khushpatel/Documents/Computer Graphics/CG-Assignment1/textures/icyterrain.jpeg", false, "block");

    ResourceManager::LoadTexture("/Users/khushpatel/Documents/Computer Graphics/CG-Assignment1/textures/grassterrain.png", true, "paddle");

    GameLevel one; one.Load("/Users/khushpatel/Documents/Computer Graphics/CG-Assignment1/levels/one.lvl", this->Width, this->Height / 2);
    GameLevel two; two.Load("/Users/khushpatel/Documents/Computer Graphics/CG-Assignment1/levels/two.lvl", this->Width, this->Height / 2);
    GameLevel three; three.Load("/Users/khushpatel/Documents/Computer Graphics/CG-Assignment1/levels/three.lvl", this->Width, this->Height / 2);
    GameLevel four; four.Load("/Users/khushpatel/Documents/Computer Graphics/CG-Assignment1/levels/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    

}

void Game::Update(float dt)
{
    // check for collisions
    this->DoCollisions();
}  

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f){
                Player->Position.x -= velocity;
                if (DoCollisions())
					Player->Position.x += velocity;
            }
                
            
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x){
                Player->Position.x += velocity;
                if (DoCollisions())
					Player->Position.x -= velocity;
            }
                
        }
         if (this->Keys[GLFW_KEY_W])
        {
            if (Player->Position.y >= 0.0f){
                Player->Position.y -= velocity;
                if (DoCollisions())
                    Player->Position.y += velocity;
            }
                
        }
        if (this->Keys[GLFW_KEY_S])
        {
            if (Player->Position.y <= this->Height - Player->Size.y)
                {Player->Position.y += velocity;
                if (DoCollisions())
                    Player->Position.y -= velocity;
                }
        }
    }
} 

bool CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}  

bool Game::DoCollisions()
{
    for (GameObject &box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            if (CheckCollision(*Player, box))
            {
                if (!box.IsSolid)
                    box.Destroyed = true; 
                return true;
            }
        }
    }
    return false;
}

void Game::Render()
{
    Shader myShader;
    Texture2D myTexture;
    if(this->State == GAME_ACTIVE){
        myTexture = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(myTexture, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);

        this->Levels[this->Level].Draw(*Renderer);
        // draw player
        Player->Draw(*Renderer);
    
    }
    
}