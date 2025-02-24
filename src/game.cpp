#include "game.hpp"

bool Game::Init() {
    
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }
    if(!SDL_CreateWindowAndRenderer("Snake Game", window_width_, window_height_ , 0, &window_, &renderer_))
    {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        return false;
    }
    
    SDL_SetWindowPosition(window_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    
    SpawnSnake();
    SpawnApple();
    running_ = true;
    return true;
}
void Game::Run() 
{
    uint64_t last_step = 0;
    uint64_t current_step = SDL_GetPerformanceCounter();
    double delta_time = 0;
    while(running_) 
    {
        last_step = current_step;
        current_step = SDL_GetPerformanceCounter();
        delta_time = (double)(current_step - last_step) / SDL_GetPerformanceFrequency();
        HandleEvents();
        Update(delta_time);
        Render();
    }
}

void Game::Quit() 
{   
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
}

void Game::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {  
        switch (event.type) 
        {
        case SDL_EVENT_QUIT:
            running_ = false;
            break;
        
        case SDL_EVENT_KEY_DOWN:
            HandleKeyPress(event.key.key);
            break;
        default:
            break;
        }   
    }
}

void Game::HandleKeyPress(SDL_Keycode key) 
{
    switch(key) 
    {
        case SDLK_ESCAPE:
        case SDLK_Q:
            running_ = false;
            break;
        case SDLK_LEFT:
        case SDLK_A:
            if(snake_[0].velocity_.x_ == 0)
            {
                snake_[0].velocity_.x_ = -SPEED;
                snake_[0].velocity_.y_ = 0;
            }
            break;
        case SDLK_RIGHT:
        case SDLK_D:
            if(snake_[0].velocity_.x_ == 0)
            {
                snake_[0].velocity_.x_ = SPEED;
                snake_[0].velocity_.y_ = 0;
            }
            break;
        case SDLK_UP:
        case SDLK_W:
            if(snake_[0].velocity_.y_ == 0)
            {
                snake_[0].velocity_.x_ = 0;
                snake_[0].velocity_.y_ = -SPEED;
            }
            break;
        case SDLK_DOWN:
        case SDLK_S:
            if(snake_[0].velocity_.y_ == 0)
            {
                snake_[0].velocity_.x_ = 0;
                snake_[0].velocity_.y_ = SPEED;
            }
            break;
        default:
            break;
    }
}

void Game::Render() 
{
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
    
    for(int i = 0; i < snake_.size(); i++)
    {
        SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer_, &snake_[i].rect_);
    }

    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer_, &apple_.rect_);
    
    SDL_RenderPresent(renderer_);
}
void Game::Update(double delta_time) 
{
    snake_[0].Update(window_width_, window_height_, delta_time);
    CheckCollision();
    MoveSnake();
}

void Game::SpawnApple()
{
    apple_.rect_.x = rand() % window_width_;
    apple_.rect_.y = rand() % window_height_;
    apple_.rect_.w = SIZE;
    apple_.rect_.h = SIZE;
}

void Game::SpawnSnake()
{
    snake_.reserve(SNAKE_MAX);
    Snake snake;
    snake_.emplace_back(snake);
    snake_[0].velocity_.y_ = -SPEED;
    snake_[0].velocity_.x_ = 0;
    snake_[0].rect_.x = window_width_/2;
    snake_[0].rect_.y = window_height_/2;
    snake_[0].rect_.w = SIZE;
    snake_[0].rect_.h = SIZE;
}

void Game::CheckCollision()
{
    if(snake_[0].rect_.x < apple_.rect_.x + apple_.rect_.w &&
       snake_[0].rect_.x + snake_[0].rect_.w > apple_.rect_.x &&
       snake_[0].rect_.y < apple_.rect_.y + apple_.rect_.h &&
       snake_[0].rect_.y + snake_[0].rect_.h > apple_.rect_.y)
    {
        for(int i = 0; i < 100; i++) //temp solution to snake showing small growth
            GrowSnake();
        SpawnApple();
    }
}

void Game::GrowSnake()
{
    Snake new_snake;
    new_snake.rect_.x = snake_[snake_.size()-1].rect_.x;
    new_snake.rect_.y = snake_[snake_.size()-1].rect_.y;
    new_snake.rect_.w = SIZE;
    new_snake.rect_.h = SIZE;
    new_snake.velocity_ = snake_[snake_.size()-1].velocity_;
   
    snake_.emplace_back(new_snake);
}

void Game::MoveSnake()
{
    for(int i = snake_.size()-1; i > 0; i--)
    {
        snake_[i].rect_.x = snake_[i-1].rect_.x;
        snake_[i].rect_.y = snake_[i-1].rect_.y;
        snake_[i].velocity_ = snake_[i-1].velocity_;

    }
}

void Game::Reset()
{
    snake_.clear();
    SpawnSnake();
    SpawnApple();
}