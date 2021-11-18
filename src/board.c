#include "board.h"
#include "piece.h"
#include "algebraic.h"
#include "moves.h"
#include "check.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <econio.h>
#include "debugmalloc.h"

const char* processFENPieceData(Board *board, const char *fenStr){
    //Pieces
    int rank = 7, file = 0;
    while(*fenStr != ' '){
        if(*fenStr == '/'){
            //Skip to next rank
            rank--;
            file = 0;
        }else if(isdigit(*fenStr)){
            //Skip ahead
            file += *fenStr - '0';
        }else{
            //Piece data
            Piece piece = isupper(*fenStr) ? COLOR_WHITE : COLOR_BLACK;
            switch(*fenStr | 32){ // OR 32 is to only deal with lowercase characters
            case 'p':
                piece |= PAWN;
                break;
            case 'r':
                piece |= ROOK;
                break;
            case 'n':
                piece |= KNIGHT;
                break;
            case 'b':
                piece |= BISHOP;
                break;
            case 'q':
                piece |= QUEEN;
                break;
            case 'k':
                piece |= KING;
                if(isWhite(piece)){
                    board->whiteKing.rank = rank;
                    board->whiteKing.file = file;
                }else{
                    board->blackKing.rank = rank;
                    board->blackKing.file = file;
                }
                break;
            default:
                printf("Unknown character in FEN string: %c\n", *fenStr);
            }
    
            board->cell[rank][file] = piece;
            file++;
        }
        fenStr++;
    }

    //++ to point to next fen-data
    return ++fenStr;
}

const char* processFENCastlingData(Board *b, const char *fenStr){
    if(*fenStr == '-')
        return fenStr+2;
    
    while(*fenStr != ' '){
        switch(*fenStr++){
        case 'K':
            b->castlingAvailability |= 1;
            break;
        case 'Q':
            b->castlingAvailability |= 2;
            break;
        case 'k':
            b->castlingAvailability |= 4;
            break;
        case 'q':
            b->castlingAvailability |= 8;
            break;    
        }
    }
    return ++fenStr;
}

const char* processFENEnPassanteData(Board *b, const char *fenStr){
    if(*fenStr == '-'){
        b->enPassante.rank = -1;
        b->enPassante.file = -1;
        return fenStr+2;
    }
    
    char algebraic[2];
    algebraic[0] = *fenStr++;
    algebraic[1] = *fenStr++;

    b->enPassante = algebraicToSquare(algebraic);

    return ++fenStr;
}

Board* newGameFromFen(const char* fenStr, SDL_Renderer *renderer){
    Board *b = calloc(1,sizeof(Board));
    fenStr = processFENPieceData(b, fenStr);
    if(*fenStr++ == 'w')
        b->nextIsWhite = true;
    else
        b->nextIsWhite = false;
    fenStr++;
    fenStr = processFENCastlingData(b, fenStr);
    fenStr = processFENEnPassanteData(b, fenStr);
    //Half and full-moves
    sscanf(fenStr, "%d %d", &b->halfmoveClock, &b->fullmoveCounter);
    b->renderer = renderer;

    b->whiteClock = newClock(3, renderer, true);
    b->blackClock = newClock(900, renderer, false);
    return b;
}

int getCastlingString(Board *board, char cast[4]){
    static const char *CASTLING_ORDER = "KQkq";
    if(board->castlingAvailability == 0){
        cast[0] = '-';
        return 1;
    }

    char *orig = cast;
    //Castling is available
    for(int i=0; i<4; i++){
        bool white = i < 2;
        bool kingside = (i % 2) == 0;
        if(canCastle(board, white, kingside))
            *cast++ = CASTLING_ORDER[i];
    }
    return cast-orig;
}

//Starts a new game
Board* newGameFromStart(SDL_Renderer *renderer, int time){
    Board *board =  newGameFromFen(START_FEN, renderer);
    board->hasReplayData = true;
    board->whiteClock = newClock(time, renderer, true);
    board->blackClock = newClock(time, renderer, false);
    return board;
}

//Prints the board
void printBoard(Board *board){
    econio_clrscr();
    printf(" abcdefgh\n");
    for(int rank = 7; rank >= 0; rank--){
        printf("%d", rank+1);
        for(int file = 0; file < 8; file++){
            Piece p = board->cell[rank][file];
            if(isValidPiece(p)){
                printf(getPieceFace(p));
            } else {
                printf(" ");
            }
        }
        printf("%d\n",rank+1);
    }
    printf(" abcdefgh\n");

        econio_gotoxy(11,0);
    if(isValidSquare(board->enPassante)){
        char alg[3] = {0};
        squareToAlgebraic(board->enPassante, alg);
        printf("En passante: %s\n", alg);
    }else{
        printf("En passante: -\n");
    }

    //Check
    CheckDataByColor cd = isBoardInCheck(board);
    econio_gotoxy(11,1);
    printf("White is %sin check\n", cd.white.inCheck ? "" : "not ");
    econio_gotoxy(11,2);
    printf("Black is %sin check\n", cd.black.inCheck ? "" : "not ");
    econio_gotoxy(11,3);
    printf("Half moves %d, Full moves %d\n", board->halfmoveClock, board->fullmoveCounter);
    econio_gotoxy(11,4);
    printf("%s to move\n", board->nextIsWhite ? "White" : "Black");
    econio_gotoxy(11, 5);
    char cast[5] = {0};
    getCastlingString(board, cast);
    printf("Castling: %s\n", cast);
    if(board->checkmate)
            printf("CHECKMATE\n");

    econio_gotoxy(0,11);
}

