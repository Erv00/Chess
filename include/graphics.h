/**
 * @file graphics.h
 * @brief Grafika
 */
#ifndef CHESS_GRAPHICS
#define CHESS_GRAPHICS

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "piece.h"
#include "square.h"
#include "board.h"

/**
 * @brief Inicializálja a grafikával kapcsolatos dolgokat és betölti a textúrákat és a fontokat 
 * @param win Ide íródik az ablak
 * @param renderer Ide íródik a renderer
 */
void initGraphics(SDL_Window **win, SDL_Renderer **renderer);

/**
 * @brief Betölti a bábuk textúráit
 * 
 * @param renderer A betöltéshez használt renderer
 */
void loadPieces(SDL_Renderer *renderer);

/**
 * @brief Felszabadítja a bábuk textúráit
 */
void unloadPieces(void);

/**
 * @brief Visszadja az adott bábu textúráját
 * 
 * @param p A bábu aminek a textúrája kell
 * @return SDL_Texture* A bábu textúrája
 */
SDL_Texture *getPieceGraphics(Piece p);

/**
 * @brief Rendereli a táblát, fordítva ha kell
 * 
 * @param renderer A használt renderer
 * @param flip Igaz, ha meg kell fordítani a táblát
 */
void renderBoard(SDL_Renderer *renderer, bool flip);

/**
 * @brief Rendereli a táblán a bábukat
 * 
 * @param renderer A használt renderer
 * @param board A tábla
 * @param flip Igaz, ha meg kell fordítani a táblát
 */
void renderPieces(SDL_Renderer *renderer, Board *board, bool flip);

/**
 * @brief Betölti a fontokat
 */
void loadFonts(void);

/**
 * @brief Felszabadítja a fontokat
 */
void unloadFonts(void);


/**
 * @brief Az adott sztringet textúrává alakítja
 * 
 * @param renderer Az átalakításhoz használt renderer
 * @param str Az átalakítandó sztring
 * @param width Ha nem NULL, ide íródik a textúra szélessége
 * @param height Ha nem NULL, ide íródik a textúra magassága
 * @return SDL_Texture* Az elkészített textúra
 */
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

/**
 * @brief Betölti a menük textúráit
 * 
 * @param renderer A betöltéshez használt renderer
 */
void loadMenuTextures(SDL_Renderer *renderer);

/**
 * @brief Felszabadítja a menük textúráit
 * 
 */
void unloadMenuTextures(void);

/**
 * @brief Menük
 */
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

/**
 * @brief Meghatározza a megadott menü textúráját
 * 
 * @param menu A menü
 * @return SDL_Texture* A menühöz tartozó textúra
 */
SDL_Texture* getMenuTexture(Menu menu);

/**
 * @brief Egy adott textúráró meghatározza a méreteit
 * @param tex A vizsgálandó textúra
 * @return SDL_Rect Tartalmazza a textúra szélességét és magasságát
 */
SDL_Rect textureDim(SDL_Texture* tex);
#endif
