#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>
#include <list>
#include <bits/stdc++.h>
using std::list;
using std::cout;

#define WIDTH 1920 / 2
#define HEIGHT 1080 / 2

#define SPEED 10

int renderCheese(SDL_Renderer* rend, SDL_Texture* tex, list<int> &cheeses, int distanceRan);
int renderBullet(SDL_Renderer* rend, SDL_Texture* tex, list<int> &bullets, int distanceRan);

int main(int argc, char *argv[])
{
 
    list<int> cheeses {};
    list<int> bullets {};
    
    int score { 0 };

    double scorey { 0 };

    srand(time(NULL));

    // if error
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "error initializing SDL: " << SDL_GetError() << '\n';
    }
    if (TTF_Init() != 0) {
        cout << "error initializing SDL TTF Renderer: " << SDL_GetError() << '\n';
    }

    // make window
    SDL_Window* win = SDL_CreateWindow("Most Monterey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    // make renderer
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("./fonts/Tiny5-Regular.ttf", 64);
    SDL_Surface* scoredisp;
    SDL_Texture* textture;
    SDL_Rect scorebox;

    // read image
    SDL_Surface* moeImg = SDL_LoadBMP("./sprites/moe.bmp");
    SDL_Surface* cheeseImg = SDL_LoadBMP("./sprites/cheese.bmp");
    SDL_Surface* bulletImg = SDL_LoadBMP("./sprites/piperbullet.bmp");


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
    moe.y = HEIGHT / 2;

    int distanceRan = 0;
    float pvel = 0;


    // game loop
    bool running = true;
    bool spacing = false;
    bool canSpace = true;

    cout << "game started omg" << '\n';
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
                            if (event.key.repeat) {
                                if (moe.y > HEIGHT / 2 - 10) spacing = false;
                                break;
                            }
                            spacing = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    case SDL_SCANCODE_SPACE:
                        spacing = false;
                        pvel = -5;
                        break;
                    break;
            }
        }
        if (spacing && moe.y == HEIGHT / 2) {
            pvel = 10;
        }
        if (moe.y == HEIGHT / 2) {
            moe.x = 50 + (SDL_sin(distanceRan / SPEED / 5) * 5);
        } else {
            moe.x = 50;
        }
        if (spacing) {
            pvel -= 0.4;
        } else {
            pvel -= 2;
        }
        moe.y -= pvel;
        moe.y = __min(moe.y, HEIGHT / 2);
        for (int x : bullets) {
            if (x - distanceRan * 1.2 < moe.x + 96 && x - distanceRan * 1.2 + 48 > moe.x && moe.y > HEIGHT / 2 - 48) {
                cout << "die" << '\n';
                running = false;
            }
        }
        for (int x : cheeses) {
            if (x - distanceRan < moe.x + 96 && x - distanceRan > moe.x && moe.y > HEIGHT / 2 - 96) {
                cout << "cheese" << '\n';
                list<int> temp {};
                for (int i : cheeses) {
                    if (i != x) {
                        temp.push_back(i);
                    }
                }
                cheeses = temp;
                score++;
                scorey = 32;
            }
        }
        distanceRan += SPEED;

        if ((distanceRan / SPEED) % 50 == 0) {
            if ((rand() % 2) + 1 == 1) {
                cheeses.push_back(distanceRan + (WIDTH));
                cout << "new cheese" << '\n';
            } else {
                bullets.push_back(distanceRan * 1.2 + (WIDTH));
                cout << "new bullet" << '\n';
            }
        }
        SDL_SetRenderDrawColor(rend, 83, 178, 237, 255);
        SDL_RenderClear(rend);

        SDL_Rect ground;
        ground.x = 0;
        ground.y = HEIGHT / 2 + (15 * 6);
        ground.w = WIDTH;
        ground.h = HEIGHT / 2;
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255 );
        SDL_RenderFillRect(rend, &ground);

        ground.x = 0;
        ground.y = HEIGHT / 2 + (16 * 6);
        ground.w = WIDTH;
        ground.h = HEIGHT / 2;
        SDL_SetRenderDrawColor(rend, 44, 199, 54, 255 );
        SDL_RenderFillRect(rend, &ground);

        SDL_Color black = {(uint8_t)std::round(255 * (scorey/32)),(uint8_t)std::round(255 * (scorey/32)),(uint8_t)std::round(255 * (scorey/32))};
        scoredisp = TTF_RenderText_Solid(font, std::to_string(score).c_str(), black);
        textture = SDL_CreateTextureFromSurface(rend, scoredisp);
        scorey += (0 - scorey) / 20;
        scorebox.x = 32;
        scorebox.y = scorey;
        scorebox.w = scoredisp->w;
        scorebox.h = scoredisp->h;

        SDL_RenderCopy(rend, moeTexture, NULL, &moe);
        renderCheese(rend, cheeseTexture, cheeses, distanceRan);
        renderBullet(rend, bulletTexture, bullets, distanceRan);
        SDL_RenderCopy(rend, textture, NULL, &scorebox);
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / 60);
    }
 
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

int renderCheese(SDL_Renderer* rend, SDL_Texture* tex, list<int> &cheeses, int distanceRan) {
    SDL_Rect cheese;
    SDL_QueryTexture(tex, NULL, NULL, &cheese.w, &cheese.h);
    cheese.w *= 6;
    cheese.h *= 6;
    for (int x : cheeses) {
        cheese.y = HEIGHT / 2  + (SDL_sin((x - distanceRan) / SPEED / 10) * 10);
        cheese.x = x - distanceRan;
        SDL_RenderCopy(rend, tex, NULL, &cheese);
    }
    return 0;
}
int renderBullet(SDL_Renderer* rend, SDL_Texture* tex, list<int> &bullets, int distanceRan) {
    SDL_Rect bullet;
    SDL_QueryTexture(tex, NULL, NULL, &bullet.w, &bullet.h);
    bullet.w *= 6;
    bullet.h *= 6;
    for (int x : bullets) {
        bullet.y = HEIGHT / 2  + (SDL_sin((x - distanceRan) / SPEED) * 4);
        bullet.x = x - distanceRan * 1.2 + (SDL_sin(((x - distanceRan) * 1.14159265) / SPEED) * 2);
        SDL_RenderCopy(rend, tex, NULL, &bullet);
    }
    return 0;
}