char* saveFENPieceData(Board *board, char* fen){
    for(int rank = 7; rank >= 0; rank--){
        int lastAt = -1;
        int file;
        for(file = 0; file < 8; file++){
            Square s = {.rank = rank, .file = file};
            if(isValidPieceAt(board, s)){
                //Padding
                if(lastAt != file-1){
                    *fen++ = '0' + file - lastAt - 1;
                }
                lastAt = file;
                *fen++ = getPieceChar(at(board, s));
            }
        }
        //Padding
        if(lastAt != 7){
            *fen++ = '0' + 7 - lastAt;
        }
        if(rank != 0)
            *fen++ = '/';
    }
    
    *fen++ = ' ';
    return fen;
}

char* saveAsFEN(Board *board){
    int len = 79+log10(board->fullmoveCounter)+1+1;
    char *fen = malloc(len); //Longest fen+extra + fullmove counter + enough space for the fullmove counter + nullbyte
    char *current = fen;
    if(fen == NULL){
        fprintf(stderr, "Failed to allocate memory for fen str\n");
        exit(-1);
    }

    memset(fen, 0, len);

    //Piece data
    current = saveFENPieceData(board, current);

    //Active color
    if(board->nextIsWhite)
        *current++ = 'w';
    else
        *current++ = 'b';

    *current++ = ' ';

    //Castling
    current += getCastlingString(board, current);
    *current++ = ' ';

    //En passante
    if(isValidSquare(board->enPassante)){
        squareToAlgebraic(board->enPassante, current);
        current += 2;
    }else{
        *current++ = '-';
    }

    *current++ = ' ';

    //Halfmove & fullmove
    sprintf(current, "%d %d", board->halfmoveClock, board->fullmoveCounter);
    return fen;
}

void checkBoardStatus(Board *board){
    ReplayNode *node = board->replayData.last;
    //Check for checkmate
    CheckDataByColor cd = isBoardInCheck(board);
    if(cd.white.inCheck || cd.black.inCheck){
        node->isCheck = true;
        if(isCheckmate(board)){
            board->checkmate = true;
            node->isCheckmate = true;
        }
    }

    //50 move rule
    if(board->halfmoveClock >= 100){
        printf("DRAW\n");
    }
}

void saveWithoutMoves(const char *path, Board *board){
    FILE *saveFile = fopen(path, "w");
    if(saveFile == NULL)
        //HANDLE: TODO
        return;

    //Save position
    char *fen = saveAsFEN(board);
    fprintf(saveFile, "%s\n", fen);
    free(fen);
    //Close file
    fclose(saveFile);
}

Board* loadWithoutMoves(const char *path, SDL_Renderer *renderer){
    FILE *saveFile = fopen(path, "r");
    if(saveFile == NULL){
        //TODO: Handle
        abort();
    }

    char fen[90]; //Bit extra for the longest possible fen string
    fgets(fen, 90, saveFile);
    Board *board = newGameFromFen(fen, renderer);

    fclose(saveFile);

    return board;
}

void destroyBoard(Board *board){
    deleteList(&board->replayData);
    free(board);
}

bool updateCorrectClock(Board *board){
    if(board->nextIsWhite)
        return clockTick(&board->whiteClock);
    return clockTick(&board->blackClock);
}
Piece at(Board *board, Square square){
    return board->cell[square.rank][square.file];
}

bool isFreeAt(Board *board, Square square){
    return !isValidPieceAt(board, square);
}

bool isBlackAt(Board *board, Square square){
    return isBlack(at(board, square));
}

bool isWhiteAt(Board *board, Square square){
    return isWhite(at(board, square));
}

bool isPawnAt(Board *board, Square square){
    return isPawn(at(board, square));
}

bool isRookAt(Board *board, Square square){
    return isRook(at(board, square));
}

bool isKnightAt(Board *board, Square square){
    return isKnight(at(board, square));
}

bool isBishopAt(Board *board, Square square){
    return isBishop(at(board, square));
}

bool isQueenAt(Board *board, Square square){
    return isQueen(at(board, square));
}

bool isKingAt(Board *board, Square square){
    return isKing(at(board, square));
}

bool isValidPieceAt(Board *board, Square square){
    return isValidPiece(at(board, square));
}

bool isOpponent(Board *board, Piece piece){
    if(board->nextIsWhite){
        return isBlack(piece);
    }
    return isWhite(piece);
}

bool isOpponentAt(Board *board, Square square){
    if(!isValidPieceAt(board, square)){
        return false;
    }
    //There is a piece, is it opponent's
    return isOpponent(board, at(board, square));
}

bool isSameColorAt(Board *board, Square s1, Square s2){
    return isSameColor(at(board,s1), at(board, s2));
}
