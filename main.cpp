#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char *argv[])
{
 
    // if error
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "error initializing SDL: " << SDL_GetError() << '\n';
    }

    // make window
    SDL_Window* win = SDL_CreateWindow("RACISM SIMULATOR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920 / 2, 1080 / 2, 0);

    // make renderer
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // read image
    SDL_Surface* img = SDL_LoadBMP("./bagel.bmp");

    // turn it into a texture
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, img);

    // setup player rectangle
    int x = 0;
    int y = 0;
    SDL_Rect plr;
    SDL_QueryTexture(tex, NULL, NULL, &plr.w, &plr.h);
    plr.w /= 10;
    plr.h /= 10;

    plr.x = 0;
    plr.y = 0;

    int speed = 300;


    // game loop
    bool running = true;

    while (running) {
        SDL_Event event;
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, tex, NULL, &plr);
        SDL_RenderPresent(rend);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
        SDL_Delay(1000 / 60);
    }
 
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}