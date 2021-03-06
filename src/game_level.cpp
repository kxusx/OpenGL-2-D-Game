/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game_level.h"

#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;


void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight)
{
    // clear old data
    this->Bricks.clear();
    // load from file
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    for (GameObject &tile : this->Bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height; 
    int id = 1;
    // cout <<"levelWidth: " << levelWidth << " levelHeight: " << levelHeight << endl;
    // cout<<"width: "<<unit_width<<" height: "<<unit_height<<endl;
    // initialize level tiles based on tileData		
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1) // solid
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                int type = 1;
                GameObject obj(id,type,pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                obj.IsSolid = true;
                this->Bricks.push_back(obj);
                id++;
            }
            else if (tileData[y][x] == 2)	// coin
            {
                glm::vec3 color = glm::vec3(0.2f, 0.6f, 1.0f);
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                int type = 2;
                this->Bricks.push_back(GameObject(id,type,pos, size, ResourceManager::GetTexture("coin"), color));
                id++;
            }
            else if(tileData[y][x]==3){ // enemy
                glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f);
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                int type = 3;
                GameObject obj(id,type,pos, size, ResourceManager::GetTexture("enemy"), color);
                obj.IsSolid = true;
                this->Bricks.push_back(obj);
                id++;
            }else if(tileData[y][x]==4){ // door
                glm::vec3 color = glm::vec3(0.7f, 0.7f, 0.7f);
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                int type = 4;
                GameObject obj(id,type,pos, size, ResourceManager::GetTexture("door"), color);
                obj.IsSolid = true;
                this->Bricks.push_back(obj);
                id++;
            }else if(tileData[y][x]==5){ //win
                glm::vec3 color = glm::vec3(0.9f, 0.9f, 0.9f);
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                int type = 5;
                GameObject obj(id,type,pos, size, ResourceManager::GetTexture("win"), color);
                obj.IsSolid = true;
                this->Bricks.push_back(obj);
                id++;
            }
        }
    }
}