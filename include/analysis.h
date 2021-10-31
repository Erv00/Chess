#ifndef CHESS_ANALYSIS
#define CHESS_ANALYSIS

#include "moves.h"
#include "piece.h"

typedef struct ReplayNode ReplayNode;

enum MoveUniqueness {
    NONE_UNIQUE,
    FILE_UNIQUE,
    RANK_UNIQUE,
    UNIQUE
};

struct ReplayNode{
    //Info
    bool isWhiteMove;
    //True if is castling move
    bool isCastling;
    bool castlingIsKingSide;
    bool isCapture;
    bool isCheck;
    bool isCheckmate;
    bool isPromotion;

    enum MoveUniqueness moveUniqueness;

    Move move;
    Piece movedPiece;
    Piece pieceCaptured;
    Piece promotionChoice;

    //Linked list
    struct ReplayNode *previous;
    struct ReplayNode *next;
};

typedef struct ReplayList{
    ReplayNode *first;
    ReplayNode *last;
} ReplayList;


void appendToList(ReplayList *list, ReplayNode data);
bool deleteFromList(ReplayList *list, ReplayNode *node);
void deleteList(ReplayList *list);

enum MoveUniqueness isMoveUnique(Board *board, Move move);

int nodeToString(ReplayNode *node, char *out);
#endif