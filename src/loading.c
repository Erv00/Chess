#include "loading.h"

#include <debugmalloc.h>

ReplayNode* parseMoveStr(const char *moveStr, Board *board){
    ReplayNode *node = malloc(sizeof(ReplayNode));
    if(node == NULL){
        fprintf(stderr, "Failed to alloc memory for node\n");
        exit(-1);
    }

    //Zero out for bools
    memset(node, 0, sizeof(ReplayNode));
    int length = strlen(moveStr);
    bool knownOriginRank = false;
    bool knownOriginFile = false;
   
    //Set defaults
    node->isWhiteMove = board->nextIsWhite;
    node->isCheckmate = moveStr[length-1] == '#';
    node->isCheck = node->isCheckmate || moveStr[length-1] == '+';
    node->moveUniqueness = UNIQUE;

    if(moveStr[0] == '0'){
        //Castling
        processCastlingStr(moveStr, node, board);
    } else {
        //Not castling
        for(int idx = 0; idx < length; idx++){
            char curr = moveStr[idx] | 32; //Make all characters lowercase
            bool isBishop = moveStr[idx] == 'B';
            if(curr == 'x'){
                //Capture
                node->isCapture = true;
            }else if(curr == '#' || curr == '+'){
                //Check(mate)
            }else if('a' <= curr && curr <= 'h' && !isBishop){ //Extra conditions for bishop
                //Move data or disambiguation
                //File
                if(node->move.to.file != 0){
                    //Already had a file, that was a disambiguation
                    node->move.from.file = node->move.to.file;
                    knownOriginFile = true;
                    if(node->movedPiece != 0)
                        node->moveUniqueness ^= FILE_UNIQUE;
                }
                node->move.to.file = curr - 'a';
            }else if('1' <= curr && curr <= '8'){
                //Move data or disambiguation
                //Rank
                if(node->move.to.rank != 0){
                    //Already had a rank, that was a disambiguation
                    node->move.from.rank = node->move.to.rank;
                    node->moveUniqueness ^= RANK_UNIQUE;
                }
                node->move.to.rank = curr - '1';
            }else{
                //Piece specification
                char color = node->isWhiteMove ? 0 : 32;
                if(idx == length-1 || idx == length-2){
                    //This is a promotion
                    node->isPromotion = true;
                    node->promotionChoice = getPieceFromChar(moveStr[idx] | color);
                }else if(node->movedPiece == 0)
                    node->movedPiece = getPieceFromChar(moveStr[idx] | color);
            }
        }
    }

    if(node->movedPiece == 0){
        node->movedPiece = PAWN;
        if(board->nextIsWhite)
            node->movedPiece |= COLOR_WHITE;
        else
            node->movedPiece |= COLOR_BLACK;
    }

    if(node->isCapture)
        node->pieceCaptured = *at(board, node->move.to);

    //Determine move origin
    if(knownOriginRank && knownOriginFile)
        //Origin is precisely known, return
        return node;

    if(knownOriginRank){
        //Rank known, file unknow
        for(int file = 0; file < 8; file++){
            node->move.from.file = file;
            if(*at(board, node->move.from) == node->movedPiece)
                //Found piece, check if can move
                if(isValidMove(board, node->move, NULL, NULL, NULL))
                    //Found piece
                    break;
        }
    }else if(knownOriginFile){
        //Rank unknown, file know
        for(int rank = 0; rank < 8; rank++){
            node->move.from.rank = rank;
            if(*at(board, node->move.from) == node->movedPiece)
                //Found piece, check if can move
                if(isValidMove(board, node->move, NULL, NULL, NULL))
                    //Found piece
                    break;
        }
    }else{
        //Neither is known
        switch(node->movedPiece & 7){
            case PAWN:
                node->move.from = findPawnMovableTo(node->move.to, node->movedPiece, board);
                break;
            case ROOK:
                node->move.from = findRookMovableTo(node->move.to, node->movedPiece, board);
                break;
            case KNIGHT:
                node->move.from = findKnightMovableTo(node->move.to, node->movedPiece, board);
                break;
            case BISHOP:
                node->move.from = findBishopMovableTo(node->move.to, node->movedPiece, board);
                break;
            case QUEEN:
                node->move.from = findQueenMovableTo(node->move.to, node->movedPiece, board);
                break;
            case KING:
                if(board->nextIsWhite)
                    node->move.from = board->whiteKing;
                else
                    node->move.from = board->blackKing;
                break;
        }
    }

    return node;
}

