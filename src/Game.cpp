#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "./Components/TransformComponents.h"
#include "../lib/glm/glm.hpp"


EntityManager manager;
SDL_Renderer* Game::renderer;

Game::Game()
{
    this->isRunning = false;
}

Game::~Game()
{
}

bool Game::IsRunning() const
{
    return this->isRunning;
}

void Game::Initialize(int width, int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS);

    if (!window)
    {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    LoadLevel(0);

    isRunning = true;
    return;
}

void Game::LoadLevel(int levelNumber) {    
    Entity& newEntity(manager.AddEntity("projectile"));
    newEntity.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
}

void Game::ProcessInput()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
        case SDL_QUIT:
        {
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN:
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
            }
        }
        default: {
            break;
        }
    }
}

void Game::Update() {

    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(timeToWait);
    }
    
    

    float deltaTime = (SDL_GetTicks() -  ticksLastFrame) / 1000.0f;

    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    ticksLastFrame = SDL_GetTicks();

    manager.Update(deltaTime);

   
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    if (manager.HasNoEntitites())
    {
        return;
    }

    manager.Render();
    

    SDL_RenderPresent(renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}