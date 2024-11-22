#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char *argv[])
{
 
    // if error
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "error initializing SDL: " << SDL_GetError() << '\n';
    }

    // make window
    SDL_Window* win = SDL_CreateWindow("Bagel Dash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920 / 2, 1080 / 2, 0);

    // make renderer
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // read image
    SDL_Surface* img = SDL_LoadBMP("./bagel.bmp");

    // turn it into a texture
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, img);

    // setup player rectangle
    SDL_Rect plr;
    SDL_QueryTexture(tex, NULL, NULL, &plr.w, &plr.h);
    plr.w /= 10;
    plr.h /= 10;

    plr.x = 50;
    plr.y = 1920 / 4;

    int speed = 300;
    int pvel = 0;


    // game loop
    bool running = true;
    bool spaced = false;
    bool spacing = false;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_SPACE:
                            spacing = true;
                            break;
                        default:
                            break;
                    }
                case SDL_KEYUP:
                    case SDL_SCANCODE_SPACE:
                        spacing = false;
                        break;
            }
        }
        if (spacing && plr.y == 1920 / 4) {
            spaced = true;
        }
        if (spaced) {
            spaced = false;
            pvel = 10;
        }
        pvel--;
        plr.y -= pvel;
        plr.y = __min(plr.y, 1920 / 4);
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, tex, NULL, &plr);
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / 60);
    }
 
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}