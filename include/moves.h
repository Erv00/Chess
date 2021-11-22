/**
 * @file moves.h
 * @brief Lépések leírása
 */
#ifndef CHESS_MOVES
#define CHESS_MOVES

#include <stdio.h>
#include <stdbool.h>

#include "piece.h"
#include "square.h"

typedef struct Board Board;

/**
 * @brief Egy lépés 
 */
typedef struct Move{
    Square from;    ///< A lépés kiindulási pontja
    Square to;      ///< A lépés végpontja
} Move;

/**
 * @brief Elomozdít egy bábut, nem ellenőrzi, hogy a lépés legális-e
 * @param board A tábla amin a bábu mozog
 * @param move A végrehajtandó lépés
 */
void movePiece(Board *board, Move move);

/**
 * @brief Elmozdít egy bábut ha az legális, és frissíti a tábla belső állapotát (en passante, sáncolás, ...)
 * @param board A tábla, amin a lépés végrehajtandó
 * @param move A végrehajtandó lépés
 */
void movePieceWithCheck(Board *board, Move move);

/**
 * @brief Meghatározza egy lépésről, hogy legális-e
 * 
 * @param board A tábla amin a bábu mozog
 * @param move Az ellenőrizendő lépés
 * @param enPassante Ide lesz írva az a négyzet, amelyre en passante lépéssel lépni lehet, ha nem NULL
 * @param rookMove Ide lesz írva az a lépés, amely sáncolásnál a bástya lépése, ha nem NULL
 * @param newCastlingAvailability Ide lesz írva a királyok sáncolási lehetőságei a lépés után, ha nem NULL 
 * @return true Ha a lépés legális
 * @return false Ha a lépés nem legális
 */
bool isValidMove(Board *board, Move move, Square *enPassante, Move *rookMove, int *newCastlingAvailability);

/**
 * @brief Meghatározza, hogy a lépés egy gyalog esetében legális-e
 * 
 * @param board A tábla amin a bábu mozog
 * @param move Az ellenőrizendő lépés
 * @param enPassante Ha nem NULL, ide lesz írva az en passant lépéssel leüthető mező, ha ez létezik
 * @return true Ha a lépés legális
 * @return false Ha a lépés nem legális
 */
bool checkPawnMove(Board *board, Move move, Square *enPassante);

/**
 * @brief Meghatározza, hogy a lépés egy bástya esetében legális-e
 * 
 * @param board A tábla, amin a bábu mozog
 * @param move Az ellenőrizendő lépés
 * @param newCastlingAwailability Ha nem NULL ide lesz írva a lépés utána a sáncolási lehetőségeket
 * @return true Ha a lépés legális
 * @return false Ha a lépés nem legális
 */
bool checkRookMove(Board *board, Move move, int *newCastlingAwailability);

/**
 * @brief Meghatározza, hogy a lépés egy huszár esetében legális-e
 * 
 * @param move Az ellenőrizendő lépés
 * @return true Ha a lépés legális
 * @return false Ha a lépés nem legális
 */
bool checkKnightMove(Move move);

/**
 * @brief Meghatározza, hogy a lépés egy futó esetében legális-e
 * 
 * @param board A tábla, amin a bábu mozog
 * @param move Az ellenőrizendő lépés
 * @return true Ha a lépés legális
 * @return false Ha a lépés nem legális
 */
bool checkBishopMove(Board *board, Move move);

/**
 * @brief Meghatározza, hogy a lépés egy vezér esetében legális-e
 * 
 * @param board A tábla, amin a bábu mozog
 * @param move Az ellenőrizendő lépés
 * @return true Ha a lépés legális
 * @return false Ha a lépés nem legális
 */
bool checkQueenMove(Board *board, Move move);

/**
 * @brief Meghatározza, hogy a lépés egy király esetében legális-e
 * 
 * @param board A tábla, amin a bábu mozog
 * @param move Az ellenőrizendő lépés
 * @param castlingMove Ha nem NULL ide íródik az a lépés, amely sáncolásnál a bástyát mozgatja
 * @param newCastlingAvailability Ha nem NULL ide lesz írva a lépés utána a sáncolási lehetőségeket
 * @return true Ha a lépés legális
 * @return false Ha a lépés nem legális
 */
bool checkKingMove(Board *board, Move move, Move *castlingMove, int *newCastlingAvailability);

/**
 * @brief Meghatározza a sáncolási lehetőségeket a megadott lépés után
 * 
 * @param board A vizsgálandó tábla
 * @param move A vizsgálandó lépés
 * @return char A sáncolási lehetőségek bitflagként
 * @see Board
 */
char getCastlingAvailabilityAfterMove(Board *board, Move move);

/**
 * @brief Meghatározza, hogy az adott szín az adott oldalon tud-e sáncolni
 * 
 * @param board A tábla amin ellenőrzünk
 * @param white Fehér szín
 * @param kingside Király oldali (short)
 * @return true Ha sáncolhat a a megadott szín abban az irányban
 * @return false Ellenkező esetben
 */
bool canCastle(Board *board, bool white, bool kingside);

/**
 * @brief Meghatározza azt a gyalogot amelyik az adott mezőre tud lépni
 * 
 * @param to Hova kell lépni
 * @param moved A mozdított bábu
 * @param b A tábla ahol a bábu mozog
 * @return Square A mező, amelyen egy megadott bábu áll, és a megadott mezőre tud lépni
 */
Square findPawnMovableTo(Square to, Piece moved, Board *b);

/**
 * @brief Meghatározza azt a bástyát amelyik az adott mezőre tud lépni
 * 
 * @param to Hova kell lépni
 * @param moved A mozdított bábu
 * @param b A tábla ahol a bábu mozog
 * @return Square A mező, amelyen egy megadott bábu áll, és a megadott mezőre tud lépni
 */
Square findRookMovableTo(Square to, Piece moved, Board *b);

/**
 * @brief Meghatározza azt a futót amelyik az adott mezőre tud lépni
 * 
 * @param to Hova kell lépni
 * @param moved A mozdított bábu
 * @param b A tábla ahol a bábu mozog
 * @return Square A mező, amelyen egy megadott bábu áll, és a megadott mezőre tud lépni
 */
Square findBishopMovableTo(Square to, Piece moved, Board *b);

/**
 * @brief Meghatározza azt a huszárt amelyik az adott mezőre tud lépni
 * 
 * @param to Hova kell lépni
 * @param moved A mozdított bábu
 * @param b A tábla ahol a bábu mozog
 * @return Square A mező, amelyen egy megadott bábu áll, és a megadott mezőre tud lépni
 */
Square findKnightMovableTo(Square to, Piece moved, Board *b);

/**
 * @brief Meghatározza azt a vezért amelyik az adott mezőre tud lépni
 * 
 * @param to Hova kell lépni
 * @param moved A mozdított bábu
 * @param b A tábla ahol a bábu mozog
 * @return Square A mező, amelyen egy megadott bábu áll, és a megadott mezőre tud lépni
 */
Square findQueenMovableTo(Square to, Piece moved, Board *b);
#endif