/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"

#include <stdlib.h>
#include <time.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "resource_manager.h"
#include "sprite_renderer.h"
using namespace std;

// Game-related State data
SpriteRenderer *Renderer;
GameObject *Player;

Game::Game(unsigned int width, unsigned int height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height) {}

Game::~Game() {
	delete Renderer;
	delete Player;
}
void Game::Init() {
	// load shaders
	this->points = 0;
	ResourceManager::LoadShader(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/shaders/sprite.vs",
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/shaders/sprite.frag",
		nullptr, "sprite");
	// configure shaders
	glm::mat4 projection =
		glm::ortho(0.0f, static_cast<float>(this->Width),
				   static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// set render-specific controls
	Shader myShader;
	myShader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(myShader);
	// load textures
	ResourceManager::LoadTexture(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/textures/awesomeface.png",
		true, "win");

	ResourceManager::LoadTexture(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/textures/white1.png",
		false, "background");

	ResourceManager::LoadTexture(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/textures/icyterrain.jpeg",
		false, "block_solid");
	ResourceManager::LoadTexture(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/textures/icyterrain.jpeg",
		false, "block");

	ResourceManager::LoadTexture(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/textures/grassterrain.png",
		true, "paddle");
	ResourceManager::LoadTexture(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/textures/coin1.png",
		true, "coin");
	ResourceManager::LoadTexture(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/textures/onix.png",
		true, "enemy");
	ResourceManager::LoadTexture(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/textures/thunderbolt.png",
		true, "door");

	GameLevel one;
	one.Load(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/levels/one.lvl",
		this->Width, this->Height);
	GameLevel two;
	two.Load(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/levels/two.lvl",
		this->Width, this->Height);
	GameLevel three;
	three.Load(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/levels/three.lvl",
		this->Width, this->Height);
	GameLevel four;
	four.Load(
		"/Users/khushpatel/Documents/Computer "
		"Graphics/CG-Assignment1/levels/four.lvl",
		this->Width, this->Height);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;
	// configure game objects

	glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
									this->Height - PLAYER_SIZE.y);
	Player = new GameObject(0, 1, playerPos, PLAYER_SIZE,
							ResourceManager::GetTexture("paddle"));
	Player->type = 0;
}

void Game::ProcessInput(float dt) {
	if (this->State == GAME_ACTIVE) {
		float velocity = PLAYER_VELOCITY * dt;
		// move playerboard
		if (this->Keys[GLFW_KEY_A]) {
			if (Player->Position.x >= 0.0f) {
				Player->Position.x -= velocity;
				if (DoCollisions())
					Player->Position.x += velocity;
				if(doorCollision()){
					Player->Position.x = this->Width / 2.0f - PLAYER_SIZE.x / 2.0f;
					Player->Position.y = this->Height - PLAYER_SIZE.y;
					this->Level++;
					if(this->Level == 4){
						this->Level = 0;
					}
				}
				if(winCollision()){
					this->State = GAME_WIN;
					cout<<"WIN";
				}
			}
		}
		if (this->Keys[GLFW_KEY_D]) {
			if (Player->Position.x <= this->Width - Player->Size.x) {
				Player->Position.x += velocity;
				if (DoCollisions())
					Player->Position.x -= velocity;
				if(doorCollision()){
					Player->Position.x = this->Width / 2.0f - PLAYER_SIZE.x / 2.0f;
					Player->Position.y = this->Height - PLAYER_SIZE.y;
					this->Level++;
					if(this->Level == 4){
						this->Level = 0;
					}
				}
				if(winCollision()){
					this->State = GAME_WIN;
					cout<<"WIN";
				}
			}
		}
		if (this->Keys[GLFW_KEY_W]) {
			if (Player->Position.y >= 0.0f) {
				Player->Position.y -= velocity;
				if (DoCollisions())
					Player->Position.y += velocity;
				if(doorCollision()){
					Player->Position.x = this->Width / 2.0f - PLAYER_SIZE.x / 2.0f;
					Player->Position.y = this->Height - PLAYER_SIZE.y;
					this->Level++;
					if(this->Level == 4){
						this->Level = 0;
					}
				}
				if(winCollision()){
					this->State = GAME_WIN;
					cout<<"WIN";
				}
			}
		}
		if (this->Keys[GLFW_KEY_S]) {
			if (Player->Position.y <= this->Height - Player->Size.y) {
				Player->Position.y += velocity;
				if (DoCollisions())
					Player->Position.y -= velocity;
				if(doorCollision()){
					Player->Position.x = this->Width / 2.0f - PLAYER_SIZE.x / 2.0f;
					Player->Position.y = this->Height - PLAYER_SIZE.y;
					this->Level++;
					if(this->Level == 4){
						this->Level = 0;
					}
				}
				if(winCollision()){
					this->State = GAME_WIN;
					cout<<"WIN";
				}
			}
		}
	}
}

