/**
 * @file fen.h
 * @brief Forsyth-Edwards nozációval foglalkozó függvények
 */
#ifndef CHESS_FEN
#define CHESS_FEN

#include <SDL2/SDL.h>

typedef struct Board Board;

/**
 * @brief Új játékot kezd a megadott állásból
 * 
 * @param fenStr A játékállás
 * @param renderer A használt renderer
 * @return Board* A játéktábla
 */
Board* newGameFromFen(const char* fenStr, SDL_Renderer *renderer);

/**
 * @brief Feldolgozza a FEN sztring bábu adatait
 * 
 * @param board A tábla, ahova a bábuk kerülnek
 * @param fenStr A FEN sztring bábu adatra mutató része
 * @return const char* A következő FEN adatmezőre mutató pointer
 */
const char* processFENPieceData(Board *board, const char *fenStr);

/**
 * @brief Feldolgozza a FEN sztring sáncolás adatait
 * 
 * @param board A tábla amire érvényes a sztring
 * @param fenStr A FEN sztring sáncolás adataira mutatós pointer
 * @return const char* A következő FEN adatmezőre mutató pointer
 */
const char* processFENCastlingData(Board *board, const char *fenStr);

/**
 * @brief Feldolgozza a FEN sztring en passante adatait
 * 
 * @param board A tábla amire érvényes a sztring
 * @param fenStr A FEN sztring en passante adataira mutatós pointer
 * @return const char* A következő FEN adatmezőre mutató pointer
 */
const char* processFENEnPassanteData(Board *board, const char *fenStr);

/**
 * @brief Elmenti a játékállást FEN sztringként
 * 
 * @param board A mentendő tábla
 * @return char* A játékállást reprezentáló FEN sztring
 */
char *saveAsFEN(Board *board);

/**
 * @brief cast-ba írja a jelenlegi sáncolási lehetőségeket
 * Fontos: nem tesz \0 a tömb végére
 * @param board A tábla
 * @param cast Ide íródik a lehetőség
 * @return int A kiírt karakterek száma
 */
int getCastlingString(Board *board, char cast[4]);

/**
 * @brief A megadott sztringbe írja a táblán lévő bábukat FEN notációval
 * 
 * @param board A bábukat tartalmaző tábla
 * @param fen Ide íródik a sztring
 * @return char* Az így írt sztring végére
 */
char* saveFENPieceData(Board *board, char* fen);
#endif