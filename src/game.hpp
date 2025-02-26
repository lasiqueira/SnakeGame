#pragma once
#include <cstdint>
#include <vector>
#include "SDL3/SDL.h"
#include "entities.hpp"

#define SPEED 1
#define SIZE 20
#define SNAKE_MAX 24000


struct Game 
{
    SDL_Window *window_;
    SDL_Renderer *renderer_;
    bool running_;
    uint16_t window_width_ = 800;
    uint16_t window_height_ = 600;
    std::vector<Snake> snake_;
    Apple apple_;
    Vec2 velocity_;
    double move_timer_ = 0;
    double move_interval_ = 0.1;
    bool Init();
    void Quit();
    void Run();

    void HandleEvents();
    void HandleKeyPress(SDL_Keycode key);
    void Render();
    void Update(double delta_time);
    void SpawnSnake();
    void GrowSnake();
    void SpawnApple();
    void MoveSnake();
    void CheckCollision();
    void MoveSnakeHead();
    void Reset();
};