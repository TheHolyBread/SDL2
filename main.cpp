#include <SDL2/SDL.h>
#include <iostream>
#include <list>

int renderCheese(std::list<int> &cheeses);

int main(int argc, char *argv[])
{
 
    // if error
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "error initializing SDL: " << SDL_GetError() << '\n';
    }

    // make window
    SDL_Window* win = SDL_CreateWindow("Most Montery", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920 / 2, 1080 / 2, 0);

    // make renderer
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);


    // read image
    SDL_Surface* moeImg = SDL_LoadBMP("./moe.bmp");
    SDL_Surface* cheeseImg = SDL_LoadBMP("./cheese.bmp");
    SDL_Surface* bulletImg = SDL_LoadBMP("./bullet.bmp");


    // turn it into a texture
    SDL_Texture* moeTexture = SDL_CreateTextureFromSurface(rend, moeImg);
    SDL_Texture* cheeseTexture = SDL_CreateTextureFromSurface(rend, cheeseImg);
    SDL_Texture* bulletTexture = SDL_CreateTextureFromSurface(rend, bulletImg);

    // setup player rectangle
    SDL_Rect moe;
    SDL_QueryTexture(moeTexture, NULL, NULL, &moe.w, &moe.h);
    moe.w *= 6;
    moe.h *= 6;

    moe.x = 50;
    moe.y = 1080 / 4;

    int speed = 300;
    int distanceRan = 0;
    float pvel = 0;

    // setup obstacles and rewards
    SDL_Rect cheese;
    SDL_QueryTexture(cheeseTexture, NULL, NULL, &cheese.w, &cheese.h);
    cheese.w *= 6;
    cheese.h *= 6;

    SDL_Rect bullet;
    SDL_QueryTexture(bulletTexture, NULL, NULL, &bullet.w, &bullet.h);
    bullet.w *= 6;
    bullet.h *= 6;

    // game loop
    bool running = true;
    bool spacing = false;
    std::cout << "game started omg";
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
                    break;
                case SDL_KEYUP:
                    case SDL_SCANCODE_SPACE:
                        spacing = false;
                        break;
                    break;
            }
        }
        if (spacing && moe.y == 1080 / 4) {
            pvel = 10;
        }
        pvel -= 0.4;
        moe.y -= pvel;
        moe.y = __min(moe.y, 1080 / 4);
        distanceRan++;

        SDL_SetRenderDrawColor(rend, 83, 178, 237, 255);
        SDL_RenderClear(rend);

        SDL_Rect ground;
        ground.x = 0;
        ground.y = 1080 / 4 + (16 * 6);
        ground.w = 1920 / 2;
        ground.h = 1080 / 4;
        SDL_SetRenderDrawColor(rend, 44, 199, 54, 255 );
        SDL_RenderFillRect(rend, &ground);

        SDL_RenderCopy(rend, moeTexture, NULL, &moe);
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / 60);
    }
 
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

int renderCheese(std::list<int> &cheeses) {
    return 0;
}