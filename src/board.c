#include "board.h"
#include "piece.h"
#include "algebraic.h"
#include "moves.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <econio.h>

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

Board* newGameFromFen(const char* fenStr){
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
    return b;
}

//Starts a new game
Board* newGameFromStart(){
    return newGameFromFen(START_FEN);
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
    bool wC, bC;
    isInCheck(board, &wC, &bC);
    econio_gotoxy(11,1);
    printf("White is %sin check\n", wC ? "" : "not ");
    econio_gotoxy(11,2);
    printf("Black is %sin check\n", bC ? "" : "not ");

    econio_gotoxy(0,11);
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

void isInCheck(Board *board, bool *whiteInCheck, bool *blackInCheck){
    for(int rank = 0; rank < 8; rank++){
        for(int file = 0; file < 8; file++){
            Square s = {.rank = rank, .file = file};
            if(isSame(s, board->whiteKing) || isSame(s, board->blackKing)){
                //No need to check kings
                continue;
            }
            if(isValidPieceAt(board, s)){
                if(isWhiteAt(board, s) && blackInCheck != NULL){
                    //Check if white can hit black
                    Move m = {
                        .from = s,
                        .to = board->blackKing
                    };

                    *blackInCheck |= isValidMove(board, m, NULL, NULL, NULL);                    
                } else if(isBlackAt(board, s) && whiteInCheck != NULL){
                    Move m = {
                        .from = s,
                        .to = board->whiteKing
                    };

                    *whiteInCheck |= isValidMove(board, m, NULL, NULL, NULL);
                }
            }
        }
    }
}

void willNextMoveBeCheck(Board *b, Move move, bool *isWhiteInCheck, bool *isBlackInCheck){
    Board copy = *b; //Copy structure
    movePiece(&copy, move);
    copy.nextIsWhite = !copy.nextIsWhite;
    isInCheck(&copy, isWhiteInCheck, isBlackInCheck);
}