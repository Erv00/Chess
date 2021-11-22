/**
 * @file analysis.h
 * @brief Analízis
 */
#ifndef CHESS_ANALYSIS
#define CHESS_ANALYSIS

#include "moves.h"
#include "piece.h"

#include <SDL2/SDL.h>

typedef struct ReplayNode ReplayNode;

/**
 * @brief Egy lépés kiindulási helyzetének egyedisége
 */
enum MoveUniqueness {
    NONE_UNIQUE,    ///< Sem a sor, sem az oszlop nem egyed
    FILE_UNIQUE,    ///< Az oszlop egyedi
    RANK_UNIQUE,    ///< A sor egyedi
    UNIQUE          ///< Mindkettő egyedi
};

/**
 * @brief Egy sáncolás leírása
 */
typedef struct CastlingData{
    bool isKingside;                ///< Igaz, ha a sáncolás rövid (király oldali)
    Move rookMove;                  ///< Ha sáncolás, ez a lépés mozdítja a bástyát
    char newCastlingAvailability;   ///< A lépés utáni sáncolási lehetőségek
    char oldCastlingAvailability;   ///< A lépés előtti sáncolási lehetőségek
} CastlingData;

/**
 * @brief Egy lépés részletes leírása
 */
struct ReplayNode{
    bool isWhiteMove;           ///< Igaz, ha a lépést fehér tette meg
    bool isCastling;            ///< Igaz, ha a lépés sáncolás
    bool isCapture;             ///< Igaz, ha a lépés leüt egy bábut
    bool isCheck;               ///< Igaz, ha a lépés sakkot okoz
    bool isCheckmate;           ///< Igaz, ha a lépés sakkmattot ad
    bool isPromotion;           ///< Igaz, ha a lépés átalakulás

    enum MoveUniqueness moveUniqueness; ///< A lépés egyedisége

    Move move;                  ///< Maga a lépés
    Piece movedPiece;           ///< A megmozdított bábu
    Piece pieceCaptured;        ///< A leütött bábu, ha volt
    Piece promotionChoice;      ///< Ha átváltozás volt, ezzé a bábuvá változott

    CastlingData castlingData;  ///< A sáncolást leíró adatok

    SDL_Texture *texture;       ///< A lépés textúrája

    //Linked list
    struct ReplayNode *previous;    ///< A lista következő eleme
    struct ReplayNode *next;        ///< A lista előző eleme
};

/**
 * @brief Láncolt lista, egy adott álláshoz vezető lépéseket tartalmazza
 */
typedef struct ReplayList{
    ReplayNode *first;  ///< A lista első eleme
    ReplayNode *last;   ///< A lista utolsó eleme
    int length;         ///< A lista hossza
} ReplayList;

/**
 * @brief Bővíti a listát egy másolattal
 * 
 * @param list A bővítendő lista
 * @param data Ennek a másolatával bővül a lista
 */
void appendToList(ReplayList *list, ReplayNode data);

/**
 * @brief Hozzáadja a megadott pointert a listához
 * 
 * @param list A bővítendő lista
 * @param node Ezzel bővül a lista
 */
void appendPointerToList(ReplayList *list, ReplayNode *node);

/**
 * @brief Felszabadítja a lista összes elemét
 * 
 * @param list A felszabadítandó lista
 */
void deleteList(ReplayList *list);

/**
 * @brief Felszabadítja a megadott elem utáni összes elemet
 * 
 * @param list A megadott lista
 * @param node A lépés ami utáni lépéseket fell kell szabadítani
 */
void freeListAfter(ReplayList *list,ReplayNode *node);

/**
 * @brief Meghatározza egy lépésről, hogy a kiindulási pozíció egyértelműen meghatározható-e
 * 
 * @param board A tábla, amin a lépés történik
 * @param move A lépés
 * @return enum MoveUniqueness A lépés egyedisége
 */
enum MoveUniqueness isMoveUnique(Board *board, Move move);

/**
 * @brief A megadott lépést alakítja sztringgé
 * 
 * @param node A sztringgé alakítandó lépés
 * @param out Ide íródik a sztring. Nem lehet NULL
 */
void nodeToString(ReplayNode *node, char *out);

/**
 * @brief Megrajzolja az adott lépés textúráját, ha még nem létezik
 * @todo Include step number here if possible
 * @param node A rajzolandó lépés
 * @param renderer A használt renderer
 */
void createNodeTexture(ReplayNode *node, SDL_Renderer *renderer);

/**
 * @brief Kirajzolja a tábla szélére az eddig lépésekközül az utolsó 15-öt
 * 
 * @param list Az idevezető lépések listája
 * @param renderer A használt renderer
 */
void renderReplay(ReplayList list, SDL_Renderer *renderer);

/**
 * @brief Elmenti a játékállást a megadott útvonalra. A fájl tartalmazni fogja az ide vezető lépéseket
 * 
 * @param path A mentési fájl elérési útja. Felül lesz írva, ha létezik
 * @param board A menteni kívánt tábla
 */
void saveWithMoves(const char *path, Board *board);

/**
 * @brief Visszavonja a megadott lépést
 * 
 * @param board A tábla maelyen a lépés visszavonódik
 * @param node A visszavonandó lépés
 */
void undoMove(Board *board, ReplayNode *node);

/**
 * @brief Ismét végrehajtja a megadott lépést
 * 
 * @param board A tábla maelyen a lépés ismét végrehajtódik
 * @param node A végrehajtandó lépés
 */
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
