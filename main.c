#include <stdio.h>
#include <SDL2/SDL.h>
#include "constants.h"

int game_is_running = FALSE;
SDL_Window* window;
SDL_Renderer* renderer;

struct square {
    float x;
    float y;
    float width;
    float height;
} square;

struct player {
    float x;
    float y;
    float width;
    float height;
} player;

int last_frame_time = 0;

int dirX = 1;
//Gravity: +/-1
int dirY = 1;

//2D Array, in das das Textfile geladen wird
char level[SQUARE_HEIGHT][SQUARE_WIDTH];


int initialize_window(void){

    SDL_Init(SDL_INIT_EVERYTHING);

//    something went wrong if the code is executed
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE;
    }

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        0
    );

    //did everything work as planned?
    if(!window){
        fprintf(stderr, "Error creating SDL Window.\n");
        return FALSE;
    }

    //which window?, driver code -> (-1) to symbolise default, flags (0 since we don't want any
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        fprintf(stderr, "Error creating SDL Renderer.\n");
    }

    return TRUE;
}

void process_input(){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type){
        case SDL_QUIT:
            //while loop stops
            game_is_running = FALSE;
            break;
//            keyboard event
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE){
                game_is_running = FALSE;
            }
//            Pressing space flips the gravity
            if(event.key.keysym.sym == SDLK_SPACE){
                dirY *= -1;
            }
            break;
    }
}

void createLevel(){
//    TODO: Creates a Square at a given position

    for(int i = 0; i < SQUARE_HEIGHT; i++){
        for(int j = 0; j < SQUARE_WIDTH; j++){
            if(level[i][j] == 'X'){

            }
        }
    }
}


//Liest Datei ein und speichert sie in 2D-char-Array
void loadLevel(){

    FILE* level_1;
    level_1 = fopen("level_1.txt", "r");

    if(level_1 == NULL){
        printf("Fehler! Datei konnte nicht geöffnet werden!\n");
        return;
    }else{
        for(int i = 0; i < SQUARE_HEIGHT; i++){
            fscanf(level_1, "%s", level[i]);
            printf("%s\n", level[i]);
        }
    }

    fclose(level_1);
}

void setup(){

    square.width = SQUARE_SIZE;
    square.height = SQUARE_SIZE;

    player.x = 200;
    player.y = 200;
    player.width = SQUARE_SIZE / 2;
    player.height = SQUARE_SIZE / 2;

    loadLevel();
}

void update(){

    //DELAY:
    //only returns true if the amount of time given in the second argument passed
    //compare current time with the time the next evaluation should happen
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));

    //Calculates the difference in ms between two frames
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();

    //"Bounce Effect" at Window boundaries
    if(player.x > WIDTH - 10 || player.x < 10){
        dirX *= -1;
    }
    if(player.y > HEIGHT - 10 || player.y < 0){
        dirY *= -1;
    }

    //rate of change in pixels per second is applied, divided into 60 FPS
    player.x += dirX * 360 * delta_time;
    player.y += dirY * 360 * delta_time;

}

void render(){

    //last value: transparency (Alpha) 255: not transparent at all
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    //clear renderer with the colour just activated
    SDL_RenderClear(renderer);

//    Rendere ab jetzt in weiß -> Quadrate
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int i = 0; i < SQUARE_HEIGHT; i++){
        for(int j = 0; j < SQUARE_WIDTH; j++){
            if(level[i][j] == 'X'){
                square.x = j * SQUARE_SIZE;
                square.y = i * SQUARE_SIZE;

                SDL_Rect obstacle = {
                    (int) square.x,
                    (int) square.y,
                    (int) square.width,
                    (int) square.height
                };

                SDL_RenderFillRect(renderer, &obstacle);
            }
        }
    }

    //Red rectangle at the player position
    SDL_Rect player_rect = {
        (int) player.x,
        (int) player.y,
        (int) player.width,
        (int) player.height
    };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &player_rect);

    //renderer is displayed
    SDL_RenderPresent(renderer);

    SDL_RenderClear(renderer);

}

void destroy_window(){
//    Destroy in reversed order
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[]){

    game_is_running = initialize_window();

    setup();

    while(game_is_running){
        process_input();
        update();
        render();
    }

    destroy_window();

    loadLevel();

    return 0;
}