bool CheckCollision(GameObject &one, GameObject &two)  // AABB - AABB collision
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

bool Game::DoCollisions() {
	for (GameObject &box : this->Levels[this->Level].Bricks) {
		if (!box.Destroyed) {
			if (CheckCollision(*Player, box)) {
				if (!box.IsSolid) {
					box.Destroyed = true;
				}
				if (box.type == 1) {  // hit wall
					return true;
				}
				if (box.type == 2) {  // hit coins
					this->points++;
					cout << this->points;
				}
				if (box.type == 3) {  // hit enemy
					this->State = GAME_OVER;
					cout << "Game Over";
				}
			}
		}
	}
	return false;
}

bool Game::enemyCollision(GameObject &enemy) {
	for (GameObject &box : this->Levels[this->Level].Bricks) {
		if (!box.Destroyed && box.id != enemy.id) {
			if (CheckCollision(enemy, box)) {
				// cout<<"TRUE"<<"ID"<<box.id<<" "<<enemy.id<<endl;
				return true;
			}
		}
	}
	return false;
}

bool Game::doorCollision() {
	for (GameObject &box : this->Levels[this->Level].Bricks) {
		if (box.type == 4) {
			if (CheckCollision(*Player, box))
				return true;
		}
	}
	return false;
}

bool Game::winCollision(){
	for (GameObject &box : this->Levels[this->Level].Bricks) {
		if (box.type == 5) {
			if (CheckCollision(*Player, box))
				return true;
		}
	}
	return false;
}

void Game::Update(float dt) {
	float velocity = PLAYER_VELOCITY * dt;
	srand(time(0));
	for (GameObject &box : this->Levels[this->Level].Bricks) {
		if (box.type == 3) {
			int direction = rand() % 4;
			// cout<<"Direction for " << box.Position.x << " " << box.Position.y
			// << " " << direction << endl;
			// cout<<"Initial "<<box.Position.x<<" "<<box.Position.y<<"
			// "<<box.Size.x<<endl;
			if (!box.Destroyed) {
				if (direction == 0) {
					// cout<<"x+\n";
					if (box.Position.x >= 0.0f) {
						box.Position.x -= velocity;
						if (enemyCollision(box)) {
							// cout<<":x+\n";
							box.Position.x += velocity;
						}
					}
					// cout<<box.Position.x<<" "<<box.Position.y<<endl;
				}

				if (direction == 1) {
					// cout<<"x-\n";
					if (box.Position.x <= this->Width - box.Size.x) {
						box.Position.x += velocity;
						if (enemyCollision(box)) {
							box.Position.x -= velocity;
							// cout<<":x-\n";
						}
					}
					// cout<<box.Position.x<<" "<<box.Position.y<<endl;
				}

				if (direction == 2) {
					// cout<<"y+\n";
					if (box.Position.y >= 0.0f) {
						box.Position.y -= velocity;
						if (enemyCollision(box)) {
							box.Position.y += velocity;
							// cout<<":y+\n";
						}
					}
					// cout<<box.Position.x<<" "<<box.Position.y<<endl;
				}

				if (direction == 3) {
					if (box.Position.y <= this->Height - box.Size.y) {
						// cout<<"y-\n";
						box.Position.y += velocity;
						if (enemyCollision(box)) {
							box.Position.y -= velocity;
							// cout<<":y-\n";
						}
					}
					// cout<<box.Position.x<<" "<<box.Position.y<<endl;
				}
			}
		}
	}
}

void Game::Render() {
	Shader myShader;
	Texture2D myTexture;
	if (this->State == GAME_ACTIVE) {
		myTexture = ResourceManager::GetTexture("background");
		Renderer->DrawSprite(myTexture, glm::vec2(0.0f, 0.0f),
							 glm::vec2(this->Width, this->Height), 0.0f);

		this->Levels[this->Level].Draw(*Renderer);
		// draw player
		Player->Draw(*Renderer);
	}
}