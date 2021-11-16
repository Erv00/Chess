#ifndef CHESS_GRAPHICS
#define CHESS_GRAPHICS

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "piece.h"
#include "square.h"
#include "board.h"

typedef struct GraphicsData{
    SDL_Window *window;
    SDL_Renderer *renderer;
} GraphicsData;

//
GraphicsData initWindow(void);

bool loadPieces(SDL_Renderer *renderer);
void unloadPieces(void);

SDL_Texture *getPieceGraphics(Piece p);

void renderPiece(SDL_Renderer *renderer, Piece p, Square at);

void renderBoard(SDL_Renderer *renderer, bool flip);

void renderPieces(SDL_Renderer *renderer, Board *board, bool flip);

//Creates a texture form a string. Stores it's width and height int with and height, any of these may be NULL
SDL_Texture* stringToTexture(SDL_Renderer *renderer, const char *str, int *width, int *height);

/**
 * @brief Az adott sztringet fekete vagy fehér színben nagy betűmérettel és vastagon rendereli
 * 
 * @param renderer Renderer
 * @param str Amit rendererelni kell
 * @param width A textúra szélessége ide íródik ha nem NULL
 * @param height A textúra magassága ide íródik ha nem NULL
 * @param white Ha igaz a szöveg fehér, egyébként fekete
 * @return SDL_Texture* A textúra
 */
SDL_Texture* winStringToTexture(SDL_Renderer *renderer, const char *str, int *width, int *height, bool white);

//Loads the menu textures
bool loadMenuTextures(SDL_Renderer *renderer);

void unloadTextures(void);

typedef enum {
    MENU_MAIN,
    MENU_LOAD,
    MENU_NEW_GAME,
    MENU_GAME,
    MENU_GAME_FLIPPED,
    MENU_SAVE,
    MENU_GAME_OVER,
    MENU_ANALYZE,

    NUM_MENUS
} Menu;

SDL_Texture* getMenuTexture(Menu menu);

//Helper function that returnes the textures width and height in an SDL_Rect
SDL_Rect textureDim(SDL_Texture* tex);
#endif
