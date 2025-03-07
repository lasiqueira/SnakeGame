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
    
    velocity_.y_ = -SPEED;
    velocity_.x_ = 0;
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
            if(velocity_.x_ == 0)
            {
                velocity_.x_ = -SPEED;
                velocity_.y_ = 0;
            }
            break;
        case SDLK_RIGHT:
        case SDLK_D:
            if(velocity_.x_ == 0)
            {
                velocity_.x_ = SPEED;
                velocity_.y_ = 0;
            }
            break;
        case SDLK_UP:
        case SDLK_W:
            if(velocity_.y_ == 0)
            {
                velocity_.x_ = 0;
                velocity_.y_ = -SPEED;
            }
            break;
        case SDLK_DOWN:
        case SDLK_S:
            if(velocity_.y_ == 0)
            {
                velocity_.x_ = 0;
                velocity_.y_ = SPEED;
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
    move_timer_ += delta_time;
    if(move_timer_ >= move_interval_)
    {
        move_timer_ -= move_interval_; 
        MoveSnake();
        CheckCollision();
        CheckWinCondition();
    }

}

void Game::SpawnApple()
{
    bool valid_position = false;

    while (!valid_position)
    {
        valid_position = true;
        apple_.rect_.w = SIZE;
        apple_.rect_.h = SIZE;
        apple_.rect_.x = (rand() % (window_width_ / SIZE)) * SIZE;
        apple_.rect_.y = (rand() % (window_height_ / SIZE)) * SIZE;

        for (const auto& segment : snake_)
        {
            if (apple_.rect_.x == segment.rect_.x && apple_.rect_.y == segment.rect_.y)
            {
                valid_position = false;
                break;
            }
        }
    }
}

void Game::SpawnSnake()
{
    snake_.reserve(SNAKE_MAX);
    GameEntity snake_head;
    
    snake_head.rect_.x = window_width_/2;
    snake_head.rect_.y = window_height_/2;
    snake_head.rect_.w = SIZE;
    snake_head.rect_.h = SIZE;
    snake_.emplace_back(std::move(snake_head));
}

void Game::CheckCollision()
{
    CollisionWithApple();
    CollisionWithSelf();
}

void Game::GrowSnake()
{
    GameEntity new_segment;
    new_segment.rect_.x = snake_[snake_.size()-1].rect_.x;
    new_segment.rect_.y = snake_[snake_.size()-1].rect_.y;
    new_segment.rect_.w = SIZE;
    new_segment.rect_.h = SIZE;
    snake_.emplace_back(std::move(new_segment));
}

void Game::CollisionWithApple()
{
    if(snake_[0].rect_.x < apple_.rect_.x + apple_.rect_.w &&
        snake_[0].rect_.x + snake_[0].rect_.w > apple_.rect_.x &&
        snake_[0].rect_.y < apple_.rect_.y + apple_.rect_.h &&
        snake_[0].rect_.y + snake_[0].rect_.h > apple_.rect_.y)
     {
        GrowSnake();        
        SpawnApple();
     }
}

void Game::CollisionWithSelf()
{
    for(int i = 2; i < snake_.size(); i++)
    {
        if(snake_[0].rect_.x < snake_[i].rect_.x + snake_[i].rect_.w &&
            snake_[0].rect_.x + snake_[0].rect_.w > snake_[i].rect_.x &&
            snake_[0].rect_.y < snake_[i].rect_.y + snake_[i].rect_.h &&
            snake_[0].rect_.y + snake_[0].rect_.h > snake_[i].rect_.y)
        {
            SDL_Log("You Lose!");
            SDL_Delay(1000);
            Reset();
        }
    }
}
void Game::MoveSnake()
{
    for(int i = snake_.size()-1; i > 0; i--)
    {
        snake_[i].rect_.x = snake_[i-1].rect_.x;
        snake_[i].rect_.y = snake_[i-1].rect_.y;

    }
    MoveSnakeHead();
}
void Game::MoveSnakeHead()
{
    snake_[0].rect_.x += velocity_.x_ * SIZE;
    snake_[0].rect_.y += velocity_.y_ * SIZE;

    if(snake_[0].rect_.x < 0)  snake_[0].rect_.x = window_width_ - SIZE;
    if(snake_[0].rect_.x >= window_width_)  snake_[0].rect_.x = 0;
    if(snake_[0].rect_.y < 0)  snake_[0].rect_.y = window_height_ - SIZE;
    if(snake_[0].rect_.y >= window_height_)  snake_[0].rect_.y = 0;
}
void Game::Reset()
{
    snake_.clear();
    SpawnSnake();
    SpawnApple();
    velocity_.y_ = -SPEED;
    velocity_.x_ = 0;
}

void Game::CheckWinCondition()
{
    if(snake_.size() == SNAKE_MAX)
    {
        SDL_Log("You Win!");
        SDL_Delay(1000);
        Reset();
    }
}