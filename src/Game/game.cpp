#include <iostream>
#include "game.h"

#include "../entt/entt.hpp"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
int speed = 2;
int player_speed = 50;

entt::registry mRegistry;

entt::entity paddle1, paddle2;

struct PositionComponent {
    int x, y;
};

struct VelocityComponent {
    int x, y;
};

struct CubeComponent {
    int w, h;
};

struct PlayerComponent{
    int playerID;
};

struct BallComponent{
};

PositionComponent playerSpawnPosition = {20, 20};
VelocityComponent playerSpawnVelocity = {200, 200};
CubeComponent playerRect = {200, 20};

void createPlayerEntity(PositionComponent pos, VelocityComponent vel, CubeComponent rect, PlayerComponent player)
{
    const entt::entity e = mRegistry.create();
    mRegistry.emplace<PositionComponent>(e, pos);
    mRegistry.emplace<VelocityComponent>(e, vel);
    mRegistry.emplace<CubeComponent>(e, rect);  

    mRegistry.emplace<PlayerComponent>(e, player);  

    if (player.playerID == 1){
        paddle1 = e;
    }
    else{
        paddle2 = e;
    }



}
void createBallEntity(PositionComponent pos, VelocityComponent vel, CubeComponent rect)
{
    const entt::entity e = mRegistry.create();
    mRegistry.emplace<PositionComponent>(e, pos);
    mRegistry.emplace<VelocityComponent>(e, vel);
    mRegistry.emplace<CubeComponent>(e, rect);  

    mRegistry.emplace<BallComponent>(e); 

}


float dx = 1.0f;
float dy = 1.0f;
bool playing = true;
bool player1Won = false;

void ballMovementSystem() {
    /*
    int pad1x, pad1y, pad1w, pad2x, pad2h, pad2w;

    
    auto view = mRegistry.view<PlayerComponent, PositionComponent, CubeComponent>();
    for (const entt::entity e : view) {
        PositionComponent& pos = view.get<PositionComponent>(e);
        const CubeComponent& rect = view.get<CubeComponent>(e);
        const PlayerComponent& player = view.get<PlayerComponent>(e);

        if (player.playerID == 1){
            pad1x = pos.x;
            pad1y = pos.y;
            pad1w = rect.w;
        }
        else{
            pad2x = pos.x;
            pad2h = rect.h;
            pad2w = rect.w;
        }
    }
    */

    PositionComponent& pad1Position = mRegistry.get<PositionComponent>(paddle1);
    CubeComponent& pad1Rect = mRegistry.get<CubeComponent>(paddle1);
    PositionComponent& pad2Position = mRegistry.get<PositionComponent>(paddle2);
    CubeComponent& pad2Rect = mRegistry.get<CubeComponent>(paddle2);
    
    auto view2 = mRegistry.view<BallComponent, PositionComponent, CubeComponent>();
    for (const entt::entity e : view2) {
        PositionComponent& pos = view2.get<PositionComponent>(e);
        CubeComponent& rect = view2.get<CubeComponent>(e);

        pos.x += speed * dx;
        pos.y += speed * dy;

        if (pos.y + rect.h >= SCREEN_HEIGHT){
            playing = false;
            player1Won = true;
        }

        if (pos.y <= 0){
            playing = false;
            player1Won = false;
            
        }

        if (pos.x + rect.w >= SCREEN_WIDTH){
            dx *= -1.05f;
        }

        if (pos.x <= 0){
            dx *= -1.05f;
        }

        if (pos.y + rect.h >= pad1Position.y && pos.x + rect.w >= pad1Position.x && pos.x <= pad1Position.x + pad1Rect.w){
            dy *= -1.05f;
            dy *= 1.05f;
        }

        if (pos.y <= pad2Rect.h && pos.x + rect.w >= pad2Position.x && pos.x <= pad2Position.x + pad2Rect.w){
            dy *= -1.05f;
            dy *= 1.05f;
        }

        pos.x += speed * dx;
        pos.y += speed * dy;
    }
}

