/**
 * @file buttons.h
 * @brief Kattintható gombok
 */
#ifndef CHESS_BUTTONS
#define CHESS_BUTTONS

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef SDL_Rect Button;

/**
 * @brief Megvárja, amíg a felhasználó megyom egy gombot a megadottak közül
 * 
 * @param buttons A választható gombok
 * @param length A választható gombok száma
 * @return int A megnyomott gomb pozíciója a listában, -1 ha a felhasználó ki szeretne lépni
 */
int waitForButtonPress(Button buttons[], int length);

/**
 * @brief Meghatározza egy eventről, hogy a megadott gombok közül megnyom-e egyet
 * 
 * @param buttons A választható gombok
 * @param length A választható gombok száma
 * @param event A vizsgálandó esemény
 * @param buttonPressed Ide íródik a megnyomott gomb sorszáma. Nem lehet NULL
 * @return true Ha az event gombnyomás volt
 * @return false Ellenkező esetben
 */
bool processEvent(Button buttons[], int length, SDL_Event *event, int *buttonPressed);
#endif
