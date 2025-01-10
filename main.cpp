// compile command
// without console
// g++ -Isrc/Include -Lsrc/lib -o main main.cpp -mwindows -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
// with console
// g++ -Isrc/Include -Lsrc/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf





#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mouse.h>
#include <iostream>
#include <algorithm>
#include <list>
#include <math.h>
#include <chrono>
#include <cstdint>
using std::list;
using std::cout;
using std::to_string;



#define SPEED 10




int WIDTH { 1920 / 2 };
int HEIGHT { 1080 / 2 };
double SCALE { 6.0 };




int renderCheese(SDL_Renderer* rend, SDL_Texture* tex, list<int> &cheeses, int distanceRan);
int renderBullet(SDL_Renderer* rend, SDL_Texture* tex, list<int> &bullets, int distanceRan);
int get(list<int> &data, int index);
void updateScale(SDL_Window* win);
uint64_t milliTime();




int main(int argc, char *argv[])
{


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
    updateScale(win);
    



    // make renderer
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);





    TTF_Font* font = TTF_OpenFont("./fonts/Tiny5-Regular.ttf", 64);
    TTF_Font* titleFont = TTF_OpenFont("./fonts/pixel-comic-sans.ttf", 96);
    SDL_Surface* scoredisp;
    SDL_Texture* textture;
    SDL_Rect scorebox;





    // read image
    SDL_Surface* moeImg = SDL_LoadBMP("./sprites/moe.bmp");
    SDL_Surface* cheeseImg = SDL_LoadBMP("./sprites/cheese.bmp");
    SDL_Surface* bulletImg = SDL_LoadBMP("./sprites/piperbullet.bmp");

    SDL_Surface *cursurface = SDL_LoadBMP("./sprites/cheeseCursor.bmp");
    

    SDL_Cursor *cursor = SDL_CreateColorCursor(cursurface, 0, 0);
    int mouseX = 0;
    int mouseY = 0;
    SDL_GetGlobalMouseState(&mouseX, &mouseY);



    // turn it into a texture
    SDL_Texture* moeTexture = SDL_CreateTextureFromSurface(rend, moeImg);
    SDL_Texture* cheeseTexture = SDL_CreateTextureFromSurface(rend, cheeseImg);
    SDL_Texture* bulletTexture = SDL_CreateTextureFromSurface(rend, bulletImg);

    SDL_Texture* cursorTexture = SDL_CreateTextureFromSurface(rend, cursurface);
    SDL_Rect mcursor;
    SDL_QueryTexture(cursorTexture, NULL, NULL, &mcursor.w, &mcursor.h);




    // setup player rectangle
    SDL_Rect moe;
    SDL_QueryTexture(moeTexture, NULL, NULL, &moe.w, &moe.h);
    moe.w *= SCALE;
    moe.h *= SCALE;

    moe.x = 50;
    moe.y = HEIGHT / 2;

    int moey = moe.y;


    // big vars
    bool running { true };
    bool FULLSCREEN { false };

    cout << "game started omg" << '\n';

    while (running) {

        bool gameloop { true };
        bool spacing { false };
        bool canSpace { true };

        bool menu { true };
        bool gameover { false };
        int select { 0 };

        int distanceRan { 0 };
        float pvel { 0 };

        int score { 0 };
        double scorey { 0 };

        list<int> cheeses {};
        list<int> bullets {};

        while (gameloop) {
            bool enter { false };

            updateScale(win);
            SDL_QueryTexture(moeTexture, NULL, NULL, &moe.w, &moe.h);
            moe.w *= SCALE;
            moe.h *= SCALE;
            TTF_SetFontSize(font, 64 * SCALE / 6);
            TTF_SetFontSize(titleFont, 96 * SCALE / 6);

            SDL_Event event;

            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        running = false;
                        gameloop = false;
                        break;
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.scancode) {
                            case SDL_SCANCODE_SPACE:
                                if (event.key.repeat) {
                                    spacing = false;
                                    break;
                                } else {
                                    enter = true;
                                }
                                spacing = true;
                                break;
                            case SDL_SCANCODE_UP:
                                select--;
                                cout << "select up" << '\n';
                                break;
                            case SDL_SCANCODE_DOWN:
                                select++;
                                cout << "select down" << '\n';
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
                    case SDL_MOUSEMOTION:
                        SDL_GetGlobalMouseState(&mouseX, &mouseY);
                        break;
                }
            }
            mcursor.x = mouseX; //- SDL_GetWindowPosition();
            mcursor.y = mouseY;
            if (menu) {
                SDL_SetRenderDrawColor(rend, 83, 178, 237, 255);
                SDL_RenderClear(rend);

                if (select > 2) {
                    select = 0;
                } else if (select < 0) {
                    select = 2;
                }
                

                SDL_Color black = {0,0,0};
                SDL_Color white = {255,255,255};
                SDL_Color yellow = {252, 194, 3};
                scoredisp = TTF_RenderText_Solid(titleFont, "Most Monterey", yellow);
                textture = SDL_CreateTextureFromSurface(rend, scoredisp);
                SDL_Rect textbox;
                textbox.x = WIDTH / 2 - scoredisp->w / 2;
                textbox.y = 64;
                textbox.w = scoredisp->w;
                textbox.h = scoredisp->h;
                SDL_RenderCopy(rend, textture, NULL, &textbox);

                scoredisp = (select == 0) ? TTF_RenderText_Solid(font, "> Play <", white) : TTF_RenderText_Solid(font, "Play", black);
                textture = SDL_CreateTextureFromSurface(rend, scoredisp);
                textbox.x = WIDTH / 2 - scoredisp->w / 2;
                textbox.y = textbox.y + textbox.h + 32;
                textbox.w = scoredisp->w;
                textbox.h = scoredisp->h;
                SDL_RenderCopy(rend, textture, NULL, &textbox);

                scoredisp = (FULLSCREEN) ? ((select == 1) ? TTF_RenderText_Solid(font, "> Unfullscreen <", white) : TTF_RenderText_Solid(font, "Unfullscreen", black)) : ((select == 1) ? TTF_RenderText_Solid(font, "> Fullscreen <", white) : TTF_RenderText_Solid(font, "Fullscreen", black));
                textture = SDL_CreateTextureFromSurface(rend, scoredisp);
                textbox.x = WIDTH / 2 - scoredisp->w / 2;
                textbox.y = textbox.y + textbox.h + 32;
                textbox.w = scoredisp->w;
                textbox.h = scoredisp->h;
                SDL_RenderCopy(rend, textture, NULL, &textbox);

                scoredisp = (select == 2) ? TTF_RenderText_Solid(font, "> Quit <", white) : TTF_RenderText_Solid(font, "Quit", black);
                textture = SDL_CreateTextureFromSurface(rend, scoredisp);
                textbox.x = WIDTH / 2 - scoredisp->w / 2;
                textbox.y = textbox.y + textbox.h + 32;
                textbox.w = scoredisp->w;
                textbox.h = scoredisp->h;
                SDL_RenderCopy(rend, textture, NULL, &textbox);

                if (enter) {
                    switch (select) {
                        case 0:
                            menu = false;
                            spacing = false;
                            break;
                        case 1:
                            FULLSCREEN = !FULLSCREEN;
                            FULLSCREEN ? SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP) : SDL_SetWindowFullscreen(win, 0);
                            break;
                        case 2:
                            running = false;
                            gameloop = false;
                            break;
                    }
                }
            } else if (gameover) {
                int endScoreX = 16 * sin(milliTime() / 100.0);
                int endScoreY = 16 * cos(milliTime() / 170.0);
                int outline = 16.0 * SCALE / 6.0;
                SDL_SetRenderDrawColor(rend, 83, 178, 237, 255);
                SDL_RenderClear(rend);

                SDL_Rect ground;
                ground.x = 0;
                ground.y = HEIGHT / 2 + (15 * SCALE);
                ground.w = WIDTH;
                ground.h = HEIGHT / 2;
                SDL_SetRenderDrawColor(rend, 0, 0, 0, 255 );
                SDL_RenderFillRect(rend, &ground);

                ground.x = 0;
                ground.y = HEIGHT / 2 + (16 * SCALE);
                ground.w = WIDTH;
                ground.h = HEIGHT / 2;
                SDL_SetRenderDrawColor(rend, 44, 199, 54, 255 );
                SDL_RenderFillRect(rend, &ground);

                SDL_Color white = {255,255,255};
                SDL_Color yellow = {252, 194, 3};
                SDL_Color black = {0, 0, 0};






                // score text outline
                scoredisp = TTF_RenderText_Solid(titleFont, to_string(score).c_str(), black);
                textture = SDL_CreateTextureFromSurface(rend, scoredisp);
                SDL_Rect textbox;
                textbox.x = (WIDTH - outline) / 2 - scoredisp->w / 2 + endScoreX;
                textbox.y = HEIGHT / 2 - scoredisp->h / 2 + endScoreY;
                textbox.w = scoredisp->w;
                textbox.h = scoredisp->h;
                SDL_RenderCopy(rend, textture, NULL, &textbox);


                scoredisp = TTF_RenderText_Solid(titleFont, to_string(score).c_str(), black);
                textture = SDL_CreateTextureFromSurface(rend, scoredisp);
                textbox.x = (WIDTH + outline) / 2 - scoredisp->w / 2 + endScoreX;
                textbox.y = HEIGHT / 2 - scoredisp->h / 2 + endScoreY;
                textbox.w = scoredisp->w;
                textbox.h = scoredisp->h;
                SDL_RenderCopy(rend, textture, NULL, &textbox);


                scoredisp = TTF_RenderText_Solid(titleFont, to_string(score).c_str(), black);
                textture = SDL_CreateTextureFromSurface(rend, scoredisp);
                textbox.x = WIDTH / 2 - scoredisp->w / 2 + endScoreX;
                textbox.y = (HEIGHT - outline) / 2 - scoredisp->h / 2 + endScoreY;
                textbox.w = scoredisp->w;
                textbox.h = scoredisp->h;
                SDL_RenderCopy(rend, textture, NULL, &textbox);


                scoredisp = TTF_RenderText_Solid(titleFont, to_string(score).c_str(), black);
                textture = SDL_CreateTextureFromSurface(rend, scoredisp);
                textbox.x = WIDTH / 2 - scoredisp->w / 2 + endScoreX;
                textbox.y = (HEIGHT + outline) / 2 - scoredisp->h / 2 + endScoreY;
                textbox.w = scoredisp->w;
                textbox.h = scoredisp->h;
                SDL_RenderCopy(rend, textture, NULL, &textbox);







                // score text fill
                scoredisp = TTF_RenderText_Solid(titleFont, to_string(score).c_str(), yellow);
                textture = SDL_CreateTextureFromSurface(rend, scoredisp);
                textbox.x = WIDTH / 2 - scoredisp->w / 2 + endScoreX;
                textbox.y = HEIGHT / 2 - scoredisp->h / 2 + endScoreY;
                textbox.w = scoredisp->w;
                textbox.h = scoredisp->h;
                SDL_RenderCopy(rend, textture, NULL, &textbox);






                // continue message
                scoredisp = (time(NULL) % 2) ? TTF_RenderText_Solid(font, "> SPACE TO CONTINUE <", white) : TTF_RenderText_Solid(font, "SPACE TO CONTINUE", white);
                textture = SDL_CreateTextureFromSurface(rend, scoredisp);
                textbox.x = WIDTH / 2 - scoredisp->w / 2;
                textbox.w = scoredisp->w;
                textbox.h = scoredisp->h;
                textbox.y = HEIGHT - textbox.h - 32;
                SDL_RenderCopy(rend, textture, NULL, &textbox);

                if (enter) {
                    gameloop = false;
                }
            } else {
                if (spacing && moe.y == HEIGHT / 2) {
                    pvel = 10;
                }
                if (moe.y == HEIGHT / 2) {
                    moe.x = 50 + (SDL_sin(distanceRan / SPEED / 5) * 5);
                } else {
                    moe.x = 50;
                }
                pvel -= 0.4;
                moey += pvel;
                moey = __max(moey, 0);
                moe.y = (HEIGHT / 2) - (moey * ((double) SCALE / 6.0));
                moe.y = __min(moe.y, HEIGHT / 2);
                for (int x : bullets) {
                    if (x - distanceRan * 1.2 < moe.x + moe.w && x - distanceRan * 1.2 + 48 > moe.x && moe.y > HEIGHT / 2 - 48) {
                        cout << "die" << '\n';
                        gameover = true;
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
                distanceRan += SPEED * SCALE / 6;

                if ((int)(distanceRan / (SPEED * SCALE / 6)) % 50 == 0) {
                    if ((rand() % 2) + 1 == 1) {
                        cheeses.push_back(distanceRan + (WIDTH));
                        //cheeses.push_back((rand() % 2) * 64);
                        cout << "new cheese" << '\n';
                    } else {
                        bullets.push_back(distanceRan * 1.2 + (WIDTH));
                        //bullets.push_back((rand() % 2) * 64);
                        cout << "new bullet" << '\n';
                    }
                }
                
                SDL_SetRenderDrawColor(rend, 83, 178, 237, 255);
                SDL_RenderClear(rend);

                SDL_Rect ground;
                ground.x = 0;
                ground.y = HEIGHT / 2 + (15 * SCALE);
                ground.w = WIDTH;
                ground.h = HEIGHT / 2;
                SDL_SetRenderDrawColor(rend, 0, 0, 0, 255 );
                SDL_RenderFillRect(rend, &ground);

                ground.x = 0;
                ground.y = HEIGHT / 2 + (16 * SCALE);
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
            }
            SDL_RenderCopy(rend, cursorTexture, NULL, &mcursor);
            SDL_RenderPresent(rend);
            SDL_Delay(1000 / 60);
        }
    }
 
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