void cubeRenderSystem(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 255 ,255, 1);

  const auto view = mRegistry.view<PositionComponent, CubeComponent>();
  for (const entt::entity e : view) {
    const PositionComponent position = view.get<PositionComponent>(e);
    const CubeComponent cube = view.get<CubeComponent>(e);

    SDL_Rect rect = { position.x, position.y, cube.w, cube.h };    
    SDL_RenderFillRect(renderer, &rect);
  }
}

Game::Game ()
{
    std::cout << "WELCOME TO PONG!" << std::endl;
    std::cout << " " << std::endl;

    FPS = 60;
    frameDuration = (1.0f / FPS) * 1000.0f;
    counter = 0;
    dT = 0.0f;

}

Game::~Game ()
{
    std::cout << "~Game" << std::endl;
}
void Game::setup(){

    PositionComponent paddle1Position = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 20};
    VelocityComponent paddle1Velocity = {0, 0};
    CubeComponent paddle1Rect = {200, 20};
    PlayerComponent paddle1Player = {1};

    createPlayerEntity(paddle1Position, paddle1Velocity, paddle1Rect, paddle1Player);

    PositionComponent paddle2Position = {SCREEN_WIDTH/2, 0};
    VelocityComponent paddle2Velocity = {0, 0};
    CubeComponent paddle2Rect = {200, 20};
    PlayerComponent paddle2Player = {2};

    createPlayerEntity(paddle2Position, paddle2Velocity, paddle2Rect, paddle2Player);

    PositionComponent ballPosition = {20, 20};
    VelocityComponent ballVelocity = {0, 0};
    CubeComponent ballRect = {20, 20};

    createBallEntity(ballPosition, ballVelocity, ballRect);
}

void Game::frameStart(){
    frameStartTimeStamp = SDL_GetTicks();

    //Delta Time: tiempo desde el ultimo frame hasta este
    if (frameEndTimeStamp){
        dT = (frameEndTimeStamp - frameStartTimeStamp) /1000.0f;
    }
}

void Game::frameEnd(){
    frameEndTimeStamp = SDL_GetTicks();

    float actualFrameDuration = frameEndTimeStamp - frameStartTimeStamp;

    if (actualFrameDuration < frameDuration){
        SDL_Delay(frameDuration - actualFrameDuration);
    }
    counter++;


}

void Game::init(const char* title){
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    isRunning = true;

}
void Game::handleEvents(){

    SDL_Event event;



    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            isRunning = false;
        }
        if(event.type == SDL_KEYDOWN){
            PositionComponent& pad1Position = mRegistry.get<PositionComponent>(paddle1);
            CubeComponent& pad1Rect = mRegistry.get<CubeComponent>(paddle1);
            PositionComponent& pad2Position = mRegistry.get<PositionComponent>(paddle2);
            CubeComponent& pad2Rect = mRegistry.get<CubeComponent>(paddle2);

            switch(event.key.keysym.sym){
                case SDLK_LEFT:
                    if (pad1Position.x - player_speed >= 0){
                        pad1Position.x -= player_speed;
                    }
                    break;
                case SDLK_RIGHT:
                    if (pad1Position.x + pad1Rect.w + player_speed <= SCREEN_WIDTH){
                        pad1Position.x += player_speed;
                    }
                    break;
                case SDLK_a:
                    if (pad2Position.x - player_speed >= 0){
                        pad2Position.x -= player_speed;
                    }
                    break;
                case SDLK_d:
                    if (pad2Position.x + pad2Rect.w + player_speed <= SCREEN_WIDTH){
                        pad2Position.x += player_speed;
                    }
                    break;
                
            }
        }

    }

}


void Game::update(){


    ballMovementSystem();
    isRunning = playing;
    winnerTop = player1Won;

}
void Game::render(){

    SDL_SetRenderDrawColor(renderer, 50, 50, 100, 1);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 200, 200, 255, 1);

    cubeRenderSystem(renderer); 

    /*
    SDL_Surface* surface = IMG_Load("./assets/1.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, nullptr, &ball);

    SDL_DestroyTexture(texture);
    */

    SDL_RenderPresent(renderer);

}
void Game::clean(){

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "GAME OVER" << std::endl;
    if (winnerTop){
        std::cout << "TOP PLAYER WINS" << std::endl;
    }
    else{
        std::cout << "BOTTOM PLAYER WINS" << std::endl;
    }
    


}
bool Game::running(){
    return isRunning;
}