#include <iostream>
#include "game.h"

SDL_Rect ball;
SDL_Rect paddle1;
SDL_Rect paddle2;
int speed = 2;
int player_speed = 50;

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

    ball.x = 20;
    ball.y = 20;
    ball.h = 20;
    ball.w = 20;

    paddle1.x = window_width/2;
    paddle1.y = window_height - 20;
    paddle1.h = 20;
    paddle1.w = 200;

    paddle2.x = window_width/2;
    paddle2.y = 0;
    paddle2.h = 20;
    paddle2.w = 200;
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

void Game::init(const char* title, int widthi, int heighti){
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title, 0, 0, widthi, heighti, 0);
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    window_height = heighti;
    window_width = widthi;
    isRunning = true;

}
void Game::handleEvents(){

    SDL_Event event;

    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            isRunning = false;
        }
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_LEFT:
                    if (paddle1.x - player_speed >= 0){
                        paddle1.x -= player_speed;
                    }
                    break;
                case SDLK_RIGHT:
                    if (paddle1.x + paddle1.w + player_speed <= window_width){
                        paddle1.x += player_speed;
                    }
                    break;
                case SDLK_a:
                    if (paddle2.x - player_speed >= 0){
                        paddle2.x -= player_speed;
                    }
                    break;
                case SDLK_d:
                    if (paddle2.x + paddle2.w + player_speed <= window_width){
                        paddle2.x += player_speed;
                    }
                    break;
                
            }
        }

    }

}
float dx = 1.0f;
float dy = 1.0f;

void Game::update(){

    ball.x += speed * dx;
    ball.y += speed * dy;

    if (ball.y + ball.h >= window_height){
        isRunning = false;
        winnerTop = true;
    }

    if (ball.y <= 0){
        isRunning = false;
        winnerTop = false;
        
    }

    if (ball.x + ball.w >= window_width){
        dx *= -1.05f;
    }

    if (ball.x <= 0){
        dx *= -1.05f;
    }

    if (ball.y + ball.h >= paddle1.y && ball.x + ball.w >= paddle1.x && ball.x <= paddle1.x + paddle1.w){
        dy *= -1.05f;
        dy *= 1.05f;
    }

    if (ball.y <= paddle2.h && ball.x + ball.w >= paddle2.x && ball.x <= paddle2.x + paddle2.w){
        dy *= -1.05f;
        dy *= 1.05f;
    }

    ball.x += speed * dx;
    ball.y += speed * dy;

}
void Game::render(){

    SDL_SetRenderDrawColor(renderer, 50, 50, 100, 1);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 200, 200, 255, 1);
    SDL_RenderFillRect(renderer, &ball);
    SDL_RenderFillRect(renderer, &paddle1);
    SDL_RenderFillRect(renderer, &paddle2);

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