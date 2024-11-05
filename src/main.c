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
    drawLogo();
    screenDrawMap();
    screenUpdate();

    while (ch != 10) //enter
    {
        // Handle user input
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
