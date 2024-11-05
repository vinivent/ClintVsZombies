/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define MAP_HEIGHT 55
#define MAP_WIDTH  100

#define COLOR_WALL GREEN
#define COLOR_FLOOR GREEN


// Posição no plano 2D
struct Posicao {
    int x, y;
};


// Posição e vida do Clint
struct Clint {
    struct Posicao posicao;
    int vida;
};


// Posição e vida do Zumbi
struct Zombie {
    struct Posicao posicao;
    int vida;
};


struct Clint clint;
struct Zombie zombies[10];


void posicionar() {

    // Posição do Clint no mapa
    clint.posicao.x = MAP_WIDTH / 2;
    clint.posicao.y = MAP_HEIGHT / 2;
    clint.vida = 100;


    // Posição dos Zumbis no mapa
    for (int i = 0; i < 10; i++) {
        zombies[i].posicao.x = rand() % MAP_WIDTH;
        zombies[i].posicao.y = rand() % MAP_HEIGHT;
        zombies[i].vida = 50; 
    }
}



// Mapa do jogo
char map[MAP_HEIGHT][MAP_WIDTH] = {
    "    #######################                     #########################",
    "    #######################                     #########################",
    "    ##                                                                 ##",
    "    ##                                                                 ##",
    "    ##                                                                 ##",
    "    ##                                                                 ##",
    "    ##                                                                 ##",
    "                                                                         ",
    "                                                                         ",
    "                                                                         ",
    "                                                                         ",
    "                                                                         ",
    "    ##                                                                 ##",
    "    ##                                                                 ##",
    "    ##                                                                 ##",
    "    ##                                                                 ##",
    "    ##                                                                 ##",
    "    #######################                     #########################",
};

void screenDrawMap() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            char cell = map[y][x];
            screenGotoxy(x, y);

            switch(cell) {
                case '#':
                    screenSetColor(COLOR_WALL, BLACK);
                    printf("▓");
                    break;
                default:
                    screenSetColor(COLOR_FLOOR, BLACK);
                    printf(" ");
                    break;
            }
        }
    }
    screenSetColor(WHITE, BLACK);
    fflush(stdout);
}


void printKey(int ch)
{
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(35, 22);
    printf("Key code :");

    screenGotoxy(34, 23);
    printf("            ");
    
    if (ch == 27) screenGotoxy(36, 23);
    else screenGotoxy(39, 23);

    printf("%d ", ch);
    while (keyhit())
    {
        printf("%d ", readch());
    }
}


int main() 
{
    static int ch = 0;
    screenInit(1);
    keyboardInit();
    timerInit(50);
    screenDrawMap();
    screenUpdate();

    while (ch != 10) 
    {
        if (keyhit()) 
        {
            ch = readch();
            printKey(ch);
            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
