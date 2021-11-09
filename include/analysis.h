#ifndef CHESS_ANALYSIS
#define CHESS_ANALYSIS

#include "moves.h"
#include "piece.h"

#include <SDL2/SDL.h>

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

    SDL_Texture *texture;

    //Linked list
    struct ReplayNode *previous;
    struct ReplayNode *next;
};

typedef struct ReplayList{
    ReplayNode *first;
    ReplayNode *last;
    int length;
} ReplayList;


void appendToList(ReplayList *list, ReplayNode data);
void appendPointerToList(ReplayList *list, ReplayNode *node);
bool deleteFromList(ReplayList *list, ReplayNode *node);
void deleteList(ReplayList *list);

enum MoveUniqueness isMoveUnique(Board *board, Move move);

int nodeToString(ReplayNode *node, char *out);

void createNodeTexture(ReplayNode *node, SDL_Renderer *renderer);

void renderReplay(ReplayList list, SDL_Renderer *renderer);


void saveWithMoves(const char *path, Board *board);

Board* loadMoves(const char *path);
#endif
