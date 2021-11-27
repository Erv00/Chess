#include "graphics.h"

#include <stdio.h>
#include <assert.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <debugmalloc.h>

static SDL_Texture *PIECE_GRAPHICS[12] = {NULL};
//[0] is normal (16) font, [1] is win font (42)
static TTF_Font *CHESS_FONTS[2] = {NULL};

void initGraphics(SDL_Window **win, SDL_Renderer **renderer){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        //Failed to initialize
        fprintf(stderr, "Failed to init sdl: %s\n", SDL_GetError());
        exit(-1);
    }

    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG){
        //Failed to init
        fprintf(stderr, "Failed to init IMG: %s\n", SDL_GetError());
        exit(-1);
    }

    if(SDL_CreateWindowAndRenderer(12*45, 8*45, SDL_WINDOW_SHOWN, win, renderer) != 0){
        //Failed to create
        fprintf(stderr, "Failed to create window and renderer: %s\n", SDL_GetError());
        exit(-1);
    }
    
    if(TTF_Init() != 0){
        //Failed to init
        fprintf(stderr, "Failed to init ttf: %s\n", SDL_GetError());
        exit(-1);
    }

    //Load pieces
    loadPieces(*renderer);

    //Load fonts
    loadFonts();

    //Load menu textures
    loadMenuTextures(*renderer);
}

void loadPieces(SDL_Renderer *renderer){
    static char PIECE_ACRONYMS[] = {'p', 'r', 'n', 'b', 'q', 'k'};
    int idx = 0;
    char path[] = "assets/Chess_XXt45.png";
    path[14] = 'l';

    for(int pass = 0; pass < 2; pass++){
        for(int i = 0;i < 6; i++){
            path[13] = PIECE_ACRONYMS[i];

            SDL_Texture *t = IMG_LoadTexture(renderer, path);
            if(t == NULL){
                fprintf(stderr, "Failed to open %s\n", path);
                exit(-1);
            }
            PIECE_GRAPHICS[idx++] = t;
        }
        //Now load blacks
        path[14] = 'd';
    }
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
    Menu toRender = MENU_GAME;
    if(flip)
        toRender = MENU_GAME_FLIPPED;
    SDL_Texture *tex = getMenuTexture(toRender);
    SDL_RenderCopy(renderer, tex, NULL, NULL);
}

void renderPieces(SDL_Renderer *renderer, Board *board, bool flip){
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
            Square s = {.rank = rank, .file = file};
            if(board->mouseState.held && isSame(s, board->mouseState.from) && !isFreeAt(board, s)){
                //Don't render that piece at position, render at mouse
                SDL_Rect mousePos = {
                    .x = board->mouseState.xPos-45/2,
                    .y = board->mouseState.yPos-45/2,
                    .w = 45,
                    .h = 45
                };
                SDL_RenderCopy(renderer, getPieceGraphics(*at(board, s)), NULL, &mousePos);
            } else if(!isFreeAt(board, s))
                SDL_RenderCopy(renderer, getPieceGraphics(*at(board, s)), NULL, &rect);
            rect.x += 45;
        }
        rect.y += 45;
    }
}

void loadFonts(void){
        CHESS_FONTS[0] = TTF_OpenFont("assets/LiberationSerif-Regular.ttf", 16);
        if(CHESS_FONTS[0] == NULL){
            fprintf(stderr, "Failed to open font: %s\n", SDL_GetError());
            exit(-1);
        }

        CHESS_FONTS[1] = TTF_OpenFont("assets/LiberationSerif-Regular.ttf", 42);
        if(CHESS_FONTS[1] == NULL){
            fprintf(stderr, "Failed to open font: %s\n", SDL_GetError());
            exit(-1);
        }
        TTF_SetFontStyle(CHESS_FONTS[1], TTF_STYLE_BOLD);
}

void unloadFonts(void){
    TTF_CloseFont(CHESS_FONTS[0]);
    TTF_CloseFont(CHESS_FONTS[1]);
}

SDL_Texture* stringToTexture(SDL_Renderer *renderer, const char *str, int *width, int *height){
    SDL_Color col = {255,255,255,255};
    SDL_Surface *surf = TTF_RenderUTF8_Blended(CHESS_FONTS[0], str, col);

    if(width != NULL)
        *width = surf->w;
    if(height != NULL)
        *height = surf->h;

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_FreeSurface(surf);
    return tex;
}

SDL_Texture* winStringToTexture(SDL_Renderer *renderer, const char *str, int *width, int *height, bool white){
    SDL_Color col = {255,255,255,255};
    if(!white){
        col.r = 0;
        col.g = 0;
        col.b = 0;
    }

    SDL_Surface *surf = TTF_RenderUTF8_Blended(CHESS_FONTS[1], str, col);

    if(width != NULL)
        *width = surf->w;
    if(height != NULL)
        *height = surf->h;

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_FreeSurface(surf);
    return tex;

}

static SDL_Texture *menuTextures[NUM_MENUS] = {NULL};

void loadMenuTextures(SDL_Renderer *renderer){
    char path[20] = {0};
    for(int i = 0; i < NUM_MENUS; i++){
        sprintf(path, "assets/menu_%d.png", i);
        SDL_Texture *t = IMG_LoadTexture(renderer, path);
        if(t == NULL){
            fprintf(stderr, "Failed to open %s\n", path);
            exit(-1);
        }
        menuTextures[i] = t;
    }
}

void unloadMenuTextures(void){
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
