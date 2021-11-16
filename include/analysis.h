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

    Move rookMove;

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
void freeListAfter(ReplayList *list,ReplayNode *node);

enum MoveUniqueness isMoveUnique(Board *board, Move move);

int nodeToString(ReplayNode *node, char *out);

void createNodeTexture(ReplayNode *node, SDL_Renderer *renderer);

void renderReplay(ReplayList list, SDL_Renderer *renderer);


void saveWithMoves(const char *path, Board *board);

Board* loadMoves(const char *path);
void undoMove(Board *board, ReplayNode *node);
void redoMove(Board *board, ReplayNode *node);

/**
 * @brief Kezel egy gombnyomást analízis nézetben
 * 
 * @param board A tábla amivel dolgozik
 * @param ev A gombnyomás event
 * 
 * @return true Ha a felhasználó ki szeretne lépni
 * @return false Ellenkező esetben
 */
bool handleAnalysisKeypress(Board *board, SDL_Event ev);
#endif