int renderCheese(SDL_Renderer* rend, SDL_Texture* tex, list<int> &cheeses, int distanceRan) {
    SDL_Rect cheese;
    SDL_QueryTexture(tex, NULL, NULL, &cheese.w, &cheese.h);
    cheese.w *= SCALE;
    cheese.h *= SCALE;
    for (int i = 0; i < sizeof(cheeses); i++) {
        int x = get(cheeses, i);
        if (x == INT_MIN) continue;
        cheese.y = HEIGHT / 2  + (SDL_sin((x - distanceRan) / SPEED / 10) * 10);
        cheese.x = x - distanceRan;
        SDL_RenderCopy(rend, tex, NULL, &cheese);
    }
    return 0;
}

int renderBullet(SDL_Renderer* rend, SDL_Texture* tex, list<int> &bullets, int distanceRan) {
    SDL_Rect bullet;
    SDL_QueryTexture(tex, NULL, NULL, &bullet.w, &bullet.h);
    bullet.w *= SCALE;
    bullet.h *= SCALE;
    for (int x : bullets) {
        bullet.y = HEIGHT / 2  + (SDL_sin((x - distanceRan) / SPEED) * 4);
        bullet.x = x - distanceRan * 1.2 + (SDL_sin(((x - distanceRan) * 1.14159265) / SPEED) * 2);
        SDL_RenderCopy(rend, tex, NULL, &bullet);
    }
    return 0;
}

int get(list<int> &data, int index) {
    list<int>::iterator i;
    i = data.begin();
    int n = 0;
    for (i = data.begin(); i != data.end(); ++i) {
        if (n == index) {
            return *i;
        }
        n++;
    }
    return INT_MIN;
}

void updateScale(SDL_Window* win) {
    SDL_GetWindowSize(win, &WIDTH, &HEIGHT);
    SCALE = ((double) WIDTH / (1920.0 / 2)) * 6;
}

uint64_t milliTime() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}