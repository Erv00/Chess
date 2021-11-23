/**
 * @file views.h
 * @brief Menük megjelenítése
 */
#ifndef CHESS_VIEWS
#define CHESS_VIEWS

#include <SDL2/SDL.h>

#include "board.h"

/**
 * @brief A mentési fájl helye
 */
#define SAVE_PATH "state.sch"

/**
 * @brief Kirajzolja a főmenüt
 * 
 * @param renderer A használt renderer
 */
void renderMenuView(SDL_Renderer *renderer);

/**
 * @brief Kirajzolja az új játék menüt
 * 
 * @param renderer A használt renderer
 */
void renderNewGameView(SDL_Renderer *renderer);

/**
 * @brief Kirajzolja a betöltés
 * 
 * @param renderer A használt renderer
 */
void renderLoadView(SDL_Renderer *renderer);

/**
 * @brief Kirajzolja a játék nézetet
 * 
 * @param board A használt tábla
 */
void renderPlayView(Board *board);

/**
 * @brief Kirajzolja a mentés menüt
 * 
 * @param renderer A használt renderer
 */
void renderSaveView(SDL_Renderer *renderer);

/**
 * @brief Kirajzolja a játék vége
 * 
 * @param board A használt tábla
 */
void renderGameOverView(Board *board);

/**
 * @brief Kirajzolja az analízis nézetet
 * 
 * @param board A használt tábla
 */
void renderAnalysisView(Board *board);


/**
 * @brief Kezeli a főmenü eseményeit
 * 
 * @param renderer A használt renderer
 * @return true Ha a felhasználó ki szeretne lépni
 * @return false Egyébként
 */
bool handleMenuView(SDL_Renderer *renderer);

/**
 * @brief Kezeli az új játék menü eseményeit
 * 
 * @param renderer A használt renderer
 * @return true Ha a felhasználó ki szeretne lépni
 * @return false Egyébként
 */
bool handleNewGameView(SDL_Renderer *renderer);

/**
 * @brief Kezeli a betöltés menü eseményeit
 * 
 * @param renderer A használt renderer
 * @return true Ha a felhasználó ki szeretne lépni
 * @return false Egyébként
 */
bool handleLoadView(SDL_Renderer *renderer);

/**
 * @brief Kezeli a játék nézet eseményeit
 * 
 * @param board A használt tábla
 */
void handlePlayView(Board *board);

/**
 * @brief Kezeli a mentés menü eseményeit
 * 
 * @param board A használt tábla
 */
void handleSaveView(Board *board);

/**
 * @brief Kezeli a játék vége menü eseményeit
 * 
 * @param board A használt tábla
 */
void handleGameOverView(Board *board);

/**
 * @brief Kezeli az analízis nézet eseményeit
 * 
 * @param board A használt tábla
 */
void handleAnalysisView(Board *board);
#endif
