#include "graphics.h"

#include <stdio.h>
#include <assert.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

static SDL_Texture *PIECE_GRAPHICS[12] = {NULL};

GraphicsData initWindow(){
    GraphicsData gp;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        //Failed to initialize
        fprintf(stderr, "Failed to init sdl: %s\n", SDL_GetError());
        exit(-1);
    }

    assert((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == IMG_INIT_PNG);

    gp.window = SDL_CreateWindow("Schakk", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 45*12, 45*8, SDL_WINDOW_SHOWN);
    assert(gp.window != NULL);

    gp.renderer = SDL_CreateRenderer(gp.window, -1, SDL_RENDERER_SOFTWARE);
    assert(gp.renderer != NULL);

    TTF_Init();
    return gp;
}

bool loadPieces(SDL_Renderer *renderer){
    static char PIECE_ACRONYMS[] = {'p', 'r', 'n', 'b', 'q', 'k'};
    int idx = 0;
    char path[] = "assets/Chess_XXt45.png";
    path[14] = 'l';

    for(int pass = 0; pass < 2; pass++){
        for(int i = 0;i < 6; i++){
            path[13] = PIECE_ACRONYMS[i];

            SDL_Texture *t = IMG_LoadTexture(renderer, path);
            if(t == NULL){
                printf("Failed to open %s\n", path);
            }
            PIECE_GRAPHICS[idx++] = t;
        }
        //Now load blacks
        path[14] = 'd';
    }
    return true;
}

void unloadPieces(void){
    for(int i = 0;i < 12; i++){
        SDL_DestroyTexture(PIECE_GRAPHICS[i]);
    }
}

SDL_Texture *getPieceGraphics(Piece p){
    char offset = isBlack(p) ? 6 : 0;
    Piece pCode = p & 7;
    for(unsigned int i = 0;i < 7;i++){
        if(pCode == i){
            return PIECE_GRAPHICS[i+offset];
        }
    }
    fprintf(stderr, "No such piece: %d\n", p);
    return NULL;
}

void renderBoard(SDL_Renderer *renderer, bool flip){
    int rank, fileStart, step;
    if(!flip){
        //Normal display
        rank = 7;
        fileStart = 0;
        step = 1;
    } else {
        //Flipped display
        rank = 0;
        fileStart = 7;
        step = -1;
    }

    SDL_Rect rect = {
        .x = 0,
        .y = 0,
        .w = 45,
        .h = 45
    };
    for(;rank < 8 && rank >= 0; rank -= step){
        rect.x = 0;
        for(int file=fileStart; file < 8 && file >= 0; file += step){
            if(rank % 2 == file % 2){
                SDL_SetRenderDrawColor(renderer, 0x76, 0x96, 0x56, 0xff);
            } else {
                SDL_SetRenderDrawColor(renderer, 0xee, 0xee, 0xee, 0xd2);
            }
            SDL_RenderFillRect(renderer, &rect);
            rect.x += 45;
        }
        rect.y += 45;
    }
}

void renderPieces(SDL_Renderer *renderer, Board *board){
    int rank, fileStart, step;
    if(board->nextIsWhite){
        //Normal display
        rank = 7;
        fileStart = 0;
        step = 1;
    } else {
        //Flipped display
        rank = 0;
        fileStart = 7;
        step = -1;
    }

    SDL_Rect rect = {
        .x = 0,
        .y = 0,
        .w = 45,
        .h = 45
    };
    for(;rank < 8 && rank >= 0; rank -= step){
        rect.x = 0;
        for(int file=fileStart; file < 8 && file >= 0; file += step){
            Square s = {.rank = rank, .file = file};
            if(board->mouseState.held && isSame(s, board->mouseState.from) && !isFreeAt(board, s)){
                //Don't render that piece at position, render at mouse
                SDL_Rect mousePos = {
                    .x = board->mouseState.xPos-45/2,
                    .y = board->mouseState.yPos-45/2,
                    .w = 45,
                    .h = 45
                };
                SDL_RenderCopy(renderer, getPieceGraphics(at(board, s)), NULL, &mousePos);
            } else if(!isFreeAt(board, s))
                SDL_RenderCopy(renderer, getPieceGraphics(at(board, s)), NULL, &rect);
            rect.x += 45;
        }
        rect.y += 45;
    }
}

SDL_Texture* stringToTexture(SDL_Renderer *renderer, const char *str, int *width, int *height){
    static TTF_Font *font = NULL;

    //Open font if not opened
    if(font == NULL){
        font = TTF_OpenFont("assets/LiberationSerif-Regular.ttf", 16);
        if(font == NULL){
            fprintf(stderr, "Failed to open font: %s\n", SDL_GetError());
            exit(-1);
        }
    }

    SDL_Color col = {255,255,255,255};
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font, str, col);

    if(width != NULL)
        *width = surf->w;
    if(height != NULL)
        *height = surf->h;

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_FreeSurface(surf);
    return tex;
}

static SDL_Texture *menuTextures[NUM_MENUS] = {NULL};

bool loadMenuTextures(SDL_Renderer *renderer){
    char path[20] = {0};
    for(int i = 0; i < NUM_MENUS; i++){
        sprintf(path, "assets/menu_%d.jpg", i);
        SDL_Texture *t = IMG_LoadTexture(renderer, path);
        if(t == NULL){
            printf("Failed to open %s\n", path);
            //exit(-1);
        }
        menuTextures[i] = t;
    }
    return true;
}

void unloadTextures(void){
    for(int i = 0; i < NUM_MENUS; i++)
        SDL_DestroyTexture(menuTextures[i]);
}

SDL_Texture* getMenuTexture(Menu menu){
    SDL_Texture *t =  menuTextures[menu];
    if(t == NULL)
        printf("No texture for %d\n", menu);

    return t;
}


SDL_Rect textureDim(SDL_Texture* tex){
    SDL_Rect r;
    SDL_QueryTexture(tex, NULL, NULL, &r.w, &r.h);
    return r;
}