void processCastlingStr(const char *moveStr, ReplayNode* node, Board *board){
    node->isCastling = true;

    if(board->nextIsWhite){
        node->move.from = board->whiteKing;
        node->move.to = board->whiteKing;
        node->movedPiece = COLOR_WHITE | KING;

        node->castlingData.rookMove.from = board->whiteKing;
        node->castlingData.rookMove.to = board->whiteKing;
    }else{
        node->move.from = board->blackKing;
        node->move.to = board->blackKing;
        node->movedPiece = COLOR_BLACK | KING;

        node->castlingData.rookMove.from = board->blackKing;
        node->castlingData.rookMove.to = board->blackKing;
    }

    if(strcmp(moveStr, "0-0") == 0){
        //Castling kingside
        node->castlingData.isKingside = true;
        node->move.to.file += 2;

        node->castlingData.rookMove.from.file = 7;
        node->castlingData.rookMove.to.file += 1;
    }else{
        //Castling queenside
        node->move.to.file -= 2;
        
        node->castlingData.rookMove.from.file = 0;
        node->castlingData.rookMove.to.file -= 1;
    }
}

Board* loadMoves(const char *path, SDL_Renderer *renderer){
    //Open file
    FILE *saveFile = fopen(path, "r");
    if(saveFile == NULL){
        fprintf(stderr, "Failed to open savefile %s. Aborting\n", path);
        exit(-1);
    }

    //Read fen
    char fen[90]; //Bit extra for the longest possible fen string
    fgets(fen, 90, saveFile);
    //Get original time
    int origTime, test;
    if(fscanf(saveFile, "%d;%d", &origTime, &test) != 1){
        //Could read second, saved for continuing
        fprintf(stderr, "File saved for continuing, not for analysis\n");
        return NULL;
    }
    Board *board = newGameFromStart(renderer, origTime);

    //While not empty parse string

    int timeLeft;
    char moveStr[8];
    while(fscanf(saveFile, "%d %s", &timeLeft, moveStr) != EOF){
        //Discard \n
        ReplayNode *node = parseMoveStr(moveStr, board);
        node->timeLeft = timeLeft;
        appendPointerToList(&board->replayData, node);

        //Check castling changes
        node->castlingData.oldCastlingAvailability = board->castlingAvailability;
        board->castlingAvailability = getCastlingAvailabilityAfterMove(board, node->move);
        node->castlingData.newCastlingAvailability = board->castlingAvailability;

        //Make move
        movePiece(board, node->move);
        //If castling also move rook
        if(node->isCastling)
            movePiece(board, node->castlingData.rookMove);
        
        //If promotion, promote
        if(node->isPromotion)
            *at(board, node->move.to) = node->promotionChoice;

        //Update board status
        board->checkmate = node->isCheckmate;
        board->nextIsWhite = !board->nextIsWhite;
    }

    if(board->replayData.length == 0){
        fprintf(stderr, "The savefile %s was not saved with analysis data. Ignoring\n", path);
    }else{
        ReplayNode *last = board->replayData.last;
        if(last->isWhiteMove)
            board->whiteClock.secondsRemaining = last->timeLeft;
        else
            board->blackClock.secondsRemaining = last->timeLeft;
        
        last = last->previous;
        if(last != NULL){
            if(last->isWhiteMove)
                board->whiteClock.secondsRemaining = last->timeLeft;
            else
                board->blackClock.secondsRemaining = last->timeLeft;
        }
        
        //Update clock textures
        updateClockTexture(&board->whiteClock);
        updateClockTexture(&board->blackClock);

    }

    return board;
}
