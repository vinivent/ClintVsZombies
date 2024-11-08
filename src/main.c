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

#define MAP_HEIGHT 22
#define MAP_WIDTH 70

#define COLOR_WALL GREEN
#define COLOR_FLOOR GREEN
#define COLOR_CLINT YELLOW
#define COLOR_ZOMBIE RED
#define COLOR_BULLET BLUE
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
    int bullets;
    int health;
    int direction;
};


// Zombies
struct Zombie
{
    struct Position position;
    int health;
};


// Bullets
struct Bullet
{
    struct Position coords;
    int direction;
    int onScreen;
};


// Mapa do jogo
char map[MAP_HEIGHT][MAP_WIDTH] = {
    " #######################                     #########################",
    " #######################                     #########################",
    " ##                                                                 ##",
    " ##                                                                 ##",
    " ##                                                                 ##",
    " ##                                                                 ##",
    " ##                                                                 ##",
    " ##                                                                 ##",
    " ##                                                                 ##",

    "                                                                      ",
    "                                                                      ",
    "                                                                      ",
    "                                                                      ",
    "                                                                      ",
    " ##                                                                 ##",
    " ##                                                                 ##",
    " ##                                                                 ##",
    " ##                                                                 ##",
    " ##                                                                 ##",
    " ##                                                                 ##",
    " ##                                                                 ##",
    " #######################                     #########################",

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
    screenSetColor(COLOR_CLINT, BLACK);
    screenGotoxy(x, y);
    printf("▓");
}

void drawBullet(int x, int y)
{
    screenSetColor(COLOR_BULLET, BLACK);
    screenGotoxy(x, y);
    printf("♥");
}

// Função para posicionar Clint no centro do mapa
void initClint(struct Clint *clint)
{
    clint->coords.x = MAP_WIDTH / 2;
    clint->coords.y = MAP_HEIGHT / 2;
   
    clint->health = 10;
    clint->bullets = -1;
}

void initBullet(struct Bullet *bullet, struct Clint *clint) 
{
    if (clint->direction == 0) {
        bullet->direction = clint->direction; // Ela é criada com a direção que o Clint está olhando
        bullet->coords.x = clint->coords.x;
        bullet->coords.y = clint->coords.y-1; // Ela é criada um espaço na frente do Clint
        bullet->onScreen = 1; // Atirou, então agora a bala está na tela
    }
    if (clint->direction == 1) {
        bullet->direction = clint->direction;
        bullet->coords.x = clint->coords.x-1;
        bullet->coords.y = clint->coords.y;
        bullet->onScreen = 1;
    }
    if (clint->direction == 2) {
        bullet->direction = clint->direction;
        bullet->coords.x = clint->coords.x;
        bullet->coords.y = clint->coords.y+1;
        bullet->onScreen = 1;
    }
    if (clint->direction == 3) {
        bullet->direction = clint->direction;
        bullet->coords.x = clint->coords.x+1;
        bullet->coords.y = clint->coords.y;
        bullet->onScreen = 1;
    }
}

// Função para verificar se uma posição é uma parede
int isWall(int x, int y)
{
    return (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT || map[y][x] == '#');
}

void updateBullet(struct Bullet *bullet) 
{
    // Determine as próximas coordenadas da bala antes de movê-la
    int nextX = bullet->coords.x;
    int nextY = bullet->coords.y;

    // Dependendo da direção da bala, calcule a próxima posição
    if (bullet->direction == 0) {  // Para cima
        nextY--;
    }
    if (bullet->direction == 1) {  // Para a esquerda
        nextX--;
    }
    if (bullet->direction == 2) {  // Para baixo
        nextY++;
    }
    if (bullet->direction == 3) {  // Para a direita
        nextX++;
    }

    // Verifique se a próxima posição está fora dos limites ou é uma parede
    if (isWall(nextX, nextY) || nextX < 0 || nextX >= MAP_WIDTH || nextY < 0 || nextY >= MAP_HEIGHT) {
        bullet->onScreen = 0;  // A bala foi destruída
        return;  // Não mova a bala
    }

    // Se não houver colisão, atualize a posição da bala
    bullet->coords.x = nextX;
    bullet->coords.y = nextY;
}



int main()
{
    srand(time(NULL)); // Initialize random seed

    screenInit(0); // No borders this time
    keyboardInit();
    timerInit(75); // Update every 75ms

    struct Clint clint;
    struct Bullet bullet;
    initClint(&clint);

    while (1)
    {
        if (timerTimeOver())
        {
            screenDrawMap();

            // Draw Clint
            screenSetColor(COLOR_CLINT, BLACK);
            drawClint(clint.coords.x, clint.coords.y);

            if(bullet.onScreen)
            {
                updateBullet(&bullet);
                if(bullet.onScreen)
                    drawBullet(bullet.coords.x, bullet.coords.y);
            }

            screenUpdate();
        }

        if (keyhit())
        {
            int key = readch();
            int newX = clint.coords.x;
            int newY = clint.coords.y;


            // Basic Movements
            if (key == 'w') {
                newY--;  // Mover para cima
                clint.direction = 0;
            }
            if (key == 'a') {
                newX--;  // Mover para a esquerda
                clint.direction = 1;
            }
            if (key == 's') {
                newY++;  // Mover para baixo
                clint.direction = 2;
            }
            if (key == 'd') {
                newX++;  // Mover para a direita
                clint.direction = 3;
            }
            if(key == 'f') {
                initBullet(&bullet, &clint);
                drawBullet(bullet.coords.x, bullet.coords.y);
            }


            // Diagonal Movements
            if (key == 'w' && keyhit() && readch() == 'a') {
                newX--;  
                newY--;
            }
            if (key == 'w' && keyhit() && readch() == 'd') {
                newX++;  
                newY--;
            }
            if (key == 's' && keyhit() && readch() == 'a') {
                newX--;  
                newY++;
            }
            if (key == 's' && keyhit() && readch() == 'd') {
                newX++;  
                newY++;
            }

            // Limitação das paredes
            if (!isWall(newX, newY) && newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT)
            {
                clint.coords.x = newX;
                clint.coords.y = newY;
            }
        }
    }

    return 0;
}
