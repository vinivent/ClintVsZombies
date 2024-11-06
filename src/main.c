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
#include <time.h>

#define MAP_HEIGHT 18
#define MAP_WIDTH 71

#define COLOR_WALL GREEN
#define COLOR_FLOOR GREEN
#define COLOR_CLINT YELLOW
#define COLOR_ZOMBIE RED
#define MAX_ZOMBIES 10

// Posição no plano 2D
struct Position
{
    int x, y;
};

// Clint
struct Clint
{
    struct Position coords;
    int health;
    int bullets;
};

// Zombies
struct Zombie
{
    struct Position position;
    int health;
};

// Mapa do jogo
char map[MAP_HEIGHT][MAP_WIDTH] = {
    "  #######################                     #########################",
    "  #######################                     #########################",
    "  ##                                                                 ##",
    "  ##                                                                 ##",
    "  ##                                                                 ##",
    "  ##                                                                 ##",
    "  ##                                                                 ##",
    "                                                                       ",
    "                                                                       ",
    "                                                                       ",
    "                                                                       ",
    "                                                                       ",
    "  ##                                                                 ##",
    "  ##                                                                 ##",
    "  ##                                                                 ##",
    "  ##                                                                 ##",
    "  ##                                                                 ##",
    "  #######################                     #########################",
};

void screenDrawMap()
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            char cell = map[y][x];
            screenGotoxy(x, y);

            switch (cell)
            {
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

// Clint ASCII Art (Simplified)
void drawClint(int x, int y)
{
    screenGotoxy(x, y);
    printf(" O ");
    screenGotoxy(x, y + 1);
    printf("/|\\");
    screenGotoxy(x, y + 2);
    printf("/ \\");
}

// Função para verificar se uma posição é uma parede
int isWall(int x, int y)
{
    return (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT || map[y][x] == '#');
}

// Função para posicionar Clint no centro do mapa
void initClint(struct Clint *clint)
{
    clint->coords.x = MAP_WIDTH / 2;
    clint->coords.y = MAP_HEIGHT / 2;
   
    clint->health = 10;
    clint->bullets = -1;
}

int main()
{
    srand(time(NULL)); // Initialize random seed

    screenInit(0); // No borders this time
    keyboardInit();
    timerInit(75); // Update every 75ms

    struct Clint clint;
    initClint(&clint);

    while (1)
    {
        if (timerTimeOver())
        {
            screenDrawMap();

            // Draw Clint
            screenSetColor(COLOR_CLINT, BLACK);
            drawClint(clint.coords.x, clint.coords.y);

            screenUpdate();
        }

        if (keyhit())
        {
            int key = readch();
            int newX = clint.coords.x;
            int newY = clint.coords.y;

            // Controle de movimento de Clint sem ultrapassar paredes
            switch (key)
            {
            case 'a':
                newX--;
                break;
            case 'd':
                newX++;
                break;
            case 'w':
                newY--;
                break;
            case 's':
                newY++;
                break;
            case 'q':
                keyboardDestroy();
                screenDestroy();
                exit(0);
            }

            // Atualiza a posição do Clint se o movimento não o levar para uma parede
            if (newX >= 0 && newX <= MAP_WIDTH - 3 &&  // Left and Right boundaries
                newY >= 1 && newY <= MAP_HEIGHT - 3 && // Top and Bottom boundaries
                !isWall(newX, newY) &&
                !isWall(newX + 2, newY) && 
                !isWall(newX, newY + 2) &&
                !isWall(newX + 2, newY + 2)) {

                clint.coords.x = newX;
                clint.coords.y = newY;
            }
        }
    }

    return 0;
}
