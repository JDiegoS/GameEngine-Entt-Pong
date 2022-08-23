#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class STexture{
    public:
        STexture(SDL_Renderer* gRenderer, SDL_Window* gWindow);
        ~STexture();
        load(std::string path);

        render(SDL_Rect* dst, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
}