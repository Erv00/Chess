#include "fen.h"

#include <stdbool.h>

#include "board.h"
#include "piece.h"
#include "algebraic.h"

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

const char* processFENCastlingData(Board *board, const char *fenStr){
    if(*fenStr == '-')
        return fenStr+2;
    
    while(*fenStr != ' '){
        switch(*fenStr++){
        case 'K':
            board->castlingAvailability |= 1;
            break;
        case 'Q':
            board->castlingAvailability |= 2;
            break;
        case 'k':
            board->castlingAvailability |= 4;
            break;
        case 'q':
            board->castlingAvailability |= 8;
            break;    
        }
    }
    return ++fenStr;
}

const char* processFENEnPassanteData(Board *board, const char *fenStr){
    if(*fenStr == '-'){
        board->enPassante.rank = -1;
        board->enPassante.file = -1;
        return fenStr+2;
    }
    
    char algebraic[2];
    algebraic[0] = *fenStr++;
    algebraic[1] = *fenStr++;

    board->enPassante = algebraicToSquare(algebraic);

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

char getPieceCharFEN(Piece p){
    static char PIECE_CHAR_FEN[] = {'P', 'R', 'N', 'B', 'Q', 'K'};

    char mask = isWhite(p) ? 0 : 32;

    for(unsigned int i=0; i<6;i++){
        if(i == (p & 7)) //&7 selects only type, discards color
            return (PIECE_CHAR_FEN[i] | mask); //Mask to return the correct case
    }

    fprintf(stderr, "No such piece %u\n", p);
    return 'X';
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
                *fen++ = getPieceCharFEN(*at(board, s));
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
