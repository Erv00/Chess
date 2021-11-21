/**
 * @file piece.h
 * @brief A bábu és függvényei
 */
#ifndef CHESS_PIECE
#define CHESS_PIECE

#include <stdbool.h>

/**
 * @brief Egy bábu reprezentálása.
 * Az alsó 3 bit (& 7) a típusa, a következő 2 a színe
 */
typedef enum Piece {
    PAWN,   ///< Gyalog
    ROOK,   ///< Bástya
    KNIGHT, ///< Huszár
    BISHOP, ///< Futó
    QUEEN,  ///< Vezér
    KING,   ///< Király

    //Color
    COLOR_WHITE = 8,    ///< Fehér
    COLOR_BLACK = 16    ///< Fekete
} Piece;

/**
 * @brief Megadja, hogy az adott bábu fekete-e
 * 
 * @param p Az ellenőrizendő bábu
 * @return true Ha, a bábu fekete
 * @return false Egyébként
 */
bool isBlack(Piece p);

/**
 * @brief Megadja, hogy az adott bábu fehér-e
 * 
 * @param p Az ellenőrizendő bábu
 * @return true Ha, a bábu fehér
 * @return false Egyébként
 */
bool isWhite(Piece p);

/**
 * @brief Megadja, hogy az adott bábu gyalog-e
 * 
 * @param p Az ellenőrizendő bábu
 * @return true Ha, a bábu gyalog
 * @return false Egyébként
 */
bool isPawn(Piece p);

/**
 * @brief Megadja, hogy az adott bábu huszár-e
 * 
 * @param p Az ellenőrizendő bábu
 * @return true Ha, a bábu huszár
 * @return false Egyébként
 */
bool isKnight(Piece p);

/**
 * @brief Megadja, hogy az adott bábu király-e
 * 
 * @param p Az ellenőrizendő bábu
 * @return true Ha, a bábu király
 * @return false Egyébként
 */
bool isKing(Piece p);

/**
 * @brief Megadja, hogy az adott bábu érvényes-e
 * 
 * @param p Az ellenőrizendő bábu
 * @return true Ha, a bábu érvényes
 * @return false Egyébként
 */
bool isValidPiece(Piece p);

//Returnes the string to print for a piece
const char* getPieceFace(Piece p);

/**
 * @brief Megadja egy adott bábu algebrai notációban megjelenő karaktert
 * 
 * @param p A bábu
 * @return char A bábuhoz tartozó karakter
 */
char getPieceChar(Piece p);

/**
 * @brief Megadja az algebrai notációban megjelenő karakterhez tartozó bábut
 * Figyelembe veszi a kis és nagybetűs eltérést
 * @param ch A bábu karaktere
 * @return Piece A karakterhez tartozó bábu
 */
Piece getPieceFromChar(const char ch);

#endif